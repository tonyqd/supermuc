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
#include "mpi.h"
#include <papi.h>

#include "initialization.h"
#include "compute_solution.h"
#include "finalization.h"

#define NUM_EVENTS 5

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
	char *prefix = argv[3];
 
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
/**********Initialize the PAPI Counters************/
	void handle_error (int retval)
	{
		 printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
		 exit(1);
	}    

	float rtime, ptime, mflops;
    long long flpops;
     
    long long counters[NUM_EVENTS];
    int PAPI_events[] = {
        PAPI_L2_TCM, /*Level 2 cache misses*/
        PAPI_L2_TCA, /*Level 2 total cache accesses*/
        PAPI_L3_TCM, /*Level 3 cache misses*/
        PAPI_L3_TCA, /*Level 3 total cache accesses*/
        PAPI_TOT_CYC, /*Total cycles*/};

    if(PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) exit(1);
    
    /**********Start Counting*********/
    if(PAPI_start_counters(PAPI_events,NUM_EVENTS) != PAPI_OK) handle_error(1);
    /*if(PAPI_flops( &rtime, &ptime, &flpops,  &mflops ) != PAPI_OK) handle_error(1); */
        

    /********** START COMPUTATIONAL LOOP **********/
    
    
    int total_iters = compute_solution(max_iters, nintci, nintcf, nextcf, lcc, bp, bs, bw, bl, bn,
                                       be, bh, cnorm, var, su, cgup, &residual_ratio);
    /********** END COMPUTATIONAL LOOP **********/
    
    /*********stop counting *********/
    /*if(PAPI_flops( &rtime, &ptime, &flpops,  &mflops ) != PAPI_OK) handle_error(1); */
    if(PAPI_stop_counters(counters,NUM_EVENTS) != PAPI_OK) handle_error(1);

    /*printf("the execution time is %f. \n", rtime);
    
    printf("the Mflops is %f.", mflops);*/
    
    printf("%lld L2 cache misses in %lld cycles, with L2 cash miss rate %.3lf \n", PAPI_events[0], PAPI_events[4], (double)PAPI_events[0]/PAPI_events[1] );
    
    printf("%lld L3 cache misses in %lld cycles, with L3 cash miss rate %.3lf \n", PAPI_events[2], PAPI_events[4], (double)PAPI_events[2]/PAPI_events[3] );

    /********** START FINALIZATION **********/
    finalization(file_in, total_iters, residual_ratio, nintci, nintcf, var, cgup, su, lcc, prefix);
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


