//This file is part of Bertini 2.
//
//homogenization_test.cpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//homogenization_test.cpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with homogenization_test.cpp.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright(C) 2015 - 2021 by Bertini2 Development Team
//
// See <http://www.gnu.org/licenses/> for a copy of the license, 
// as well as COPYING.  Bertini2 is provided with permitted 
// additional terms in the b2/licenses/ directory.

// individual authors of this file include:
// silviana amethyst, university of wisconsin eau claire

//  homogenization_test.cpp



#include <boost/test/unit_test.hpp>



#include "bertini2/system/system.hpp"

#include "externs.hpp"


BOOST_AUTO_TEST_SUITE(homogenization)

using Variable = bertini::node::Variable;
using Float = bertini::node::Float;
using mpfr_float = bertini::mpfr_float;
using Var = std::shared_ptr<bertini::node::Variable>;

using Flt = std::shared_ptr<bertini::node::Float>;
using VariableGroup = bertini::VariableGroup;
using dbl = bertini::dbl;
using mpfr = bertini::mpfr_complex;





BOOST_AUTO_TEST_CASE(no_homogenization_needed_x)
{
	Var x = Variable::Make("x");
	Var h = Variable::Make("h");

	auto f1 = x;
	
	BOOST_CHECK(f1->IsHomogeneous());

	VariableGroup vars;
	vars.push_back(x);

	f1->Homogenize(vars,h);
	BOOST_CHECK_EQUAL(f1->Degree(h), 0);
	BOOST_CHECK(f1->IsHomogeneous());
	BOOST_CHECK( f1->IsHomogeneous(x));
	BOOST_CHECK( f1->IsHomogeneous(h));

	vars.push_back(h);
	BOOST_CHECK(f1->IsHomogeneous(vars));


	BOOST_CHECK(f1->IsPolynomial());
}




BOOST_AUTO_TEST_CASE(homogenization_needed_x_minus_1)
{
	Var x = Variable::Make("x");
	Var t = Variable::Make("t");
	Var h = Variable::Make("h");

	auto f1 = x-1;
	
	BOOST_CHECK(!f1->IsHomogeneous());

	VariableGroup vars;
	vars.push_back(x);

	f1->Homogenize(vars,h);
	BOOST_CHECK_EQUAL(f1->Degree(h), 1);

	BOOST_CHECK(f1->IsHomogeneous());
	BOOST_CHECK(!f1->IsHomogeneous(x));
	BOOST_CHECK(!f1->IsHomogeneous(h));


	vars.push_back(h);
	BOOST_CHECK(f1->IsHomogeneous(vars));

	BOOST_CHECK(f1->IsPolynomial());
}



BOOST_AUTO_TEST_CASE(homogenization_needed_1_minus_t_x_plus_t_1_minus_x)
{
	std::shared_ptr<bertini::node::Variable> x = Variable::Make("x");
	std::shared_ptr<bertini::node::Variable> t = Variable::Make("t");
	auto f1 = (1-t)*x + t*(1-x);

	BOOST_CHECK(!f1->IsHomogeneous());

	Var h = Variable::Make("h");
	VariableGroup vars;
	vars.push_back(x);

	f1->Homogenize(vars,h);
	BOOST_CHECK_EQUAL(f1->Degree(h), 1);
	BOOST_CHECK(!f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsHomogeneous(x));
	BOOST_CHECK(!f1->IsHomogeneous(h));
	BOOST_CHECK(!f1->IsHomogeneous(t));

	vars.push_back(h);
	BOOST_CHECK(f1->IsHomogeneous(vars));

	BOOST_CHECK(f1->IsPolynomial());
}



BOOST_AUTO_TEST_CASE(homogenization_needed_x_minus_t)
{
	Var x = Variable::Make("x");
	Var t = Variable::Make("t");
	Var h = Variable::Make("h");

	auto f1 = x-t;
	
	BOOST_CHECK(f1->IsHomogeneous());

	VariableGroup vars;
	vars.push_back(x);

	f1->Homogenize(vars,h);
	BOOST_CHECK_EQUAL(f1->Degree(h), 1);
	BOOST_CHECK(!f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsHomogeneous(x));
	BOOST_CHECK(!f1->IsHomogeneous(h));
	BOOST_CHECK(!f1->IsHomogeneous(t));

	vars.push_back(h);
	BOOST_CHECK(f1->IsHomogeneous(vars));

	BOOST_CHECK(f1->IsPolynomial());
}



BOOST_AUTO_TEST_CASE(no_homogenization_needed_x_minus_y_t)
{
	Var x = Variable::Make("x");
	Var y = Variable::Make("y");
	Var t = Variable::Make("t");
	Var h = Variable::Make("h");

	auto f1 = x-y*t;
	
	BOOST_CHECK(!f1->IsHomogeneous());

	VariableGroup vars;
	vars.push_back(x);
	vars.push_back(y);

	f1->Homogenize(vars,h);
	BOOST_CHECK_EQUAL(f1->Degree(h), 0);
	BOOST_CHECK(!f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsHomogeneous(x));
	BOOST_CHECK( f1->IsHomogeneous(h));
	BOOST_CHECK(!f1->IsHomogeneous(t));

	vars.push_back(h);
	BOOST_CHECK(f1->IsHomogeneous(vars));

	BOOST_CHECK(f1->IsPolynomial());
}






BOOST_AUTO_TEST_CASE(homogenization_needed_sphere)
{
	Var x = Variable::Make("x");
	Var y = Variable::Make("y");
	Var z = Variable::Make("z");
	Var h = Variable::Make("h");

	auto f1 = pow(x,2) + pow(y,2) + pow(z,2)-1;
	
	BOOST_CHECK(!f1->IsHomogeneous());

	VariableGroup vars;
	vars.push_back(x);
	vars.push_back(y);
	vars.push_back(z);

	f1->Homogenize(vars,h);
	BOOST_CHECK_EQUAL(f1->Degree(h), 2);
	BOOST_CHECK(f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsHomogeneous(x));
	BOOST_CHECK(!f1->IsHomogeneous(y));
	BOOST_CHECK(!f1->IsHomogeneous(z));
	BOOST_CHECK(!f1->IsHomogeneous(h));


	vars.push_back(h);
	BOOST_CHECK(f1->IsHomogeneous(vars));

	BOOST_CHECK(f1->IsPolynomial());
}




BOOST_AUTO_TEST_CASE(homogenization_needed_quadric)
{
	Var x = Variable::Make("x");
	Var y = Variable::Make("y");
	Var z = Variable::Make("z");
	Var h = Variable::Make("h");

	auto f1 = x*y+x*z+y*z-1;
	BOOST_CHECK(!f1->IsHomogeneous());

	VariableGroup vars;
	vars.push_back(x);
	vars.push_back(y);
	vars.push_back(z);

	f1->Homogenize(vars,h);
	BOOST_CHECK_EQUAL(f1->Degree(h), 2);
	BOOST_CHECK(f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsHomogeneous(x));
	BOOST_CHECK(!f1->IsHomogeneous(y));
	BOOST_CHECK(!f1->IsHomogeneous(z));
	BOOST_CHECK(!f1->IsHomogeneous(h));

	vars.push_back(h);
	BOOST_CHECK(f1->IsHomogeneous(vars));


	BOOST_CHECK(f1->IsPolynomial());
}	





BOOST_AUTO_TEST_CASE(homogenization_needed_quadratic)
{
	Var x = Variable::Make("x");
	Var h = Variable::Make("h");
	

	auto f1 = pow(x,2) + x + 1;
	
	BOOST_CHECK(!f1->IsHomogeneous());

	VariableGroup vars;
	vars.push_back(x);

	f1->Homogenize(vars,h);
	BOOST_CHECK_EQUAL(f1->Degree(h), 2);
	BOOST_CHECK(f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsHomogeneous(x));
	BOOST_CHECK(!f1->IsHomogeneous(h));

	vars.push_back(h);
	BOOST_CHECK(f1->IsHomogeneous(vars));

	BOOST_CHECK(f1->IsPolynomial());
}



BOOST_AUTO_TEST_CASE(homogenization_needed_quadratic_no_constant)
{
	Var x = Variable::Make("x");
	Var h = Variable::Make("h");
	

	auto f1 = pow(x,2) + x;
	
	BOOST_CHECK(!f1->IsHomogeneous());

	VariableGroup vars;
	vars.push_back(x);

	f1->Homogenize(vars,h);
	BOOST_CHECK_EQUAL(f1->Degree(h), 1);
	BOOST_CHECK(f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsHomogeneous(x));
	BOOST_CHECK(!f1->IsHomogeneous(h));

	vars.push_back(h);
	BOOST_CHECK(f1->IsHomogeneous(vars));

	BOOST_CHECK(f1->IsPolynomial());
}



BOOST_AUTO_TEST_CASE(homogenization_needed_quadratic_no_constant_wrt_y)
{
	Var x = Variable::Make("x");
	Var y = Variable::Make("y");
	Var h = Variable::Make("h");
	

	auto f1 = pow(x,2) + x;
	
	BOOST_CHECK(!f1->IsHomogeneous());

	VariableGroup vars;
	vars.push_back(y);

	f1->Homogenize(vars,h);
	BOOST_CHECK_EQUAL(f1->Degree(h), 0);
	BOOST_CHECK(!f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsHomogeneous(x));
	BOOST_CHECK( f1->IsHomogeneous(h));
	BOOST_CHECK( f1->IsHomogeneous(y));

	vars.push_back(h);
	BOOST_CHECK(f1->IsHomogeneous(vars));

	BOOST_CHECK(f1->IsPolynomial());
}





BOOST_AUTO_TEST_CASE(nothomogeneous_sin_x)
{
	Var x = Variable::Make("x");

	auto f1 = sin(x);
	
	BOOST_CHECK(!f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsPolynomial());
}



BOOST_AUTO_TEST_CASE(nothomogeneous_cos_x)
{
	Var x = Variable::Make("x");

	auto f1 = cos(x);
	
	BOOST_CHECK(!f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsPolynomial());
}

BOOST_AUTO_TEST_CASE(nothomogeneous_tan_x)
{
	Var x = Variable::Make("x");

	auto f1 = tan(x);
	
	BOOST_CHECK(!f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsPolynomial());
}



BOOST_AUTO_TEST_CASE(nothomogeneous_exp_x)
{
	Var x = Variable::Make("x");

	auto f1 = exp(x);
	
	BOOST_CHECK(!f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsPolynomial());
}


BOOST_AUTO_TEST_CASE(nothomogeneous_sqrt_x)
{
	Var x = Variable::Make("x");

	auto f1 = sqrt(x);
	
	BOOST_CHECK(!f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsPolynomial());
}



BOOST_AUTO_TEST_CASE(is_homogeneous_sin_0)
{
	Flt n = Float::Make("1");

	auto f1 = sin(n);
	
	BOOST_CHECK(f1->IsHomogeneous());

	BOOST_CHECK(f1->IsPolynomial());
}

BOOST_AUTO_TEST_CASE(is_homogeneous_cos_1)
{
	Flt n = Float::Make("1");

	auto f1 = cos(n);
	
	BOOST_CHECK(f1->IsHomogeneous());

	BOOST_CHECK(f1->IsPolynomial());
}

BOOST_AUTO_TEST_CASE(is_homogeneous_sin_1_plus_1)
{
	Flt n = Float::Make("1");

	auto f1 = sin(n + n);
	
	BOOST_CHECK(f1->IsHomogeneous());

	BOOST_CHECK(f1->IsPolynomial());
}




BOOST_AUTO_TEST_CASE(is_homogeneous_summands_homogeneous)
{
	Var x = Variable::Make("x");
	Var y = Variable::Make("y");

	auto a = pow(x,3) / 2;
	auto b = pow(x,2) * mpfr_float("4.12331") * pow(x,1);
	
	auto f1 = a+b;
	BOOST_CHECK(f1->IsHomogeneous());

	BOOST_CHECK(f1->IsHomogeneous(x));
	BOOST_CHECK(f1->IsHomogeneous(y));


	BOOST_CHECK(f1->IsPolynomial());

}

BOOST_AUTO_TEST_CASE(not_homogeneous_summands_inhomogeneous)
{
	Var x = Variable::Make("x");
	Var y = Variable::Make("y");

	auto a = pow(x,3) / 2;
	auto b = pow(x,2) * mpfr_float("4.12331");
	
	auto f1 = a+b;
	BOOST_CHECK(!f1->IsHomogeneous());

	BOOST_CHECK(!f1->IsHomogeneous(x));
	BOOST_CHECK( f1->IsHomogeneous(y));

	BOOST_CHECK(f1->IsPolynomial());
}



BOOST_AUTO_TEST_CASE(linprod_ishom)
{
	using bertini::VariableGroup;
	using namespace bertini::node;
	
	auto x = Variable::Make("x");
	auto y = Variable::Make("y");
	auto z = Variable::Make("z");
	auto w = Variable::Make("w");
	
	
	
	VariableGroup v1{x,w};
	VariableGroup v2{y};
	VariableGroup v3{z};
	VariableGroup hom{x,w,z};
	
	std::shared_ptr<Node> linprod_node = (2 + 3*x + 2*w) * (4 - 2*x - 6*w)*(4+8*y);
	std::shared_ptr<Node> linprod = LinearProduct::Make(v1,2)*LinearProduct::Make(v2,1);
	
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(v1), linprod_node->IsHomogeneous(v1));
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(v2), linprod_node->IsHomogeneous(v2));
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(x), linprod_node->IsHomogeneous(x));
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(y), linprod_node->IsHomogeneous(y));
	
	linprod_node->Homogenize(v1, z);
	linprod->Homogenize(v1,z);
	VariableGroup badgp{x,y};
	
	
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(v1), linprod_node->IsHomogeneous(v1));
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(v2), linprod_node->IsHomogeneous(v2));
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(x), linprod_node->IsHomogeneous(x));
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(y), linprod_node->IsHomogeneous(y));
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(badgp), linprod_node->IsHomogeneous(badgp));
	
    linprod = LinearProduct::Make(v1,2)*LinearProduct::Make(v2,1);
    BOOST_CHECK_THROW(linprod->Homogenize(badgp,y), std::exception);
	
	linprod_node = (2 + 3*x + 2*w) * (4 - 2*x - 6*w)*(4+8*y);
	linprod = LinearProduct::Make(v1,2)*LinearProduct::Make(v2,1);
	
	BOOST_CHECK_THROW(linprod->Homogenize(badgp,y), std::exception);
	
	linprod = LinearProduct::Make(v1,2)*LinearProduct::Make(v2,1);
	
	linprod_node->Homogenize(hom, z);
	linprod->Homogenize(hom,z);
	
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(v1), linprod_node->IsHomogeneous(v1));
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(v2), linprod_node->IsHomogeneous(v2));
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(x), linprod_node->IsHomogeneous(x));
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(y), linprod_node->IsHomogeneous(y));
	BOOST_CHECK_EQUAL(linprod->IsHomogeneous(badgp), linprod_node->IsHomogeneous(badgp));
	
	
	
}



BOOST_AUTO_TEST_CASE(linprod_hom_eval)
{
	using bertini::VariableGroup;
	using namespace bertini::node;
	
	auto x = Variable::Make("x");
	auto y = Variable::Make("y");
	auto h0 = Variable::Make("HOM0");
	auto h1 = Variable::Make("HOM1");
	auto z = Variable::Make("z");
	
	
	
	VariableGroup v0{x,z};
	VariableGroup v1{y};
	Mat<dbl> coeff_dbl(2,3);
	Mat<mpfr> coeff_mpfr(2,3);
	
	for(int ii = 0; ii < 2; ++ii)
	{
		for(int jj = 0; jj < 3; ++jj)
		{
			coeff_dbl(ii,jj) = dbl(ii+1, jj+1);
			coeff_mpfr(ii,jj) = mpfr(ii+1, jj+1);
		}
	}
	
	std::shared_ptr<bertini::node::Node>linprod1 = bertini::node::LinearProduct::Make(v0, coeff_mpfr);
	
	coeff_dbl = Mat<dbl>(1,2);
	coeff_mpfr = Mat<mpfr>(1,2);
	
	for(int ii = 0; ii < 1; ++ii)
	{
		for(int jj = 0; jj < 2; ++jj)
		{
			coeff_dbl(ii,jj) = dbl(ii+3, jj+3);
			coeff_mpfr(ii,jj) = mpfr(ii+3, jj+3);
		}
	}
	
	std::shared_ptr<bertini::node::Node> linprod2 = bertini::node::LinearProduct::Make(v1, coeff_mpfr);
	
	std::shared_ptr<bertini::node::Node> linprod_node = (mpfr(1,1)*x + mpfr(1,2)*z + mpfr(1,3)) * (mpfr(2,1)*x + mpfr(2,2)*z+ mpfr(2,3))* (mpfr(3,3)*y + mpfr(3,4));
	std::shared_ptr<bertini::node::Node> linprod = linprod1*linprod2;
	
	dbl xval_d = dbl(.5,1);
	dbl yval_d = dbl(.6,1);
	dbl zval_d = dbl(.7,1);
	dbl h0val_d = dbl(.34, -2.1);
	dbl h1val_d = dbl(-1.2, .0043);
	mpfr xval_mp = mpfr(".5", "1");
	mpfr yval_mp = mpfr(".6", "1");
	mpfr zval_mp = mpfr(".7", "1");
	mpfr h0val_mp = mpfr(".34", "-2.1");
	mpfr h1val_mp = mpfr("-1.2", ".0043");
	
	v0[0]->set_current_value(xval_d);
	v0[1]->set_current_value(zval_d);
	v1[0]->set_current_value(yval_d);
	v0[0]->set_current_value(xval_mp);
	v0[1]->set_current_value(zval_mp);
	v1[0]->set_current_value(yval_mp);
	
	h0->set_current_value(h0val_d);
	h1->set_current_value(h1val_d);
	h0->set_current_value(h0val_mp);
	h1->set_current_value(h1val_mp);
	
	
	linprod_node->Homogenize(v0,h0);
	linprod->Homogenize(v0,h0);
	
	dbl eval_d = linprod->Eval<dbl>();
	dbl exact_d = linprod_node->Eval<dbl>();
	mpfr eval_mp = linprod->Eval<mpfr>();
	mpfr exact_mp = linprod_node->Eval<mpfr>();
	
	BOOST_CHECK(fabs(eval_d.real()/exact_d.real() - 1) < threshold_clearance_d);
	BOOST_CHECK(fabs(eval_d.imag()/exact_d.imag() - 1) < threshold_clearance_d);
	BOOST_CHECK(fabs(eval_mp.real()/exact_mp.real() - 1) < threshold_clearance_mp);
	BOOST_CHECK(fabs(eval_mp.imag()/exact_mp.imag() - 1) < threshold_clearance_mp);
	
	linprod_node->Homogenize(v1,h1);
	linprod->Homogenize(v1,h1);
	
	eval_d = linprod->Eval<dbl>();
	exact_d = linprod_node->Eval<dbl>();
	eval_mp = linprod->Eval<mpfr>();
	exact_mp = linprod_node->Eval<mpfr>();
	
	BOOST_CHECK(fabs(eval_d.real()/exact_d.real() - 1) < threshold_clearance_d);
	BOOST_CHECK(fabs(eval_d.imag()/exact_d.imag() - 1) < threshold_clearance_d);
	BOOST_CHECK(fabs(eval_mp.real()/exact_mp.real() - 1) < threshold_clearance_mp);
	BOOST_CHECK(fabs(eval_mp.imag()/exact_mp.imag() - 1) < threshold_clearance_mp);
	
	
}

BOOST_AUTO_TEST_CASE(linprod_hom_eval_homvars)
{
	using bertini::VariableGroup;
	using namespace bertini::node;
	
	auto x = Variable::Make("x");
	auto y = Variable::Make("y");
	auto h0 = Variable::Make("HOM0");
	auto h1 = Variable::Make("HOM1");
	auto z = Variable::Make("z");
	
	
	
	VariableGroup v0{x,z};
	VariableGroup v1{y};
	Mat<dbl> coeff_dbl(2,3);
	Mat<mpfr> coeff_mpfr(2,3);
	
	for(int ii = 0; ii < 2; ++ii)
	{
		for(int jj = 0; jj < 3; ++jj)
		{
			coeff_dbl(ii,jj) = dbl(ii+1, jj+1);
			coeff_mpfr(ii,jj) = mpfr(ii+1, jj+1);
		}
	}
	
	std::shared_ptr<bertini::node::Node>linprod1 = bertini::node::LinearProduct::Make(v0, coeff_mpfr, true);
	
	coeff_dbl = Mat<dbl>(1,2);
	coeff_mpfr = Mat<mpfr>(1,2);
	
	for(int ii = 0; ii < 1; ++ii)
	{
		for(int jj = 0; jj < 2; ++jj)
		{
			coeff_dbl(ii,jj) = dbl(ii+3, jj+3);
			coeff_mpfr(ii,jj) = mpfr(ii+3, jj+3);
		}
	}
	
	std::shared_ptr<bertini::node::Node> linprod2 = bertini::node::LinearProduct::Make(v1, coeff_mpfr, true);
	
	std::shared_ptr<bertini::node::Node> linprod_node = (mpfr(1,1)*x + mpfr(1,2)*z) * (mpfr(2,1)*x + mpfr(2,2)*z)* (mpfr(3,3)*y);
	std::shared_ptr<bertini::node::Node> linprod = linprod1*linprod2;
	
	dbl xval_d = dbl(.5,1);
	dbl yval_d = dbl(.6,1);
	dbl zval_d = dbl(.7,1);
	dbl h0val_d = dbl(.34, -2.1);
	dbl h1val_d = dbl(-1.2, .0043);
	mpfr xval_mp = mpfr(".5", "1");
	mpfr yval_mp = mpfr(".6", "1");
	mpfr zval_mp = mpfr(".7", "1");
	mpfr h0val_mp = mpfr(".34", "-2.1");
	mpfr h1val_mp = mpfr("-1.2", ".0043");
	
	v0[0]->set_current_value(xval_d);
	v0[1]->set_current_value(zval_d);
	v1[0]->set_current_value(yval_d);
	v0[0]->set_current_value(xval_mp);
	v0[1]->set_current_value(zval_mp);
	v1[0]->set_current_value(yval_mp);
	
	h0->set_current_value(h0val_d);
	h1->set_current_value(h1val_d);
	h0->set_current_value(h0val_mp);
	h1->set_current_value(h1val_mp);
	
	
	linprod_node->Homogenize(v0,h0);
	linprod->Homogenize(v0,h0);
	
	dbl eval_d = linprod->Eval<dbl>();
	dbl exact_d = linprod_node->Eval<dbl>();
	mpfr eval_mp = linprod->Eval<mpfr>();
	mpfr exact_mp = linprod_node->Eval<mpfr>();
	
	BOOST_CHECK(fabs(eval_d.real()/exact_d.real() - 1) < threshold_clearance_d);
	BOOST_CHECK(fabs(eval_d.imag()/exact_d.imag() - 1) < threshold_clearance_d);
	BOOST_CHECK(fabs(eval_mp.real()/exact_mp.real() - 1) < threshold_clearance_mp);
	BOOST_CHECK(fabs(eval_mp.imag()/exact_mp.imag() - 1) < threshold_clearance_mp);
	
	linprod_node->Homogenize(v1,h1);
	linprod->Homogenize(v1,h1);
	
	eval_d = linprod->Eval<dbl>();
	exact_d = linprod_node->Eval<dbl>();
	eval_mp = linprod->Eval<mpfr>();
	exact_mp = linprod_node->Eval<mpfr>();
	
	BOOST_CHECK(fabs(eval_d.real()/exact_d.real() - 1) < threshold_clearance_d);
	BOOST_CHECK(fabs(eval_d.imag()/exact_d.imag() - 1) < threshold_clearance_d);
	BOOST_CHECK(fabs(eval_mp.real()/exact_mp.real() - 1) < threshold_clearance_mp);
	BOOST_CHECK(fabs(eval_mp.imag()/exact_mp.imag() - 1) < threshold_clearance_mp);
	
	
}



BOOST_AUTO_TEST_CASE(linprod_hom_diff_eval)
{
    using bertini::VariableGroup;
    using namespace bertini::node;
    
    auto x = Variable::Make("x");
    auto y = Variable::Make("y");
    auto h0 = Variable::Make("HOM0");
    auto h1 = Variable::Make("HOM1");
    auto z = Variable::Make("z");
    auto w = Variable::Make("w");
    
    
    
    VariableGroup v0{x,z,y};
    VariableGroup v1{w};
    int num_factors = 3;
    int num_variables = v0.size();
    Mat<dbl> coeff_dbl(num_factors,num_variables+1);
    Mat<mpfr> coeff_mpfr(num_factors,num_variables+1);
    
    for(int ii = 0; ii < num_factors; ++ii)
    {
        for(int jj = 0; jj < num_variables+1; ++jj)
        {
            coeff_dbl(ii,jj) = dbl(ii+1, jj+1);
            coeff_mpfr(ii,jj) = mpfr(ii+1, jj+1);
        }
    }
    
    std::shared_ptr<bertini::node::Node> linprod1 = bertini::node::LinearProduct::Make(v0, coeff_mpfr);
    
    num_factors = 1;
    num_variables = v1.size();
    coeff_dbl = Mat<dbl>(num_factors,num_variables+1);
    coeff_mpfr = Mat<mpfr>(num_factors,num_variables+1);
    
    for(int ii = 0; ii < num_factors; ++ii)
    {
        for(int jj = 0; jj < num_variables+1; ++jj)
        {
            coeff_dbl(ii,jj) = dbl(ii+3, jj+3);
            coeff_mpfr(ii,jj) = mpfr(ii+3, jj+3);
        }
    }
    
    
    std::shared_ptr<bertini::node::Node> linprod2 = bertini::node::LinearProduct::Make(v1, coeff_mpfr);
    
    std::shared_ptr<bertini::node::Node> linprod1_node = (mpfr(1,1)*x + mpfr(1,2)*z + mpfr(1,3)*y+ mpfr(1,4)) * (mpfr(2,1)*x + mpfr(2,2)*z + mpfr(2,3)*y+ mpfr(2,4))*(mpfr(3,1)*x + mpfr(3,2)*z + mpfr(3,3)*y+ mpfr(3,4));
    std::shared_ptr<bertini::node::Node> linprod2_node = (mpfr(3,3)*w + mpfr(3,4));
    
    std::shared_ptr<bertini::node::Node> linprod_node = linprod1_node*linprod2_node;
    std::shared_ptr<bertini::node::Node> linprod = linprod1*linprod2;
    
    
    dbl xval_d = dbl(.5,1);
    dbl yval_d = dbl(.6,1);
    dbl zval_d = dbl(.7,1);
    dbl wval_d = dbl(2.1, -.03);
    dbl h0val_d = dbl(.34, -2.1);
    dbl h1val_d = dbl(-1.2, .0043);
    mpfr xval_mp = mpfr(".5", "1");
    mpfr yval_mp = mpfr(".6", "1");
    mpfr zval_mp = mpfr(".7", "1");
    mpfr wval_mp = mpfr("2.1", "-.03");
    mpfr h0val_mp = mpfr(".34", "-2.1");
    mpfr h1val_mp = mpfr("-1.2", ".0043");
    
    v0[0]->set_current_value(xval_d);
    v0[1]->set_current_value(zval_d);
    v0[2]->set_current_value(yval_d);
    v0[0]->set_current_value(xval_mp);
    v0[1]->set_current_value(zval_mp);
    v0[2]->set_current_value(yval_mp);
    
    v1[0]->set_current_value(wval_d);
    v1[0]->set_current_value(wval_mp);
    
    h0->set_current_value(h0val_d);
    h1->set_current_value(h1val_d);
    h0->set_current_value(h0val_mp);
    h1->set_current_value(h1val_mp);
    
    linprod_node->Homogenize(v0,h0);
    linprod->Homogenize(v0,h0);
    
    linprod_node->Homogenize(v1,h1);
    linprod->Homogenize(v1,h1);
    
    
    auto J_node = bertini::node::Jacobian::Make(linprod_node->Differentiate());
    auto J = bertini::node::Jacobian::Make(linprod->Differentiate());
    
    
    dbl evalx_d = J->EvalJ<dbl>(x);
    dbl exactx_d = J_node->EvalJ<dbl>(x);
    mpfr evalx_mp = J->EvalJ<mpfr>(x);
    mpfr exactx_mp = J_node->EvalJ<mpfr>(x);
    
    
    
    BOOST_CHECK(fabs(evalx_d.real()/exactx_d.real() - 1) < threshold_clearance_d);
    BOOST_CHECK(fabs(evalx_d.imag()/exactx_d.imag() - 1) < threshold_clearance_d);
    BOOST_CHECK(fabs(evalx_mp.real()/exactx_mp.real() - 1) < threshold_clearance_mp);
    BOOST_CHECK(fabs(evalx_mp.imag()/exactx_mp.imag() - 1) < threshold_clearance_mp);
    
    
    evalx_d = J->EvalJ<dbl>(z);
    exactx_d = J_node->EvalJ<dbl>(z);
    evalx_mp = J->EvalJ<mpfr>(z);
    exactx_mp = J_node->EvalJ<mpfr>(z);
    
    
    
    BOOST_CHECK(fabs(evalx_d.real()/exactx_d.real() - 1) < threshold_clearance_d);
    BOOST_CHECK(fabs(evalx_d.imag()/exactx_d.imag() - 1) < threshold_clearance_d);
    BOOST_CHECK(fabs(evalx_mp.real()/exactx_mp.real() - 1) < threshold_clearance_mp);
    BOOST_CHECK(fabs(evalx_mp.imag()/exactx_mp.imag() - 1) < threshold_clearance_mp);
    
    
    evalx_d = J->EvalJ<dbl>(y);
    exactx_d = J_node->EvalJ<dbl>(y);
    evalx_mp = J->EvalJ<mpfr>(y);
    exactx_mp = J_node->EvalJ<mpfr>(y);
    
    
    BOOST_CHECK(fabs(evalx_d.real()/exactx_d.real() - 1) < threshold_clearance_d);
    BOOST_CHECK(fabs(evalx_d.imag()/exactx_d.imag() - 1) < relaxed_threshold_clearance_d);
    BOOST_CHECK(fabs(evalx_mp.real()/exactx_mp.real() - 1) < threshold_clearance_mp);
    BOOST_CHECK(fabs(evalx_mp.imag()/exactx_mp.imag() - 1) < threshold_clearance_mp);
    
    
    evalx_d = J->EvalJ<dbl>(w);
    exactx_d = J_node->EvalJ<dbl>(w);
    evalx_mp = J->EvalJ<mpfr>(w);
    exactx_mp = J_node->EvalJ<mpfr>(w);
    
    
    
    BOOST_CHECK(fabs(evalx_d.real()/exactx_d.real() - 1) < threshold_clearance_d);
    BOOST_CHECK(fabs(evalx_d.imag()/exactx_d.imag() - 1) < threshold_clearance_d);
    BOOST_CHECK(fabs(evalx_mp.real()/exactx_mp.real() - 1) < threshold_clearance_mp);
    BOOST_CHECK(fabs(evalx_mp.imag()/exactx_mp.imag() - 1) < threshold_clearance_mp);
    
}




BOOST_AUTO_TEST_SUITE_END()





