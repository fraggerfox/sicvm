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
 * Module Name: sicvm.h                                                *
 *                                                                     * 
 * Purpose: Describes the Macros and Exposes Function Prototypes that  *
 *          will be shared among other modules also                    *
 *                                                                     *  
 * Comments: None                                                      *     
 ***********************************************************************
 */                                                                     
#ifndef _SICVM_H_
#define _SICVM_H_

//Detect the environement in which the compiler is used
//to set system command for clearing the screen
#if defined (__unix__) || defined (__APPLE__)
#define CLEARCMD "clear"
#endif

#if defined (__WINNT__) || defined (__WIN32__)
#define CLEARCMD "cls"
#endif

#define CLEARSCR(CMD) system(CMD)
#define MAXLENGTH 100

#endif
