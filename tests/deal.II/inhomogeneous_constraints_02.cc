//------------------  inhomogeneous_constraints.cc  ------------------------
//    $Id: inhomogeneous_constraints.cc 23710 2011-05-17 04:50:10Z bangerth $
//    Version: $Name$ 
//
//    Copyright (C) 2009 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//------------------  inhomogeneous_constraints.cc  ------------------------


// We take one cell and create the system_matrix and the
// right-hand-side-vector.  But we have a few inhomogeneous
// constraints and want to test if the
// distribute_local_to_global-function supplies the correct
// right-hand-side-vector if the use_inhomogeneities_for_rhs-parameter
// is set to true or false.

#include "../tests.h"

#include <deal.II/base/quadrature_lib.h>
#include <deal.II/base/function.h>
#include <deal.II/base/logstream.h>
#include <deal.II/base/utilities.h>
#include <deal.II/lac/vector.h>
#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/sparse_matrix.h>
#include <deal.II/lac/solver_cg.h>
#include <deal.II/lac/precondition.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>
#include <deal.II/grid/grid_refinement.h>
#include <deal.II/dofs/dof_accessor.h>
#include <deal.II/dofs/dof_tools.h>
#include <deal.II/lac/constraint_matrix.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/numerics/vectors.h>
#include <deal.II/numerics/matrices.h>
#include <deal.II/numerics/error_estimator.h>
#include <deal.II/lac/compressed_simple_sparsity_pattern.h>

#include <fstream>
#include <iostream>
#include <complex>

std::ofstream logfile("inhomogeneous_constraints_02/output");

using namespace dealii;

void test(bool use_inhomogeneity_for_rhs)
{
  ConstraintMatrix cm;

  cm.add_line(2);
  cm.set_inhomogeneity(2, 3.0);
  cm.add_line(3);
  cm.set_inhomogeneity(3, 0.0);
  cm.add_line(4);
  cm.add_entry(4, 2, 2.0);

  cm.close();
  cm.print(logfile);

  
  CompressedSimpleSparsityPattern csp(5,5);
  for (unsigned int i=0;i<5;++i)
    csp.add(i,i);
    
  SparsityPattern sp;
  sp.copy_from(csp);
  SparseMatrix<double> mat(sp);
  Vector<double> rhs(5);

  std::vector<unsigned int> local_dofs;
  for (unsigned int i=0;i<5;++i)
    local_dofs.push_back(i);

  FullMatrix<double> local_mat(5,5);
  Vector<double> local_vec(5);
  for (unsigned int i=0;i<5;++i)
    local_mat(i,i)=2.0;
  
  local_vec = 0;

  cm.distribute_local_to_global(local_mat, local_vec, local_dofs, mat, rhs, use_inhomogeneity_for_rhs);

  mat.print(logfile);
  rhs.print(logfile);
  
}


int main () 
{
  deallog << std::setprecision (2);
  logfile << std::setprecision (2);
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  // Use the constraints for the right-hand-side
  {
    test(true);	
  }

  // Don not use the constraints for the right-hand-side
  {
    test(false);
  }
}