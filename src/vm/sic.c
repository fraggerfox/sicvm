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
 * Module Name: sic.c                                                  *
 *                                                                     * 
 * Purpose: The Main SIC machine Architecture work is written here     *
 *          The Execution of the machine takes place in the following  *
 *          1) Initialize the Registers, Memory and I/O Devices        *
 *          2) Run the Machine, but setting the execution Mode         *
 *                                                                     *
 * Comments: Need to Refine my emulation logic, this is n00b stuff     *     
 ***********************************************************************
 */
//Standard Headers
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//Custom Headers
#include "sic.h"
#include "delay.h"
#include "device.h"
#include "interrupt.h"
#include "memory.h"
#include "register.h"

/*
 * Define the SIC Instruction Set Over here
 * I do not know if this is a flexible Idea, but when I gave
 * it a thought, Instruction Set + CU + ALU + Registers define
 * a given machine in a rough sense, so I though of clubbing 
 * Instruction Set + CU + ALU and keeping registers in a separate
 * header file
 */
//Arithmetic Logic Instruction
#define ADD 0x18
#define SUB 0x1C
#define MUL 0x20
#define DIV 0x24
#define OR 0x44
#define AND 0x40
#define COMP 0x28
#define TIX 0x2C

//Branch Instructions
#define J 0x3C
#define JEQ 0x30
#define JGT 0x34
#define JLT 0x38
#define JSUB 0x48
#define RSUB 0x4C

//Load Store Instructions
#define LDA 0x00
#define LDCH 0x50
#define LDL 0x08
#define LDX 0x04
#define STA 0x0C
#define STCH 0x54
#define STL 0x14
#define STX 0x10

//Device Access Instructions
#define RD 0xD8
#define TD 0xE0
#define WD 0xDC

//System Instructions
#define HLT 0x76

//Type of Instruction
#define ARITHMETIC 0
#define LOGIC 1
#define LOADSTORE 2
#define BRANCH 4
#define IO 5

//Input Device Codes
//New to SIC Definition. Helps outputing 
//This to Standard Output and getting things from
//Standard Input
#define STDI 0xF0
#define STDO 0xA0

//Type of Fetch
//Different Types of Fetches executed by SIC
#define DATA_FETCH 0
#define OPCODE_FETCH 1
#define OPERAND_FETCH 2
#define DEVCODE_FETCH 4

//Defines the Flags helping in identifying the running mode
struct Flags {
    unsigned int isBootExecution     : 1; //Set when machine is doing a Boot Execution
    unsigned int isShellExecution    : 1; //Set when machine is doing an execution from Shell
    unsigned int isLoaderExecution   : 1; //Set when machine is doing a loader execution
    unsigned int isUserExecution     : 1; //Set when machine is doing a User Execution
    unsigned int hasInterrupt        : 1; //Set when the condition for an interrupt occurs
};

int iInterruptType = -1; //Set to the Type od Interrupt when an Interrupt Occurs

//The Hex Code for OPCODE and OPERAND got from the SIC Memory
unsigned int uiHexOpcode;
unsigned int uiHexOperand;

struct Flags flags;

/*
 ***********************************************************************
 * Function Name: init                                                 *
 *                                                                     * 
 * Purpose: Initializes the SIC registers, Main memory and I/O Devices *
 *                                                                     *  
 * Parameters: None                                                    * 
 *                                                                     *
 * Returns: 0 on a successfull Initialization else 1                   *
 *                                                                     *
 * Comments: The SIC Machine is defined by the following components    *
 *           1) Memory and Registers                                   *
 *           2) The SIC Logic and Instruction set                      *
 *           3) The I/O Devices attached t it                          *
 *           4) The interrupts (this is a new modification for SIC)    * 
 *                                                                     *
 *           This function initializes, the Memory, Registers, I/O     *
 *           Devices to cetain default values                          *
 ***********************************************************************
 */
int init (void)
{
    int i = 0;
    int iReturnValue = 0;

    //Register Initialization
    A = X = L = 0xFFFFFF;
    SW = SW & CLEAR;
    PC = 0x000000;
  
    //Memory Initialization
    ucrgMemory = (unsigned char *) malloc (MEMSIZE);
  
    if (ucrgMemory == NULL) {
	raiseInterrupt (INT_BADMEM);
	iReturnValue = 1;
    }

    for (i = 0; i < MEMSIZE; i++) {
	ucrgMemory[i] = 0xFF;
    }

    if (iReturnValue == 0) {
	printf ("Registers & Memory Initialized\n");
    }

    //Initialize I/O Devices
    if (initIODevice ("DEV00", BOOT_DEVICE) == NULL) { //Initialize the Boot ROM Device
	raiseInterrupt (INT_BADBOOT);
	iReturnValue = 1;
    }
  
    if (initIODevice ("DEVF1", INPUT_DEVICE) == NULL) { //Initialize the Standard Input Device
	raiseInterrupt (INT_BADLOAD);
	iReturnValue = 1;
    } 

    if (iReturnValue == 0) {
	printf ("I/O Devices Initialized Successfully\n");
    }

    return iReturnValue;
}

/*
 ***********************************************************************
 * Function Name: run                                                  *
 *                                                                     * 
 * Purpose: Function for running the machine.It basically identifies   *
 *          the kind of opcode, The calls the execute fuction          *
 *                                                                     *  
 * Parameters: The kind of execution that will take place              * 
 *                                                                     *
 * Returns: Interrupt Value after run to see if an error has occured   *
 *                                                                     *
 * Comments: This is the code that actually determines how a given     *
 *           instruction will be executed in SIC machine, basically    *
 *           it acts like a control unit, it will decide the how the   *
 *           data is fetched / stored and how instructions will be     *
 *           executed.                                                 * 
 *                                                                     *
 *           This CU of SIC is able to differentiated between a System *
 *           Boot Run and a Normal Shell run, it also does simple      *
 *           checks like Segment Accesses, Memory Bound and Invalid    *
 *           instruction codes.                                        *
 ***********************************************************************
 */
int run (int iExecutionType)
{
    int i = 0;
    iInterruptType = INT_NONE; //Reset all interrupts
    flags.hasInterrupt = 0; //No Interrupts are Set

    /*
     * The values of These two variables is the current Device number 
     * in use it varies from 0 to MAX_DEVICES - 1, -1 indicates no
     * device is in use
     */
    iInUseOutputDevice = iInUseInputDevice = -1; //No Devices are in Use

    /*
     * The Device Access Flag is an array associated with a Device
     * using the index number in the array as an I/O device Identifier
     * 
     */
    for (i = 0; i < MAX_DEVICES; i++) {
	irgDeviceAccessFlags[i] = 0;
    }

    initSeed = 0;

    //Set the type of execution being carried out
    switch (iExecutionType) {
    case BOOT:
	flags.isBootExecution = 1;
	flags.isShellExecution = 0;
	flags.isLoaderExecution = 0;
	flags.isUserExecution = 1;
	break;

    case SHELL:
	flags.isShellExecution = 1;
	flags.isBootExecution = 0;
	flags.isLoaderExecution = 0;
	flags.isUserExecution = 0;
	break;

    case LOADER:
	flags.isLoaderExecution = 1;
	flags.isShellExecution = 0;
	flags.isBootExecution = 0;
	flags.isUserExecution = 0;
	break;

    case USER:
	flags.isUserExecution = 1;
	flags.isLoaderExecution = 0;
	flags.isShellExecution = 0;
	flags.isBootExecution = 0;
	break;

    default:
	break;
    }
    
    /*
     * This is the core SIC CPU Logic. Acts as the entry point into the machine excution
     * 
     * The Value for the First OPCODE Fetch is got from the Memory Location pointed to
     * by PC Register
     *
     * Support functions include - Compute, Errors, Interrupts, device handling...etc
     * Basically this loop does all the hardwork
     *
     */
    do {
	if (flags.hasInterrupt) { //Check for Interrupts if any
	    raiseInterrupt (iInterruptType);
	    break;
	}
	else {  //Do Normal Execution Routine
	    uiHexOpcode = fetch (OPCODE_FETCH); 	    //Type 1 Fetch: Fetch Opcode for execution
	}
	//i++

	/*
	 * Get the OPERAND from the Memory Location if required
	 * after identifying the OPCODE
	 *
	 * Then Execute the Instruction
	 *
	 */
	switch (uiHexOpcode) {
	
	case ADD: case SUB: case MUL: case DIV:
	    uiHexOperand = fetch (OPERAND_FETCH); //Type 2 Fetch: Fetch Operand for execution
	    execute (ARITHMETIC, uiHexOpcode, uiHexOperand);
	    break;

	case AND: case OR: case TIX: case COMP:
	    uiHexOperand = fetch (OPERAND_FETCH);
	    execute (LOGIC, uiHexOpcode, uiHexOperand);
	    break;
	  
	case LDA: case STA: case STL: case LDL: case STX: case LDX: case STCH: case LDCH:
	    uiHexOperand = fetch (OPERAND_FETCH);

	    //Check if the user program is trying to access System Memory 0000H-0999H, if so raise a Segmentation fault Interrupt
	    if ((uiHexOperand < 0x1000) && ((uiHexOpcode == STA) || (uiHexOpcode == STX) || (uiHexOpcode == STL) || (uiHexOpcode == STCH)) && ((SW & GET_MODE) != SU_MODE)) {
		flags.hasInterrupt = 1;
		iInterruptType = INT_SEGFAULT;
		break;
	    }

	    //Current SIC has a memory limit of FFFFFH, 
	    //any refence beyond it raises an Out of Memory Interrupt
	    if (uiHexOperand > 0x0FFFFF) {
		flags.hasInterrupt = 1;
		iInterruptType = INT_OUTOFMEM;
		break;
	    }
	    execute (LOADSTORE, uiHexOpcode, uiHexOperand);
	    break;

	case J: case JEQ: case JGT: case JLT: case JSUB: case RSUB:
	    uiHexOperand = fetch (OPERAND_FETCH);

	    //This little IF switches the execution mode
	    //between User Mode and Super User Mode
	    //This is not a safe implementation, its more of a temporary mockup :-D
	    if (uiHexOperand >= 0x1000) {
		SW = SW & U_MODE;
	    }
	    else {
		SW = SW | SU_MODE;
	    }

	    //Current SIC has a memory limit of FFFFFH, 
	    //any refence beyond it raises an Out of Memory Interrupt	  
	    if (uiHexOperand > 0x0FFFFF) {
		flags.hasInterrupt = 1;
		iInterruptType = INT_OUTOFMEM;
		break;
	    }
	    execute (BRANCH, uiHexOpcode, uiHexOperand);
	    break;

	case TD: case WD: case RD:
	    uiHexOperand = fetch(OPERAND_FETCH);
	    execute(IO, uiHexOpcode, uiHexOperand);
	    break;

	case HLT:
	    break;

	default:
	    flags.hasInterrupt = 1;
	    iInterruptType = INT_INVALID;
	    break; //A bit of defensive programming ;-)
	}
	/*if (i > 10000)
	  break;*/
    }  while (uiHexOpcode != HLT); //SIC CPU Logic Ends here

    return iInterruptType;
}

/*
 ***********************************************************************
 * Function Name: execute                                              *
 *                                                                     * 
 * Purpose: Function for executing the code. It basically identifies   *
 *          the kind of opcode, The calls the execute fuction          *
 *                                                                     *  
 * Parameters: The kind of execution that will take place              * 
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: This is the code that actually does the operation on the  *
 *           OPCODE given in the current instruction. They are divided *
 *           into the following types                                  *
 *                                                                     *
 *           Arithmetic Instructions                                   * 
 *           Logic Instructions                                        *
 *           Load Store Instructions                                   *
 *           Branch Instructions                                       *
 *           I/O Instructions                                          *
 *                                                                     *
 *           This acts like the ALU of SIC                             *
 ***********************************************************************
 */
void execute (unsigned int uiInstructionType, unsigned int uiHexOpcode, unsigned int uiHexOperand)
{
    int temp = 0;
    char filename[10];

    switch (uiInstructionType) {
	//Here we handle all the Arithmetic Instructions
	//Do not assume its Arithmetic-Logic :-D
    case ARITHMETIC:
      
	switch (uiHexOpcode) {
	case ADD:
	    A += fetch (DATA_FETCH); //Type 3 Fetch: Fetch Data from memory
	    if (A > 0xFFFFFF) {
		flags.hasInterrupt = 1;
		iInterruptType = INT_OVF;
	    }
	    break;

	case SUB:
	    A -= fetch(DATA_FETCH);
	    break;

	case MUL:
	    A *= fetch (DATA_FETCH);
	    break;

	case DIV:
	    temp = fetch (DATA_FETCH);
	    if (temp == 0) {
		flags.hasInterrupt = 1;
		iInterruptType = INT_DIVZERO;
	    }
	    else {       
		A /= temp;
	    }
	    break;
	}
	break;
      

	//Here we handle all the Logic Instructions
    case LOGIC:
      
	switch (uiHexOpcode) {
	case AND:
	    A &= fetch (DATA_FETCH);
	    break;
	    
	case OR:
	    A |= fetch (DATA_FETCH);
	    break;

	case TIX:
	    X++;
	    SW = SW & CLEAR;
	    if (X == (temp = fetch (DATA_FETCH))) {
		SW = SW | EQ;
	    }
	    else {
		if (X > temp) {
		    SW = SW | GT;
		}
		else {
		    SW = SW | LT;
		}
	    }
	    break;

	case COMP:
	    SW = SW & CLEAR;
	    if (A == (temp = fetch (DATA_FETCH))) {
		SW = SW | EQ;
	    }
	    else {
		if (A > temp) {
		    SW = SW | GT;
		}
		else {
		    SW = SW | LT;
		}
	    }
	    break;
	}
	break;
    
      
	//Here we handle all the Load-Store Instructions
	//Its nothing but the memory (internal) operations, but it dosen't handle, devices
    case LOADSTORE:
      
	switch (uiHexOpcode)	  {
	case LDA:
	    A = fetch (DATA_FETCH);
	    break;

	case STA:
	    store (A);
	    break;

	case LDX:
	    X = fetch (DATA_FETCH);
	    break;

	case STX:
	    store (X);
	    break;

	case LDL:
	    L = fetch (DATA_FETCH);
	    break;

	case STL:
	    store (L);
	    break;

	case STCH:
	    ucrgMemory[uiHexOperand] = A & 0xFF;
	    break;

	case LDCH:
	    A = A & 0xFFFF00;
	    A = A | ucrgMemory[uiHexOperand];
	    break;
	}
	break;
      
      
	//Here we handle all the possible Branch instructions
	//Includes both conditional and un-conditional branching
    case BRANCH:
      
	switch (uiHexOpcode) {
	case J:
	    PC = uiHexOperand;
	    break;

	case JEQ:
	    if ((SW & CCMASK) == EQ) {
		PC =  uiHexOperand;
	    }
	    break;

	case JLT:
	    if ((SW & CCMASK) == LT) {
		PC =  uiHexOperand;
	    }
	    break;

	case JGT:
	    if ((SW & CCMASK) == GT) {
		PC =  uiHexOperand;
	    }
	    break;

	case JSUB:
	    L = PC;
	    PC = uiHexOperand;
	    break;

	case RSUB:
	    PC = L;
	    break;
	}	  
	break;

      

	//Here we handle all the external memory operations
	//Basically device handling, includes Standard Input / Output handling also
    case IO :
      
	switch (uiHexOpcode)  {
	case TD :
	    uiHexOperand=fetch(DEVCODE_FETCH); //Type 3 (Byte Fetching)
	
	    //iDeviceIndex is an array index used to keep the status of device
	    //OPERAND is converted into a valid Array value
	    /*
	     * Convention used ----
	     * All Input Devices start from 0xF1 ie 241 ie operand - 0xF1 is iDeviceIndex
	     * All Output Devices start from 0x00 hence gives iDeviceIndex without any change
	     */
	    if (uiHexOperand >= 0xF1) {
		iDeviceIndex = uiHexOperand - 0xF1;
	    }
	    else {
		iDeviceIndex = uiHexOperand;
	    }
	    
	    //Get input or put output to standard devices
	    if ((uiHexOperand == STDI) || (uiHexOperand == STDO)) {
		filename[0] = '\0';
		if (uiHexOperand == STDI) {
		    if (iInUseInputDevice >= 0) {
			irgDeviceAccessFlags[iInUseInputDevice] = 0;
			closeIODevice (fpInputDevice);
			iInUseInputDevice = -1;
		    }
		    initIODevice (NULL, INPUT_DEVICE);
		}
		else {
		    if (iInUseOutputDevice >= 0) {
			irgDeviceAccessFlags[iInUseOutputDevice] = 0;
			closeIODevice (fpOutputDevice);
			iInUseInputDevice = -1;
		    }
		    initIODevice (NULL, OUTPUT_DEVICE);
		}
		SW = SW & CLEAR;
		SW = SW | LT;
	    }
	    else { //The device called was a file
	      
		if (irgDeviceAccessFlags[iDeviceIndex] == 0) {
		    if (flags.isBootExecution == 0)  {
			sprintf (filename, "DEV%02X", uiHexOperand);
			if (uiHexOperand >= 0xF1) {
			    if (initIODevice (filename, INPUT_DEVICE) == NULL) {
				flags.hasInterrupt = 1;
				iInterruptType = INT_DEVERR;
			    }
			    iInUseInputDevice = iDeviceIndex;
			}
			else {
			    if (initIODevice (filename, OUTPUT_DEVICE) == NULL) {
				flags.hasInterrupt = 1;
				iInterruptType = INT_DEVERR;
			    }
			    iInUseOutputDevice = iDeviceIndex;
			}
		    }
		    irgDeviceAccessFlags[iDeviceIndex] = 1;
		    SW = SW & CLEAR;
		    SW = SW | LT;
		}
		else {
		    SW = SW & CLEAR;

		    //Implementation of device delay emulation
		    //This is not a necessay feature, it was implemented to emulate
		    //Device delays as seen in case of a real machine
		    if (initSeed == 0) {
			srand (time(NULL));
			initSeed = 1;
		    }
		    
		    iAccessValue = rand() % MAX_RANDOM_ACCESS_VALUES;
		    if (iAccessValue == ALLOW_ACCESS) {
			SW = SW | LT;
		    }
		    else {
			SW = SW | EQ;
			delay ();
		    }
		}
	    }
	      
	    break;      

	case RD:
	    uiHexOperand = fetch (DEVCODE_FETCH); //Type 3 (Byte Fetching)
	    A = readDevice (fpInputDevice);

	    //The following was done so that the Device I/O works correctly
	    //I am still working on a better logic for this
	    /*
	     * Currently of a -ve value of readDevice, we convert the value
	     * into a End of Transmission Signal in ASCII Hex
	     */
	    if (A < 0) {
		A = 0x04;
	    }
	    
	    /*
	     * Currently of a  value of 10 (Line Feed) returned by readDevice,is converted
	     * into a NULL in ASCII Hex
	     */
	    if (A == 10) {
		A = 0x00;
	    }
	    break;

	case WD:
	    uiHexOperand = fetch (DEVCODE_FETCH);
	    /*
	     * Currently of a  value of NULL returned by to be written,is converted
	     * into a 10 (Line Feed) in ASCII Hex
	     */
	    if (A == 0x00) {
		A = 10;
	    }
	    writeDevice (fpOutputDevice, A);
	    break;
	}
    }
} 

/*
 ***********************************************************************
 * Function Name: fetch                                                *
 *                                                                     * 
 * Purpose: Function performs the fetch function for the SIC CPU       *
 *          Gets different Items from the SIC memory                   *
 *                                                                     *  
 * Parameters: The type of Fetch that needs to be done for data        * 
 *                                                                     *
 * Returns: The Fetched Data                                           *
 *                                                                     *
 * Comments: There are 3 types of fetch defined in the SIC system      *
 *                                                                     *
 *           into the following types                                  *
 *                                                                     *
 *           Type 1 - Basically an OPCODE Fetch                        * 
 *           Type 2 - Basically an OPERAND Fetch                       *
 *           Type 3 - This involved 2 kinds of fetch DATA fetch or a   *
 *           Byte Fetch (Usually the Device Code)                      *
 ***********************************************************************
 */
int fetch (unsigned int uiFetchType)
{
    int value = -1;

    switch (uiFetchType)  {
    case OPCODE_FETCH:      //Type 1
	value = ucrgMemory[PC++];
	break;
    
    case OPERAND_FETCH:     //Type 2
	value = ucrgMemory[PC++];
	value = (value * 0x100) + ucrgMemory[PC++];
	if (value >= 0x8000) { //Detect the Addressing Mode Direct, Indexed ??
      	    value -= 0x8000;
	    value += X;
	}
	break;

    case DATA_FETCH:        //Type 3 Data Fetch
	value =  ucrgMemory[uiHexOperand++];
	value = (value * 0x100) + ucrgMemory[uiHexOperand++];
	value = (value * 0x100) + ucrgMemory[uiHexOperand];
	break;

    case DEVCODE_FETCH:     //Type 3 (Byte Fetch)
	value = ucrgMemory[uiHexOperand++];
	break;
    
    default:
	break;
    }
  
    return value;
}
	   
/*
 ***********************************************************************
 * Function Name: store                                                *
 *                                                                     * 
 * Purpose: Function performs the store function for the SIC CPU       *
 *          Stores items back into the SIC memory, from registers      *
 *                                                                     *  
 * Parameters: The data that needs to be stored                        * 
 *                                                                     *
 * Returns: None                                                       *
 *                                                                     *
 * Comments: Only one type store as of now, 3 Byte Items are stored    *
 *           back into memory                                          *
 ***********************************************************************
 */
void store (int iRegister)
{
    int temp;

    //Store 1st 8 bits
    temp = iRegister & 0xFF0000;
    temp = temp >> 16;
    ucrgMemory[uiHexOperand++] = temp;

    //Store 2nd 8 bits
    temp = iRegister & 0xFF00;
    temp = temp >> 8;
    ucrgMemory[uiHexOperand++] = temp;

    //Store Last 8 bits
    temp = iRegister & 0xFF;
    ucrgMemory[uiHexOperand] = temp;
}
