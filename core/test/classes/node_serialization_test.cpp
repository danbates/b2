//This file is part of Bertini 2.
//
//node_serialization.cpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//node_serialization.cpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with node_serialization.cpp.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright(C) 2015 - 2017 by Bertini2 Development Team
//
// See <http://www.gnu.org/licenses/> for a copy of the license, 
// as well as COPYING.  Bertini2 is provided with permitted 
// additional terms in the b2/licenses/ directory.

// individual authors of this file include:
// silviana amethyst, university of wisconsin eau claire

//  node_serialization.cpp
//
//  Created by Collins, James B. on 4/30/15.
//  Copyright (c) 2015 West Texas A&M University. All rights reserved.


#include <iostream>

#include <cstdlib>
#include <cmath>



#include <boost/spirit/include/qi.hpp>
#include <boost/test/unit_test.hpp>

#include <fstream>

#include "bertini2/function_tree.hpp"
#include "bertini2/system/system.hpp"
#include "bertini2/io/parsing/system_parsers.hpp"
#include "bertini2/system/precon.hpp"



#include "externs.hpp"



BOOST_AUTO_TEST_SUITE(node_serialization)

template<typename NumType> using Vec = bertini::Vec<NumType>;
template<typename NumType> using Mat = bertini::Mat<NumType>;
using Variable = bertini::node::Variable;
using Node = bertini::node::Node;
using Float = bertini::node::Float;


using dbl = bertini::dbl;
using mpfr = bertini::mpfr_complex;

using System = bertini::System;


BOOST_AUTO_TEST_CASE(serialize_variable)
{
	std::shared_ptr<Variable> x = Variable::Make("x");


	{
		std::ofstream fout("serialization_test_node");
		
		boost::archive::text_oarchive oa(fout);
		
		// write class instance to archive
		oa << x;
	}
	
	std::shared_ptr<Variable> x2;
	{
		std::ifstream fin("serialization_test_node");
		
		boost::archive::text_iarchive ia(fin);
		// read class state from archive
		ia >> x2;
	}

	BOOST_CHECK(x->name()==x2->name());
}


BOOST_AUTO_TEST_CASE(serialize_float)
{
	std::shared_ptr<Float> two_point_oh_four = Float::Make("2.04");

	{
		std::ofstream fout("serialization_test_node");
		
		boost::archive::text_oarchive oa(fout);
		
		// write class instance to archive
		oa << two_point_oh_four;
	}
	
	std::shared_ptr<Float> two_point_oh_four2;
	{
		std::ifstream fin("serialization_test_node");
		
		boost::archive::text_iarchive ia(fin);
		// read class state from archive
		ia >> two_point_oh_four2;
	}

	BOOST_CHECK(two_point_oh_four->Eval<dbl>()==two_point_oh_four2->Eval<dbl>());
}

BOOST_AUTO_TEST_CASE(serialize_complicated_expression)
{
	std::shared_ptr<Variable> x = Variable::Make("x");

	auto f = exp(sqrt(pow(pow(x*x+ (-x) -sin(x)+cos(x)+tan(x),x),3)))/x;

	{
		std::ofstream fout("serialization_test_node");
		
		boost::archive::text_oarchive oa(fout);
		
		// write class instance to archive
		oa << x;
		oa << f;
	}
	
	std::shared_ptr<Node> f2;
	std::shared_ptr<Variable> x2;
	{
		std::ifstream fin("serialization_test_node");
		
		boost::archive::text_iarchive ia(fin);
		// read class state from archive
		ia >> x2;
		ia >> f2;
	}

	BOOST_CHECK(x->name()==x2->name());

	x->set_current_value(dbl(1.2,0.9));
	x2->set_current_value(dbl(1.2,0.9));

	BOOST_CHECK(abs(f->Eval<dbl>() - f2->Eval<dbl>()) < threshold_clearance_d);

}




BOOST_AUTO_TEST_CASE(system_serialize_scopes)
{


	
	Vec<dbl> values(2);

	values(0) = dbl(2.0);
	values(1) = dbl(3.0);

	
	
	{ // to create a scope

		bertini::System sys;
		auto x = Variable::Make("x");
		auto y = Variable::Make("y");

		bertini::VariableGroup vg{x,y};

		sys.AddVariableGroup(vg);

		sys.AddFunction(x+y);
		sys.AddFunction(x-y);


		std::ofstream fout("serialization_test_node");
		
		boost::archive::text_oarchive oa(fout);
		
		// write class instance to archive
		oa << sys;
	}
	
	
	{
		std::ifstream fin("serialization_test_node");
		
		boost::archive::text_iarchive ia(fin);
		// read class state from archive
		bertini::System sys2;
		ia >> sys2;

		Vec<dbl> v = sys2.Eval(values);


		BOOST_CHECK_EQUAL(v.size(),2);


		BOOST_CHECK_EQUAL(v(0).real(), 5);
		BOOST_CHECK_EQUAL(v(0).imag(), 0);
		BOOST_CHECK_EQUAL(v(1).real(), -1);
		BOOST_CHECK_EQUAL(v(1).imag(), 0);


	}
	
}



BOOST_AUTO_TEST_CASE(system_serialize_scopes_via_parsing)
{


	
	Vec<dbl> x(2);

	x(0) = dbl(2.0);
	x(1) = dbl(3.0);

	Vec<dbl> y_before(2);
	
	{ // to create a scope

		std::string str = "function f1, f2; variable_group x1, x2;  f1 = x1^2 * x2^2; f2 = x1^2*x2; ";

		bertini::System sys;
		bertini::parsing::classic::parse(str.begin(), str.end(), sys);


		y_before = sys.Eval(x);

		std::ofstream fout("serialization_test_node");
		boost::archive::text_oarchive oa(fout);
		
		// write class instance to archive
		oa << sys;
	}
	
	
	{
		std::ifstream fin("serialization_test_node");
		
		boost::archive::text_iarchive ia(fin);
		// read class state from archive
		bertini::System sys2;
		ia >> sys2;

		Vec<dbl> y_after = sys2.Eval(x);


		BOOST_CHECK_EQUAL(y_after.size(),2);

		BOOST_CHECK_EQUAL(y_before(0), y_after(0));
		BOOST_CHECK_EQUAL(y_before(1), y_after(1));

	}
	
}





BOOST_AUTO_TEST_CASE(system_serialize_scopes_using_subfunctions_via_parsing)
{


	
	Vec<dbl> values(2);

	values(0) = dbl(2.0);
	values(1) = dbl(3.0);

	
	
	{ // to create a scope

		std::string str = "function f1, f2; variable_group x1, x2; y = x1*x2; f1 = y*y; f2 = x1*y; ";

		bertini::System sys;
		bertini::parsing::classic::parse(str.begin(), str.end(), sys);


		std::ofstream fout("serialization_test_node");
		
		boost::archive::text_oarchive oa(fout);
		
		// write class instance to archive
		oa << sys;
	}
	
	
	{
		std::ifstream fin("serialization_test_node");
		
		boost::archive::text_iarchive ia(fin);
		// read class state from archive
		bertini::System sys2;
		ia >> sys2;

		Vec<dbl> v = sys2.Eval(values);


		BOOST_CHECK_EQUAL(v.size(),2);

		BOOST_CHECK_EQUAL(v(0), 36.0);
		BOOST_CHECK_EQUAL(v(1), 12.0);

	}
	
}





BOOST_AUTO_TEST_CASE(system_clone)
{
	std::string str = "function f1, f2; variable_group x1, x2; y = x1*x2; f1 = y*y; f2 = x1*y; ";

	bertini::System sys1;
	bertini::parsing::classic::parse(str.begin(), str.end(), sys1);

	
	auto sys2 = Clone(sys1);
	

	Vec<dbl> values(2);

	values(0) = dbl(2.0);
	values(1) = dbl(3.0);

	Vec<dbl> v = sys2.Eval(values);

	BOOST_CHECK_EQUAL(v.size(),2);

	BOOST_CHECK_EQUAL(v(0), 36.0);
	BOOST_CHECK_EQUAL(v(1), 12.0);

	auto variables1 = sys1.Variables();
	auto variables2 = sys2.Variables();

	BOOST_CHECK_EQUAL(variables1.size(), variables2.size());


	for (int ii=0; ii<variables2.size(); ++ii)
	{
		BOOST_CHECK(variables1[ii].get() != variables2[ii].get());
	}

}


BOOST_AUTO_TEST_CASE(clone_griewank_osborn)
{

	auto gw = bertini::system::Precon::GriewankOsborn();

	auto clone = bertini::Clone(gw);
}

BOOST_AUTO_TEST_SUITE_END()




