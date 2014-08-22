SHELL    START   300              
	 STA	 ADDRESS
CLOOP	 JSUB    PROMPT  
	 JSUB    RDCMD   
	 JSUB    EXECMD
	 J	 CLOOP	           
. 
.       SUBROUTINE TO READ RECORD INTO BUFFER
. 
RDCMD	 LDX     ZERO	
	 LDA     ZERO   
RLOOP    TD      INPUT  
         JEQ     RLOOP             
         RD      INPUT   
         COMP    EOL     
         JEQ     FINISH     
	 STCH    BUFFER,X  
         TIX     MAXLEN            
         J       RLOOP      
FINISH   STCH    BUFFER,X          
         STX     LENGTH            
         LDA     LENGTH            
         ADD     ONE               
         STA     LENGTH 
	 RSUB                      
.
.	Subroutine for executing commands
.
EXECMD   LDX	 ZERO
ELOOP	 LDCH    BUFFER,X
	 COMP    q
	 JEQ	 EXIT
	 COMP    l
	 JEQ     LOAD
	 COMP    e
	 JEQ	 EXE
	 COMP    EOL
	 JEQ	 RETPROM
	 STL	 RETURN
	 JSUB	 CMDERR
	 LDL	 RETURN
RETPROM	 RSUB
LOAD	 STL	 RETURN
	 TIX	 LENGTH
	 LDCH    BUFFER,X
	 COMP    SPACE
	 JEQ	 GETDEV
	 JSUB	 CMDERR
	 LDL	 RETURN
	 RSUB
GETDEV	 TIX	 LENGTH
	 LDA	 ZERO
	 LDCH    BUFFER,X
	 JSUB    CONVERT
	 MUL	 K16
	 STA	 TEMP
	 TIX	 LENGTH
	 LDCH    BUFFER,X
	 JSUB    CONVERT
	 LDL	 RETURN
	 ADD	 TEMP
	 LDX	 DEVNAME
	 STCH    0,X
	 LDA	 SHELLAD
.        HLT
	 LDX	 EIGHTY
	 J	 0,X
EXE	 STL	 RETURN
	 TIX	 LENGTH
	 LDCH	 BUFFER,X
	 COMP    SPACE
	 JEQ	 GETADD
	 COMP	 EOL
	 JEQ	 JUMPADD
	 JSUB	 CMDERR
	 LDL	 RETURN
	 RSUB
GETADD	 TIX	 LENGTH
	 LDA	 ZERO
	 STA	 TEMP
ADDR	 LDA	 ZERO
	 LDCH    BUFFER,X
	 COMP	 EOL
	 JEQ	 GOTADD
	 COMP	 SPACE
	 JEQ	 ADDR
	 JSUB    CONVERT
	 STA	 ADDRESS
	 LDA	 TEMP
	 MUL	 K16
	 ADD	 ADDRESS
	 STA	 TEMP
	 TIX	 LENGTH
	 J	 ADDR
GOTADD	 LDA	 TEMP
	 STA	 ADDRESS
JUMPADD	 LDA	 USERAD
	 LDX	 ADDRESS
	 J	 0,X
.
.	SUBROUTINE TO CONVERT FROM HEX TO ASCII
.
CONVERT  SUB	 HEX30
	 COMP    K10
	 JLT	 DONE
	 SUB	 K7
DONE 	 RSUB 
.
. 
.       SUBROUTINE TO DISPLAY PROMPT 
. 
PROMPT   TD      OUTPUT            
         JEQ     PROMPT            
         LDA     S         
         WD      OUTPUT
PNEXT1	 TD      OUTPUT            
         JEQ     PNEXT1            
         LDA     I         
         WD      OUTPUT
PNEXT2	 TD      OUTPUT            
         JEQ     PNEXT2            
         LDA     C         
         WD      OUTPUT
PNEXT3	 TD      OUTPUT            
         JEQ     PNEXT3            
         LDA     v         
         WD      OUTPUT
PNEXT4	 TD      OUTPUT            
         JEQ     PNEXT4            
         LDA     m         
         WD      OUTPUT
PNEXT5	 TD      OUTPUT            
         JEQ     PNEXT5            
         LDA     DOLLAR         
         WD      OUTPUT                        
         RSUB         
.
.	SUBROUTINE TO DISPLAY ERRORS
.
CMDERR	 TD      OUTPUT            
         JEQ     CMDERR            
         LDA     B         
         WD      OUTPUT
ENEXT1	 TD      OUTPUT            
         JEQ     ENEXT1            
         LDA     a         
         WD      OUTPUT
ENEXT2	 TD      OUTPUT            
         JEQ     ENEXT2            
         LDA     d         
         WD      OUTPUT
ENEXT3	 TD      OUTPUT            
         JEQ     ENEXT3            
         LDA     SPACE         
         WD      OUTPUT
ENEXT4	 TD      OUTPUT            
         JEQ     ENEXT4            
         LDA     C         
         WD      OUTPUT
ENEXT5	 TD      OUTPUT            
         JEQ     ENEXT5            
         LDA     o         
         WD      OUTPUT
ENEXT6	 TD      OUTPUT            
         JEQ     ENEXT6            
         LDA     m         
         WD      OUTPUT                        
ENEXT7	 TD      OUTPUT            
         JEQ     ENEXT7            
         LDA     m         
         WD      OUTPUT                        
ENEXT8	 TD      OUTPUT            
         JEQ     ENEXT8            
         LDA     a         
         WD      OUTPUT                        
ENEXT9	 TD      OUTPUT            
         JEQ     ENEXT9            
         LDA     n         
         WD      OUTPUT                        
ENEXT10	 TD      OUTPUT            
         JEQ     ENEXT10            
         LDA     d         
         WD      OUTPUT                        
ENEXT11	 TD      OUTPUT            
         JEQ     ENEXT11            
         LDA     EOL         
         WD      OUTPUT                        
         RSUB
.
.	SUBROUTINE TO EXIT
.
EXIT	 TD      OUTPUT            
         JEQ     CMDERR            
         LDA     S         
         WD      OUTPUT
SNEXT1	 TD      OUTPUT            
         JEQ     SNEXT1            
         LDA     h         
         WD      OUTPUT
SNEXT2	 TD      OUTPUT            
         JEQ     SNEXT2            
         LDA     e         
         WD      OUTPUT
SNEXT3	 TD      OUTPUT            
         JEQ     SNEXT3            
         LDA     l
         WD      OUTPUT
SNEXT4	 TD      OUTPUT            
         JEQ     SNEXT4            
         LDA     l         
         WD      OUTPUT
SNEXT5	 TD      OUTPUT            
         JEQ     SNEXT5            
         LDA     SPACE         
         WD      OUTPUT
SNEXT6	 TD      OUTPUT            
         JEQ     SNEXT6            
         LDA     T         
         WD      OUTPUT                        
SNEXT7	 TD      OUTPUT            
         JEQ     SNEXT7            
         LDA     e         
         WD      OUTPUT                        
SNEXT8	 TD      OUTPUT            
         JEQ     SNEXT8            
         LDA     r         
         WD      OUTPUT                        
SNEXT9	 TD      OUTPUT            
         JEQ     SNEXT9            
         LDA     m         
         WD      OUTPUT                        
SNEXT10	 TD      OUTPUT            
         JEQ     SNEXT10            
         LDA     i         
         WD      OUTPUT
SNEXT11	 TD      OUTPUT            
         JEQ     SNEXT11            
         LDA     n         
         WD      OUTPUT                        
SNEXT12	 TD      OUTPUT            
         JEQ     SNEXT12            
         LDA     a         
         WD      OUTPUT                        
SNEXT13	 TD      OUTPUT            
         JEQ     SNEXT13            
         LDA     t         
         WD      OUTPUT                        
SNEXT14	 TD      OUTPUT            
         JEQ     SNEXT14            
         LDA     e         
         WD      OUTPUT                        
SNEXT15	 TD      OUTPUT            
         JEQ     SNEXT15            
         LDA     d         
         WD      OUTPUT                        
SNEXT16	 TD      OUTPUT            
         JEQ     SNEXT16            
         LDA     EOL         
         WD      OUTPUT                        
         HLT
.       
.	RESERVATIONS & DECLARATIONS
.
K7	 WORD    7
K10	 WORD    10
K15	 WORD    15
K16	 WORD    16
SHELLAD  WORD    768
USERAD   WORD    771
ZERO     WORD    0
ONE      WORD    1
MAXLEN   WORD    100
DEVNAME  WORD    411
EIGHTY   WORD    128
l	 BYTE    X'00006C'
e	 BYTE    X'000065'
HEX04	 BYTE    X'000004'
HEX30	 BYTE    X'000030'
q	 BYTE    X'000071'
INPUT    BYTE    X'F0'
OUTPUT   BYTE    X'A0'             
S	 BYTE	 X'000053'
I	 BYTE	 X'000049'
C	 BYTE	 X'000043'
v	 BYTE	 X'000076'
m	 BYTE	 X'00006D'
DOLLAR   BYTE    X'000024'
B	 BYTE	 X'000042'
a	 BYTE	 X'000061'
d	 BYTE	 X'000064'
SPACE	 BYTE	 X'000020'
o	 BYTE	 X'00006F'
n	 BYTE	 X'00006E'
h	 BYTE	 X'000068'
T	 BYTE	 X'000054'
r	 BYTE	 X'000072'
i	 BYTE	 X'000069'
t	 BYTE	 X'000074'
EOL	 BYTE	 X'000000'
ADDRESS  RESW    1
LENGTH   RESW    1
RETURN   RESW    1
TEMP	 RESW    1
BUFFER   RESB    100
         END     SHELL