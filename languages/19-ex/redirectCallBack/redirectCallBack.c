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
 * This example program illustrates how to use the redirectCallBack function
 * in an OpenMaple application.  Users are encouraged to use and modify  
 * this code as a starting point for learning the OpenMaple API.  
 *
 *********************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "maplec.h"

static FILE *outfile = NULL;

/* callback used for directing result outpui*/
static void M_DECL textCallBack( void *data, int tag, char *output )
{
    fprintf(outfile,"%s\n",output);
}

static M_BOOL M_DECL redirectCallBack( void *data, char *name, char *mode )
{
    if( !name ) { 
        if( outfile != stdout ) {
	    fclose(outfile);
	    outfile = stdout;
        }
    }
    else {
        if( outfile != stdout ) {
	    fclose(outfile);
        }
        if( !strcmp(name,"default") || !strcmp(name,"terminal") ) {
            outfile = stdout;
            return( TRUE );
        }
	printf("Opening %s\n",name);
        if( (outfile=fopen(name,mode)) == NULL ) {
	    printf("Could not open %s\n",name);
            outfile = stdout;
	    return( FALSE );
        }
    }

    return( TRUE );
}

int main( int argc, char *argv[] )
{
    int c;
    char err[2048];  /* command input and error string buffers */
    MKernelVector kv;  /* Maple kernel handle */
    MCallBackVectorDesc cb = {  textCallBack, 
				0,   /* errorCallBack not used */
				0,   /* statusCallBack not used */
				0,   /* readLineCallBack not used */
				redirectCallBack,
				0,   /* streamCallBack not used */
			        0,   /* queryInterrupt not used */ 
				0    /* callBackCallBack not used */
			    };

    /* set up default output stream */
    outfile = stdout;

    /* initialize Maple */
    if( (kv=StartMaple(argc,argv,&cb,NULL,NULL,err)) == NULL ) {
	printf("Fatal error, %s\n",err);
	return( 1 );
    }

    /* test redirecting output */
    EvalMapleStatement(kv,"writeto(\"redirect.out\");");
    EvalMapleStatement(kv,"randpoly(x);");
    EvalMapleStatement(kv,"writeto('terminal');");
    EvalMapleStatement(kv,"nextprime(97);");
    EvalMapleStatement(kv,"appendto(\"redirect.out\");");
    EvalMapleStatement(kv,"evalf(Pi,30);");
    StopMaple(kv);

    if( outfile != stdout )
        fclose(outfile);

    /* open the output file and dump the contents */
    if( (outfile = fopen("redirect.out","r")) ) {
        printf("\n\nContents of redirect log: \n\n");
        while( (c=fgetc(outfile)) != EOF )
            putchar(c);
        fclose(outfile);
    }

    return( 0 );
}
