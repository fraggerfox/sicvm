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
 * Module Name: interrupt.h                                            *
 *                                                                     * 
 * Purpose: Describes the Macros and Exposes Function Prototypes that  *
 *          will be shared among other modules also                    *
 *                                                                     *  
 * Comments: None                                                      *     
 ***********************************************************************
 */                                                                     
#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_


#define INT_OVF      0 //Arithmetic Overflow Interrupt
#define INT_DIVZERO  1 //Divide By Zero Interrupt
#define INT_SEGFAULT 2 //Segementation Fault
#define INT_INVALID  3 //Invalid Instruction
#define INT_BADMEM   4 //Lack of Memory in Host System
#define INT_DEVERR   5 //Bad Device or Device Inaccessible
#define INT_BADBOOT  6 //DEV00 Boot Device Not found
#define INT_BADLOAD  7 //DEVF1 Loader Device Not found
#define INT_OUTOFMEM 8 //Out of SIC's Memory Bounds
#define INT_NONE    -1 //No Interrupts Enabled

void raiseInterrupt (int iInterruptType);

#endif
