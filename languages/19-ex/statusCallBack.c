/* ***********************************************************************
 * OpenMaple Example Program
 *
 * Copyright (c) Maplesoft, a division of Waterloo Maple Inc. 2004.
 * You are permitted to copy, modify and distribute this code, as long as
 * this copyright notice is prominently included and remains intact. If any
 * modifications were done, a prominent notice of the fact that the code has
 * been modified, as well as a list of the modifications, must also be
 * included. To the maximum extent permitted by applicable laws, this
 * material is provided "as is" without any warranty or condition of any kind.
 *
 * This example program illustrates how to use the statusCallBack function
 * in an OpenMaple application.  Users are encouraged to use and modify  
 * this code as a starting point for learning the OpenMaple API.  
 *
 *********************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "maplec.h"

/* callback used for directing result output */
static void M_DECL textCallBack( void *data, int tag, const char *output )
{
    /* do nothing (disable regular output) */
}

static void M_DECL statusCallBack( void *data, long kilobytesUsed,
                                     long kilobytesAlloc, double cpuTime )
{
    static long lastAlloc = 0;

    printf("Total time = %.2f; ",cpuTime);
    printf("New memory since last status message = %ld kB\n",
        (kilobytesAlloc - lastAlloc) );
    lastAlloc = kilobytesAlloc;
}

int main( int argc, char *argv[] )
{
    char err[2048];  /* command input and error string buffers */
    MKernelVector kv;  /* Maple kernel handle */
    MCallBackVectorDesc cb = {  textCallBack, 
				0,   /* errorCallBack not used */
				statusCallBack,
				0,   /* readLineCallBack not used */
				0,   /* redirectCallBack not used */
				0,   /* streamCallBack not used */
			        0,   /* queryInterrupt not used */ 
				0    /* callBackCallBack not used */
			    };

    /* initialize Maple */
    if( (kv=StartMaple(argc,argv,&cb,NULL,NULL,err)) == NULL ) {
	printf("Fatal error, %s\n",err);
	return( 1 );
    }

    /* bump up the frequency of gc's so we get more status updates */
    MapleKernelOptions(kv,"gcfreq",ToMapleInteger(kv,100000));

    /* do something that generates status messages */
    EvalMapleStatement(kv,"int(1/(randpoly(x)^4+1),x);");
    StopMaple(kv);

    return( 0 );
}
