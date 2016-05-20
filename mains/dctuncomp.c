/* Copyright (c) Colorado School of Mines, 2011.*/
/* All rights reserved.                       */

/* DCTUNCOMP: $Revision: 1.5 $ ; $Date: 2011/11/17 00:17:48 $	*/

#include "../includes/comp.h"
#include "../includes/cwp.h"
#include "../includes/membitbuff.h"
#include "../includes/membuff.h"
#include "../includes/par.h"
#include "dctuncomp_fn.h"

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

int
main(int argc, char **argv)
{
	int nsize, n1, n2, blocksize1, blocksize2;
	float ave, step;

	initargs(argc, argv);
	requestdoc(1);

	/* get the parameters */
	fread(&nsize, sizeof(int), 1, stdin);
	fread(&n1, sizeof(int), 1, stdin);
	fread(&n2, sizeof(int), 1, stdin);
	fread(&blocksize1, sizeof(int), 1, stdin);
	fread(&blocksize2, sizeof(int), 1, stdin);
	fread(&ave, sizeof(float), 1, stdin);
	fread(&step, sizeof(float), 1, stdin);
	
	return decompress(nsize, n1, n2, blocksize1, blocksize2, ave, step);
}
