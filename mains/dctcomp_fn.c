/* Copyright (c) Colorado School of Mines, 2011.*/
/* All rights reserved.                       */

/* DCTCOMP: $Revision: 1.6 $ ; $Date: 2011/11/17 00:17:48 $	*/


#include "../includes/comp.h"
#include "../includes/cwp.h"
#include "../includes/membitbuff.h"
#include "../includes/membuff.h"
#include "../includes/par.h"
#include <spawn.h>

/*
 * Author:  CWP: Tong Chen   Dec 1995
 */

/**************** end self doc ********************************/

int compress(int n1, int n2, int blocksize1, int blocksize2) {
	int intnpad1, npad2, nblock1, nblock2;
	int i1, i2, j1, j2, ibeg1, ibeg2, iblock1, iblock2, nsize;
	float **f, **g, **c1, **c2;
	int *qx;
	memBUFF *ibuff, *obuff;
	float error, ave, step;
		
	/* regular sizes */
	nblock1 = (n1-1)/blocksize1 + 1;
	nblock2 = (n2-1)/blocksize2 + 1;
	npad1 = nblock1*blocksize1;
	npad2 = nblock2*blocksize2;
	
	/* allocate spaces */
	f = alloc2float(npad1, npad2);
	g = alloc2float(blocksize1, blocksize2);
	qx = alloc1int(npad1*npad2);

	/* generate the transform tables */
	c1 = dctAlloc(blocksize1);
	c2 = dctAlloc(blocksize2);
	
	/* input data */
	for(i2=0; i2<n2; i2++) {
	    fread(f[i2],sizeof(float),n1,stdin);
	    for(i1=n1; i1<npad1; i1++) f[i2][i1] = 0.;
	 }
	
	/* test output of raw data */
	FILE *fp;
	fp = fopen("temp","w");
	for (i1 = 0; i1 < n2; i1++){
		fwrite(f[i1], sizeof(float), n1, fp);
	}
	fclose(fp);
	/***************************/

	/* pad with zeroes */
	for(i2=n2; i2<npad2; i2++)
		for(i1=0; i1<npad1; i1++)
			f[i2][i1] = 0.;
	
	/* DCT for each block */
	for(iblock2=0, ibeg2=0; iblock2<nblock2; 
	    iblock2++, ibeg2+=blocksize2)
	   for(iblock1=0, ibeg1=0; iblock1<nblock1; 
	       iblock1++, ibeg1+=blocksize1)
	   {
	      for(j2=0, i2=ibeg2; j2<blocksize2; i2++, j2++)
		 for(j1=0, i1=ibeg1; j1<blocksize1; i1++, j1++)
		    g[j2][j1] = f[i2][i1];
	      
	      dct_2(g, blocksize1, blocksize2, c1, c2, 0);
	      
	      for(j2=0, i2=ibeg2; j2<blocksize2; i2++, j2++)
		 for(j1=0, i1=ibeg1; j1<blocksize1; i1++, j1++)
		    f[i2][i1] = g[j2][j1];
	   }
	
	/* quantization */
	step = -1.;

	uniQuant(f[0], npad1*npad2, error, &ave, &step, qx);

	/* prefix encoding */
	ibuff = pEncode(qx, npad1*npad2);

	/* allocate out buffer */
	obuff = buffAlloc1(ibuff->mbound);

	/* rewind the in buffer */
	buffRewind(ibuff);
	
	/* run-length coding */
	codeSilence(ibuff, obuff);
	
	/* Huffman coding */
	buffRealloc1(obuff, obuff->pos);
	buffRewind(obuff);
	buffRewind(ibuff);
	nsize = huffCompress(obuff, ibuff);
	
	fprintf(stderr,"size after compression = %d bytes\n", nsize);
	fprintf(stderr,"compression ratio = %f\n", 
		(float)n1*n2*sizeof(int)/nsize);

	fwrite(&nsize, sizeof(int), 1, stdout);
	fwrite(&n1, sizeof(int), 1, stdout);
	fwrite(&n2, sizeof(int), 1, stdout);
	fwrite(&blocksize1, sizeof(int), 1, stdout);
	fwrite(&blocksize2, sizeof(int), 1, stdout);
	fwrite(&ave, sizeof(float), 1, stdout);
	fwrite(&step, sizeof(float), 1, stdout);
	fwrite(ibuff->code, sizeof(char), ibuff->pos, stdout);
	
	return EXIT_SUCCESS;
}
