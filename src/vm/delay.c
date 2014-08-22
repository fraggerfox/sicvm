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
 * Purpose: Implements the Device Delay. This module causes Device     *
 *          delay emulation for SIC's I/O Devices                      *
 *                                                                     *
 * Comments: This module can be excluded with any change in            *     
 *           functionality                                             *
 ***********************************************************************
 */
//Custom Headers
#include "delay.h"

/*
 ***********************************************************************
 * Function Name: delay                                                *
 *                                                                     * 
 * Purpose: This function causes a an empty loop to run for DELAY_TIME *
 *          times as defined in the header of delay                    *
 *                                                                     *  
 * Parameters: None                                                    * 
 *                                                                     *
 * Returns: None Successful Load else 1                                *
 *                                                                     *
 * Comments: None                                                      *
 ***********************************************************************
 */
void delay (void)
{
    long int liCount;

    //DELAY_TIME defined in "delay.h"
    for (liCount = 0; liCount < DELAY_TIME; liCount++) {
	; //Do-Nothing loop to cause the Delay
    }
}
