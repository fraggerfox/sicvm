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
 * Module Name: symtab.h                                               *
 *                                                                     * 
 * Purpose: Describes the Macros and Exposes Function Prototypes that  *
 *          will be shared among other modules also                    *
 *                                                                     *  
 * Comments: None                                                      *     
 ***********************************************************************
 */                                                                     
#ifndef _SYMTAB_
#define _SYMTAB_

//Struture for SYMTAB that contain Symbols
typedef struct SYMTAB SYMnode;


//Function Prototypes
SYMnode *createSYMTabRoot (SYMnode *SYMroot, char *strSymbol, unsigned int uiAddress);
int insertSYMTab (SYMnode *root, char *strSymbol, unsigned int uiAddress);
int searchSYMTab (SYMnode *root, char *strSymbol);
void DisplaySYMTAB (SYMnode *root);

#endif
