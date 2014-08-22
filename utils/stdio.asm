TEST     START   1000              
FIRST    STA     SHELLAD
.FIRST    STA     RETADR
CLOOP    JSUB    RDREC             
         LDA     LENGTH            
         COMP    ONE               
         JEQ     ENDFIL            
         JSUB    WRREC             
         J       CLOOP             
ENDFIL   LDA     EOF               
         STA     BUFFER 
         LDA     THREE             
         STA     LENGTH 
         JSUB    WRREC             
         LDL     SHELLAD
.	 LDL	 RETADR
.         HLT  
         RSUB                      
EOF      BYTE    C'EOF' 
THREE    WORD    3
ZERO     WORD    0
ONE      WORD    1
FIVE     WORD    5
SHELLAD	 RESW	 1
.RETADR   RESW    1
LENGTH   RESW    1
BUFFER   RESB    4096              
. 
.       SUBROUTINE TO READ RECORD INTO BUFFER
. 
RDREC    LDX     ZERO              
         LDA     ZERO              
RLOOP    TD      INPUT             
         JEQ     RLOOP             
         RD      INPUT             
         COMP    FIVE              
         JLT     EXIT              
         STCH    BUFFER,X          
         TIX     MAXLEN            
         JLT     RLOOP             
EXIT     STCH    BUFFER,X          
         STX     LENGTH            
         LDA     LENGTH            
         ADD     ONE               
         STA     LENGTH 
         RSUB                      
INPUT    BYTE    X'F0'             
MAXLEN   WORD    4096 
. 
.       SUBROUTINE TO WRITE RECORD FROM BUFFER 
. 
WRREC    LDX     ZERO              
WLOOP    TD      OUTPUT            
         JEQ     WLOOP             
         LDCH    BUFFER,X          
         WD      OUTPUT            
         TIX     LENGTH            
         JLT     WLOOP             
         RSUB                      
OUTPUT   BYTE    X'A0'             
         END     FIRST
