/*
 ***********************************************************************
 * CVS Information
 *    $Author$
 *    $Date$
 *    $Revision$
 *    $RCSfile$
 *    $Name$
 ***********************************************************************
 */
/*
 ***********************************************************************
 * Module Name: sicvm.c                                                *
 *                                                                     * 
 * Purpose: This is the start point of execution of the SICvm Virtual  *
 *          Machine. This initializes the following components of the  *
 *	    Machine. The code is meant to be a wrapper for the actual  *
 *          SIC Machine code.                                          *
 *                                                                     *
 *	    1) Registers & Memory				       *
 *	    2) I/O Devices					       *
 *	    3) Loads the Bootstrap				       *
 *								       *
 *	    After this process We can load the required Programs into  *
 *	    SIC Memory and execute them. This module acts as the UI for* 
 *	    the VM. The following commands can  be carried out from    *
 *	    the wrapper shell for the VM.			       *
 *	    							       *
 *	    i - initalize the SIC machine 			       *
 *	    r - load the bootstrap from DEV00			       *
 *	    b - bootstrap the SIC loader from DEVF1		       *
 *	    l - load a sic program present in DEVF2		       *
 *	    e - exec the sic program				       *
 *	    d - dump mem 					       *
 *                                                                     *
 * Comments: Refer the TODOs                                           *     
 ***********************************************************************
 */
/*
 * TODO: Support SIC/XE Instruction Set - well not in this module
 * TODO: The Machine Wrapper is still very basic, need to give more 
 *       control over the execution
 * TODO: No SIC Code Debugging option
 * TODO: Loads of Clean up to do :-(
 */

/* Standard Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Custom Headers */
#include "helpvm.h"
#include "dump.h"
#include "sicvm.h"
#include "bootstrap.h"
#include "memory.h"
#include "register.h"
#include "device.h"
#include "sic.h"

#define NO_INTERRUPT -1

char strCommand[MAXLENGTH];

int main (/*int argv, char *argc[]*/) 
{
    CLEARSCR(CLEARCMD);
    printf ("SICvm v1.2.0 beta");
    printf ("\nType h for Help\n");
    printf ("Report Bugs to (fragger_fox@users.sourceforge.net).\n\n");

    while (strCommand[0] != 'q') {
	printf ("ss>");
	fgets (strCommand, MAXLENGTH, stdin);
      
	switch (strCommand[0]) {
	case 'i':
	    if (init () == 0) {
		printf ("SIC Machine Initialized Successfully!!!\n");
	    } 
	    else {
		printf ("SIC Machine Initialization Failed!!!\n");
	    }

	    break;

	case 'r':
	    if (loadBootstrap (fpBootROM) == 0) {
		printf ("Bootstrap Loaded Successfully!\n");
	    } else {
		printf ("Bootstrap Failed!!!\n");
	    }

	    if (fpBootROM != NULL) {
		closeIODevice (fpBootROM);
	    }
	    break;

	case 'b':
	    if (run (BOOT) == NO_INTERRUPT) {
		printf ("Absolute Loader Initialized Successfully\n");
	    } 
	    else {
		printf ("Absolute Loader Initialization Failed\n");
		printf ("Please Reinitialize Machine\n");
	    }

	    if (fpInputDevice != NULL) {
		closeIODevice (fpInputDevice);
	    }
	    break;

	case 'l':
	    PC = LOADER_ADDRESS;
	    A = 0;

	    if (run (LOADER) == NO_INTERRUPT) {
		printf ("SIC Shell Loaded Successfully\n");
	    }
	    else {
		printf ("SIC Shell Loading Failed\n");
		printf ("Please Reinitialize Machine\n");
	    }
	    break;

	case 'e':
	    PC = SHELL_ADDRESS;
	    SW = SW | SU_MODE;

	    if (run (SHELL) == NO_INTERRUPT) {
		printf ("Shell Execution Completed Successful\n");
	    }
	    else {
		printf ("Shell Execution Terminated Abnormally\n");
	    }
	    
	    if (fpOutputDevice != NULL) {
		closeIODevice (fpOutputDevice);
	    }
	    break;

	case 'd':
	    dump (strCommand);
	    break;

	case 'c':
	    CLEARSCR(CLEARCMD);
	    printf ("SICvm v1.2.0 beta");
	    printf ("\nType h for Help\n");
	    printf ("Report Bugs to (fragger_fox@users.sourceforge.net).\n\n");
	    break;

	case 'h':
	    display_help ('c');
	    break;
	
	case 'q':
	    printf ("Machine Controller Terminated\n");
	    break;
      
	case '\n': case '\0':
	    break;
	
	default:
	    printf ("Command Not Found!\n");
	    break; //A Bit of Defensive Programming ;-)
	}
    }      
    return 0;
}
