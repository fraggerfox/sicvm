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
 * Module Name: errorasm.h                                             *
 *                                                                     * 
 * Purpose: Describes the Macros and Exposes Function Prototypes that  *
 *          will be shared among other modules also                    *
 *                                                                     *  
 * Comments: None                                                      *     
 ***********************************************************************
 */                                                                     
#ifndef _ERRORASM_
#define _ERRORASM_

#define FATAL 0
#define ERROR 1
#define WARNING 2

#define F_OPTAB_FILE_NOT_FOUND 0
#define F_INPUT_FILE_NOT_FOUND 1
#define F_INTERMEDIATE_FILE_CREATE_FAILED 2
#define F_INTERMEDIATE_FILE_NOT_FOUND 3
#define F_OUTPUT_FILE_CREATE_FAILED 4
#define F_INPUT_FILE_MISSING 5
#define F_OPTAB_CREATION_FAILED 6
#define F_SYMTAB_CREATION_FAILED 7
#define F_NO_FILE_NAME 8
#define F_BOOT_AND_LOAD_ASSEMBLE 9
#define F_BOOTSTRAP_LENGTH_EXCEEDED 10
#define F_DEVICE_NAME_MISSING 11
#define F_GENERIC 12

#define E_DUPLICATE_LABEL_FOUND 0
#define E_OPCODE_NOT_FOUND 1
#define E_UNDEFINED_SYMBOL 2

/*#define NO_OPCODE_FOUND 
#define NO_SYMBOL_FOUND 
*/
#define MAXLEN 100
#define MAX_FATAL 20
#define MAX_ERROR 20
#define MAX_WARN 20

void raiseError (unsigned int iErrorLevel, unsigned int iErrorType, char strMessage[]);

#endif
