//This file is part of Bertini 2.
//
//src/function_tree/symbols/number.cpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//src/function_tree/symbols/number.cpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with src/function_tree/symbols/number.cpp.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright(C) 2015 - 2021 by Bertini2 Development Team
//
// See <http://www.gnu.org/licenses/> for a copy of the license, 
// as well as COPYING.  Bertini2 is provided with permitted 
// additional terms in the b2/licenses/ directory.

// individual authors of this file include:
// silviana amethyst, university of notre dame
// Jeb Collins, West Texas A&M


#include "bertini2/function_tree/symbols/number.hpp"




namespace bertini{
	namespace node{
		using ::pow;
		
void Number::Reset() const
{
	// ResetStoredValues();
}


void Number::precision(unsigned int prec) const
{
	auto& val_pair = std::get< std::pair<mpfr_complex,bool> >(current_value_);
	val_pair.first.precision(prec);
	val_pair.second = false; // false indicates to re-evaluate 
}


std::shared_ptr<Node> Number::Differentiate(std::shared_ptr<Variable> const& v) const
{
	return Integer::Make(0);
}

///////////////////
//
//  INTEGERS
//
////////////////////////


void Integer::print(std::ostream & target) const
{
	target << true_value_;
}

// Return value of constant
dbl Integer::FreshEval_d(std::shared_ptr<Variable> const& diff_variable) const
{
	return dbl(double(true_value_),0);
}

void Integer::FreshEval_d(dbl& evaluation_value, std::shared_ptr<Variable> const& diff_variable) const
{
	evaluation_value = dbl(double(true_value_),0);
}


mpfr_complex Integer::FreshEval_mp(std::shared_ptr<Variable> const& diff_variable) const
{
	return mpfr_complex(true_value_,0,DefaultPrecision());
}

void Integer::FreshEval_mp(mpfr_complex& evaluation_value, std::shared_ptr<Variable> const& diff_variable) const
{
	evaluation_value = mpfr_complex(true_value_,0,DefaultPrecision());
}


/////////////
//
//  Floats
//
////////////////


void Float::print(std::ostream & target) const
{
	target << highest_precision_value_;
}

// Return value of constant
dbl Float::FreshEval_d(std::shared_ptr<Variable> const& diff_variable) const
{
	return dbl(highest_precision_value_);
}

void Float::FreshEval_d(dbl& evaluation_value, std::shared_ptr<Variable> const& diff_variable) const
{
	evaluation_value = dbl(highest_precision_value_);
}


mpfr_complex Float::FreshEval_mp(std::shared_ptr<Variable> const& diff_variable) const
{
	return mpfr_complex(highest_precision_value_,DefaultPrecision());
}

void Float::FreshEval_mp(mpfr_complex& evaluation_value, std::shared_ptr<Variable> const& diff_variable) const
{
	evaluation_value = mpfr_complex(highest_precision_value_,DefaultPrecision());
}


//
//  Rational
//



void Rational::print(std::ostream & target) const
{
	target << "(" << true_value_real_ << "," << true_value_imag_ << ")";
}

// Return value of constant
dbl Rational::FreshEval_d(std::shared_ptr<Variable> const& diff_variable) const
{
	return dbl(double(true_value_real_),double(true_value_imag_));
}

void Rational::FreshEval_d(dbl& evaluation_value, std::shared_ptr<Variable> const& diff_variable) const
{
	evaluation_value = dbl(double(true_value_real_),double(true_value_imag_));
}


mpfr_complex Rational::FreshEval_mp(std::shared_ptr<Variable> const& diff_variable) const
{
	return mpfr_complex(boost::multiprecision::mpfr_float(true_value_real_,DefaultPrecision()),boost::multiprecision::mpfr_float(true_value_imag_,DefaultPrecision()));
}

void Rational::FreshEval_mp(mpfr_complex& evaluation_value, std::shared_ptr<Variable> const& diff_variable) const
{
	evaluation_value = mpfr_complex(boost::multiprecision::mpfr_float(true_value_real_,DefaultPrecision()),boost::multiprecision::mpfr_float(true_value_imag_,DefaultPrecision()));
}


	} // re: namespace node
} // re: namespace bertini
