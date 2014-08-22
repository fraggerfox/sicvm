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
 * Module Name: helpvm.c                                               *
 *                                                                     * 
 * Purpose: A simple module that prints out variour help text on user  *
 *          request                                                    *
 *                                                                     *
 * Comments: This module needs to be developed further for being       *     
 *           helpful to the user                                       *
 ***********************************************************************
 */
/*
 * TODO: Its supposed to help the user But its not yet fully implemented
 *       So its not very helpful yet
 */

/* Standard Headers */
#include <stdio.h>

/* Custom Headers */
#include "helpvm.h"

/*
 ***********************************************************************
 * Function Name: display_help                                         *
 *                                                                     * 
 * Purpose: This function displays helps for each of the implemented   *
 *          command for the controlling the SIC machine.               *
 *                                                                     *  
 * Parameters: None                                                    * 
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: Need to implement more help functionality                 *
 ***********************************************************************
 */
void display_help (char cCommand)
{
    switch (cCommand) {
    default:
	printf ("Currently Supported Command List\n");
	printf ("i - Initialize the SIC Machine\n");
	printf ("r - Load the Bootstrap Program from DEV00 into SIC's Memory\n");
	printf ("b - Bootstrap the SIC Loader from DEVF1 to SIC's Memory\n");
	printf ("l - Load the executable program from DEVF2 to SIC's Memory\n");
	printf ("e - Execute the Loaded program in SIC's Memory\n");
	printf ("d - Display Contents of Registers and memory\n");
	printf ("c - Clear Screen\n");
	printf ("q - Stop SIC Machine Controller\n");
	break;
    }
}
