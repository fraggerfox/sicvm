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
 * Module Name: helpasm.c                                              *
 *                                                                     * 
 * Purpose: Simple Module that displays brief help on the arguments    *
 *                                                                     *  
 * Comments: No detailed help for the commands have been given         *     
 ***********************************************************************
 */
/* Standard Header Files */
#include <stdio.h>

/* Custom Header Files */
#include "helpasm.h"

/*
 ***********************************************************************
 * Function Name: help                                                 *
 *                                                                     * 
 * Purpose: Prints the help text on screen when called                 *
 *                                                                     *  
 * Parameters: The argument that requires detail help (not implemented)* 
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: The on screen text is printed assuming an 80 Column       *
 *           Display like in Command Line environment, I dont know How *
 *           the help will be displayed                                * 
 ***********************************************************************
 */
void display_help (char cCommand)
{
    //The technique for printing on screen is as follows
    //I assume that the help will be used in a 80 Column Command Line window
    //I restrict the argument in a 20 character padding with Right Align
    //The rest of the line is given a 60 character precision
    //for lines without Arguments, there a 50 spaces padding and 80 Character Precision
    switch (cCommand) {
    default:
	//Displays the header and a brief description of the program
	printf ("A 2 Pass Assembler for SIC\n\n");
	printf ("%.80s", "sicasm is a Simple 2 Pass Assembler for SIC, it Assembles given SIC Assembly\n");
	printf ("%.80s", "into correspinding Object. Currently it dosen't Support the SIC/XE Instruction\n");
	printf ("%.80s", "Set. The assembled Object code can be run in SICvm.\n");
	printf ("%.80s", "(Based on Leland L.Beck's SIC in \"System Software\")\n");

	//Displays How to use the Utility
	printf ("%.80s", "\nUSAGE:\tsicasm [OPTIONS]... file...\n\n");
	
	//Displays the various Arguments and gives a brief description
	printf ("OPTIONS:\n");
	printf ("%-20.20s%-.60s", "  -o <file>", "Place the output into <file>\n");

	printf ("%-20.20s%-.60s", "  -b", "Indicates that after the Assembly, Strip the Object code\n");
	printf ("%-20.20s%-.60s", " ", "into Bootstrap Format for SICvm and place it in the Default\n");
	printf ("%-20.20s%-.60s", " ", "Device \"DEV00\"\n");
	  
	printf ("%-20.20s%-.60s", "  -l", "Indicates that after the Assembly, Strip the Object code\n");
	printf ("%-20.20s%-.60s", " ", "into Loader Format for SICvm and place it in the Default\n");
	printf ("%-20.20s%-.60s", " ", "Device \"DEVF1\"\n");
	  
	printf ("%-20.20s%-.60s", "  -d <devicename>", "Place the Object Code into <devicename>\n");
	printf ("%-20.20s%-.60s", " ", "1) Device Names are of the Format DEVxx where xx is any\n");
	printf ("%-20.20s%-.60s", " ", "   Hexadecimal Number ranging from '00' to 'FF'\n");
	printf ("%-20.20s%-.60s", " ", "2) Devices DEV00, DEVF1, DEVF0, DEVA0 are reserved and\n");
	printf ("%-20.20s%-.60s", " ", "   and should not be used.\n");
	  
	printf ("%-20.20s%-.60s", "  -O[\"file\"]", "Indicate the location of external OPTAB\n");
	printf ("%-20.20s%-.60s", " ", "1) \"file\" is Optional.\n");
	printf ("%-20.20s%-.60s", " ", "2) If no \"file\" is supplied, by default \"optab.txt\"\n");
	printf ("%-20.20s%-.60s", " ", "   will be taken as the external OPTAB.\n");
	printf ("%-20.20s%-.60s", " ", "3) \"file\" if supplied should be enclosed in \"\".\n");
	printf ("%-20.20s%-.60s", " ", "4) \"file\" and O should not have any space in between\n");
	printf ("%-20.20s%-.60s", " ", "   for example -O\"optab.txt\".\n");
	printf ("%-20.20s%-.60s", " ", "5) If -O is omitted then the Internal OPTAB will be used.\n");

	printf ("%-20.20s%-.60s", "  -V", "Gives a more verbose Assembly Output\n");
	  
	printf ("%-20.20s%-.60s", "  -v", "Displays the version of the Assembler.\n");

	printf ("%-20.20s%-.60s", "  -h", "Display this help\n");

	printf ("%.80s", "\nArguments of Options enclosed in \"<>\" (like \"-o <file>\") are mandatory\n");
	printf ("%.80s", "others (like \"-O [file]\") are optional\n");

	printf ("%.80s", "\nReport Bugs to (fragger_fox@users.sourceforge.net).\n");
	break;
    }
}
