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
 * Module Name: sic.h                                                  *
 *                                                                     * 
 * Purpose: Describes the Macros and Exposes Function Prototypes that  *
 *          will be shared among other modules also                    *
 *                                                                     *  
 * Comments: None                                                      *     
 ***********************************************************************
 */                                                                     
#ifndef _SIC_H_
#define _SIC_H_

//Condition Codes
#define LT 0x00
#define EQ 0x40
#define GT 0x80

#define CLEAR 0xFFFF3F
#define CCMASK 0xC0

//Running Modes
#define SU_MODE 0x800000
#define U_MODE 0x7FFFFF
#define GET_MODE 0x800000


//Type of program being executed
#define BOOT 0
#define LOADER 1
#define SHELL 2
#define USER 3



//Function Prototypes
int fetch (unsigned int iFetchType);
void store (int iRegister);
void execute (unsigned int uiInstructionType, unsigned int uiHexOpcode, unsigned int uiHexOperand);
int run (int iExecutionType);
int init (void);

#endif
