//This file is part of Bertini 2.0.
//
//predict.hpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//predict.hpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with predict.hpp.  If not, see <http://www.gnu.org/licenses/>.
//

//  predict.hpp
//
//  copyright 2015
//  Daniel Brake
//  University of Notre Dame
//  ACMS
//  Summer 2015

#ifndef predict_hpp
#define predict_hpp

#include "tracking/ode_predictors.hpp"

namespace bertini{
	namespace tracking{
		namespace predict{

			template <typename T>
			SuccessCode Predict(int predictor_choice)
			{
				switch predictor_choice
				{
					case EULER:
					{
						
					}

					default:
					{

					}
				}

				return SuccessCode::Success;
			}

		}

	}

	
}



#endif
