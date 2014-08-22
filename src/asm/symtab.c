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
 * Module Name: symtab.c                                               *
 *                                                                     * 
 * Purpose: This is the module that maintains the                      *
 *                   LABEL (Symbols) <-> Memory Address                *
 *          Relation for the given Assembly Code written, current      *
 *          implementation uses a "Binary Tree" Structure to maintain  *  
 *          the LABEL, ADDRESS pair in memory, I believe that this     * 
 *          will provide a better search and retrive mechanism when    *  
 *          the SYMTAB becomes large                                   *  
 *          The SYMTAB is created during pass1 of the Assembly process *  
 *          The references created during pass1 are resolved in pass2  *  
 *          when the LABELs are used on the OPERAND side               *  
 *                                                                     *  
 * Comments: Well nothing much to say, still needs bug testings        *     
 ***********************************************************************
 */
/* Standard Headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* Custom Headers */
#include "symtab.h"

//Structure that Holds the SYMTAB (One node of the Binary Tree)
struct SYMTAB
{
    char symbol[10]; //The symbol found in pass1 LABEL
    unsigned int address; //The address of LABEL
    struct SYMTAB *rptr; //Points to the right sub tree
    struct SYMTAB *lptr; //Points to the left sub tree
};

/*
 ***********************************************************************
 * Function Name: createSYMTabRoot                                     *
 *                                                                     * 
 * Purpose: Creats the 1st node of the SYMTAB,this will be the root of *
 *          the SYMTAB that will be used throughout the Assembly       *
 *                                                                     *  
 * Parameters: SYMTAB Root, the LABEL to insert and the Address of the * 
 *             LABEL to insert                                         *
 *                                                                     *
 * Returns: SYMTAB root, that will be used throughout the Assembly     *
 *                                                                     *
 * Comments: None as of now                                            *
 ***********************************************************************
 */
SYMnode *createSYMTabRoot (SYMnode *SYMroot, char *strLabel, unsigned int uiAddress)
{
    //Malloc the node 
    SYMroot = (SYMnode *) malloc (sizeof (SYMnode));
    //Check if the memory is allocated successfully
    if (SYMroot != NULL) {
	//Woo Hoo!!!, We have the root node
	//Copy the respective values into the node
	strcpy (SYMroot->symbol, strLabel);
	SYMroot->address = uiAddress;
	SYMroot->lptr = SYMroot->rptr = NULL;
    }

    //Return the created node
    return SYMroot;
}

/*
 ***********************************************************************
 * Function Name: insertSYMTab                                         *
 *                                                                     * 
 * Purpose: Does a Binary tree insert of the LABEL, address pairs in   * 
 *          the binary tree that is used during the Assembly process   *
 *          The function does a recursive insert.                      *  
 *                                                                     *  
 * Parameters: SYMTAB root, LABEL string and the Address               * 
 *                                                                     *
 * Returns: 0 on Successful Insert and 1 on Duplicate detection        *
 *                                                                     *
 * Comments: The function is used only during pass1 as of now          *
 ***********************************************************************
 */
int insertSYMTab (SYMnode *root, char *strLabel, unsigned int uiAddress)
{
    static int retval = 0;

    SYMnode *current = root;
    
    //Does a normal binary tree insert, Has the Duplicate Checking
    if (strcmp(current->symbol, strLabel) != 0) {
	if (strcmp (strLabel, current->symbol) < 0) {
	    if (current->lptr == NULL) {
		current->lptr = (SYMnode *)malloc (sizeof (SYMnode));
		current = current->lptr;
		current->lptr = current->rptr = NULL;
		strcpy (current->symbol, strLabel);
		current->address = uiAddress;
		retval = 0;
	    }  
	    else {
		insertSYMTab (current->lptr, strLabel, uiAddress);
	    }
	}
	else {
	    if (current->rptr == NULL) {
		current->rptr = (SYMnode *)malloc (sizeof (SYMnode));
		current = current->rptr;
		current->lptr = current->rptr = NULL;
		strcpy (current->symbol, strLabel);
		current->address = uiAddress;
		retval = 0;
	    }  
	    else {
		insertSYMTab (current->rptr, strLabel, uiAddress);
	    }
	}
    }
    else {
	//Duplicate LABELs Detected
	retval = 1;
    }
  
    return retval;
}

/*
 ***********************************************************************
 * Function Name: searchSYMTab                                         *
 *                                                                     * 
 * Purpose: Searches the SYMTAB Tree, takes in root of SYMTAB and the  * 
 *          Symbol to be searched for. The function is self-recursive  *
 *          and does a Binary Search to try to get the value, if       *  
 *          found will return the corresponding address else will      *
 *          return  -1.                                                *  
 *                                                                     *  
 * Parameters: SYMTAB root and the Symbol string to be searched for    * 
 *                                                                     *
 * Returns: The value of the Symbol if found else -1                   *
 *                                                                     *
 * Comments: None                                                      *
 ***********************************************************************
 */
int searchSYMTab (SYMnode *root, char *strSymbol)
{
    static int retval = -1;//Since the function is recursive the return value
                            //Value need to be maintained between function cal

    SYMnode *current = root;

    //Classic Binary Tree Search in a Binary Tree (Recursive)
    //Need I explain more??? 
    //Hint: If you dont get it, go look into Data Stuctures :-p  
    if (strcmp (strSymbol, current->symbol) == 0) {
	//Symbol found
	retval = current->address;
    }
    else {
	if (strcmp (strSymbol, current->symbol) < 0) {
	    if (current->lptr == NULL) {           
		//No Such Entry
		retval = -1;
	    }
	    else {
		searchSYMTab (current->lptr, strSymbol);
	    }
	}
	else {
	    if (current->rptr == NULL) {           
		//No Such Entry
		retval = -1;
	    }
	    else {
		searchSYMTab (current->rptr, strSymbol);
	    }
	}
    }

    return retval;
}

/*
 ***********************************************************************
 * Function Name: DisplaySYMTAB                                        *
 *                                                                     * 
 * Purpose: Does an inorder traversal of the Binary Tree and displays  * 
 *          the contents of the nodes. The function is recursive       *
 *                                                                     *  
 * Parameters: SYMTAB root                                             * 
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: This is more of a Debug function, not used practically    *
 ***********************************************************************
 */
void DisplaySYMTAB (SYMnode *root)
{
    //A Typical Inorder Traversal
    if (root->lptr != NULL) {
	DisplaySYMTAB (root->lptr);
    }

    printf ("%s\t%X\n", root->symbol, root->address);

    if (root->rptr != NULL) {
	DisplaySYMTAB (root->rptr);
    }
}
