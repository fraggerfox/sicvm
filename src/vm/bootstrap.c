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
 * Module Name: bootstrap.c                                            *
 *                                                                     * 
 * Purpose: The code to start any sort of execution in the SIC machine *
 *          is situated in its ROM, we use DEV00 as ROM in the SIC     *
 *          Machine, In an actual scenario (eg x86) The CPU starts     *
 *          executing the Bootstrap at a specific Hardcoded Memory     *
 *          location in the ROM. Unfortunately in SIC, we do not have  *
 *          this system, it starts to execute the Code Starting from   *
 *          0x00000 address of SIC's Initialized memory. This module   *
 *          loads the required Assembled SIC Bootstrap loader into the *
 *          SIC Memory which will start the bootstrap process in SIC.  *
 *                                                                     *
 * Comments: Refer the TODOs                                           *     
 ***********************************************************************
 */
//Standard Headers
#include <stdio.h>

//Custom Headers
#include "memory.h"

/*
 ***********************************************************************
 * Function Name: loadBootstrap                                        *
 *                                                                     * 
 * Purpose: This function reads the Bootstrap for SIC Located in DEV00 *
 *          (ROM) and puts it into the SIC Memory, the Bootloader is   *
 *          around 128 Bytes (1/4 of a x86 machine). The function reads* 
 *          DEV00 one character at a time and then converts 2          *   
 *          consecutive characters into a Hex Value that will be put   *   
 *          into SIC memory starting from 0x00000                      *  
 *                                                                     *  
 * Parameters: The file pointer to the BootROM                         * 
 *                                                                     *
 * Returns: 0 on Successful Load else 1                                *
 *                                                                     *
 * Comments: Since the File is read using normal C Libraries each      *
 *           character will be a byte but in reality 2 consecutive     *
 *           Characters will form a Hex Value that was got from the    *
 *           Assembled SIC Instruction Set                             * 
 *           Also this function does not initialize the connection     *
 *           to DEV00 rather it just copies instructions from the      *
 *           device "boot" into the memory. boot is initialized when   *
 *           SIC machine is initialized.                               *
 ***********************************************************************
 */
int loadBootstrap (FILE *fpBootROM)
{
    int i = 0;
    int iReturnValue = 0;
    int iActualHexValue = 0x00;
    char szStringHexValue[3]; 

    szStringHexValue[2] = '\n';
  
    if (fpBootROM == NULL) {
	iReturnValue = 1;
    } else {
	while (!feof(fpBootROM)) {
	    szStringHexValue[0] = fgetc (fpBootROM);
	  
	    if ((szStringHexValue[0] != '\n') && (szStringHexValue[0] != EOF)) {
		szStringHexValue[1] = fgetc (fpBootROM);
		sscanf (szStringHexValue, "%x", &iActualHexValue);
		ucrgMemory[i++] = iActualHexValue;
	    }
	  
	}
	iReturnValue = 0;
    }

    return iReturnValue;
}
