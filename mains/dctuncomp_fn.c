/* Copyright (c) Colorado School of Mines, 2011.*/
/* All rights reserved.                       */

/* DCTUNCOMP: $Revision: 1.5 $ ; $Date: 2011/11/17 00:17:48 $	*/

#include "../includes/comp.h"
#include "../includes/cwp.h"
#include "../includes/membitbuff.h"
#include "../includes/membuff.h"
#include "../includes/par.h"

/*********************** self documentation **********************/
char *sdoc[] = {
"									",
" DCTUNCOMP - Discrete Cosine Transform Uncompression 			",
"									",
"   dctuncomp < stdin >stdout 						",
"									",
" Required Parameters:							",
" none									",
" Optional Parameters:							",
" none									",
"									",
" Notes:								",
" The input of this program is a file compressed by dctcomp.		",
"									",
NULL};

/*
 * Author:  CWP:  Tong Chen  Dec 1995
 */

/**************** end self doc ********************************/
int decompress(void* input, void* output) 
{	
	int npad1, npad2, nblock1, nblock2;
	int i1, i2, j1, j2, ibeg1, ibeg2, iblock1, iblock2, nmax;
	int* pInt;
	float* pFloat;
	float **f, **g, **c1, **c2;
	int *qx;
	memBUFF *ibuff, *obuff;
		
	int nsize, n1, n2, blocksize1, blocksize2;
	float ave, step;
	unsigned int offset;
	const unsigned int FLOAT_HDR = 5*sizeof(int);
	const unsigned int DATA_START = FLOAT_HDR + 2*sizeof(float);
	
	printf("Input Pointer %u\n", (unsigned int)input);
	printf("Input Pointer %u\n", (unsigned int)output);
		
	pInt = (int*)input;
		
	nsize = *pInt++;
	n1 = *pInt++;
	n2 = *pInt++;
	blocksize1 = *pInt++;
	blocksize2 = *pInt++;
	pFloat = (float*)(input+FLOAT_HDR);
	ave = *pFloat++;
	step = *pFloat;
	
	printf("Reading header data\n");
	printf("nsize %u\n", nsize);
	printf("n1 %d\n", n1);
	printf("n2 %d\n", n2);
	printf("blocksize1 %d\n", blocksize1);
	printf("blocksize2 %d\n", blocksize2);
	printf("ave %f\n", ave);
	printf("ave %f\n", step);
	
	return 0;

	/* get the parameters */
	// fread(&nsize, sizeof(int), 1, input);
	// fread(&n1, sizeof(int), 1, input);
	// fread(&n2, sizeof(int), 1, input);
	// fread(&blocksize1, sizeof(int), 1, input);
	// fread(&blocksize2, sizeof(int), 1, input);
	// fread(&ave, sizeof(float), 1, input);
	// fread(&step, sizeof(float), 1, input);
	
	
		
	/* read data */
	ibuff = buffAlloc1(nsize);
	fread(ibuff->code, sizeof(char), nsize, stdin);	
		

	/* regular sizes */
	nblock1 = (n1-1)/blocksize1 + 1;
	nblock2 = (n2-1)/blocksize2 + 1;
	npad1 = nblock1*blocksize1;
	npad2 = nblock2*blocksize2;
	
	/* allocate spaces */
	f = alloc2float(npad1, npad2);
	g = alloc2float(blocksize1, blocksize2);
	qx = alloc1int(npad1*npad2);

	/* allocate buffers */
	nmax = 2*npad1*npad2;
	obuff = buffAlloc1(nmax);
	
	/* Huffman decoding */
	if(huffDecompress(ibuff, obuff) == MEM_EOB) 
	   err("Inconsistent data \n");
	
	/* run-length decoding */
	buffRealloc1(obuff, obuff->pos);
	buffRewind(obuff);
	buffRealloc1(ibuff, nmax);
	buffRewind(ibuff);
	codeDesilence(obuff, ibuff);
	
	/* prefix decoding */
	buffRealloc1(ibuff, ibuff->pos);
	buffRewind(ibuff);
	if(pDecode(ibuff, qx, npad1*npad2) == MEM_EOB) 
	   err("Inconsistent data \n");
	
	/* dequantization */
	uniDequant(f[0], npad1*npad2, ave, step, qx);

	/* generate the transform tables */
	c1 = dctAlloc(blocksize1);
	c2 = dctAlloc(blocksize2);
	
	/* inverse DCT for each block */
	for(iblock2=0, ibeg2=0; iblock2<nblock2; 
	    iblock2++, ibeg2+=blocksize2)
	   for(iblock1=0, ibeg1=0; iblock1<nblock1; 
	       iblock1++, ibeg1+=blocksize1)
	   {
	      for(j2=0, i2=ibeg2; j2<blocksize2; i2++, j2++)
		 for(j1=0, i1=ibeg1; j1<blocksize1; i1++, j1++)
		    g[j2][j1] = f[i2][i1];
	      
	      dct_2(g, blocksize1, blocksize2, c1, c2, 1);
	      
	      for(j2=0, i2=ibeg2; j2<blocksize2; i2++, j2++)
		 for(j1=0, i1=ibeg1; j1<blocksize1; i1++, j1++)
		    f[i2][i1] = g[j2][j1];
	   }

	for(i2=0; i2<n2; i2++)
	   fwrite(f[i2], sizeof(float), n1, stdout);
	
	return EXIT_SUCCESS;
}





