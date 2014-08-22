BOOT     START   0                
. 
. THIS BOOTSTRAP READS CODE FROM DEVICE F1 AND ENTERS IT INTO
. MEMORY STARTING AT ADDRESS 80 (HEX). AFTER ALL OF THE CODE
. HAS BEEN ENTERED INTO MEMORY, THE BOOTSTRAP EXECUTES A 
. JUMP TO ADDRESS 80 TO BEGIN EXECUTION OF THE PROGRAM.
. 
.               REGISTER X = NEXT ADDRESS TO BE LOADED 
. 
         LDA     ZERO              
         LDX     HEX80             
LOOP     JSUB    GETC              
         MUL     K16               
         STA     TEMP              
         JSUB    GETC              
         ADD     TEMP              
         STCH    0,X               
         TIX     ZERO              
         J       LOOP              
. 
. SUBROUTINE TO READ ONE CHARACTER AND CONVERT IT
. FROM ASCII CODE TO HEX DIGIT VALUE. THE CONVERTED HEX DIGIT 
. VALUE IS RETURNED IN REGISTER A. WHEN AN END-OF-FILE IS READ, 
. CONTROL IS TRANSFERRED TO THE STARTING ADDRESS (HEX 80). 
. 
GETC     TD      INPUT             
         JEQ     GETC              
         RD      INPUT             
         COMP    HEX04             
         JEQ     STOP                
         COMP    HEX30             
         JLT     GETC              
         SUB     HEX30             
         COMP    K10               
         JLT     RETURN            
         SUB     K7               
RETURN   RSUB
STOP     HLT                     
HEX04    BYTE    X'000004'
HEX30    BYTE    X'000030'
HEX80    BYTE    X'000080'
ZERO     WORD    0
K7       WORD    7
K10      WORD    10 
K16      WORD    16 
INPUT    BYTE    X'F1'
TEMP     RESW    1
         END     BOOT 