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
 * This example program illustrates how to use the queryInterrupt callback 
 * in an OpenMaple application.  Users are encouraged to use and modify
 * this code as a starting point for learning the OpenMaple API.
 *
 *********************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>

#include "maplec.h"

#ifdef _MSC_VER
#  define CDECL __cdecl
#else
#  define CDECL
#endif

/* global variable used by queryInterrupt() */
static int Interrupted = 0;

/* interrupt signal handler: sets global variable when user hits Ctrl-C */
void CDECL catch_intr( int signo )
{
    Interrupted = TRUE;
    signal(SIGINT,catch_intr);
#ifdef _MSC_VER
    signal(SIGBREAK,catch_intr);
#endif
}

/* interrupt callback: stops computation when `Interrupted' is true */
static M_BOOL M_DECL queryInterrupt( void *data )
{
    if( Interrupted ) {
  	return( TRUE );
    }
    return( FALSE );
}

/* callback used for directing result output */
static void M_DECL textCallBack( void *data, int tag, const char *output )
{
    printf("%s\n",output);
}

/* simple program to execute a long loop */
int main( int argc, char *argv[] )
{
    char err[2048];  /* error string buffers */
    MKernelVector kv;  /* Maple kernel handle */
    MCallBackVectorDesc cb = {  textCallBack, 
				0,   /* errorCallBack not used */
				0,   /* statusCallBack not used */
				0,   /* readLineCallBack not used */
				0,   /* redirectCallBack not used */
				0,   /* streamCallBack not used */
			        queryInterrupt, 
				0    /* callBackCallBack not used */
			    };
    ALGEB r;

    /* initialize Maple */
    if( (kv=StartMaple(argc,argv,&cb,NULL,NULL,err)) == NULL ) {
	printf("Fatal error, %s\n",err);
	return( 1 );
    }
 
    /* catch ^C */
    signal(SIGINT,catch_intr);


    printf("Executing a loop to 2^20.  Press Ctrl-C or Ctrl-Break to stop.\n");
    EvalMapleStatement(kv,"for i to 2^20 do if i mod 1000 = 0 then print(i); fi; od:");

    if( Interrupted )
        printf("\nThank you for preventing the waste of valuable CPU cycles!\n");
    else
        printf("I can't believe you let that finish!");

    r = EvalMapleStatement(kv,"i:");
    printf("The last value of i was %ld\n",(long)MapleToM_INT(kv,r));
    StopMaple(kv);         

    return( 0 );
}
