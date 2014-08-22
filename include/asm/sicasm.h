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
 * Module Name: sicasm.h                                               *
 *                                                                     * 
 * Purpose: Describes the Macros and Exposes Function Prototypes that  *
 *          will be shared among other modules also                    *
 *                                                                     *  
 * Comments: None                                                      *     
 ***********************************************************************
 */                                                                     
#ifndef _SICASM_
#define _SICASM_

#define HEXADECIMAL_BASE 16
#define TEXT_RECORD_HEAD_LENGTH 9
#define TEXT_RECORD_MAX_LENGTH 30
#define TEXT_RECORD_LENGTH_POSITION 2
#define TEXT_RECORD_WORD_LENGTH 3
#define BOOTSTRAP_RECORD_MAX_LENGTH 32
#define BOOTSTRAP_MAX_LENGTH 128

//Uncomment if Debug Output is required
//#define DEBUG 

//Funtion Prototypes
void pass1 (void);
void pass2 (void);
void createBootstrap (void);
void createLoader (void);
void parseArguments (int argc, char *argv[]);

typedef struct Flags Flags;

#endif
