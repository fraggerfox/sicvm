all: sicasm sicvm mac
asm: sicasm
vm: sicvm
mac: macro

#Need to refine the Makefile 

#Compiler Stuff
CC = gcc
INCLUDE = ../include
ASM_INCLUDE_DIR = asm
VM_INCLUDE_DIR = vm
MACRO_INCLUDE_DIR = macro
OBJ_DIR = ../obj
BIN_DIR = ../bin
ASM_DIR = ./asm
SIM_DIR = ./vm
MACRO_DIR = ./macro
CFLAGS = -g -W -Wall

sicasm:	sicasm.o optab.o getline.o symtab.o helpasm.o errorasm.o $(INCLUDE)/$(ASM_INCLUDE_DIR)/sicasm.h $(INCLUDE)/$(ASM_INCLUDE_DIR)/optab.h $(INCLUDE)/$(ASM_INCLUDE_DIR)/getline.h $(INCLUDE)/$(ASM_INCLUDE_DIR)/symtab.h $(INCLUDE)/$(ASM_INCLUDE_DIR)/helpasm.h $(INCLUDE)/$(ASM_INCLUDE_DIR)/errorasm.h
	$(CC) -I$(INCLUDE)/$(ASM_INCLUDE_DIR) $(CFLAGS) -o$(BIN_DIR)/sicasm $(OBJ_DIR)/sicasm.o $(OBJ_DIR)/optab.o $(OBJ_DIR)/getline.o $(OBJ_DIR)/symtab.o $(OBJ_DIR)/helpasm.o $(OBJ_DIR)/errorasm.o 

optab.o: $(ASM_DIR)/optab.c $(INCLUDE)/$(ASM_INCLUDE_DIR)/optab.h
	$(CC) -I$(INCLUDE)/$(ASM_INCLUDE_DIR) $(CFLAGS) -c $(ASM_DIR)/optab.c -o$(OBJ_DIR)/optab.o

sicasm.o: $(ASM_DIR)/sicasm.c $(INCLUDE)/$(ASM_INCLUDE_DIR)/sicasm.h
	$(CC) -I$(INCLUDE)/$(ASM_INCLUDE_DIR) $(CFLAGS) -c $(ASM_DIR)/sicasm.c -o$(OBJ_DIR)/sicasm.o

getline.o: $(ASM_DIR)/getline.c $(INCLUDE)/$(ASM_INCLUDE_DIR)/getline.h
	$(CC) -I$(INCLUDE)/$(ASM_INCLUDE_DIR) $(CFLAGS) -c $(ASM_DIR)/getline.c -o$(OBJ_DIR)/getline.o

symtab.o: $(ASM_DIR)/symtab.c $(INCLUDE)/$(ASM_INCLUDE_DIR)/symtab.h
	$(CC) -I$(INCLUDE)/$(ASM_INCLUDE_DIR) $(CFLAGS) -c $(ASM_DIR)/symtab.c -o$(OBJ_DIR)/symtab.o

helpasm.o: $(ASM_DIR)/helpasm.c $(INCLUDE)/$(ASM_INCLUDE_DIR)/helpasm.h
	$(CC) -I$(INCLUDE)/$(ASM_INCLUDE_DIR) $(CFLAGS) -c $(ASM_DIR)/helpasm.c -o$(OBJ_DIR)/helpasm.o

errorasm.o: $(ASM_DIR)/errorasm.c $(INCLUDE)/$(ASM_INCLUDE_DIR)/errorasm.h
	$(CC) -I$(INCLUDE)/$(ASM_INCLUDE_DIR) $(CFLAGS) -c $(ASM_DIR)/errorasm.c -o$(OBJ_DIR)/errorasm.o

sicvm: sicvm.o helpvm.o dump.o bootstrap.o sic.o device.o errorvm.o interrupt.o delay.o
	$(CC) -o$(BIN_DIR)/sicvm $(OBJ_DIR)/sicvm.o $(OBJ_DIR)/helpvm.o $(OBJ_DIR)/dump.o $(OBJ_DIR)/bootstrap.o $(OBJ_DIR)/sic.o  $(OBJ_DIR)/device.o $(OBJ_DIR)/errorvm.o $(OBJ_DIR)/interrupt.o $(OBJ_DIR)/delay.o 

sicvm.o: $(SIM_DIR)/sicvm.c $(INCLUDE)/$(VM_INCLUDE_DIR)/sicvm.h $(INCLUDE)/$(VM_INCLUDE_DIR)/helpvm.h $(INCLUDE)/$(VM_INCLUDE_DIR)/dump.h
	$(CC) -I$(INCLUDE)/$(VM_INCLUDE_DIR) $(CFLAGS) -c $(SIM_DIR)/sicvm.c -o$(OBJ_DIR)/sicvm.o

helpvm.o: $(SIM_DIR)/helpvm.c 
	$(CC) -I$(INCLUDE)/$(VM_INCLUDE_DIR) $(CFLAGS) -c $(SIM_DIR)/helpvm.c -o$(OBJ_DIR)/helpvm.o

dump.o: $(SIM_DIR)/dump.c $(INCLUDE)/$(VM_INCLUDE_DIR)/sicvm.h
	$(CC) -I$(INCLUDE)/$(VM_INCLUDE_DIR) $(CFLAGS) -c $(SIM_DIR)/dump.c -o$(OBJ_DIR)/dump.o

bootstrap.o: $(SIM_DIR)/bootstrap.c $(INCLUDE)/$(VM_INCLUDE_DIR)/sicvm.h
	$(CC) -I$(INCLUDE)/$(VM_INCLUDE_DIR) $(CFLAGS) -c $(SIM_DIR)/bootstrap.c -o$(OBJ_DIR)/bootstrap.o

sic.o: $(SIM_DIR)/sic.c $(INCLUDE)/$(VM_INCLUDE_DIR)/sic.h $(INCLUDE)/$(VM_INCLUDE_DIR)/sicvm.h
	$(CC) -I$(INCLUDE)/$(VM_INCLUDE_DIR) $(CFLAGS) -c $(SIM_DIR)/sic.c -o$(OBJ_DIR)/sic.o

device.o: $(SIM_DIR)/device.c $(INCLUDE)/$(VM_INCLUDE_DIR)/device.h $(INCLUDE)/$(VM_INCLUDE_DIR)/sicvm.h
	$(CC) -I$(INCLUDE)/$(VM_INCLUDE_DIR) $(CFLAGS) -c $(SIM_DIR)/device.c -o$(OBJ_DIR)/device.o

errorvm.o: $(SIM_DIR)/errorvm.c $(INCLUDE)/$(VM_INCLUDE_DIR)/errorvm.h
	$(CC) -I$(INCLUDE)/$(VM_INCLUDE_DIR) $(CFLAGS) -c $(SIM_DIR)/errorvm.c -o$(OBJ_DIR)/errorvm.o

interrupt.o: $(SIM_DIR)/interrupt.c $(INCLUDE)/$(VM_INCLUDE_DIR)/interrupt.h
	$(CC) -I$(INCLUDE)/$(VM_INCLUDE_DIR) $(CFLAGS) -c $(SIM_DIR)/interrupt.c -o$(OBJ_DIR)/interrupt.o

delay.o: $(SIM_DIR)/delay.c $(INCLUDE)/$(VM_INCLUDE_DIR)/delay.h
	$(CC) -I$(INCLUDE)/$(VM_INCLUDE_DIR) $(CFLAGS) -c $(SIM_DIR)/delay.c -o$(OBJ_DIR)/delay.o

macro: macro.o
	$(CC) -o$(BIN_DIR)/macro $(OBJ_DIR)/macro.o

macro.o: $(MACRO_DIR)/macro.c $(INCLUDE)/macro/macro.h
	$(CC) -I$(INCLUDE)/$(MACRO_INCLUDE_DIR) $(CFLAGS) -c $(MACRO_DIR)/macro.c -o$(OBJ_DIR)/macro.o

clean:
	rm -f $(OBJ_DIR)/*
	rm -f $(BIN_DIR)/*