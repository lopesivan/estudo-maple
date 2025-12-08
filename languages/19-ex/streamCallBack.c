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
 * This example program illustrates how to use the streamCallBack function
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

static void app_help( M_INT nargs, char **args ) 
{
    if( nargs < 0 ) {
        printf("Error, unspecified topic\n");
        return;
    }
    if( strcmp(args[0],"app_test") ) {
        printf("Usage: app_test(args)\n");
        printf("       - displays args in reverse order\n");
    }
    else {
        printf("unrecognized topic %s\n",args[0]);
    }
}

static char *app_test( M_INT nargs, char **args ) 
{
    M_INT i;
    static char ret[30];

    for( i=nargs-1; i>=0; --i ) {
        printf("args[%ld] = %s\n",i,args[i]);
    }
    sprintf(ret,"%d;",nargs);

    return( ret );
}

static char * M_DECL streamCallBack( void *data, const char *name,
                                       M_INT nargs, char **args )
{
    if( strcmp(name,"app_version") == 0 ) {
        return( "1.1;" );
    }
    else if( strcmp(name,"app_help") == 0 ) {
        app_help(nargs,args); 
        return( NULL );
    }
    else if( strcmp(name,"app_test") == 0 ) {
        return( app_test(nargs,args) ); 
    }
    printf("unrecognized stream %s\n",name);
    return( NULL );
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
				streamCallBack, 
			        0,   /* queryInterrupt not used */ 
				0    /* callBackCallBack not used */
			    };

    /* initialize Maple */
    if( (kv=StartMaple(argc,argv,&cb,NULL,NULL,err)) == NULL ) {
	printf("Fatal error, %s\n",err);
	return( 1 );
    }

    /* setup a package */
    EvalMapleStatement(kv,"app := module() export help, test, version; "
        "help := proc(topic) streamcall(INTERFACE_app_help(topic)) end; "
        "version := proc() streamcall(INTERFACE_app_version()) end; "
        "test := proc() streamcall(INTERFACE_app_test(args)) end; "
        "end module;");
    StopMaple(kv);

    /* execute some of the package commands */
    EvalMapleStatement(kv,"with(app);");
    EvalMapleStatement(kv,"version();");
    EvalMapleStatement(kv,"help(test);");
    EvalMapleStatement(kv,"test(1,2,3,4);");

    return( 0 );
}
