//This file is part of Bertini 2.0.
//
//mpfr_extensions.hpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//mpfr_extensions.hpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with .  If not, see <http://www.gnu.org/licenses/>.
//
//  Daniel Brake
//  University of Notre Dame
//  ACMS
//  Spring, Summer 2015
//
//  This file contains the bertini::NumTraits struct, which extends the Eigen NumTraits struct
//
//

/**
\file num_traits.hpp 

\brief Provides an Eigen-like NumTraits struct for querying traits of a number type.  

The bertini::NumTraits struct provides NumDigits and NumFuzzyDigits functions.
*/

#ifndef BERTINI_NUM_TRAITS_HPP
#define BERTINI_NUM_TRAITS_HPP


#include <eigen3/Eigen/Core>

namespace bertini
{

	using dbl = std::complex<double>;


	template<typename T>
	struct NumTraits : public Eigen::NumTraits<T>
	{};



	template <> struct NumTraits<double> 
	{
		inline static unsigned NumDigits()
		{
			return 16;
		}

		inline static unsigned NumFuzzyDigits()
		{
			return 14;
		}
	};


	template <> struct NumTraits<std::complex<double> > 
	{
		inline static unsigned NumDigits()
		{
			return 16;
		}

		inline static unsigned NumFuzzyDigits()
		{
			return 14;
		}
	};

	/**
	\brief Get the precision of a number.

	For doubles, this is trivially 16.
	*/
	inline
	unsigned Precision(const double num)
	{
		return 16;
	}

	/**
	\brief Get the precision of a number.

	For complex doubles, this is trivially 16.
	*/
	inline
	unsigned Precision(const std::complex<double> num)
	{
		return 16;
	}



	enum 
	{
		PrecisionIncrement = 10,
		DoublePrecision = 16,
		LowestMultiplePrecision = 30,
		MaxPrecisionAllowed = 1000
	};


}// re: namespace bertini


#endif


