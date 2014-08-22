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
 * Module Name: errorvm.c                                              *
 *                                                                     * 
 * Purpose: This module define the various actions that needs to be    *
 *          taken in case of an error is raised at run time in the     *
 *          SIC Machine.                                               *
 *                                                                     *
 * Comments: Refer the TODOs                                           *     
 ***********************************************************************
 */
/*
 * TODO: This is a stub functions, implemented to complete the 
 *       functionality
 */
/* Standard Headers */
#include <stdio.h>
#include <string.h>

/* Custom Headers */
#include "errorvm.h"

#define MAX_MESSAGES 20
#define MAX_LENGTH 100

//An Array of Error Message Strings
char strrgErrorMessage[MAX_MESSAGES][MAX_LENGTH];

/*
 ***********************************************************************
 * Function Name: display_error                                        *
 *                                                                     * 
 * Purpose: This function does the action of displaying the Error      *
 *          message to the user using the SIC Machine, in future       *
 *          version of SIC, this function needs to be implemented as   * 
 *          SIC Assembly code, in the memory                           *   
 *          Currently other than displaying error messages no other    *   
 *          actions are performed                                      *  
 *                                                                     *  
 * Parameters: The Error Type, Currently there is a one-to-one mapping * 
 *             between interrupt types                                 *
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: This is only a stub function, lots of improvements are    *
 *           pending.                                                  *
 ***********************************************************************
 */
void display_error(int iErrorType)
{
    strcpy (strrgErrorMessage[0], "Arithmetic overflow");
    strcpy (strrgErrorMessage[1], "Divide by zero error");
    strcpy (strrgErrorMessage[2], "Illegal Memory Access");
    strcpy (strrgErrorMessage[3], "Invalid Instruction");
    strcpy (strrgErrorMessage[4], "Memory Initializtion Failed");
    strcpy (strrgErrorMessage[5], "Bad Device or Device Inaccessible");
    strcpy (strrgErrorMessage[6], "No Boot Device (DEV00) Found");
    strcpy (strrgErrorMessage[7], "No Loader Device (DEVF1) Found");
    strcpy (strrgErrorMessage[8], "Memory Out of Bounds, Greater than 1 MB");
    
    //Print the Errors to stderr
    fprintf(stderr, "Abnormal Termination : %s\n",strrgErrorMessage[iErrorType]);
}

