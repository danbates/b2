//This file is part of Bertini 2.0.
//
//tracking/observers.hpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//tracking/observers.hpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with tracking/observers.hpp.  If not, see <http://www.gnu.org/licenses/>.
//

//  tracking/observers.hpp
//
//  copyright 2016
//  Daniel Brake
//  University of Notre Dame
//  ACMS
//  Spring 2016

/**
\file tracking/observers.hpp

\brief Contains the tracking/observers base types
*/

#pragma once

#include "bertini2/tracking/events.hpp"
#include "bertini2/tracking/base_tracker.hpp"
#include "bertini2/logging.hpp"
#include <boost/type_index.hpp>

namespace bertini {

	namespace tracking{

		template<class ObservedT>
		class PrecisionAccumulator : public Observer<ObservedT>
		{ BOOST_TYPE_INDEX_REGISTER_CLASS
			virtual void Observe(AnyEvent const& e) override
			{
				const TrackingEvent<ObservedT>* p = dynamic_cast<const TrackingEvent<ObservedT>*>(&e);
				if (p)
				{
					Visit(p->Get());
				}
			}


			virtual void Visit(ObservedT const& t) override
			{
				precisions_.push_back(t.CurrentPrecision());
			}

		public:
			const std::vector<unsigned>& Precisions() const
			{
				return precisions_;
			}

		private:
			std::vector<unsigned> precisions_;
		};

		/**
		Example usage:
		PathAccumulator<AMPTracker> path_accumulator;
		*/
		template<class ObservedT, template<class> class EventT = SuccessfulStep>
		class AMPPathAccumulator : public Observer<ObservedT>
		{ BOOST_TYPE_INDEX_REGISTER_CLASS
			virtual void Observe(AnyEvent const& e) override
			{
				const EventT<ObservedT>* p = dynamic_cast<const EventT<ObservedT>*>(&e);
				if (p)
				{
					Visit(p->Get());
				}
			}


			virtual void Visit(ObservedT const& t) override
			{
				if (t.CurrentPrecision()==DoublePrecision())
				{
					Vec<mpfr> temp(t.NumVariables());
					for (unsigned ii=0; ii<t.NumVariables(); ++ii)
						temp(ii) = mpfr(t.template CurrentPoint<dbl>()(ii));
					
					path_.push_back(temp);
				}
				else
					path_.push_back(t.template CurrentPoint<mpfr>());
			}

		public:
			const std::vector<Vec<mpfr> >& Path() const
			{
				return path_;
			}

		private:
			std::vector<Vec<mpfr> > path_;
		};



		template<class ObservedT>
		class GoryDetailLogger : public Observer<ObservedT>
		{ BOOST_TYPE_INDEX_REGISTER_CLASS
		public:


			virtual void Observe(AnyEvent const& e) override
			{
				

				if (auto p = dynamic_cast<const Initializing<ObservedT,dbl>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::debug) << "initializing, tracking path\nfrom\tt = " << p->StartTime() << "\nto\tt = " << p->EndTime() << "\n from\tx = \n" << p->StartPoint();// << "\n\nusing predictor " << p->Get().Predictor();
				}
				else if (auto p = dynamic_cast<const Initializing<ObservedT,mpfr>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::debug) << "initializing, tracking path\nfrom\tt = " << p->StartTime() << "\nto\tt = " << p->EndTime() << "\n from\tx = \n" << p->StartPoint();// << "\n\nusing predictor " << p->Get().Predictor();
				}

				else if(auto p = dynamic_cast<const TrackingEnded<ObservedT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "tracking ended";

				else if (auto p = dynamic_cast<const NewStep<ObservedT>*>(&e))
				{
					auto t = p->Get();
					BOOST_LOG_TRIVIAL(severity_level::trace) << "Tracker iteration " << t.NumTotalStepsTaken() << "\ncurrent precision: " << t.CurrentPrecision();

					
					BOOST_LOG_TRIVIAL(severity_level::trace) 
						<< "t = " << t.CurrentTime() 
						<< "\ncurrent stepsize: " << t.CurrentStepsize() 
						<< "\ndelta_t = " << t.DeltaT() << "\ncurrent x = ";
						if (t.CurrentPrecision()==DoublePrecision())
						    BOOST_LOG_TRIVIAL(severity_level::trace) << t.template CurrentPoint<dbl>();
					    else
						    BOOST_LOG_TRIVIAL(severity_level::trace) << t.template CurrentPoint<mpfr>();
				}



				else if (auto p = dynamic_cast<const SingularStartPoint<ObservedT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "singular start point";
				else if (auto p = dynamic_cast<const InfinitePathTruncation<ObservedT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "tracker iteration indicated going to infinity, truncated path";




				else if (auto p = dynamic_cast<const SuccessfulStep<ObservedT>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::trace) << "tracker iteration successful\n\n\n";
				}
				
				else if (auto p = dynamic_cast<const FailedStep<ObservedT>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::trace) << "tracker iteration unsuccessful\n\n\n";
				}




				

				else if (auto p = dynamic_cast<const SuccessfulPredict<ObservedT,mpfr>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::trace) << "prediction successful, result:\n" << p->ResultingPoint();
				}
				else if (auto p = dynamic_cast<const SuccessfulPredict<ObservedT,dbl>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::trace) << "prediction successful, result:\n" << p->ResultingPoint();
				}

				else if (auto p = dynamic_cast<const SuccessfulCorrect<ObservedT,mpfr>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::trace) << "correction successful, result:\n" << p->ResultingPoint();
				}
				else if (auto p = dynamic_cast<const SuccessfulCorrect<ObservedT,dbl>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::trace) << "correction successful, result:\n" << p->ResultingPoint();
				}


				else if (auto p = dynamic_cast<const PredictorHigherPrecisionNecessary<ObservedT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "Predictor, higher precision necessary";
				else if (auto p = dynamic_cast<const CorrectorHigherPrecisionNecessary<ObservedT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "corrector, higher precision necessary";				



				else if (auto p = dynamic_cast<const CorrectorMatrixSolveFailure<ObservedT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "corrector, higher precision necessary";				
				else if (auto p = dynamic_cast<const PredictorMatrixSolveFailure<ObservedT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "predictor, matrix solve failure or failure to converge";	
				else if (auto p = dynamic_cast<const FirstStepPredictorMatrixSolveFailure<ObservedT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "Predictor, matrix solve failure in initial solve of prediction";	

					
				else if (auto p = dynamic_cast<const PrecisionChanged<ObservedT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::debug) << "changing precision from " << p->Previous() << " to " << p->Next();
				
				else
					BOOST_LOG_TRIVIAL(severity_level::debug) << "unlogged event, of type: " << boost::typeindex::type_id_runtime(e).pretty_name();
			}

			virtual void Visit(ObservedT const& t) override
			{}
		};



		template<class ObservedT>
		class StepFailScreenPrinter : public Observer<ObservedT>
		{ BOOST_TYPE_INDEX_REGISTER_CLASS
		public:


			virtual void Observe(AnyEvent const& e) override
			{
				if (auto p = dynamic_cast<const FailedStep<ObservedT>*>(&e))
					std::cout << "observed step failure" << std::endl;
			}

			virtual void Visit(ObservedT const& t) override
			{}
		};

	} //re: namespace tracking

}// re: namespace bertini