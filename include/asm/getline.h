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
 * Module Name: getline.h                                              *
 *                                                                     * 
 * Purpose: Describes the Macros and Exposes Function Prototypes that  *
 *          will be shared among other modules also                    *
 *                                                                     *  
 * Comments: None                                                      *     
 ***********************************************************************
 */                                                                     
#ifndef _GETLINE_
#define _GETLINE_

#define LINE 0
#define LABEL 1
#define OPCODE 2
#define OPERAND 3
//This is not used as of now
#define ADDRESS 4

#define MAXLEN 100

//Function Prototypes
void getLine (FILE * fpSource, char* parsedInfo[]);


#endif
