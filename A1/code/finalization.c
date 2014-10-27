/**
 * Finalization step - write results and other computational vectors to files
 *
 * @date 22-Oct-2012
 * @author V. Petkov
 */

#include <stdio.h>
#include "util_write_files.h"
#include "vol2mesh.h"

void finalization(char* file_in, int total_iters, double residual_ratio,
		  int nintci, int nintcf, double* var, double* cgup, double* su, int **lcc){
  
  char file_out[30] = "summary.out";
  char file_su_vtk[30] = "SU.vtk";
  char file_var_vtk[30] = "VAR.vtk";
  char file_cgup_vtk[30] = "CGUP.vtk";
  
  int status = write_result(file_in, file_out, nintci, nintcf, var, total_iters, residual_ratio);
  
  if ( status != 0 ) fprintf(stderr, "Error when trying to write to file %s\n", file_out);
  
  // Writing VTK files
  int nodeCnt;
  int **points, **elems;
  
  vol2mesh(nintci, nintcf, lcc, &nodeCnt, &points, &elems);
  write_result_vtk(file_su_vtk, nintci, nintcf, nodeCnt, points, elems, su);
  write_result_vtk(file_var_vtk, nintci, nintcf, nodeCnt, points, elems, var);
  write_result_vtk(file_cgup_vtk, nintci, nintcf, nodeCnt, points, elems, cgup);
  
		  }
		  
		  