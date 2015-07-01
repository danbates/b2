//This file is part of Bertini 2.0.
//
//sum_operator.hpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//sum_operator.hpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with sum_operator.hpp.  If not, see <http://www.gnu.org/licenses/>.
//
//  Created by Collins, James B. on 4/30/15.
//
//
// sum_operator.hpp:  Declares the class SumOperator.

#ifndef b2Test_SumOperator_h
#define b2Test_SumOperator_h

#include <vector>
#include <string>

#include "function_tree/node.hpp"
#include "function_tree/operators/nary_operator.hpp"

#include "function_tree/symbols/number.hpp"

#include "function_tree/operators/mult_operator.hpp"
#include "function_tree/operators/intpow_operator.hpp"

#include "function_tree/symbols/variable.hpp"




namespace bertini {
	
	// Node -> NaryOperator -> SumOperator
	// Description: This class represents summation and difference operators.  All children are terms and are stored
	// in a single vector, and a vector of bools is used to determine the sign of each term.  FreshEval method
	// is defined for summation and difference.
	// class IntegerPowerOperator;
	// class MultOperator;
	

	class SumOperator : public virtual NaryOperator
	{
	public:
		virtual ~SumOperator() = default;
		
		
		
		SumOperator(){}
		
		SumOperator(const std::shared_ptr<Node> & left, const std::shared_ptr<Node> & right)
		{
			AddChild(left);
			AddChild(right);
		}
		
		
		SumOperator(const std::shared_ptr<Node> & left, bool add_or_sub_left, const std::shared_ptr<Node> & right, bool add_or_sub_right)
		{
			AddChild(left, add_or_sub_left);
			AddChild(right, add_or_sub_right);
		}
		
		
		SumOperator& operator+=(const std::shared_ptr<Node> & rhs)
		{
			this->AddChild(rhs);
			return *this;
		}
		
		SumOperator& operator-=(const std::shared_ptr<Node> & rhs)
		{
			this->AddChild(rhs,false);
			return *this;
		}
		
		
		// Print the data for this node, as well as all it's children
		//TODO (JBC): Implement this method
		virtual std::string PrintNode() override {return "";}
		
		
		
		//Special Behaviour: by default all terms added are positive
		virtual void AddChild(std::shared_ptr<Node> child) override
		{
			NaryOperator::AddChild(std::move(child));
			children_sign_.push_back(true);
		}
		
		
		
		//Special Behaviour: Pass bool to set sign of term: true = add, false = subtract
		void AddChild(std::shared_ptr<Node> child, bool sign) // not an override
		{
			NaryOperator::AddChild(std::move(child));
			children_sign_.push_back(sign);
		}
		
		
		virtual void print(std::ostream & target) const override
		{
			target << "(";
			for (auto iter = children_.begin(); iter!= children_.end(); iter++) {
				if (iter==children_.begin()) {
					// on the first iteration, no need to put a + if a +
					if ( !(*(children_sign_.begin()+(iter-children_.begin()))) )
						target << "-";
				}
				else
				{
					if ( !(*(children_sign_.begin()+(iter-children_.begin()))) )
						target << "-";
					else
						target << "+";
				}
				(*iter)->print(target);
				
			}
			target << ")";
		}
        
        
        
        
        /**
         Return SumOperator whose children are derivatives of children_
         */
        virtual std::shared_ptr<Node> Differentiate() override
        {
        	unsigned int counter = 0;
            std::shared_ptr<SumOperator> ret_sum = std::make_shared<SumOperator>();
            for (int ii = 0; ii < children_.size(); ++ii)
            {
            	auto converted = std::dynamic_pointer_cast<Number>(children_[ii]);
            	if (converted)
            		continue;
            	
            	auto temp_node = children_[ii]->Differentiate();
				converted = std::dynamic_pointer_cast<Number>(temp_node);
				if (converted)
					if (converted->Eval<dbl>()==dbl(0.0))
						continue;

                ret_sum->AddChild(temp_node,children_sign_[ii]);
                counter++;
            }
            
            if (counter>0)
            	return ret_sum;
            else
            	return std::make_shared<Number>(0.0);
        }





        /**
		Compute the degree of a node.  For sum functions, the degree is the max among summands.
        */
		virtual int Degree(std::shared_ptr<Variable> const& v = nullptr) const override
		{
			int deg = 0;

			for (auto iter: children_)
			{
				auto curr_deg = iter->Degree(v);
				if (curr_deg<0)
				    return curr_deg;

				deg = std::max(deg, curr_deg);
			}
			return deg;
		}



		void Homogenize(std::vector< std::shared_ptr< Variable > > const& vars, std::shared_ptr<Variable> const& homvar);
		// {
		// 	// first homogenize each summand.
		// 	for (auto iter: children_)
		// 	{
		// 		iter->Homogenize(vars, homvar);
		// 	}

		// 	// then, homogenize this sum.

		// 	// compute the highest degree among all summands.
		// 	int maxdegree = 0;
		// 	std::vector<int> term_degrees;
		// 	// first homogenize each summand.
		// 	for (auto iter: children_)
		// 	{
		// 		auto local_degree = iter->Degree(vars);
		// 		if (local_degree<0)
		// 			throw std::runtime_error("asking for homogenization on non-polynomial node");
		// 			// TODO: this throw would leave the tree in a partially homogenized state.  this is scary.

		// 		term_degrees.push_back(local_degree);
		// 		maxdegree = std::max(maxdegree, local_degree);
		// 	}

		// 	for (auto iter = children_.begin(); iter!=children_.end(); iter++)
		// 	{
		// 		auto degree_deficiency = maxdegree - *(term_degrees.begin() + (iter-children_.begin()));
		// 		if ( degree_deficiency > 0)
		// 		{
		// 			// hold the child temporarily.
		// 			auto new_multiplication_node = pow(homvar,degree_deficiency) * (iter);
		// 			swap(*iter,new_multiplication_node);
		// 		}
		// 	}

		// }

		
	protected:
		// Specific implementation of FreshEval for add and subtract.
		//  If child_sign_ = true, then add, else subtract
//		virtual dbl FreshEval(dbl) override
//		{
//			dbl retval{0};
//			for(int ii = 0; ii < children_.size(); ++ii)
//			{
//				if(children_sign_[ii])
//				{
//					retval += children_[ii]->Eval<dbl>();
//				}
//				else
//				{
//					retval -= children_[ii]->Eval<dbl>();
//				}
//			}
//			
//			return retval;
//		}
//		
//		
//		
//		
//		virtual mpfr FreshEval(mpfr) override
//		{
//			mpfr retval{0};
//			for(int ii = 0; ii < children_.size(); ++ii)
//			{
//				if(children_sign_[ii])
//				{
//					retval += children_[ii]->Eval<mpfr>();
//				}
//				else
//				{
//					retval -= children_[ii]->Eval<mpfr>();
//				}
//			}
//			
//			return retval;
//		}
		

        virtual dbl FreshEval(dbl, std::shared_ptr<Variable> diff_variable) override
        {
            dbl retval{0};
            for(int ii = 0; ii < children_.size(); ++ii)
            {
                if(children_sign_[ii])
                {
                    retval += children_[ii]->Eval<dbl>(diff_variable);
                }
                else
                {
                    retval -= children_[ii]->Eval<dbl>(diff_variable);
                }
            }
            
            return retval;
        }
        
        
        
        
        virtual mpfr FreshEval(mpfr, std::shared_ptr<Variable> diff_variable) override
        {
            mpfr retval{0};
            for(int ii = 0; ii < children_.size(); ++ii)
            {
                if(children_sign_[ii])
                {
                    retval += children_[ii]->Eval<mpfr>(diff_variable);
                }
                else
                {
                    retval -= children_[ii]->Eval<mpfr>(diff_variable);
                }
            }
            
            return retval;
        }

        
        

		
		
		
		
	private:
		// Stores the sign of the particular term.  There is a one-one
		// correspondence between elements of children_sign_ and children_.  This
		// is enforced by the AddChild method below, redefined in SumOperator.
		
		// TODO(JBC): If we add method to delete child, must also delete children_sign_ entry.
		std::vector<bool> children_sign_;
		
	};
	
	
	
} // re: namespace bertini








///////////////////
//
//     SUM AND DIFFERENCE ARITHMETIC OPERATORS
//
/////////////////////

namespace  {
	
	using Node = bertini::Node;
	using SumOperator = bertini::SumOperator;
	using Number = bertini::Number;
	
	
	
	
	
	///////////////
	//
	//  addition operators
	//
	///////////////
	
	
	inline std::shared_ptr<Node>& operator+=(std::shared_ptr<Node> & lhs, const std::shared_ptr<Node> & rhs)
	{
		std::shared_ptr<Node> temp = std::make_shared<SumOperator>();
		
		std::dynamic_pointer_cast<SumOperator>(temp)->AddChild(lhs);
		std::dynamic_pointer_cast<SumOperator>(temp)->AddChild(rhs);
		
		lhs.swap(temp);
		return lhs;
	}
	
	inline std::shared_ptr<Node>& operator+=(std::shared_ptr<Node> & lhs, double rhs)
	{
		std::shared_ptr<Node> temp = std::make_shared<SumOperator>();
		
		std::dynamic_pointer_cast<SumOperator>(temp)->AddChild(lhs);
		std::dynamic_pointer_cast<SumOperator>(temp)->AddChild(std::make_shared<Number>(rhs));
		
		lhs.swap(temp);
		return lhs;
	}
	
	
	
	
	
	inline std::shared_ptr<Node> operator+(std::shared_ptr<Node> lhs, const std::shared_ptr<Node> & rhs)
	{
		return std::make_shared<SumOperator>(lhs,rhs);
	}
	
	
	inline std::shared_ptr<Node> operator+(std::shared_ptr<Node> lhs, double rhs)
	{
		return std::make_shared<SumOperator>(lhs,std::make_shared<Number>(rhs));
	}
	
	inline std::shared_ptr<Node> operator+(double lhs,  std::shared_ptr<Node> rhs)
	{
		return std::make_shared<SumOperator>(std::make_shared<Number>(lhs), rhs);
	}
	
	
	
	
	
	///////////////
	//
	//  subtraction operators
	//
	///////////////
	
	
	inline std::shared_ptr<Node>& operator-=(std::shared_ptr<Node> & lhs, const std::shared_ptr<Node> & rhs)
	{
		std::shared_ptr<Node> temp = std::make_shared<SumOperator>();
		
		std::dynamic_pointer_cast<SumOperator>(temp)->AddChild(lhs);
		std::dynamic_pointer_cast<SumOperator>(temp)->AddChild(rhs,false);
		
		lhs.swap(temp);
		return lhs;
	}
	
	inline std::shared_ptr<Node>& operator-=(std::shared_ptr<Node> & lhs, double rhs)
	{
		std::shared_ptr<Node> temp = std::make_shared<SumOperator>();
		
		std::dynamic_pointer_cast<SumOperator>(temp)->AddChild(lhs);
		std::dynamic_pointer_cast<SumOperator>(temp)->AddChild(std::make_shared<Number>(rhs),false);
		
		lhs.swap(temp);
		return lhs;
	}
	
	inline std::shared_ptr<Node> operator-(std::shared_ptr<Node> lhs, const std::shared_ptr<Node> & rhs)
	{
		return std::make_shared<SumOperator>(lhs,true,rhs,false);
	}
	
	inline std::shared_ptr<Node> operator-(std::shared_ptr<Node> lhs, double rhs)
	{
		return std::make_shared<SumOperator>(lhs, true, std::make_shared<Number>(rhs), false);
	}
	
	inline std::shared_ptr<Node> operator-(double lhs,  std::shared_ptr<Node> rhs)
	{
		return std::make_shared<SumOperator>(std::make_shared<Number>(lhs), true, rhs, false);
	}
	
	
	
} // re: namespace {}
#endif