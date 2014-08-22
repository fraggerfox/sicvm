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
 * Module Name: delay.c                                                *
 *                                                                     * 
 * Purpose: Implements the Interrupt Handling method in SIC. The       *
 *          various Interrupt Types are defined in the Header          *
 *                                                                     *
 * Comments: This is more or less a stub method, in future versions    *
 *           More detailed interrupt handling mechanisms will be       *
 *           implemented                                               *
 ***********************************************************************
 */
/*
 * TODO: To Implement more detailed interrupt service routines in SIC
 */

/* Custom Headers */
#include "interrupt.h"
#include "errorvm.h"

/*
 ***********************************************************************
 * Function Name: raiseInterrupt                                       *
 *                                                                     * 
 * Purpose: This function does the Interrupt action for the type of    *
 *          interrupt raised during the SIC execution                  *
 *                                                                     *  
 * Parameters: The Interrupt Type                                      * 
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: Refer the TODOs                                           *
 ***********************************************************************
 */
void raiseInterrupt (int iInterruptType)
{
    switch (iInterruptType) {
    case INT_OVF:
	display_error (INT_OVF);
	break;

    case INT_DIVZERO:
	display_error (INT_DIVZERO);
	break;

    case INT_SEGFAULT:
	display_error (INT_SEGFAULT);
	break;

    case INT_INVALID:
	display_error (INT_INVALID);
	break;

    case INT_BADMEM:
	display_error (INT_BADMEM);
	break;

    case INT_DEVERR:
	display_error (INT_DEVERR);
	break;
      
    case INT_BADBOOT:
	display_error (INT_BADBOOT);
	break;

    case INT_BADLOAD:
	display_error (INT_BADLOAD);

    case INT_OUTOFMEM:
	display_error (INT_OUTOFMEM);
	break;
    }
}


      
