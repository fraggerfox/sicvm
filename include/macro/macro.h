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
 * Module Name: macro.h                                                *
 *                                                                     * 
 * Purpose: Describes the Macros and Exposes Function Prototypes that  *
 *          will be shared among other modules also                    *
 *                                                                     *  
 * Comments: None                                                      *     
 ***********************************************************************
 */                                                                     
#ifndef _MACRO_H_
#define _MACRO_H_

#define TRUE 1

//Structure for NAMTAB that contain OPCODEs that does MACRO calls
typedef struct NAMTAB
{
  char OPCODE[20];
  int page, beg, end;
  struct NAMTAB *next;
}NAMlink;

//Global Variable Declarations
char ARGTAB[3][10], DEFTAB[10][50][100], OPCODE[30], LINE[100], LABEL[30], OPERAND[30], type[3][10], temp[5][10], posParm[4][3], macroNam[10]; 
FILE *src, *dst;
int EXPANDING, page, line, retval, params = 3;
NAMlink *NAMhead;

//Function Prototypes
void processMacro (void);
void getLine (void);
void processLine (void);
void define (void);
void expand (void);
void substitute (char type);
void InsertNAM (NAMlink *head);
int SearchNAM (NAMlink *head);
void replace (char *token, char *new, int n);
void replace2 (char *token, char *new);

#endif
