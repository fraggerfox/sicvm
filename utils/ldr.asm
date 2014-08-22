LOADER   START   80                ABSOLUTE LOADER FOR SIC
. 
. THIS LOADER READS AN OBJECT PROGRAM FROM DEVICE F2 AND LOADS IT
. INTO MEMORY. THE HEADER RECORD FOR THE OBJECT PROGRAM IS NOT
. PROCESSED. WHEN THE END RECORD IS READ, THE LOADER JUMPS TO 
. THE INDICATED STARTING ADDRESS.
. 
. ERRORS IN THE OBJECT PROGRAM WILL RESULT IN AN ERROR HALT
. (ON AN ILLEGAL INSTRUCTION TRAP). THE ADDRESS OF THE TRAP 
. (THE BYTES LABELLED HALT1 THROUGH HALT4) WILL INDICATE THE 
. TYPE OF ERROR DETECTED.
. 
.     REGISTER X = NEXT ADDRESS TO BE LOADED 
. 
         STA	 SHELLAD
	 JSUB    READCHAR          
         COMP    HEADREC           
         JEQ     SKIPHEAD          
HALT1    BYTE    X'FF'             
SKIPHEAD JSUB    SKIPEOL           
PROCLOOP JSUB    READCHAR          
         COMP    TEXTREC           
         JEQ     PROCTEXT 
         COMP    ENDREC            
         JEQ     PROCEND
HALT2    BYTE    X'FF'             
PROCTEXT JSUB    GETADDR           
         LDA     ZERO 
         JSUB    GETD              
         MUL     K16               
         STA     TEMP              
         JSUB    GETD              
         ADD     TEMP              
         ADD     ADDRESS           
         STA     LIMIT             
         LDX     ADDRESS           
TEXTLOOP LDA     ZERO 
         JSUB    GETD              
         MUL     K16               
         STA     TEMP              
         JSUB    GETD             
         ADD     TEMP              
         STCH    0,X               
         TIX     LIMIT            
         JLT     TEXTLOOP         
         JSUB    SKIPEOL          
         J       PROCLOOP         
PROCEND  JSUB    GETADDR          
         LDA     SHELLAD
	 COMP    ZERO
	 JEQ     EXEC
	 LDA     ADDRESS
	 LDX     SHELLAD
.	 HLT	
	 J       0,X
EXEC	 LDX     ADDRESS
         HLT 
.         J       0,X              
. 
. THIS SUBROUTINE READS AN ADDRESS FROM AN OBJECT PROGRAM
. RECORD (6 HEX DIGITS) AND STORES IT IN INTEGER FORM IN THE 
. VARIABLE NAMED ADDRESS.
. 
GETADDR  STL     RETGA            
         LDX     ZERO             
         STX     ADDRESS
ADDRLOOP JSUB    GETD             
         STA     TEMP 
         LDA     ADDRESS          
         MUL     K16
         ADD     TEMP             
         STA     ADDRESS
         TIX     K6               
         JLT     ADDRLOOP         
         LDL     RETGA            
         RSUB 
. 
. THIS SUBROUTINE READS THE NEXT CHARACTER OF THE OBJECT PROGRAM 
. 
READCHAR LDA     ZERO 
READLOOP TD      INPUT            
         JEQ     READLOOP         
         RD      INPUT            
         RSUB                     
. 
. THIS SUBROUTINE READS ONE CHARACTER FROM THE INPUT AND COVERTS 
. IT FROM ASCII CODE TO HEX DIGIT VALUE. THE CONVERTED HEX
. DIGIT VALUE IS RETURNED IN REGISTER A
. 
GETD     STL     RETGD            
GETDLOOP JSUB    READCHAR         
         COMP    HEX04            
         JEQ     HALT3            
         COMP    HEX30            
         JLT     GETDLOOP         
         SUB     HEX30            
         COMP    K10              
         JLT     RETURN           
         SUB     K7               
         COMP    K10              
         JLT     HALT4            
         COMP    K15
         JGT     HALT4
RETURN   LDL     RETGD            
         RSUB 
HALT3    BYTE    X'FF'            
HALT4    BYTE    X'FF'            
. 
. THIS SUBROUTINE SKIPS THE REST OF THE CURRENT OBJECT 
. PROGRAM RECORD, INCLUDING THE END-OF-LINE MARKER 
. 
SKIPEOL  STL     RETS             
EOLLOOP  JSUB    READCHAR         
         COMP    ZERO             
         JEQ     EXIT             
         J       EOLLOOP          
EXIT     LDL     RETS             
         RSUB 
HEADREC  BYTE    X'000048'        
TEXTREC  BYTE    X'000054'        
ENDREC   BYTE    X'000045'        
ZERO     WORD    0
K2       WORD    2
K6       WORD    6
K7       WORD    7
K10      WORD    10 
K15      WORD    15 
K16      WORD    16 
HEX04    BYTE    X'000004'        
HEX30    BYTE    X'000030'        
INPUT    BYTE    X'F2'
SHELLAD	 RESW    1
ADDRESS  RESW    1
LIMIT    RESW    1
TEMP     RESW    1
RETGA    RESW    1
RETGD    RESW    1
RETS     RESW    1
         END     LOADER 
