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
 * This example program illustrates how to use the errorCallBack function
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
    printf("%s\n",output);
}

static char *input;

static void M_DECL errorCallBack( void *data, M_INT offset, const char *msg )
{
    M_INT i;

    if( offset < 0 )
	printf("%s\n",msg);
    else {
        /* put ^^^ under the original input to indicate where 
           the syntax error probably was
        */
	printf("Syntax Error, %s\n\t> %s\n\t",msg,input);
        for( i=0; i<offset; ++i ) 
            putchar(' ');
	printf("^\n");
    }
}

int main( int argc, char *argv[] )
{
    char err[2048];  /* command input and error string buffers */
    MKernelVector kv;  /* Maple kernel handle */
    MCallBackVectorDesc cb = {  textCallBack, 
				errorCallBack,
				0,   /* statusCallBack not used */
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

    /* do stuff that generates errors */
    EvalMapleStatement(kv,"int();");
    EvalMapleStatement(kv,"1 := -1;");
    input = "val := a ++ b;";
    EvalMapleStatement(kv,input);
    StopMaple(kv);

    return( 0 );
}
