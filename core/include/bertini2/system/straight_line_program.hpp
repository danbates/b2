//This file is part of Bertini 2.
//
//system.hpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//system.hpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with system.hpp.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright(C) 2015 - 2021 by Bertini2 Development Team
//
// See <http://www.gnu.org/licenses/> for a copy of the license, 
// as well as COPYING.  Bertini2 is provided with permitted 
// additional terms in the b2/licenses/ directory.

// individual authors of this file include:
// silviana amethyst, university of wisconsin eau claire

/**
\file system.hpp 

\brief Provides the bertini::System class.
*/

#ifndef BERTINI_SYSTEM_HPP
#define BERTINI_SYSTEM_HPP

#include <assert.h>
#include <vector>


#include "bertini2/mpfr_complex.hpp"
#include "bertini2/mpfr_extensions.hpp"
#include "bertini2/eigen_extensions.hpp"



// code copied from Bertini1's file include/bertini.h


/*

typedef struct
{
  int num_funcs;
  int num_hom_var_gp;
  int num_var_gp;
  int *type; // 0 - hom_var_gp, 1 - var_gp
  int *size; // size of the group of the user listed variables (total size = size + type)
} preproc_data;


typedef struct
{
  point_d funcVals;
  point_d parVals;
  vec_d parDer;
  mat_d Jv;
  mat_d Jp;
} eval_struct_d;

typedef struct
{
  point_mp funcVals;
  point_mp parVals;
  vec_mp parDer;
  mat_mp Jv;
  mat_mp Jp;
} eval_struct_mp;


The straight-line program structure.  This is the way that polynomials are stored internally.
typedef struct {
  int *prog;     //  The program instructions. (a big integer array)  
  int  size;     //  size of the instruction program.      
  int  memSize;  // Amount of memory it needs in workspace (for temp and final results).   
  num_t *nums;   // The array of real numbers. 
  int precision; // The precision at which evaluation should occur 
  
  // INFO NEEDED FOR M-HOM: 
  int num_var_gps;  // The total number of variable groups (i.e., m from m-hom).
  int *var_gp_sizes;  // The size of each of the groups. 
  int index_of_first_number_for_proj_trans;  // The address of the first number used in the projective transformation polynomials. 
                                                                                    
  // STOP LOCATIONS: 
  int  numInstAtEndUpdate; // instruction number at end of update. i.e. i = 0; while (i < numInstAtEndUpdate) .. 
  int  numInstAtEndParams; // instruction number at end of params. i.e. i = numInstAtEndUpdate; while (i < numInstAtEndParams) .. 
  int  numInstAtEndFnEval; // instruction number at end of function eval. i.e. i = numInstAtEndParams; while (i < numInstAtEndFnEval) .. 
  int  numInstAtEndPDeriv; // instruction number at end of param diff. i.e. i = numInstAtEndFnEval; while (i < numInstAtEndPDeriv) .. 
  int  numInstAtEndJvEval; // instruction number at end of Jv eval. i.e. i = numInstAtEndPDeriv; while (i < numInstAtEndJvEval) .. 
                           // for Jp eval: i = numInstAtEndJvEval; while (i < size) ..  

  // INPUT AMOUNTS: 
  int  numVars;  //  Number of variables in the function being computed.   
  int  numPathVars;  //  Number of path variables.  Ought to be 1 usually.   
  int  numNums;  //  Number of real numbers used in evaluation.  
  int  numConsts;  //  Number of constants.     
                                                                                   
  // OUTPUT AMOUNTS: 
  int  numPars;  //  Number of parameters   
  int  numFuncs; //  Number of coordinate functions in the homotopy.   
  int  numSubfuncs;  //  Number of subfunctions.  
                                                                                      
  // INPUT LOCATIONS: 
  int  inpVars;  //  Where the input variable values are stored.   
  int  inpPathVars;  //  Where the values of the path variables are stored.   
  int  IAddr;  //  Where the constant I is stored.  
  int  numAddr;  //  Where the first num_t is stored.  
  int  constAddr;  //  Where the first constant is stored.  
                                                                                      
  // OUTPUT LOCATIONS: 
  int  evalPars;  //  Where U(t), for given t, is stored.   
  int  evalDPars;  //  Where the derivatives of the parameters are stored.   
  int  evalFuncs;  //  Where H(x,t) is stored.   
  int  evalJVars;  //  Where the Jacobian w.r.t. vars is stored.   
  int  evalJPars;  //  Where the Jacobian w.r.t. pars is stored.   
  int  evalSubs;  //  Where the subfunctions are stored 
  int  evalJSubsV;  //  Where the derivatives of the subfunctions w.r.t. vars are stored.  
  int  evalJSubsP;  //  Where the derivatives of the subfunctions w.r.t. pars are stored.  
} prog_t;
*/



namespace bertini {

	class StraightLineProgram{
	public:

		template<typename Derived>
		void EvalInPlace(Eigen::MatrixBase<Derived> & function_values) const
		{
			typedef typename Derived::Scalar T;

			if(function_values.size() < NumFunctions())
			{
				std::stringstream ss;
				ss << "trying to evaluate system in place, but number of input functions (" << function_values.size() << ") doesn't match number of system functions (" << NumFunctions() << ").";
				throw std::runtime_error(ss.str());
			}

			unsigned counter(0);
			for (auto iter=functions_.begin(); iter!=functions_.end(); iter++, counter++) {
				(*iter)->EvalInPlace<T>(function_values(counter));
			}

			if (IsPatched())
				patch_.EvalInPlace(function_values,
									std::get<Vec<T> >(current_variable_values_));
									// .segment(NumFunctions(),NumTotalVariableGroups())
			
		}


		template<typename T>
		Vec<T> Eval() const
		{
			Vec<T> function_values(NumTotalFunctions()); // create vector with correct number of entries.
			EvalInPlace(function_values);

			return function_values;
		}

		/**
		\brief Get the current precision of a system.
		*/
		inline
		unsigned precision() const
		{
			return precision_;
		}


		void precision(unsigned new_precision) const;

	private:

	};
	
}









#endif // for the ifndef include guards


