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
 * Module Name: getline.c                                              *
 *                                                                     * 
 * Purpose: This is the module that reads the assembly code from file  *
 *          and converts them to tokens                                *
 *                                                                     *  
 * Comments: Refer the TODOs                                           *     
 ***********************************************************************
 */
/*
 *  TODO: Have to develop a Finite Automata based Parsing Technique
 *  TODO: Can I make better use of the address field ?? In second pass
 *
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
/* Custom Headers */
#include "getline.h"

/*
 ***********************************************************************
 * Function Name: getLine                                              *
 *                                                                     * 
 * Purpose: A brute force parser for reding the current line of code   *
 *          the file and then seperate them into LABEL, OPCODE,        *
 *          OPERAND                                                    *  
 *                                                                     *  
 * Parameters: The Source File Pointer and an array of Strings         * 
 *             The Array holds the Parsed Values                       *
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: The current implementation is a simple Brute Force Parser *
 *           A better implementation can be achieved through a Finite  *
 *           Automata based parser                                     * 
 ***********************************************************************
 */
void getLine (FILE * fpSource, char* parsedInfo[])
{
    int length = 0;
    char strBuffer[MAXLEN];
    
    //Get the current line into the buffer
    fgets (strBuffer, MAXLEN, fpSource);
    //Get the Length
    length = (unsigned int) strlen (strBuffer);
    //Insert an EOL Marker removing the New Line Marker if it exists
    if (strBuffer[length - 1] == '\n') {
	strBuffer[length - 1] = '\0';
    }
    //Copy the LINE
    strcpy (parsedInfo[LINE], strBuffer);

    //Reset the LABEL, OPCODE & OPERAND
    parsedInfo[LABEL][0] = parsedInfo[OPCODE][0] = parsedInfo[OPERAND][0] = '\0';
    //Detect if the LINE is a Comment
    if (parsedInfo[LINE][0] != '.') {
	//Detect if the Instruction has a LABEL
	if (!isspace(parsedInfo[LINE][0])) {
	    //If so copy the LABEL, OPCODE, OPERAND
	    sscanf (parsedInfo[LINE], "%s%s%s", parsedInfo[LABEL], parsedInfo[OPCODE], parsedInfo[OPERAND]);
	}
	else {
	    //Else copy the OPCODE, OPERAND
	    sscanf (parsedInfo[LINE], "%s%s", parsedInfo[OPCODE], parsedInfo[OPERAND]);
	}
	
	//Just for safety clean up Operand in case of Operand-Less Opcodes
	if ((strcmp (parsedInfo[OPCODE], "RSUB") == 0) && (strcmp (parsedInfo[OPERAND], "4C") != 0)) {
	    parsedInfo[OPERAND][0] = '\0';
	}
    }
}
