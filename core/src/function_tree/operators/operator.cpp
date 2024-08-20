//This file is part of Bertini 2.
//
//operator.cpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//operator.cpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with operator.cpp.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright(C) 2015 - 2021 by Bertini2 Development Team
//
// See <http://www.gnu.org/licenses/> for a copy of the license, 
// as well as COPYING.  Bertini2 is provided with permitted 
// additional terms in the b2/licenses/ directory.

// individual authors of this file include:
// silviana amethyst, university of wisconsin eau claire
// Jeb Collins, West Texas A&M


#include "bertini2/function_tree/operators/operator.hpp"


namespace bertini {
namespace node{	

void UnaryOperator::Reset() const
{
	Node::ResetStoredValues();
	operand_->Reset();
}

void UnaryOperator::SetOperand(std::shared_ptr<Node> n)
{
	operand_ = n;
}


//Return the only child for the unary operator
std::shared_ptr<Node> UnaryOperator::Operand() const
{
	return operand_;
}



int UnaryOperator::Degree(std::shared_ptr<Variable> const& v) const
{
	return operand_->Degree(v);
}




int UnaryOperator::Degree(VariableGroup const& vars) const
{
	auto multideg = MultiDegree(vars);
	auto deg = 0;
	std::for_each(multideg.begin(),multideg.end(),[&](int n){
					if (n < 0)
						deg = n;
					else
						deg += n;
					});
	return deg;
}

std::vector<int> UnaryOperator::MultiDegree(VariableGroup const& vars) const
{
	
	std::vector<int> deg(vars.size());
	for (auto iter = vars.begin(); iter!= vars.end(); ++iter)
	{
		*(deg.begin()+(iter-vars.begin())) = this->Degree(*iter);
	}
	return deg;
}

void UnaryOperator::Homogenize(VariableGroup const& vars, std::shared_ptr<Variable> const& homvar)
{
	operand_->Homogenize(vars, homvar);
}


bool UnaryOperator::IsHomogeneous(std::shared_ptr<Variable> const& v) const
{
	if (Degree(v)==0)
	{
		return true;
	}
	else
		return false;
}


/**
Check for homogeneity, with respect to a variable group.
*/
bool UnaryOperator::IsHomogeneous(VariableGroup const& vars) const
{
	if (Degree(vars)==0)
	{
		return true;
	}
	else
		return false;
}



/**
 Change the precision of this variable-precision tree node.
 
 \param prec the number of digits to change precision to.
 */
void UnaryOperator::precision(unsigned int prec) const
{
	auto& val_pair = std::get< std::pair<mpfr_complex,bool> >(current_value_);
	val_pair.first.precision(prec);

	operand_->precision(prec);
}



////////////
//
//  Nary 
//
////////////

void NaryOperator::Reset() const
{
	Node::ResetStoredValues();
	for (const auto& ii : operands_)
		ii->Reset();

}

// Add an operand onto the container for this operator
void NaryOperator::AddOperand(std::shared_ptr<Node> n)
{
	operands_.push_back(std::move(n));
}






size_t NaryOperator::NumOperands() const
{
	return operands_.size();
}

std::shared_ptr<Node> NaryOperator::FirstOperand() const
{
	return operands_[0];
}




 /**
 Change the precision of this variable-precision tree node.
 
 \param prec the number of digits to change precision to.
 */
void NaryOperator::precision(unsigned int prec) const
{
	auto& val_pair = std::get< std::pair<mpfr_complex,bool> >(current_value_);
	val_pair.first.precision(prec);
	
	this->PrecisionChangeSpecific(prec);

	for (const auto& iter : operands_)
		iter->precision(prec);
}


void NaryOperator::PrecisionChangeSpecific(unsigned prec) const
{}


} // re: namespace node	
} // re: bertini namespace
