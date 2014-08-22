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
 * Module Name: sicasm.c                                               *
 *                                                                     * 
 * Purpose: This is the core of the SIC assembler based on Leland L.   *
 *           Beck's "System Software" Book. Refer Pages 53-54.         *
 *                                                                     *  
 * Comments: Refer the TODOs                                           *     
 ***********************************************************************
 */
/*
 * TODO: Improve the Naming Conventions, well atleast I m trying to stick to it
 * TODO: Support SIC/XE Instruction Set - well not in this module
 * TODO: Pass 1 and Pass 2 algorithms improved, requires checking
 * TODO: Loads of Clean up to do :-(
 */
/* Standard Headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>

/* Custom Headers */
#include "sicasm.h"
#include "optab.h"
#include "getline.h"
#include "symtab.h"
#include "helpasm.h"
#include "errorasm.h"

//Assembler Variable Values
#define LOCCTR 			 0	//Location Counter
#define STARTING_ADDRESS 	 1	//The Starting Address of the Program
#define SYMBOL_ADDRESS 		 2	//The Address of the Current Symbol in the OPERAND Field
#define FIRST_EXECUTABLE_ADDRESS 3 	//The Address of the 1st Executable Instruction
#define PROGRAM_LENGTH  	 4	//The Total Length of the Assembly Program
#define OBJECT_CODE 		 5	//The Assembled Object Code 
#define MAX_VALUES 		 6 	//Maximum Number of Assembler Variables

//The Flags that is used during the assembly process
//All Flags are bit fields optimized 
struct Flags {
    //Flags set on Assembly error
    unsigned int hasDuplicateLabels     : 1; //Set when Duplicate Labels are Detected
    unsigned int hasInvalidOpcode       : 1; //Set when Invalid OPCODE (not found in OPTAB) is Detected
    unsigned int hasUndefinedSymbol     : 1; //Set one Undefined Symbols are found in Pass 2 (not present in SYMTAB)
    unsigned int hasError               : 1; //Set when any Error Condition is Detected during Assembly
    //Flags set on Various Assembly Conditions
    unsigned int hasIndexMode           : 1; //Set when the instruction being Processed is in Index Mode
    unsigned int hasImmediateIndexMode  : 1; //Set when the instruction being Processes is in Immediate Index Mode
    unsigned int isExecutableAddressSet : 1; //Set when the First Executable Instruction's Address is got
    unsigned int isNewTextRecord        : 1; //Set when a New Record is requested
    //Flags set on user Request from command line
    unsigned int isBootStrapAssemble    : 1; //Sets when the User gives a -b flag from Command Line
    unsigned int isLoaderAssemble       : 1; //Sets when the User gives a -l flag from Command Line
    unsigned int hasVerboseOutput       : 1; //Sets when User gives a -V flag from Command Line
    unsigned int hasExternOptab         : 1; //Sets when the User gives -O flag from Command Line
};

//Global Variable Declarations
FILE *fpSource, *fpDestination;                     //Current Source File Pointer and Destination File Pointer
char* strParsedInfo[4];                             //The Parsed information about each assembly line is present here
char strOptabFileName[MAXLEN] = "optab.txt";        //The OPTAB File
char strDeviceName[10]= "";                         //The Device Name to which the object code is placed
char strSourceFileName[MAXLEN]= "";                 //The Source File Name
char strDestinationFileName[MAXLEN] = "";           //The Destination File Name
char strIntermediateFileName[MAXLEN] = "inter.txt"; //The Intermediate File name
OPnode *OPTabRoot;                                  //The OPTAB Root
SYMnode *SYMTabRoot;                                //The SYMTAB Root
Flags flags;                                        //Flags that will be used during the assembly
unsigned int uiAssemblerVariables[MAX_VALUES];      //Various Assembly time Variables like LOCCTR etc

int main(int argc, char *argv[])
{
    int i = 0; //Counter/Generic Variables
   
    /****************Initilalization**************************/

    //Initilialize string array
    for (i = 0; i < 4; i++) {
	strParsedInfo[i] = (char *) malloc (MAXLEN * sizeof (char));
    }
    
    //Parse the Command Line arguments
    parseArguments (argc, argv);
    
    printf ("Starting Assembly Process...\n\n");

    //Initialize the OPTAB
    //Check if the OPTAB is External or not
    if (flags.hasExternOptab) {
	//Try opening the file
	fpSource = fopen (strOptabFileName, "r");
	
	//Check if successfully opened
	if (fpSource != NULL) {
	    OPTabRoot = createOPTabRoot (fpSource, OPTabRoot); //Create the OPTab used by the assembler
	}
	else {
	    flags.hasError = 1;
	    raiseError (FATAL, F_OPTAB_FILE_NOT_FOUND, strOptabFileName);
	}
    }
    else {
	//Use Internal OPTAB, set fpSource NULL
	fpSource = NULL;	
	OPTabRoot = createOPTabRoot (fpSource, OPTabRoot);
    }
    
    //If Memory Allocation for OPTAB failed
    if (OPTabRoot == NULL) {
      flags.hasError = 1;
      raiseError (FATAL,F_OPTAB_CREATION_FAILED, "");
    }

    //Close file if it is Open
    if (fpSource != NULL) {
	fclose (fpSource);
	fpSource = NULL;
    }
    
    /********* Debug Information **********/
#ifdef DEBUG
    DisplayOPTAB (OPTabRoot);
    //exit (0);
#endif
    /********* Debug Ends *****************/ 
    printf ("OPTAB Initialized Successfully!!!\n");

    /****************Prepare for Pass 1**************************/

    //The Source SIC Assembly is the input file for Pass 1
    fpSource = fopen (strSourceFileName, "r"); 
    
    //Check if the file opened Successfully
    if (fpSource == NULL) {
	flags.hasError = 1;
	raiseError (FATAL,F_INPUT_FILE_NOT_FOUND, strSourceFileName);
    }

    //Is there a better way to output the intermediate results ???
    //More flexible commenting must be allowed in the assembly language

    //The intermediate File is the output for Pass 1
    fpDestination = fopen (strIntermediateFileName, "w"); 
  
    //Check if the file opened Successfully
    if (fpDestination == NULL) {
	flags.hasError = 1;
	raiseError (FATAL, F_INTERMEDIATE_FILE_CREATE_FAILED, strIntermediateFileName);
    }
      
    pass1(); //Call Pass 1
  
    //Clean Up 
    //Close any open file pointers
    if (fpSource != NULL) {
	fclose (fpSource);
	fpSource = NULL;
    }
    
    if (fpDestination != NULL) {
	fclose (fpDestination);
	fpDestination = NULL;
    }
    
    //Non fatal errors that allows pass1 to complete but 
    //cannot continue because of errors
    if (flags.hasError) {
	raiseError (FATAL, F_GENERIC, "Pass1");	
    }
    else {
	printf ("Pass 1 Completed Successfully!!!\n");
    }

    /********* Debug Information **********/    
#ifdef DEBUG
    DisplaySYMTAB (SYMTabRoot);
    //exit (0);
#endif
    /********* Debug Ends *****************/ 

    /****************Prepare for Pass 2**************************/

    //The Intermediate file is the input for pass 2 
    fpSource = fopen (strIntermediateFileName, "r"); 

    //Check if the file opened Successfully
    if (fpSource == NULL) {
	flags.hasError = 1;
	raiseError (FATAL, F_INPUT_FILE_NOT_FOUND, strIntermediateFileName);
    }

    fpDestination = fopen (strDestinationFileName, "w"); //The Assembled Object file is output for pass 2

    //Check if the file opened Successfully
    if (fpDestination == NULL) {
	flags.hasError = 1;
	raiseError (FATAL, F_OUTPUT_FILE_CREATE_FAILED, strDestinationFileName);
    }
  
    pass2 (); //Call Pass 2
    
    //Close the file Handlers
    if (fpSource != NULL) {
	fclose (fpSource);
	fpSource = NULL;
    }
    
    if (fpDestination != NULL) {
	fclose (fpDestination);
	fpDestination = NULL;
    }
    
    //Pass 2 non fatal errors, the object code is not generated and the program exits
    if (flags.hasError == 1) {
	raiseError (FATAL, F_GENERIC, "Pass2");	
    }
    else {
	printf ("Pass 2 Completed Successfully!!!\n\n");
    }

    /*****************Check for Special Assembly options*************************/

    //Check if the Special Assembly options are ON, 
    //if so open up the corresponding file pointers
    if (flags.isBootStrapAssemble || flags.isLoaderAssemble) {
      fpSource = fopen (strDestinationFileName, "r"); 
      
      if (fpSource == NULL) {
	flags.hasError = 1;
	raiseError (FATAL, F_INPUT_FILE_NOT_FOUND, strDestinationFileName);
      }
      
      fpDestination = fopen (strDeviceName, "w"); 
      
      if (fpDestination == NULL) {
	flags.hasError = 1;
	raiseError (FATAL, F_OUTPUT_FILE_CREATE_FAILED, strDeviceName);
      }
    }

    //If the assembly is of Bootstrap type
    if (flags.isBootStrapAssemble) {
      if (uiAssemblerVariables[PROGRAM_LENGTH] <= BOOTSTRAP_MAX_LENGTH) {
	createBootstrap ();
	printf ("Bootstrap Assembly Created Successfully!!! and stored in %s Device\n\n", strDeviceName);
      }
      else {
	flags.hasError = 1;
	raiseError (FATAL, F_BOOTSTRAP_LENGTH_EXCEEDED, strSourceFileName);
      }
    }

    //If the assembly is of Loader type
    if (flags.isLoaderAssemble) {
	createLoader ();
	printf ("Loader Assembly Created Successfully!!! and stored in %s Device\n\n", strDeviceName);
    }
    
    //Close the file Handlers
    if (fpSource != NULL) {
	fclose (fpSource);
	fpSource = NULL;
    }
    
    if (fpDestination != NULL) {
	fclose (fpDestination);
	fpDestination = NULL;
    }

    //Print Messages According to Error Status
    if (!flags.hasError) {
      printf ("Assembly Successful.\n");
      printf ("Assembled code in \"%s\".\n", strDestinationFileName);
      printf ("The Intermediate file is \"inter.txt\"\n"); 
    }
    else {
	printf ("Assembly Failed :-(.\n");
    }
   
    /********* Debug Information **********/
#ifdef DEBUG
    printf ("\nThe Symbol table created is\n");
    DisplaySYMTAB (SYMTabRoot);
#endif
    /********* Debug Ends *****************/

    return 0;
}

/*
 ***********************************************************************
 * Function Name: pass1                                                *
 *                                                                     * 
 * Purpose: Implements the Pass 1 Algorithm based on Leland L.Beck's   *
 *          "System Software" Book. Refer Pages 53-54.                 *
 *                                                                     *  
 * Parameters: None                                                    * 
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: The implementation tries to stick maximum to the original *
 *           Algorithm as such. Certain Performace Improvement steps   *
 *           are pending as of now                                     * 
 ***********************************************************************
 */
void pass1 (void)
{
    unsigned int uiLength = 0;
    unsigned int uiCurrentAddress = 0;

    //Read first Input line
    getLine (fpSource, strParsedInfo);

    if (strcmp (strParsedInfo[OPCODE], "START") == 0) {
	//Save the operand starting address
	sscanf (strParsedInfo[OPERAND], "%x",  &uiAssemblerVariables[STARTING_ADDRESS]);
	//Initialize LOCCTR to starting Address
	uiAssemblerVariables[LOCCTR] = uiAssemblerVariables[STARTING_ADDRESS];
	//Write Line to Intermediate File
	fprintf (fpDestination, "%s\t\t%04X\n", strParsedInfo[LINE], uiAssemblerVariables[LOCCTR]);
	//Get Next Input Line
	getLine (fpSource, strParsedInfo);
    }
    else {
	uiAssemblerVariables[LOCCTR] = 0; //Initialize LOCCTR to Zero
    }

    //While OPCODE not End
    while (strcmp (strParsedInfo[OPCODE], "END") != 0) {
	//If the line is not blank or a comment
	if ((strParsedInfo[LINE][0] != '.') && (strParsedInfo[LINE][0] != '\0')){
	    //If there is a SYMBOL in the LABEL Field
	    if (strParsedInfo[LABEL][0] !=  '\0') {
		//Check if the SYMTAB is initialized
		if (SYMTabRoot == NULL) {		  
		    SYMTabRoot = createSYMTabRoot (SYMTabRoot, strParsedInfo[LABEL], uiAssemblerVariables[LOCCTR]);
		    //replace with SYMTAB creation failed
		    if (SYMTabRoot == NULL) {
		      flags.hasError = 1;
		      raiseError (FATAL,F_SYMTAB_CREATION_FAILED,"");
		    }
		}//If SYMTAB not initialized
		else {
		    //Check for Duplicate Label, if not insert the LABEL
		    flags.hasDuplicateLabels = insertSYMTab (SYMTabRoot, strParsedInfo[LABEL], uiAssemblerVariables[LOCCTR]);
		    
		    //If there are Duplicates set the Error Flag
		    //non fatal error, duplicate labels
		    if (flags.hasDuplicateLabels == 1) {
			flags.hasError = 1;
			raiseError (ERROR, E_DUPLICATE_LABEL_FOUND, strParsedInfo[LABEL]); //Raise the Error
			//Output the Error information into the intermediate file
			fprintf (fpDestination, ".Label %s is Repeated in Code\n", strParsedInfo[LABEL]);
		    }//If Duplicates
		}
	    }//If there is symbol in LABEL
	    
	    //Save the Current Address for Future use...(Currently for storing Addresses into Intermediate File)
	    uiCurrentAddress = uiAssemblerVariables[LOCCTR];

	    //Search OPTAB for OPCODE, then increment  LOCCTR 
	    if (searchOPTab (OPTabRoot, strParsedInfo[OPCODE]) < 0){
		flags.hasInvalidOpcode = 1;
	    }
	    if (!flags.hasInvalidOpcode) {
		if (flags.isExecutableAddressSet == 0) {
		    flags.isExecutableAddressSet = 1;
		    uiAssemblerVariables[FIRST_EXECUTABLE_ADDRESS] = uiAssemblerVariables[LOCCTR];
		}
		uiAssemblerVariables[LOCCTR] += TEXT_RECORD_WORD_LENGTH;
	    } else if (strcmp (strParsedInfo[OPCODE], "WORD") == 0) {
		uiAssemblerVariables[LOCCTR] += TEXT_RECORD_WORD_LENGTH;
		flags.hasInvalidOpcode = 0;
	    } else if (strcmp (strParsedInfo[OPCODE], "RESW") == 0) {
		uiAssemblerVariables[LOCCTR] += (atoi(strParsedInfo[OPERAND]) * TEXT_RECORD_WORD_LENGTH);
		flags.hasInvalidOpcode = 0;
	    }  else if (strcmp (strParsedInfo[OPCODE], "RESB") == 0) {
		uiAssemblerVariables[LOCCTR] += atoi (strParsedInfo[OPERAND]);
		flags.hasInvalidOpcode = 0;
	    } else  if (strcmp (strParsedInfo[OPCODE], "BYTE") == 0) {
		uiLength = (unsigned int) strlen(strParsedInfo[OPERAND]) - 3;
		uiAssemblerVariables[LOCCTR] += strParsedInfo[OPERAND][0] == 'C' ?  uiLength : uiLength / 2 ;
		flags.hasInvalidOpcode = 0;
	    }
	    else  {
		//If OPCODE not found set error flag
		flags.hasInvalidOpcode = 0;
		flags.hasError = 1;
		raiseError (ERROR, E_OPCODE_NOT_FOUND, strParsedInfo[OPCODE]); //Raise the Error
		//Output the Error information into the intermediate file
		fprintf (fpDestination, ".Operation Code %s doesn't exist\n", strParsedInfo[OPCODE]);
	    }//If OPCODE not found
	    
	    //Write Line to Intermediate File
	    fprintf (fpDestination, "%s\t\t%04X\n", strParsedInfo[LINE], uiCurrentAddress);
	}//If not comment

	getLine (fpSource, strParsedInfo);
    }//while not end
  
    //Write Last Line to intermediate file
    fprintf (fpDestination, "%s\t\t%04X\n", strParsedInfo[LINE], uiAssemblerVariables[LOCCTR]);
    //Save LOCCTR - Starting Address as Program Length
    uiAssemblerVariables[PROGRAM_LENGTH] = uiAssemblerVariables[LOCCTR] - uiAssemblerVariables[STARTING_ADDRESS];

    //DisplaySYMTAB (SYMTabRoot);
}//end Pass 1

/*
 ***********************************************************************
 * Function Name: pass2                                                *
 *                                                                     * 
 * Purpose: Implements the Pass 2 Algorithm based on Leland L.Beck's   *
 *          "System Software" Book. Refer Pages 53-54.                 *
 *                                                                     *  
 * Parameters: None                                                    * 
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: The implementation tries to stick maximum to the original *
 *           Algorithm as such. Currently running for SIC only         *
 *                                                                     * 
 *           An Extra addressing mode called Immediate Index Mode was  *
 *           introduced to support Bootstrapper code, but was later    *
 *           added to the standard SIC instruction set and as of now   *
 *           is supported by the assembler                             *
 *           Its similar to indexed type, except we explicity mention  *
 *           the memory location it is going to address in the OPERAND *
 *           part of the instruction (address is mentioned in HEX)     *
 *                                  eg: 1000,X                         *
 *                                                                     *
 *           This function was modified when I was on a Heavy Hangover *
 *           from the previous day's 3 Pegs of Bottoms Up, Whisky Drink* 
 *           :-P Don't Mind if the function has the same mood!!!       * 
 ***********************************************************************
 */
void pass2 (void)
{
    unsigned int i = 0; //Generic / Counter Variable
    unsigned int uiRecordLength = 0; //Holds the Current Record Length
    unsigned int uiSymbolAddress = 0; //Holds the Address from SYMTAB of Symbol in OPERAN field
    unsigned int uiOpcodeValue = 0;
    char strObjectRecord[MAXLEN] = "", *ptrObjectRecord; //Object Record and a pointer to it
    char strBuffer[30] = ""; //A temporary string buffer
    char strByteOrWordOperand[30] = ""; //A string buffer that stores the Operand for Byte or Word, it is used 

    //Initialize the Object Record
    for (i = 0; i < MAXLEN; i++) {
	strObjectRecord[i] = '\0';
    }
    //Initialize the pointer
    ptrObjectRecord = strObjectRecord;
        
    //Read 1st input line
    getLine (fpSource, strParsedInfo);
    
    //If OPCODE == "START"
    if ((strcmp (strParsedInfo[OPCODE], "START") == 0)){
	//Convert OPERAND to HEX Value
	i = (unsigned int) strtoul (strParsedInfo[OPERAND], NULL, HEXADECIMAL_BASE);
	fprintf (fpDestination, "H%-6s%06X%06X\n", strParsedInfo[LABEL], i , uiAssemblerVariables[PROGRAM_LENGTH]);
    }//If START
  
    //Reads from Intermediate File
    getLine (fpSource, strParsedInfo);
    
    //If the 1st OPCODE is RESB / RESW, Dont generate TEXT Record as of yet
    if (strncmp(strParsedInfo[OPCODE], "RES", 3) != 0) {
	sprintf (strObjectRecord, "T%06X00", uiAssemblerVariables[STARTING_ADDRESS]);
    }

    //While OPCODE not End
    while (strcmp (strParsedInfo[OPCODE], "END") != 0) {
	//If the line is not a comment
	if (strParsedInfo[LINE][0] != '.') {
	    //Search OPTAB for OPCODE
	    flags.hasInvalidOpcode = uiOpcodeValue = searchOPTab (OPTabRoot,  strParsedInfo[OPCODE]);	   
	    if (!flags.hasInvalidOpcode) {
		//If there is a symbol in the operand field
		if (strParsedInfo[OPERAND][0] != '\0') {
		    //Check if the Operand is of type Indexed
		    if (strpbrk (strParsedInfo[OPERAND], ",") != NULL) {
			//In SIC, Indexed Address is always with Register X so the part after "," can be ignored
			strcpy (strBuffer, strtok (strParsedInfo[OPERAND], ","));
			strcpy (strParsedInfo[OPERAND], strBuffer);

			//Check the Type of Indexing used
			if (isdigit (strParsedInfo[OPERAND][0])) {
			    flags.hasImmediateIndexMode = 1;
			}
			else {
			    flags.hasIndexMode = 1;
			}
		    }
		    //----Modify for efficiency--- (Kind of looks efficent now :-P)
		    //Search SYMTAB for the OPERAND
		    //Convert the returned value to unsigned int
		    uiSymbolAddress = (unsigned int) searchSYMTab (SYMTabRoot, strParsedInfo[OPERAND]);
		    if (uiSymbolAddress == (unsigned int) -1) {
			flags.hasUndefinedSymbol = 1;
		    }		  
		    if (!flags.hasUndefinedSymbol || flags.hasImmediateIndexMode) {
			//Check the type of Addressing mode and Generate the Address
			if (flags.hasImmediateIndexMode) {
			    //Get the Immediate Value then add 0x8000
			    uiSymbolAddress = (unsigned int) strtoul (strParsedInfo[OPERAND], NULL, HEXADECIMAL_BASE);
			    uiAssemblerVariables[SYMBOL_ADDRESS] = 0x8000 + uiSymbolAddress;
			} else if (flags.hasIndexMode) {
			    //Add 0x8000 to Symbol Value
			    uiAssemblerVariables[SYMBOL_ADDRESS] = 0x8000 + uiSymbolAddress;
			}
			else {
			    //Normal (Direct) Addressing
			    //Store Symbol Value as OPERAND Address
			    uiAssemblerVariables[SYMBOL_ADDRESS] = uiSymbolAddress;
			}
			/*
			 * Why add 0x8000 you ask ??? Remember the SIC Instruction format :-?
			 * [OPCODE 8 bits][X 1 bit][OPERAND-ADDRESS 15 bits]
			 * X - The Index Flag in the Instruction has to be set to show indexed Addressing
			 */
			
			flags.hasInvalidOpcode = flags.hasUndefinedSymbol = flags.hasIndexMode = flags.hasImmediateIndexMode = 0;
		    }
		    else {
			uiAssemblerVariables[SYMBOL_ADDRESS] = 0x0000;
			flags.hasError = 1;
			flags.hasUndefinedSymbol = 0;
			raiseError (ERROR, E_UNDEFINED_SYMBOL, strParsedInfo[OPERAND]); //Raise the Error
		    }
		    //Assemble the Code
		    /*
		     * Assembly Logic is as follows
		     * ----------------------------
		     * opcode value * 0x10000 (HEX) + Symbol address = Object code of that instruction		     
		     * eg: For instruction "SUB ONE"
		     *     OPCODE for SUB is 0x1C
		     *     ONE is a LABEL at address 0x1234
		     * Object Code of instruction is 0x1C * 0x10000 + 0x1234
		     * i.e: 1C1234
		     */
		    uiAssemblerVariables[OBJECT_CODE] = 0x10000 * uiOpcodeValue +  uiAssemblerVariables[SYMBOL_ADDRESS];
		}//If symbol
		else {
		    //Case where the OPCODE has no OPERAND (RSUB in SIC)
 		    //Reset Address and Assemble the Code
		    uiAssemblerVariables[SYMBOL_ADDRESS] = 0x0000;
		    uiAssemblerVariables[OBJECT_CODE] = 0x10000 * uiOpcodeValue +  uiAssemblerVariables[SYMBOL_ADDRESS];  
		}
		
		uiRecordLength += TEXT_RECORD_WORD_LENGTH; //Compute Current Record's Length

		//Generate Object Code Only if a New Text Record Request is not Pending
		if (!flags.isNewTextRecord) {
		    sprintf (ptrObjectRecord + strlen (strObjectRecord), "%06X", uiAssemblerVariables[OBJECT_CODE]);
		}
		
	    }//If OPCODE found
	    else {
		//Handle BYTE, WORD, RESB, RESW
		
		//Handle BYTE
		if (strcmp (strParsedInfo[OPCODE], "BYTE") == 0) {

		    //Is the Content Hex ??
		    if (strParsedInfo[OPERAND][0] == 'X') {
			sscanf (strParsedInfo[OPERAND], "X'%[^\']", strByteOrWordOperand);
			//in X' ' Two characters represent 1 Byte
			uiRecordLength += ((strlen(strParsedInfo[OPERAND]) - 3) / 2); //Compute Record Length

			//Generate Object Code Only if a New Text Record Request is not Pending
			if (!flags.isNewTextRecord) {
			    //Print String since it is in HEX already			  
			    sprintf (ptrObjectRecord + strlen (strObjectRecord), "%s", strByteOrWordOperand);
			}
		    }
		     
		    //Is the Content Character ??
		    if (strParsedInfo[OPERAND][0] == 'C') {
			//sscanf (strParsedInfo[OPERAND], "C'%[^\']", tempchar);
			sscanf (strParsedInfo[OPERAND], "C'%[^\']", strByteOrWordOperand);
			//in 'C ' Each character represent 1 Byte
			uiRecordLength += (strlen(strParsedInfo[OPERAND]) - 3); //Compute Record Length

			//Generate Object Code Only if a New Text Request is not Pending
			if (!flags.isNewTextRecord) {
			    //Convert the Characters to ASCII (Hex) and store them
			    for (i = 0; i < (unsigned int) strlen (strByteOrWordOperand); i++) {
				sprintf (ptrObjectRecord + strlen (strObjectRecord), "%01X", (unsigned int) strByteOrWordOperand[i]);
			    }
			}
		    }
		}

		//Handle WORD
		if(strcmp (strParsedInfo[OPCODE], "WORD") == 0) {
		    uiAssemblerVariables[OBJECT_CODE] = atoi (strParsedInfo[OPERAND]);
		    uiRecordLength += TEXT_RECORD_WORD_LENGTH;
		    //Generate Object Code Only if a New Text Request is not Pending
		    if (!flags.isNewTextRecord) {
			sprintf (ptrObjectRecord + strlen (strObjectRecord), "%06X", uiAssemblerVariables[OBJECT_CODE]);
		    }		      
		}

		//Handle RESB
		if (strcmp (strParsedInfo[OPCODE], "RESB") == 0) {
		    uiRecordLength += atoi (strParsedInfo[OPERAND]);
		    //Force to break current TEXT Record
		    flags.isNewTextRecord = 1;
		}
		    
		//Handle RESW
		if (strcmp (strParsedInfo[OPCODE], "RESW") == 0) {
		    uiRecordLength += (atoi (strParsedInfo[OPERAND]) * TEXT_RECORD_WORD_LENGTH);
		    //Force to break current TEXT Record
		    flags.isNewTextRecord = 1; 
		}
	    }//Handle BYTE, WORD, RESB, RESW
	    
	    //Check if the next record insertion will fit the current TEXT Record OR a New Record is forced
	    if ((uiRecordLength + TEXT_RECORD_WORD_LENGTH >  TEXT_RECORD_MAX_LENGTH) || (flags.isNewTextRecord)) {
		
		//Do not generate Entries for RESB / RESW
		if (strncmp (strParsedInfo[OPCODE], "RES", 3) != 0) {
		    //Finalize the Text Record and Insert the Record Length
		    ptrObjectRecord = ptrObjectRecord + TEXT_RECORD_HEAD_LENGTH - TEXT_RECORD_LENGTH_POSITION;
		    sprintf (strBuffer, "%02X", (unsigned int) (strlen (strObjectRecord) - TEXT_RECORD_HEAD_LENGTH) / 2);
		    strncpy (ptrObjectRecord, strBuffer, TEXT_RECORD_LENGTH_POSITION);

		    //Output the Record into the File
		    //If there are RESB/RESW in the beginning of the Assembly,
		    //there is a chance of blank TEXT Record being generated
		    if (strObjectRecord[0] == 'T') {
			fprintf (fpDestination, "%s\n", strObjectRecord);
		    }

		    //Reset the Record
		    for (i = 0; i < MAXLEN; i++) {
			strObjectRecord[i] = '\0';
		    }
		
		    //Roll Back the Excess Record Number after Detecting the End of Reservations
		    /*
		     * Logic for Seeing if a Reservation section has ended is when the Instructions read is
		     * over shot and Enters non RESB / RESW. This causes the RecordLength to increase excessively
		     * by the size of that instruction so a roll back is necessary before generating fresh records
		     */
		    if (flags.isNewTextRecord) {
			//If it is a RESB
			if (strcmp (strParsedInfo[OPCODE], "BYTE") == 0) {
			    //Case for Character
			    if (strParsedInfo[OPERAND][0] == 'C') {
				uiRecordLength -= strlen (strByteOrWordOperand);
			    }

			    //Case for Hex
			    if (strParsedInfo[OPERAND][0] == 'X') {
				uiRecordLength -=  (strlen (strByteOrWordOperand) / 2);
			    }
			} 
			else {
			    //This is the case for RESW
			    uiRecordLength -= TEXT_RECORD_WORD_LENGTH;
			}
		    }
		    
		    //Prepare a fresh record
		     //Compute Starting  Address of Next Text Record
		    uiAssemblerVariables[STARTING_ADDRESS] += uiRecordLength;
		    sprintf (strObjectRecord, "T%06X00", uiAssemblerVariables[STARTING_ADDRESS]);
		    ptrObjectRecord = strObjectRecord; //Reset Pointer
		    uiRecordLength = 0; //Reset Values
		    
		    //Generate Object Code for the Currently read OPCODE,
		    //After the New Text Record Request was made
		    if (flags.isNewTextRecord) {
			//Code generation is a bit Different for BYTE
			if (strcmp (strParsedInfo[OPCODE], "BYTE") == 0) {
			    if (strParsedInfo[OPERAND][0] == 'X') {
				//Print String since it is in HEX already
				sprintf (ptrObjectRecord + TEXT_RECORD_HEAD_LENGTH, "%s", strByteOrWordOperand);
			    }
			    
			    if (strParsedInfo[OPERAND][0] == 'C') {
				//Convert the Characters to ASCII (Hex) and print them		
				for (i = 0; i < (unsigned int) strlen (strByteOrWordOperand); i++) {
				    sprintf (ptrObjectRecord + TEXT_RECORD_HEAD_LENGTH, "%01X", (unsigned int) strByteOrWordOperand[i]);
				}
			    }
			} 
			else {
			    //Rest of the cases (all other OPCODEs)
			    sprintf (ptrObjectRecord + TEXT_RECORD_HEAD_LENGTH, "%06X",  uiAssemblerVariables[OBJECT_CODE]);
			}
			//Set the Record Length
			uiRecordLength = (unsigned int) (strlen (strObjectRecord) - TEXT_RECORD_HEAD_LENGTH) / 2;
		    }
		    //Reset the New Record Flag
		    flags.isNewTextRecord = 0;
		}
	    }
	}//If not comment
	
	//Reset the Object Code
	uiAssemblerVariables[OBJECT_CODE] = 0x000000;

	//Get the next Line from Intermediate File
	getLine (fpSource, strParsedInfo);
	    
	//Check for HLT Instruction
	if (strcmp (strParsedInfo[OPCODE], "HLT") == 0) {
	    strParsedInfo[OPERAND][0] = '\0';
	}
    }//while not END
    
    //Write the final TEXT Record
    //Finalize the Text Record and Insert the Record Length
    ptrObjectRecord = ptrObjectRecord + TEXT_RECORD_HEAD_LENGTH - TEXT_RECORD_LENGTH_POSITION;
    sprintf (strBuffer, "%02X", (unsigned int) (strlen (strObjectRecord) - TEXT_RECORD_HEAD_LENGTH) / 2);
    strncpy (ptrObjectRecord, strBuffer, TEXT_RECORD_LENGTH_POSITION);

    //If there are RESB/RESW in the beginning of the Assembly,
    //there is a chance of blank TEXT Record being generated
    if (strObjectRecord[0] == 'T') {
	//Output the Final Record into the File
	fprintf (fpDestination, "%s\n", strObjectRecord);
    }
    
    //Write the END Record
    fprintf (fpDestination, "E%06X", uiAssemblerVariables[FIRST_EXECUTABLE_ADDRESS]);
}//end Pass 2

/*
 ***********************************************************************
 * Function Name: createBootstrap                                      *
 *                                                                     * 
 * Purpose: Implements a custom stipper to stip off the Header, Text & *
 *          End records to modify it to a bootstrap assembly           *
 *                                                                     *  
 * Parameters: None                                                    * 
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: This is only called if -b flag is set on assemble time    *
 *           Text Record size for Boot strap assembly is 32 compared   *
 *           to 30 bytes in normal Text Records                        * 
 *                                                                     * 
 *           Bootstraps have a maximum size limit of 128 Bytes         * 
 *                                                                     * 
 *           Error Handling and Size checking needs to be implemented  * 
 ***********************************************************************
 */
void createBootstrap (void)
{
    char strObjectRecord[MAXLEN]; //Object Record from assembly file
    char strStrippedObjectRecord[MAXLEN]; //Stripped Record for Bootstrap
    unsigned int uiTotalRecordLength = 0; //Total Record Length
    
    //Counter Variables
    unsigned int i = 0;
    unsigned int j = 0;    
    
    //Initialize the string
    for (i = 0; i < MAXLEN; i++) {
	strStrippedObjectRecord[i] = '\0';
    }

    //Read till the EOF of Object File
    while (!feof(fpSource)) {
	//Initialize i to Record Head Length	
	i = TEXT_RECORD_HEAD_LENGTH; 
	
	//Get the Object code from Source file
	fgets (strObjectRecord, MAXLEN, fpSource);
	
	//Process only TEXT Records
	if (strObjectRecord[0] == 'T') {
	    //Check for the End of current TEXT Record
	    while ((strObjectRecord[i] != '\0') && (strObjectRecord[i] != '\n')) {
		//Put the Assembled code (Excluding The Record Head) to the Stripped Record
		strStrippedObjectRecord[j++] = strObjectRecord[i++];
		
		//Check if it has reached the maximum Record Size
		if (strlen (strStrippedObjectRecord)/ 2 == BOOTSTRAP_RECORD_MAX_LENGTH) {
		    //Put an End Marker
		    strStrippedObjectRecord[j] = '\0'; 
		    //Increment the Total Record Length
		    uiTotalRecordLength += (strlen (strStrippedObjectRecord) / 2);
		    //Write Output to Device
		    fprintf (fpDestination, "%s\n", strStrippedObjectRecord);
		    //Reinitialize the string
		    for (j = 0; j < MAXLEN; j++) {
			strStrippedObjectRecord[j] = '\0';
		    }
		    //Reset j
		    j = 0;
		}
	    }	    
	}
    }
    
    //Caculate the Final Written Record Length
    uiTotalRecordLength += (strlen (strStrippedObjectRecord) / 2);

    //Pad the rest with Zeros, to fill up the bootstrap record
    for (i = uiTotalRecordLength; i < BOOTSTRAP_MAX_LENGTH; i++) {
	//Remember that 2 Colums is 1 Byte so 2 Zeros for Each Byte of Padding
	strStrippedObjectRecord[j++] = '0';
	strStrippedObjectRecord[j++] = '0';
	//Check if it has reached the maximum Record Size
	if (strlen (strStrippedObjectRecord)/ 2 == BOOTSTRAP_RECORD_MAX_LENGTH) {
	    //Put an End Marker
	    strStrippedObjectRecord[j] = '\0';
	    //Write Output to Device
	    fprintf (fpDestination, "%s\n", strStrippedObjectRecord);
	    //Reinitialize the string
	    for (j = 0; j < MAXLEN; j++) {
		strStrippedObjectRecord[j] = '\0';
	    }
	    //Reset j
	    j = 0;
	}
    }
}

/*
 ***********************************************************************
 * Function Name: createLoader                                         *
 *                                                                     * 
 * Purpose: Implements a custom stipper to stip off the Header, Text & *
 *          End records to modify it to a loader assembly              *
 *                                                                     *  
 * Parameters: None                                                    * 
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: This is only called if -l flag is set on assemble time    *
 *           Text Record size for loader assembly is same as that of   *
 *           normal SIC Text records, it just lacks the Text Header    *
 *                                                                     * 
 *                                                                     * 
 *           Error Handling needs to be implemented                    * 
 ***********************************************************************
 */
void createLoader (void)
{
    char strObjectRecord[MAXLEN]; //Object Record from assembly file
    char strStrippedObjectRecord[MAXLEN]; //Stripped Record for Bootstrap
    
    //Counter Variables
    unsigned int i = 0;
    unsigned int j = 0;    
    
    //Initialize the string
    for (i = 0; i < MAXLEN; i++) {
	strStrippedObjectRecord[i] = '\0';
    }

    //Read till the EOF of Object File
    while (!feof(fpSource)) {
	//Initialize i to Record Head Length	
	i = TEXT_RECORD_HEAD_LENGTH; 
	
	//Get the Object code from Source file
	fgets (strObjectRecord, MAXLEN, fpSource);
	
	//Process only TEXT Records
	if (strObjectRecord[0] == 'T') {
	    //Check for the End of current TEXT Record
	    while ((strObjectRecord[i] != '\0') && (strObjectRecord[i] != '\n')) {
		//Put the Assembled code (Excluding The Record Head) to the Stripped Record
		strStrippedObjectRecord[j++] = strObjectRecord[i++];				
	    }
	}
	
	//Put the End marker
	strStrippedObjectRecord[j] = '\0';	
	//Output the line, it is not empty
	if (strStrippedObjectRecord[0] != '\0') {
	  fprintf (fpDestination, "%s\n", strStrippedObjectRecord);
	}
	
	//Initialize the string
	for (i = 0; i < MAXLEN; i++) {
	    strStrippedObjectRecord[i] = '\0';
	}
	//Reset j
	j = 0;
    }
}

/*
 ***********************************************************************
 * Function Name: parseArguments                                       *
 *                                                                     * 
 * Purpose: This function parses the command line arguments passed to  *
 *          the assembler from user at command line and tries to make  *
 *          sense out of it (if it is not nonsense :-D)                *  
 *                                                                     *  
 * Parameters: argc - Argument Count got from command line in main()   * 
 *             argv - Argument Values got from command line in main()  *
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: Incomplete and Error Handling needs to be implemented     * 
 *           It is a minimal implementation and still require work     *
 *           Supported Arguments:                                      *
 *           'o' - The target output file                              *
 *           'b' - Indicates a Bootstrap Assembly                      *
 *           'l' - Indicates a Loader Assembly                         *
 *           'd' - The SIC Device to which the Assembly needs to be    *
 *                 loaded to for the SICvm                             *
 *           'h' - Displays onscreen help                              *
 *           'O' - Specify an External OPTAB for Assembly              *
 *           'V' - Gives a more verbose Assembly Output                *
 *           'v' - Displays the version number :-P                     *
 *                                                                     *
 ***********************************************************************
 */
void parseArguments (int argc, char *argv[])
{
    //int index;
    char cArgument;

    opterr = 0; //Set opterr to 0 to supress display of Default Argument Parse Errors
    
    if (argc == 1) {
	display_help ('\0');
	exit (EXIT_SUCCESS);
    }
    //Parse through each of the arguments
    while ((cArgument = getopt (argc, argv, "o:bld:O::vVh")) != -1) {
	switch (cArgument) {
	case 'o': //Determines the output file
	    strcpy (strDestinationFileName, optarg);
	    break;

	case 'b': //Determines if the current assembly is of Bootstrap type
	    //Safety check to Disable Setting both Bootstrap and Loader type Assembly
	    if (!flags.isLoaderAssemble) {
		flags.isBootStrapAssemble = 1;
		strcpy (strDeviceName, "DEV00"); //Copy the Default Device name
	    }
	    else {	      
	      raiseError (FATAL,F_BOOT_AND_LOAD_ASSEMBLE,"");
	      //printf ("Cannot do both Bootstrap and Loader Assembly in one shot\n");
	    }
	    break;

	case 'l': //Determines if the current assembly is of Loader Type
	    //Safety check to Disable Setting both Bootstrap and Loader type Assembly
	    if (!flags.isBootStrapAssemble) {
		flags.isLoaderAssemble = 1;
		strcpy (strDeviceName, "DEVF1"); //Copy the Default Device name
	    }
	    else {
	      //printf ("Cannot do both Bootstrap and Loader Assembly in one shot\n");
	      raiseError (FATAL,F_BOOT_AND_LOAD_ASSEMBLE,"");
	    }
	    break;	    

	case 'd': //Gets the output SIC Device to which the Assembly has to be loaded
	    strcpy (strDeviceName, optarg); //Copy the user supplied Device name
	    break;

	case 'O': //Specifies the External OPTAB to be used for assembly process
	    if (optarg != NULL) {
	      flags.hasExternOptab = 1;
	      strcpy (strOptabFileName, optarg);  //Copy the user supplied external Optab file name
	    }
	    else {
	      flags.hasExternOptab = 1;
	      strcpy (strOptabFileName, "optab.txt"); //Copy the default optab file name
	    }
	    break;
	    
	case 'v': //Displays the Version number
	  printf ("sicasm: A Simple SIC Assembler v0.0.1\n\n");
	  exit (EXIT_SUCCESS);
	  //break;

	case 'V': //Enables Verbose Output
	  flags.hasVerboseOutput = 1;
	  break;

	case 'h'://Displays the help text for sicasm
	  display_help ('\0');
	  exit (EXIT_SUCCESS);
	  //break;

	case '?'://Handles the Cases of Missing Params for Arguments with Required Params
	    switch (optopt) {
	    case 'o':
	      raiseError (FATAL,F_NO_FILE_NAME,"");
	      break;

	    case 'd':
	      raiseError (FATAL,F_DEVICE_NAME_MISSING,"");
	      break;

	    default: 
		break;
	    }
	    break;

	default:
	    break;
	}
    }
    
    //Checks for the, non-option argument (the file to be assembled)
    if (optind == argc) {
      flags.hasError = 1;
      raiseError (FATAL, F_INPUT_FILE_MISSING, "");	    
    }
    else {
	//Copy the source file name
	strcpy (strSourceFileName, argv[optind]);
	//If no output file have been specified, generate default filename 
	if (strcmp (strDestinationFileName, "") == 0)  {
	    strcpy (strDestinationFileName, strtok (strSourceFileName, ".")); 
	    strcat (strDestinationFileName, ".obj");
	}
	//Restore the file name
	strcpy (strSourceFileName, argv[optind]);
    }
    
    //Enable Verbose output for the application
    if (flags.hasVerboseOutput) {
	printf ("Using \"%s\" as Source File\n", strSourceFileName);
	printf ("Using \"%s\" as Assembled File\n",strDestinationFileName);
	printf ("Using \"%s\" as Intermediate File\n",strIntermediateFileName);
	
	if (flags.hasExternOptab) {
	    printf ("Using External OPTAB \"%s\" to load Opcodes\n",strOptabFileName);
	}
	else {
	    printf ("Using Internal OPTAB to load Opcodes\n");
	}

	if (flags.isBootStrapAssemble) {
	    printf ("\"%s\" will be stipped to Bootloader format and stored in \"%s\" device\n",strDestinationFileName, strDeviceName);
	}

	if (flags.isLoaderAssemble) {
	    printf ("\"%s\" will be stipped to Loader format and stored in \"%s\" device\n",strDestinationFileName, strDeviceName);
	}
	printf ("\n");
    }
}
