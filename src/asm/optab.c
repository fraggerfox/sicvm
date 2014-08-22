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
 * Module Name: optab.c                                                *
 *                                                                     * 
 * Purpose: This is the module that maintains the MNEMONIC <-> VALUE   *
 *          Relation for the given Assembly Language, the current      *
 *          implementation uses a "Binary Tree" Structure to maintain  *  
 *          the MNEMONIC, VALUE pair in memory, I believe that this    * 
 *          will provide a better search and retrive mechanism when    *  
 *          the OPTAB becomes large                                    *  
 *                                                                     *  
 * Comments: Detecting duplicate OPCODEs during insert is not there    * 
 *           Still needs bug testings                                  *      
 ***********************************************************************
 */
/* Standard Headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Custom Headers */
#include "optab.h"
#include "getline.h"

//The total number of Instructions in the Instruction Set
#define TOTAL_INSTRUCTIONS 26
//Array Index that Stores MNEMONIC
#define MNEMONIC 1
//Array Index that Stores VALUE for the MNEMONIC
#define VALUE 2

//Structure that Holds the OPTAB (One node of the Binary Tree)
struct OPTAB
{
    char opcode[10]; //The opcode MNEMONIC
    unsigned int value; //The VALUE of MNEMONIC
    struct OPTAB *rptr; //Points to the right sub tree
    struct OPTAB *lptr; //Points to the left sub tree
};

//Internal OPTAB, used if External OPTAB not supplied
const char OPCodes[TOTAL_INSTRUCTIONS][3][MAXLEN] = {
    { "J\t3C\0", "J\0", "3C\0" },
    { "LDA\t00\0", "LDA\0", "00\0" },
    { "SUB\t1C\0", "SUB\0", "1C\0" },
    { "CLEAR\tB4\0", "CLEAR\0", "B4\0" },
    { "JLT\t38\0", "JLT\0", "38\0" },
    { "STX\t10\0", "STX\0", "10\0" },
    { "LDCH\t50\0", "LDCH\0", "50\0" },
    { "STCH\t54\0", "STCH\0", "54\0" },
    { "TD\tE0\0", "TD\0", "E0\0" },
    { "RSUB\t4C\0", "RSUB\0", "4C\0" },
    { "STC\t14\0", "STC\0", "14\0" },
    { "LDX\t04\0", "LDX\0", "04\0" },
    { "STA\t0C\0", "STA\0", "0C\0" },
    { "ADD\t18\0", "ADD\0", "18\0" },
    { "JSUB\t48\0", "JSUB\0", "48\0" },
    { "STL\t14\0", "STL\0", "14\0" },
    { "COMP\t28\0", "COMP\0", "28\0" },
    { "JEQ\t30\0", "JEQ\0", "30\0" },
    { "LDL\t08\0", "LDL\0", "08\0" },
    { "RD\tD8\0", "RD\0", "D8\0" },
    { "TIX\t2C\0", "TIX\0", "2C\0" },
    { "WD\tDC\0", "WD\0", "DC\0" },
    { "MUL\t20\0", "MUL\0", "20\0" },
    { "HLT\t76\0", "HLT\0", "76\0" },
    { "JGT\t34\0", "JGT\0", "34\0" },
    { "DIV\t24\0", "DIV\0", "24\0" },
};

char* strParsedInfo[4]; //Stores the Parsed Info from each line

/*
 ***********************************************************************
 * Function Name: createOPTabRoot                                      *
 *                                                                     * 
 * Purpose: Creats the 1st node of the OPTAB, this will be the root of *
 *          the OPTAB that will be used throughout the Assembly        *
 *                                                                     *  
 * Parameters: Source File of OPTAB, if Kept NULL will load the        * 
 *             internal OPTAB, and the OPTAB Root                      *
 *                                                                     *
 * Returns: OPTAB root, that will be used throughout the Assembly      *
 *                                                                     *
 * Comments: Can it be made efficient ???                              *
 ***********************************************************************
 */
OPnode *createOPTabRoot (FILE *fpSource, OPnode *OProot)
{
    int i = 0; //Counter Variable

    //Initialize the string Array which is gonna store the Parsed Information
    for (i = 0; i < 4; i++) {
	strParsedInfo[i] = (char *) malloc (100 * sizeof (char));
    }
    
    //Check to see whether to load the internal or external OPTAB
    if (fpSource == NULL) {
	//Loop until all the Instructions are loaded
	for (i = 0; i < TOTAL_INSTRUCTIONS; i++) {
	    strcpy (strParsedInfo[LINE], OPCodes[i][LINE]); //Copy the Line
	    strcpy (strParsedInfo[OPCODE], OPCodes[i][MNEMONIC]); //Copy the MNEMONIC
	    strcpy (strParsedInfo[OPERAND], OPCodes[i][VALUE]); //Copy the VALUE
	    
	    //Actually if you ask me why this NULL Check exists,
	    //It was done for Saftey checks in the previous Assembler builds
	    //As of now the modules are Safe, But the Check Exists as an extra 
	    //Barricade, just in case !!!
	    //Check to see if the root is NULL (first insert)	    
	    if (OProot == NULL) {
		//Malloc the root Root
		OProot = (OPnode *) malloc (sizeof (OPnode));
		if (OProot == NULL) {
		    printf ("Assembly Error: OPTAB Creation Failed, Insufficient Memory\n");
		    exit (EXIT_FAILURE);
		}
		//Woo Hoo!!!, We have the root node
		//Copy the respective values
		strcpy (OProot->opcode,  strParsedInfo[OPCODE]);
		sscanf (strParsedInfo[OPERAND], "%x", &OProot->value);
		OProot->lptr = OProot->rptr = NULL;
	    }
	    else {
		//Else do a Normal Insert
		if (insertOPTab (OProot) == 1) {
		    printf ("Assembly Error: Duplicate MNEMONIC Values in Table\n");
		    exit (EXIT_FAILURE);
		}
	    }
	}
    }
    else {
	//This is done if the Input is from a file
	//Except for the getLine, rest of the Logic remains the same
	while (!feof(fpSource)) {
	    //Get the line calling our parser
	    getLine (fpSource, strParsedInfo);
	    if (OProot == NULL) {	       
		OProot = (OPnode *) malloc (sizeof (OPnode));
		if (OProot == NULL) {
		    printf ("Assembly Error: OPTAB Creation Failed, Insufficient Memory\n");
		    exit (EXIT_FAILURE);
		}
		
		strcpy (OProot->opcode,  strParsedInfo[OPCODE]);
		sscanf (strParsedInfo[OPERAND], "%x", &OProot->value);
		OProot->lptr = OProot->rptr = NULL;
	    }
	    else {
		if (insertOPTab (OProot) == 1) {
		    printf ("Assembly Error: Duplicate MNEMONIC Values in Table\n");
		    exit (EXIT_FAILURE);
		}	       
	    }
	}
    }
    
    //DisplayOPTAB (OProot);
    //exit(0);
    //Return the root node
    return OProot;
}

/*
 ***********************************************************************
 * Function Name: searchOPTab                                          *
 *                                                                     * 
 * Purpose: Searches the OPTAB Tree, takes in root of OPTAB and the    * 
 *          Opcode to be searched for. The function is self-recursive  *
 *          and does a Binary Search to try to get the value, if       *  
 *          found will return the corresponding value else will return *  
 *          -1.                                                        *  
 *                                                                     *  
 * Parameters: OPTAB root and the Opcode string to be searched for     * 
 *                                                                     *
 * Returns: The value of the Opcode if found else -1                   *
 *                                                                     *
 * Comments: None                                                      *
 ***********************************************************************
 */
int searchOPTab (OPnode *root, char *strOpcode)
{
    static int retval = -1; //Since the function is recursive the return value
                            //Value need to be maintained between function calls

    OPnode *current = root;
    
    //Classic Binary Tree Search in a Binary Tree (Recursive)
    //Need I explain more??? 
    //Hint: If you dont get it, go look into Data Stuctures :-p
    if (strcmp (strOpcode, current->opcode) == 0) {
	//Opcode Found
	retval = current->value;  
    }
    else {
	if (strcmp (strOpcode, current->opcode) < 0) {
	    if (current->lptr == NULL) {           
		//No Such Entry
		retval = -1;
	    }
	    else {
		searchOPTab (current->lptr, strOpcode);
	    }
	}
	else {
	    if (current->rptr == NULL) {
		//No Such Entry
		retval = -1;
	    }
	    else {
		searchOPTab (current->rptr, strOpcode);
	    }
	}
    }

    return retval;
}

/*
 ***********************************************************************
 * Function Name: insertOPTab                                          *
 *                                                                     * 
 * Purpose: Does a Binary tree insert of the MNEMONIC, VALUE pairs in  * 
 *          the binary tree that is used during the Assembly process   *
 *          The function does a recursive insert.                      *  
 *                                                                     *  
 * Parameters: OPTAB root                                              * 
 *                                                                     *
 * Returns: 0 on Successful Insert and 1 on Duplicate detection        *
 *                                                                     *
 * Comments: Duplicates insertion How? you ask, What if a lousy user   *
 *           writes duplicate Entry in the OPTAB Text file  :-P        *
 ***********************************************************************
 */
int insertOPTab (OPnode *root)
{
    static int retval = 0;
     
    OPnode *current = root;
    
    //Does a normal binary tree insert, Has the Duplicate Checking
    if (strcmp(current->opcode, strParsedInfo[OPCODE]) != 0) {
	if (strcmp (strParsedInfo[OPCODE], current->opcode) < 0) {
	    if (current->lptr == NULL) {
		current->lptr = (OPnode *)malloc (sizeof (OPnode));
		current = current->lptr;
		current->lptr = current->rptr = NULL;
		strcpy (current->opcode,strParsedInfo[OPCODE]);
		sscanf (strParsedInfo[OPERAND], "%x", &current->value);
		retval = 0;
	    }  
	    else {
		insertOPTab (current->lptr);
	    }
	}
	else {
	    if (current->rptr == NULL) {
		current->rptr = (OPnode *)malloc (sizeof (OPnode));
		current = current->rptr;
		current->lptr = current->rptr = NULL;
		strcpy (current->opcode, strParsedInfo[OPCODE]);
		sscanf (strParsedInfo[OPERAND], "%x", &current->value);
		retval = 0;
	    }  
	    else {
		insertOPTab (current->rptr);
	    }
	}
    }
    else {
	//Duplicates in MNEMONIC Detected
	retval = 1;
    }

    return retval;
}

/*
 ***********************************************************************
 * Function Name: DisplayOPTAB                                         *
 *                                                                     * 
 * Purpose: Does an inorder traversal of the Binary Tree and displays  * 
 *          the contents of the nodes. The function is recursive       *
 *                                                                     *  
 * Parameters: OPTAB root                                              * 
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: This is more of a Debug function, not used practically    *
 ***********************************************************************
 */
void DisplayOPTAB (OPnode *root)
{
    //A Typical Inorder Traversal
    if (root->lptr != NULL) {
	DisplayOPTAB (root->lptr);
    }

    printf ("%s\t", root->opcode);
    printf ("%02X\n", root->value);

    if (root->rptr != NULL) {
	DisplayOPTAB (root->rptr);
    }
}
