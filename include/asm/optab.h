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
 * Module Name: optab.h                                                *
 *                                                                     * 
 * Purpose: Describes the Macros and Exposes Function Prototypes that  *
 *          will be shared among other modules also                    *
 *                                                                     *  
 * Comments: None                                                      *     
 ***********************************************************************
 */                                                                     
#ifndef _OPTAB_
#define _OPTAB_

//Structure for OPTAB that contain OPCODEs
typedef struct OPTAB OPnode;

//Function Prototypes
OPnode *createOPTabRoot (FILE *fpSource, OPnode *OProot);
int searchOPTab (OPnode *root, char *strOpcode);
int insertOPTab (OPnode *root);
void DisplayOPTAB (OPnode *root);

#endif
