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
 * Module Name: memory.h                                               *
 *                                                                     * 
 * Purpose: Describes the Macros and Exposes Function Prototypes that  *
 *          will be shared among other modules also                    *
 *                                                                     *  
 * Comments: None                                                      *     
 ***********************************************************************
 */                                                                     
#ifndef _MEMORY_H_
#define _MEMORY_H_

//Memory Size
#define MEMSIZE 1024 * 1024

#define BOOTSTRAP_ADDRESS 0x00
#define LOADER_ADDRESS 0x80
#define SHELL_ADDRESS 0x300
#define USER_ADDRESS 0x1000
#define SYSTEM_ADDRESS 0x00

// SIC's Main Memory Unintialized
unsigned char *ucrgMemory;

#endif
