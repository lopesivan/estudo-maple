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
 * This example program illustrates how to use the readLineCallBack function
 * in an OpenMaple application.  Users are encouraged to use and modify  
 * this code as a starting point for learning the OpenMaple API.  
 *
 *********************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "maplec.h"

/* callback used for directing result output */
static void M_DECL textCallBack( void *data, int tag, const char *output )
{
    char *s;

    /* ignore the warning generated when a string contains a newline */
    if( tag == MAPLE_TEXT_WARNING 
        && !strncmp(output,"at offset ",10) 
	&& (s=strchr(output,',')) != NULL
	&& !strcmp(s,", incomplete string;  use \" to end the string") ) {
	return;
    }
    printf("%s\n",output);
}

/* callback used for capturing user input */
static char* M_DECL readLineCallBack( void *data, M_BOOL debug )
{
    static char input[1024];  /* static so it can be returned */
    static M_BOOL seen_debugger = FALSE;
    if( debug ) {
        if( !seen_debugger ) {
            printf("Entering the debugger.  Type help for list of possible commands\n");
            seen_debugger = TRUE;
        }
	printf("\nDBG ---> ");
    }
    else
	printf("\n---> ");
    fgets(input,1024,stdin);
    
    return( input );
}


int main( int argc, char *argv[] )
{
    char err[2048];  /* command input and error string buffers */
    MKernelVector kv;  /* Maple kernel handle */
    MCallBackVectorDesc cb = {  textCallBack, 
				0,   /* errorCallBack not used */
				0,   /* statusCallBack not used */
				readLineCallBack,
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

    /* do stuff that asks for user input */
    EvalMapleStatement(kv,"printf(\"Pick a number between 1 and 10\n\");");
    EvalMapleStatement(kv,"num := readline();");
    EvalMapleStatement(kv,"if num = rand() mod 10 then print(\"right!\"); else print(\"sorry.\"); fi;");
    EvalMapleStatement(kv,"stopat(int);");
    EvalMapleStatement(kv,"int(1/(x^4+1),x);");
    StopMaple(kv);

    return( 0 );
}
