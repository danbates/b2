//This file is part of Bertini 2.0.
//
//tracker_test.cpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//tracker_test.cpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with tracker_test.cpp.  If not, see <http://www.gnu.org/licenses/>.
//

//  copyright 2015
//  Daniel Brake
//  University of Notre Dame
//  ACMS
//  Fall 2015

//tracker_test.cpp
//


#include <boost/test/unit_test.hpp>
#include "start_system.hpp"
#include "tracking/fixed_precision_tracker.hpp"

using System = bertini::System;
using Variable = bertini::node::Variable;

using Var = std::shared_ptr<Variable>;

using VariableGroup = bertini::VariableGroup;


using dbl = std::complex<double>;
using mpfr = bertini::complex;
using mpfr_float = bertini::mpfr_float;


template<typename NumType> using Vec = bertini::Vec<NumType>;
template<typename NumType> using Mat = bertini::Mat<NumType>;

extern double threshold_clearance_d;
extern bertini::mpfr_float threshold_clearance_mp;
extern unsigned TRACKING_TEST_MPFR_DEFAULT_DIGITS;



BOOST_AUTO_TEST_SUITE(fixed_precision_tracker_basics)



BOOST_AUTO_TEST_CASE(double_tracker_track_linear)
{
	mpfr_float::default_precision(100);
	using namespace bertini::tracking;

	Var y = std::make_shared<Variable>("y");
	Var t = std::make_shared<Variable>("t");

	System sys;

	VariableGroup v{y};

	sys.AddFunction(y-t);
	sys.AddPathVariable(t);
	sys.AddVariableGroup(v);


	bertini::tracking::DoublePrecisionTracker tracker(sys);


	config::Stepping<double> stepping_preferences;
	config::Newton newton_preferences;


	tracker.Setup(config::Predictor::Euler,
	              double(1e-5),
					double(1e5),
					stepping_preferences,
					newton_preferences);

	
	dbl t_start(1);
	dbl t_end(0);
	
	Vec<dbl> y_start(1);
	y_start << dbl(1);

	Vec<dbl> y_end;

	tracker.TrackPath(y_end,
	                  t_start, t_end, y_start);

	BOOST_CHECK_EQUAL(y_end.size(),1);
	BOOST_CHECK(abs(y_end(0)-dbl(0)) < 1e-5);

}

BOOST_AUTO_TEST_SUITE_END()



