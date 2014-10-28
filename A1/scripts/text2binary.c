/**
 * Helper functions for reading from input data file
 *
 * @author M.Bujny, Y.Dong
 * @date 22-May-2009, 22-Oct-2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main(int *argc, char *argv[])
{
  
  int i, j;
  char *filename_txt, *filename_bin;
  int nintci;
  int nintcf; 
  int nextci; 
  int nextcf; 
  int **lcc;
  double *bs, *be, *bn, *bw, *bl, *bh, *bp, *su;
  
  filename_txt = argv[1];
  filename_bin = argv[2];
  
  printf("%s\n", filename_txt);
  
  // Part of the code from util_read_files.c
  //=================================================================================
  FILE *fp = fopen(filename_txt, "r");
  if (fp == NULL)
  {
    printf("Error opening file %s\n", filename_txt);
    return -1;
  }
  //4 variables in total!!!
  fscanf(fp, "%d", &nintci);
  fscanf(fp, "%d", &nintcf);
  fscanf(fp, "%d", &nextci);
  fscanf(fp, "%d", &nextcf);
  //allocating lcc
  if ((lcc = (int**) malloc( ( (nintcf) - (nintci) + 1) * sizeof(int*) )) == NULL)
  {
    printf("malloc failed to allocate first dimension of lcc (nintcf)");
    return -1;
  }
  for (i = (nintci); i <= (nintcf); i++)
  {
    if ((lcc[i] = (int *) malloc( 6 * sizeof(int) )) == NULL)
    {
      printf("malloc(lcc) failed\n");
      return -1;
    }
  }
  
  //start reading lcc
  //note that c array index starts from 0 while fortran starts from 1!
  for (i = nintci; i <= nintcf; i++)
  {
    fscanf(fp, "%d", &(lcc[i][0]));
    fscanf(fp, "%d", &(lcc[i][1]));
    fscanf(fp, "%d", &(lcc[i][2]));
    fscanf(fp, "%d", &(lcc[i][3]));
    fscanf(fp, "%d", &(lcc[i][4]));
    fscanf(fp, "%d", &(lcc[i][5]));
  }
  
  
  // allocate other arrays
  if ((bs = (double *) malloc((nextcf + 1) * sizeof(double))) == NULL)
  {
    printf("malloc() failed\n");
    return -1;
  }
  if ((be = (double *) malloc((nextcf + 1) * sizeof(double))) == NULL)
  {
    printf("malloc() failed\n");
    return -1;
  }
  if ((bn = (double *) malloc((nextcf + 1) * sizeof(double))) == NULL)
  {
    printf("malloc() failed\n");
    return -1;
  }
  if ((bw = (double *) malloc((nextcf + 1) * sizeof(double))) == NULL)
  {
    printf("malloc() failed\n");
    return -1;
  }
  if ((bl = (double *) malloc((nextcf + 1) * sizeof(double))) == NULL)
  {
    printf("malloc() failed\n");
    return -1;
  }
  if ((bh = (double *) malloc((nextcf + 1) * sizeof(double))) == NULL)
  {
    printf("malloc() failed\n");
    return -1;
  }
  if ((bp = (double *) malloc((nextcf + 1) * sizeof(double))) == NULL)
  {
    printf("malloc() failed\n");
    return -1;
  }
  if ((su = (double *) malloc((nextcf + 1) * sizeof(double))) == NULL)
  {
    printf("malloc() failed\n");
    return -1;
  }
  
  // read the other arrays
  for (i = nintci; i <= nintcf; i++)
  {
    fscanf(fp, "%lf", &(bs[i]));
    fscanf(fp, "%lf", &(be[i]));
    fscanf(fp, "%lf", &(bn[i]));
    fscanf(fp, "%lf", &(bw[i]));
    fscanf(fp, "%lf", &(bl[i]));
    fscanf(fp, "%lf", &(bh[i]));
    fscanf(fp, "%lf", &(bp[i]));
    fscanf(fp, "%lf", &(su[i]));
  }
  
  fclose(fp);
  //=================================================================================
  
  // Writing to the binary file
  
  FILE *fbin = fopen(filename_bin, "wb");
  
  if (!fbin)
  {
    printf("Unable to open file!");
    return 1;
  }
  
  fwrite(&nintci, sizeof(int), 1, fbin);
  fwrite(&nintcf, sizeof(int), 1, fbin);
  fwrite(&nextci, sizeof(int), 1, fbin);
  fwrite(&nextcf, sizeof(int), 1, fbin);
  
  for (i = nintci; i <= nintcf; i++)
  {
    fwrite(lcc[i], sizeof(int), 6, fbin);
  }
  
  fwrite(bs, sizeof(double), (nextcf + 1), fbin);
  fwrite(be, sizeof(double), (nextcf + 1), fbin);
  fwrite(bn, sizeof(double), (nextcf + 1), fbin);
  fwrite(bw, sizeof(double), (nextcf + 1), fbin);
  fwrite(bl, sizeof(double), (nextcf + 1), fbin);
  fwrite(bh, sizeof(double), (nextcf + 1), fbin);
  fwrite(bp, sizeof(double), (nextcf + 1), fbin);
  fwrite(su, sizeof(double), (nextcf + 1), fbin);
  
  
  fclose(fbin);
  
  //=================================================================================
  
  // Reading from binary file
  
  fbin = fopen(filename_bin, "rb");
  
  if (!fbin)
  {
    printf("Unable to open file!");
    return 1;
  }
  
  fread(&nintci, sizeof(int), 1, fbin);
  fread(&nintcf, sizeof(int), 1, fbin);
  fread(&nextci, sizeof(int), 1, fbin);
  fread(&nextcf, sizeof(int), 1, fbin);
  
  for (i = nintci; i <= nintcf; i++)
  {
    fread(lcc[i], sizeof(int), 6, fbin);
  }
  
  fread(bs, sizeof(double), (nextcf + 1), fbin);
  fread(be, sizeof(double), (nextcf + 1), fbin);
  fread(bn, sizeof(double), (nextcf + 1), fbin);
  fread(bw, sizeof(double), (nextcf + 1), fbin);
  fread(bl, sizeof(double), (nextcf + 1), fbin);
  fread(bh, sizeof(double), (nextcf + 1), fbin);
  fread(bp, sizeof(double), (nextcf + 1), fbin);
  fread(su, sizeof(double), (nextcf + 1), fbin);
  
  fclose(fbin);
  
  //   //TODO: Just printinf out the array -> to be deleted
  //   for (i = nintci; i <= nintcf; i++)
  //   {
  //     for (j = 0; j < 6; j++)
  //     {
  //       printf("%d\t", lcc[i][j]);
  //     }
  //     printf("\n");
  //   }
  //   
  //   for (i = nintci; i <= nintcf; i++)
  //   {
  //     printf("%lf\t", (bs[i]));
  //     printf("%lf\t", (be[i]));
  //     printf("%lf\t", (bn[i]));
  //     printf("%lf\t", (bw[i]));
  //     printf("%lf\t", (bl[i]));
  //     printf("%lf\t", (bh[i]));
  //     printf("%lf\t", (bp[i]));
  //     printf("%lf\n", (su[i]));
  //   }
  
  
  
  
  //=================================================================================
  // Freeing the memory
  free(su);
  free(bp);
  free(bh);
  free(bl);
  free(bw);
  free(bn);
  free(be);
  free(bs);
  
  for (i=0; i < (nintcf - nintci + 1); i++)
  {
    free(lcc[i]);
  }
  free(lcc);
  
}