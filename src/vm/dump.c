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
 * Module Name: dump.c                                                 *
 *                                                                     * 
 * Purpose: The dump function helps to dump the memory contents from   *
 *          SIC after it has executed a piece of code, this a utility  *
 *          function. It works from the SICvm command line where we    *
 *          give instructions to control the SIC machine.              *
 *                                                                     *
 * Comments: None as of now                                            *     
 ***********************************************************************
 */
/* Standard Headers */
#include <stdio.h>
#include <string.h>

/* Custom Headers */
#include "memory.h"
#include "register.h"

/*
 ***********************************************************************
 * Function Name: dump                                                 *
 *                                                                     * 
 * Purpose: This function dumps either SIC memory contents or SIC      *
 *          Register contents. And the current values can be viewed    *
 *                                                                     *  
 * Parameters: The commands are passed as the argument                 * 
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: Can we make a function to modify the contents of memory   *
 *           and registers during run time? This is still under        *
 *           throught process                                          *
 *           Need to modify this function for a better debugging       *
 *           functionality                                             *
 ***********************************************************************
 */
void dump (char *dCommand)
{
    int i = 0,rDump=0;
    int lRange = 0, uRange = 0;
    char temp[20],range[20] = "\0",t[20];
  
    if (ucrgMemory == NULL) {
	printf ("Memory Not Initialized, Cannot Dump\n");
	return;
    }
  
    if (strlen (dCommand) <= 2)   {
	printf ("Syntax Error!\n");
	printf ("Syntax is d [R | lower addr-upper addr]\n");
	printf ("Addresses are in HEX\n");
	return;
    }
  
    //Tokenize Arguments
    strcpy(t,dCommand);
    strtok(dCommand," ");
    strcpy(temp,strtok(NULL,","));
  
    //Determine if there is Register Dump Request
    if(temp[0] == 'r' || temp[0] == 'R') { 
	rDump=1;
	if (strchr (t, ',') != NULL) {
	    strcpy(range,strtok(NULL,"\n"));
	}
    }
    else {
	strcpy(range,temp); //Get Address Range
    }
  
    //Memory Dump
    if (range[0] != '\0') {   
	sscanf (range, "%x-%x", &lRange, &uRange);
	lRange-=lRange%16; 
	uRange+=16-uRange%16;

	for (i = lRange; i < uRange;i++){
	    if(i%16==0) {
		printf("\n%04X ",i);
	    }
	    else { 
		if(i%4==0) {
		    printf(" ");
		}
	    }
	    printf("%02X",ucrgMemory[i]); 	  
	}  
	printf ("\n");
    }

    //Register Dump
    if (rDump == 1) {
	printf ("\nA = %06X X = %06X L = %06X\nPC = %06X SW = %06X\n", A, X, L, PC, SW);
    }
}
