DIV0 	START  	1000
	STA	SHELLAD
    	LDA    	FIRST
    	DIV	SECOND
    	STA    	THREE
	LDL	SHELLAD
	RSUB
.	HLT


FIRST 	WORD 	15
SECOND  WORD    0
SHELLAD	RESW	1
THREE   RESW    1
	END	DIV0