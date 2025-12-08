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
 * This example program illustrates how to use the callBackCallBack function
 * in an OpenMaple application.  Users are encouraged to use and modify  
 * this code as a starting point for learning the OpenMaple API.  
 *
 *********************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "maplec.h"

static M_BOOL show_output = FALSE;

/* callback used for directing result output */
/* hide all output except when processing the given iteration */
static void M_DECL textCallBack( void *data, int tag, const char *output )
{
    if( show_output ) 
	printf("%s\n",output);
}

/* when this function is called with args = "get_data", get a
   number from 1..100 from the user.  Use this input to decide
   which iteration of the loop to display the output of.  During 
   this iteration, compute nextprime( 2*i );
*/
static char* M_DECL callBackCallBack( void *data, char *args )
{
    static int num = 0;
    int iter, r;

    if( strcmp(args,"\"get_data\"") == 0 ) {
	printf("Pick a number from 1 .. 100\n> ");
        while( (r=scanf("%d",&num)) != 1 || num < 1 || num > 100 ) {
            if( r != 1 ) scanf("%*s");  /* clear input */
	    printf("Invalid input.  Try again.\n");
            printf("Pick a number from 1 .. 100\n> ");
        }
	printf("I will show you the output of iteration #%d of my loop\n", num);
        show_output = FALSE;
        return( NULL );
    }
    else if( sscanf(args,"%d",&iter) == 1 && iter == num ) {
	show_output = TRUE;
        return( "nextprime( 2*i );" );
    }
    else {
	show_output = FALSE;
	return( NULL );
    }
}

int main( int argc, char *argv[] )
{
    char err[2048];  /* command input and error string buffers */
    MKernelVector kv;  /* Maple kernel handle */
    MCallBackVectorDesc cb = {  textCallBack, 
				0,   /* errorCallBack not used */
				0,   /* statusCallBack not used */
				0,   /* readLineCallBack not used */
				0,   /* redirectCallBack not used */
				0,   /* streamCallBack not used */
			        0,   /* queryInterrupt not used */ 
				callBackCallBack
			    };

    /* initialize Maple */
    if( (kv=StartMaple(argc,argv,&cb,NULL,NULL,err)) == NULL ) {
	printf("Fatal error, %s\n",err);
	return( 1 );
    }

    /* call through to your callback handler to setup some data */
    EvalMapleStatement(kv,"callback(\"get_data\");");

    /* execute a callback at the beginning of each iteration of a loop. */
    EvalMapleStatement(kv,"for i to 100 do callback(i); eval(%)*x^2; od;");
    StopMaple(kv);

    return( 0 );
}
