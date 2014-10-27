/**
 * Main GCCG program
 *
 * @author E. Xue, V. Petkov
 * @date 22-May-2009, 22-Oct-2012
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include "mpi.h"

#include "initialization.h"
#include "compute_solution.h"
#include "finalization.h"

int main(int argc, char *argv[]) {
    int i;

    const int max_iters = 10000;    /// maximum number of iteration to perform

    /** Simulation parameters parsed from the input datasets */
    int nintci, nintcf;    /// internal cells start and end index
    /// external cells start and end index. The external cells are only ghost cells.
    /// They are accessed only through internal cells
    int nextci, nextcf;
    int **lcc;    /// link cell-to-cell array - stores neighboring information
    /// Boundary coefficients for each volume cell (South, East, North, West, High, Low)
    double *bs, *be, *bn, *bw, *bh, *bl;
    double *bp;    /// Pole coefficient
    double *su;    /// Source values

    double residual_ratio;    /// the ratio between the reference and the current residual
    double *var;    /// the variation vector -> keeps the result in the end

    /** Additional vectors required for the computation */
    double *cgup, *oc, *cnorm;

	/*check if the input is correct and return an error message with instruction*/
	if (argc !=4)
	{
		printf("Error: input failed, please input as gccg <format> <input file> <output prefix>");	
	return 0;
	}

	 /*we should use format <format> <input file> <output prefix>*/
	char *format = argv[1];    
	char *file_in = argv[2];
 
    /********** START INITIALIZATION **********/
    // read-in the input file
    int init_status = initialization(file_in, format, &nintci, &nintcf, &nextci, &nextcf, &lcc,
                                     &bs, &be, &bn, &bw, &bl, &bh, &bp, &su, &var, &cgup, &oc, 
                                     &cnorm, argv);

    if ( init_status != 0 ) {
        fprintf(stderr, "Failed to initialize data!\n");
        abort();
    } 

   
    /********** END INITIALIZATION **********/

    /********** START COMPUTATIONAL LOOP **********/
    int total_iters = compute_solution(max_iters, nintci, nintcf, nextcf, lcc, bp, bs, bw, bl, bn,
                                       be, bh, cnorm, var, su, cgup, &residual_ratio);
    /********** END COMPUTATIONAL LOOP **********/

    /********** START FINALIZATION **********/
    finalization(file_in, total_iters, residual_ratio, nintci, nintcf, var, cgup, su, lcc);
    /********** END FINALIZATION **********/


    free(cnorm);
    free(var);
    free(cgup);
    free(su);
    free(bp);
    free(bh);
    free(bl);
    free(bw);
    free(bn);
    free(be);
    free(bs);

for ( i = 0; i < 6; ++i)
{
   printf("lcc[1][%d] = %d\n", i, lcc[1][i] );
}

   for ( i = nintci; i <= nintcf; i++ ) {
        free(lcc[i]);
    }
   free(lcc);
 
    return 0;
}

