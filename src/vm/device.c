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
 * Module Name: device.c                                               *
 *                                                                     * 
 * Purpose: The I/O device utility functions, and their definition is  *
 *          given here, to do operations with IO Devices in SIC        *
 *          since most of the code was written in a hurry, I am not    *  
 *          satisfied with the code                                    *  
 *                                                                     *  
 * Comments: Need to do more refinement here                           *     
 ***********************************************************************
 */

//Standard Headers
#include <stdio.h>

//Custom Headers
#include "device.h"

/*
 ***********************************************************************
 * Function Name: initIODevice                                         *
 *                                                                     * 
 * Purpose: The function initialized the IO Devices to the Respective  *
 *          file pointers and make them either point to Input or Output*
 *          as per the given Device Type, BOOT Device is a Read only   *  
 *          device, the initialization can also be one to a standard   *  
 *          input or standard output device too.                       *  
 *                                                                     *  
 * Parameters: The Device Name and the Device Type, If the Device      * 
 *             Name is kept as a NULL then it returns a pointer to     *
 *             Standard Input / Output depending on Type               *  
 *                                                                     *
 * Returns: The file pointer to the Device                             *
 *                                                                     *
 * Comments: None as of now                                            *
 ***********************************************************************
 */
FILE *initIODevice (char *strDeviceName, int iDeviceType)
{
    FILE *fpDevice = NULL;

    switch (iDeviceType) {
    case BOOT_DEVICE:
	if (strDeviceName != NULL) { //The Boot Device cannot be null
	    fpBootROM = fopen (strDeviceName, "r");
	    fpDevice = fpBootROM;
	}
	else {
	    fpDevice = NULL;
	}
	break;

    case INPUT_DEVICE:
	if (strDeviceName == NULL) { 
	    fpInputDevice = stdin;
	}
	else {
	    fpInputDevice = fopen (strDeviceName, "r");
	}
      
	fpDevice = fpInputDevice;
	break;
      
    case OUTPUT_DEVICE:
	if (strDeviceName == NULL) {
	    fpOutputDevice = stdout;
	}
	else {
	    fpOutputDevice = fopen (strDeviceName, "w");
	}
    
	fpDevice = fpOutputDevice;
	break;
    }
  
    return fpDevice;
}

/*
 ***********************************************************************
 * Function Name: readDevice                                           *
 *                                                                     * 
 * Purpose: Reads one Character wise from a device                     *
 *                                                                     *  
 * Parameters: The Device Name as FILE pointer                         * 
 *                                                                     *
 * Returns: The read character from the Device                         *
 *                                                                     *
 * Comments: None                                                      *
 ***********************************************************************
 */
int readDevice (FILE *fpDeviceName)
{
    return(fgetc (fpDeviceName));
}

/*
 ***********************************************************************
 * Function Name: readDevice                                           *
 *                                                                     * 
 * Purpose: Puts the last 8 bits of register A into device             *
 *                                                                     *  
 * Parameters: The Device Name as FILE pointer and The Byte to be      * 
 *             written                                                 *
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: None                                                      *
 ***********************************************************************
 */
void writeDevice (FILE *fpDeviceName, char A)
{
    fputc (A, fpDeviceName);
}

/*
 ***********************************************************************
 * Function Name: readDevice                                           *
 *                                                                     * 
 * Purpose: Closes the device and terminates further use of a device   *
 *          Except Standard Input and Standard Output Devices          *
 *                                                                     *  
 * Parameters: The Device Name as FILE pointer                         *
 *                                                                     *
 * Returns: Zero on Success and EOF if failed to close                 *
 *                                                                     *
 * Comments: None                                                      *
 ***********************************************************************
 */
int closeIODevice (FILE *fpDeviceName)
{
    int iResult;
    
    if ((fpDeviceName == stdout) || (fpDeviceName == stdin)) { 
	//For Standard I/O just the content of the Device is flushed
	/*if (devname == stdout) {
	  fflush (output);
	  }
	  else {
	  fflush (input);
	  }*/
	fflush(fpDeviceName);
	iResult = EOF;
    }
    else {
	iResult = fclose(fpDeviceName);
    }
  
    return iResult;
}
