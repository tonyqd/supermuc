/**
 * Finalization step - write results and other computational vectors to files
 *
 * @date 22-Oct-2012
 * @author V. Petkov
 */

#ifndef FINALIZATION_H_
#define FINALIZATION_H_

void finalization(char* file_in, int total_iters, double residual_ratio, int nintci, int nintcf, double* var, double* cgup, double* su, int** lcc, char *prefix, long long *counters, float rtime, float ptime, float mflops, long long flpops);

#endif /* FINALIZATION_H_ */

