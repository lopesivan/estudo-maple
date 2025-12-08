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
 * This example program illustrates how to use the textCallBack function
 * in an OpenMaple application.  Users are encouraged to use and modify  
 * this code as a starting point for learning the OpenMaple API.  
 *
 *********************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "maplec.h"

/* callback used for directing result output */
static void M_DECL textCallBack( void *data, int tag, char *output )
{
    char *c;
    int i;
    static int last_tag = MAPLE_TEXT_OUTPUT;

    if( tag != last_tag && last_tag == MAPLE_TEXT_HELP ) 
	printf("\n????????????????????????????????????????????????????\n");

    switch( tag ) {
    case MAPLE_TEXT_PRETTY:
    case MAPLE_TEXT_OUTPUT:
	printf("%s\n",output);
        break;
    case MAPLE_TEXT_ERROR:
	printf("=================== ERROR! ==================\n");
	printf("%s\n",output);
	printf("=============================================\n");
        break;
    case MAPLE_TEXT_WARNING:
	printf("------------------ WARNING! -----------------\n");
	printf("%s\n",output);
	printf("---------------------------------------------\n");
        break;
    case MAPLE_TEXT_DIAG:
        /* write this out to the log file */
        if( data ) 
	    fprintf((FILE*)data,"%s\n",output);
        break;
    case MAPLE_TEXT_DEBUG:
    case MAPLE_TEXT_MISC:
        /* format the output inside a box */
	printf("[ ");
        for( i=0,c=output; ; ++c ) {
            if( *c == '\n' || *c == '\0' ) {
                for( i; i<75; ++i ) 
                    putchar(' ');
		printf("]\n");
                if( *c == '\0' )
                    break;
		printf("[ ");
                i = 0;
            }
            else {
                i++;
		putchar(*c);
            }
        }
        break;
    case MAPLE_TEXT_QUIT:
	printf("bye!\n");
        break;
    case MAPLE_TEXT_STATUS:
        /* don't display */
        break;
    case MAPLE_TEXT_HELP:
        if( last_tag != MAPLE_TEXT_HELP ) 
	    printf("\n????????????????????????????????????????????????????\n");
	printf("%s\n",output);
        break;
    default:
       printf("unrecognized tag %d\n",tag);
    }

    last_tag = tag;
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
				0    /* callBackCallBack not used */
			    };
    FILE *fd;
    int c;

    /* Open a file for writing diagnostic messages.
       Pass the file handle as the user_data parameter to StartMaple. 
    */
    fd = fopen("maple.out","w+");

    /* initialize Maple */
    if( (kv=StartMaple(argc,argv,&cb,(void*)fd,NULL,err)) == NULL ) {
	printf("Fatal error, %s\n",err);
	return( 1 );
    }

    /* do stuff that generates output */
    EvalMapleStatement(kv,"interface(screenwidth);");
    EvalMapleStatement(kv,"help(int);");
    EvalMapleStatement(kv,"showstat(int);");
    EvalMapleStatement(kv,"printlevel := 6;");
    EvalMapleStatement(kv,"int(1/(x^4+1),x);");
    EvalMapleStatement(kv,"1 := -1;");
    EvalMapleStatement(kv,"with(linalg):");
    StopMaple(kv);

    if( fd )
	fclose(fd);

    /* open the output file and dump the contents */
    if( (fd = fopen("maple.out","r")) ) {
        printf("\n\nContents of diagnostic log: \n\n");
	while( (c=fgetc(fd)) != EOF )
	    putchar(c);
	fclose(fd);
    }

    return( 0 );
}
