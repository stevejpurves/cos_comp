/* Copyright (c) Colorado School of Mines, 2011.*/
/* All rights reserved.                       */

/* DCTCOMP: $Revision: 1.6 $ ; $Date: 2011/11/17 00:17:48 $	*/


#include "../includes/comp.h"
#include "../includes/cwp.h"
#include "../includes/membitbuff.h"
#include "../includes/membuff.h"
#include "../includes/par.h"
#include <spawn.h>
#include "dctcomp_fn.h"

/*********************** self documentation **********************/
char *sdoc[] = {
"									",
" DCTCOMP - Compression by Discrete Cosine Transform			",
"									",
"   dctcomp < stdin n1= n2=   [optional parameter] > sdtout		",
"									",
" Required Parameters:							",
" n1=			number of samples in the fast (first) dimension	",
" n2=			number of samples in the slow (second) dimension",
" Optional Parameters:							",
" blocksize1=16		blocksize in direction 1			",
" blocksize2=16		blocksize in direction 2			",
" error=0.01		acceptable error				",
"									",
NULL};

/*
 * Author:  CWP: Tong Chen   Dec 1995
 */

/**************** end self doc ********************************/

int
main(int argc, char **argv)
{
	int n1, n2, blocksize1, blocksize2;
	initargs(argc, argv);
	requestdoc(1);

	/* get the parameters */
	if(!getparint("n1",&n1)) err("Must specify n1");
	if(!getparint("n2",&n2)) err("Must specify n2");
	if(!getparint("blocksize1",&blocksize1)) blocksize1 = 16;
	if(!getparint("blocksize2",&blocksize2)) blocksize2 = 16;
	if(!getparfloat("error",&error)) error=.01;
    checkpars();
		
	return compress(n1, n2, blocksize1, blocksize2); 
}
