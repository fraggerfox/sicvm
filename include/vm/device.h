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
 * Module Name: device.h                                               *
 *                                                                     * 
 * Purpose: Describes the Macros and Exposes Function Prototypes that  *
 *          will be shared among other modules also                    *
 *                                                                     *  
 * Comments: None                                                      *     
 ***********************************************************************
 */                                                                     
#ifndef _DEVICE_H_
#define _DEVICE_H_

//Type of devices
#define BOOT_DEVICE 0
#define INPUT_DEVICE 1
#define OUTPUT_DEVICE 2

#define MAX_DEVICES 7 //Maximum Devices in SIC

//device pointers
FILE *fpBootROM;
FILE *fpInputDevice;
FILE *fpOutputDevice;

//Indicates the current device in use, varies from 0 to MAX_DEVICES - 1
//-1 indicates no devices are in use
int iInUseInputDevice, iInUseOutputDevice;

//Device Access and Control
int irgDeviceAccessFlags[MAX_DEVICES], iDeviceIndex;

//Function Prototypes
FILE *initIODevice (char *strDeviceName, int iDeviceType);
int readDevice (FILE *fpDeviceName);
void writeDevice (FILE *fpDeviceName, char A);
int closeIODevice (FILE *fpDeviceName);

#endif
