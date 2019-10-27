// Copyright (C) 2015 - IIT Bombay - FOSSEE
//
// Author: Harpreet Singh
// Organization: FOSSEE, IIT Bombay
// Email: toolbox@scilab.in
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

#include "minbndTMINLP.hpp"
#include "sci_iofunc.hpp"

extern "C"
{
#include "call_scilab.h"
#include <api_scilab.h>
#include <Scierror.h>
#include <BOOL.h>
#include <localization.h>
#include <sciprint.h>
#include <string.h>
#include <assert.h>
}

using namespace Ipopt;
using namespace Bonmin;

#define LOCAL_DEBUG 0

minbndTMINLP::~minbndTMINLP()
{
	if(finalX_) delete[] finalX_;
}

bool minbndTMINLP::getScilabFunc(Index n, scilabVar* out, const Number* x, wchar_t* name, int nin, int nout)
{
	
	scilabVar* funcIn = (scilabVar*)malloc(sizeof(double) * (numVars_) * 1);
	funcIn[0] = scilab_createDoubleMatrix2d(env_, 1, n, 0);
	scilab_setDoubleArray(env_, funcIn[0], x);

	#if LOCAL_DEBUG
		printf("Calling the relevant function\n");
	#endif
	scilab_call(env_, name, nin, funcIn, nout, out);
	
	
	return true;
}

// Set the type of every variable - CONTINUOUS or INTEGER
bool minbndTMINLP::get_variables_types(Index n, VariableType* var_types)
{
  n = numVars_;
  for(int i=0; i < n; i++)
    var_types[i] = CONTINUOUS;
  for(int i=0 ; i < intconSize_ ; ++i)
  	var_types[(int)(intcon_[i]-1)] = INTEGER;
  return true;
}

// The linearity of the variables - LINEAR or NON_LINEAR
bool minbndTMINLP::get_variables_linearity(Index n, Ipopt::TNLP::LinearityType* var_types)
{  return true; }

// The linearity of the constraints - LINEAR or NON_LINEAR
bool minbndTMINLP::get_constraints_linearity(Index m, Ipopt::TNLP::LinearityType* const_types)
{  return true;}

//get NLP info such as number of variables,constraints,no.of elements in jacobian and hessian to allocate memory
bool minbndTMINLP::get_nlp_info(Index& n, Index& m, Index& nnz_jac_g, Index& nnz_h_lag, TNLP::IndexStyleEnum& index_style)
{
	n=numVars_; // Number of variables
	m=0; // Number of constraints
	nnz_jac_g = 0; // No. of elements in Jacobian of constraints 
	nnz_h_lag = n*(n+1)/2; // No. of elements in lower traingle of Hessian of the Lagrangian.
	index_style=TNLP::C_STYLE; // Index style of matrices
	return true;
}

//get variable and constraint bound info
bool minbndTMINLP::get_bounds_info(Index n, Number* x_l, Number* x_u, Index m, Number* g_l, Number* g_u)
{
	unsigned int i;
	for(i=0;i<n;i++)
	{
		x_l[i]=lb_[i]+0.0000001;
		x_u[i]=ub_[i]-0.0000001;
	}

        g_l=NULL;
        g_u=NULL;
	return true;
}

// return the value of the constraints: g(x)
bool minbndTMINLP::eval_g(Index n, const Number* x, bool new_x, Index m, Number* g)
{

  	// return the value of the constraints: g(x)
  	g=NULL;
  	return true;
}

// return the structure or values of the jacobian
bool minbndTMINLP::eval_jac_g(Index n, const Number* x, bool new_x,Index m, Index nele_jac, Index* iRow, Index *jCol,Number* values)
{

 	if (values == NULL) 
 	{
    		// return the structure of the jacobian of the constraints
    		iRow=NULL; 
    		jCol=NULL;
  	}
  	else 
	{
		values=NULL;
  	}
  	return true;
}

//get value of objective function at vector x
bool minbndTMINLP::eval_f(Index n,  const Number* x, bool new_x, Number& obj_value)	
{	
  	scilabVar* out = (scilabVar*)malloc(sizeof(scilabVar) * (numVars_) * 1);
	
	#if LOCAL_DEBUG
		printf("Calling eval_f\n");
	#endif	
  	double check;
	double obj=0;
	getScilabFunc(n, out, x, L"_f", 1, 2);

	
	
                               
  	if (scilab_isDouble(env_, out[1]) == 0 || scilab_isScalar(env_, out[1]) == 0)
	{
    	Scierror(999, " Wrong type for input argument #%d: An int expected.\n", 8);
    	return 1;
	}
	

	scilab_getDouble(env_, out[1], &check);

	if (check==1)
	{
		return true;
	}	
	else
	{ 

		if (scilab_isDouble(env_, out[0]) == 0 || scilab_isScalar(env_, out[0]) == 0)
		{
			sciprint("No obj value\n");
			return 1;
		}

		scilab_getDouble(env_, out[0], &obj);
  		obj_value=obj;  
	}
	#if LOCAL_DEBUG
		printf("Obj value obtained\n");
	#endif
  	return true;
}

//get value of gradient of objective function at vector x.
bool minbndTMINLP::eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f)	
{
	Number *resg;

	#if LOCAL_DEBUG
		printf("eval_grad_f started\n");
	#endif	

	const Number *xNew=x;
	scilabVar* out = (scilabVar*)malloc(sizeof(scilabVar) * (numVars_) * 2);
  	double check = 0;

	scilabVar* funcIn = (scilabVar*)malloc(sizeof(scilabVar) * (numVars_) * 2);
	funcIn[0] = scilab_createDoubleMatrix2d(env_, 1, n, 0);
	scilab_setDoubleArray(env_, funcIn[0], x);

	#if LOCAL_DEBUG
		printf("eval_grad_f scilab_setDoubleArray\n");
	#endif	

	scilab_call(env_, L"_gradf", 1, funcIn, 2, out);

	#if LOCAL_DEBUG
		printf("eval_grad_f scilab_call\n");
	#endif	

	if (scilab_isDouble(env_, out[1]) == 0 || scilab_isScalar(env_, out[1]) == 0)
	{
    	Scierror(999, "Wrong type for input argument #%d: An int expected.\n", 2);
    	return 1;
	}
	
	#if LOCAL_DEBUG
		printf("eval_grad_f check\n");
	#endif
	scilab_getDouble(env_, out[1], &check);
	

	if (check==1)
	{
		return true;
	}	
	else
	{ 
		double* resg;  
  		if (scilab_isDouble(env_, out[0]) == 0 || scilab_isMatrix2d(env_, out[0]) == 0)
		{
			Scierror(999, "Wrong type for input argument #%d: An int expected.\n", 2);
			return 1;
		}
	
		scilab_getDoubleArray(env_, out[0], &resg);
  		Index i;
  		for(i=0;i<numVars_;i++)
  		{
			grad_f[i]=resg[i];
  		}
	}
  	return true;
}

// This method sets initial values for required vectors . For now we are assuming 0 to all values. 
bool minbndTMINLP::get_starting_point(Index n, bool init_x, Number* x,bool init_z, Number* z_L, Number* z_U,Index m, bool init_lambda,Number* lambda)
{

 	assert(init_x == true);
  	assert(init_z == false);
  	assert(init_lambda == false);
	if (init_x == true)
	{ //we need to set initial values for vector x
		for (Index var=0;var<n;var++)
			{x[var]=0.0;}//initialize with 0.
	}
	return true;
}

/*
 * Return either the sparsity structure of the Hessian of the Lagrangian, 
 * or the values of the Hessian of the Lagrangian  for the given values for
 * x,lambda,obj_factor.
*/

bool minbndTMINLP::eval_h(Index n,  const Number* x, bool new_x,Number obj_factor, Index m, const Number* lambda,bool new_lambda, Index nele_hess, Index* iRow,Index* jCol, Number* values)
{	
	#if LOCAL_DEBUG
		printf("eval_h started\n");
	#endif

	
	double check;
	if (values==NULL)
	{
		Index idx=0;
		
		for (Index row = 0; row < numVars_; row++) 
		{
			for (Index col = 0; col <= row; col++)
			{
				iRow[idx] = row;
				jCol[idx] = col;
				idx++;
		  	}
		}
	}

	else 
	{			
	  	Number *resh;

		const Number *xNew=x;
		#if LOCAL_DEBUG
			printf("in the gradhess block\n");
		#endif	
		scilabVar* out = (scilabVar*)malloc(sizeof(scilabVar) * (numVars_) * 2);
	  	double check = 0;

		scilabVar* funcIn = (scilabVar*)malloc(sizeof(scilabVar) * (numVars_) * 2);
		funcIn[0] = scilab_createDoubleMatrix2d(env_, 1, n, 0);
		scilab_setDoubleArray(env_, funcIn[0], x);

	
		scilab_call(env_, L"_gradhess", 1, funcIn, 2, out);

	#if LOCAL_DEBUG
		printf("eval_h scilab_call\n");
	#endif

		if (scilab_isDouble(env_, out[1]) == 0 || scilab_isScalar(env_, out[1]) == 0)
		{
			Scierror(999, "Wrong type for input argument #%d: An int expected.\n", 2);
			return 1;
		}
		

		scilab_getDouble(env_, out[1], &check);

		if (check==1)
		{
			
			return true;
		}	

		else
		{
			if (scilab_isDouble(env_, out[0]) == 0 || scilab_isMatrix2d(env_, out[0]) == 0)
			{
				Scierror(999, "Wrong type for input argument #%d: An int expected.\n", 2);
				return 1;
			}
		
			scilab_getDoubleArray(env_, out[0], &resh);

			Index index=0;
			for (Index row=0;row < numVars_ ;++row)
			{
				for (Index col=0; col <= row; ++col)
				{
					values[index++]=obj_factor*(resh[numVars_*row+col]);

				}
			}
		}

    }
	return true;
}

void minbndTMINLP::finalize_solution(SolverReturn status,Index n, const Number* x, Number obj_value)
{
	finalObjVal_ = obj_value;
	status_ = status;
	if(status==0 ||status== 3)
	{
		finalX_ =  new double[n];
		for (Index i=0; i<numVars_; i++) 
		{
	    		 finalX_[i] = x[i];
		}
	}

}

const double * minbndTMINLP::getX()
{	
	return finalX_;
}

double minbndTMINLP::getObjVal()
{	
	return finalObjVal_;
}

int minbndTMINLP::returnStatus()
{	
	return status_;
}
