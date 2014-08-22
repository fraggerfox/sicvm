all: sicasm sicvm mac
asm: sicasm
vm: sicvm
mac: macro

#Compiler Stuff
CC = gcc
INCLUDE = ..\include
ASM_INCLUDE_DIR = asm
VM_INCLUDE_DIR = vm
MACRO_INCLUDE_DIR = macro
OBJ_DIR = ..\obj
BIN_DIR = ..\bin
ASM_DIR = .\asm
VM_DIR = .\vm
MACRO_DIR = .\macro
CFLAGS = -g -W -Wall
SHELL=%SYSTEMROOT%\system32\cmd.exe

sicasm:	sicasm.o optab.o getline.o symtab.o helpasm.o errorasm.o $(INCLUDE)\$(ASM_INCLUDE_DIR)\sicasm.h $(INCLUDE)\$(ASM_INCLUDE_DIR)\optab.h $(INCLUDE)\$(ASM_INCLUDE_DIR)\getline.h $(INCLUDE)\$(ASM_INCLUDE_DIR)\symtab.h $(INCLUDE)\$(ASM_INCLUDE_DIR)\helpasm.h $(INCLUDE)\$(ASM_INCLUDE_DIR)\errorasm.h
	$(CC) -I$(INCLUDE)\$(ASM_INCLUDE_DIR) $(CFLAGS) -o$(BIN_DIR)\sicasm.exe $(OBJ_DIR)\sicasm.o $(OBJ_DIR)\optab.o $(OBJ_DIR)\getline.o $(OBJ_DIR)\symtab.o $(OBJ_DIR)\helpasm.o $(OBJ_DIR)\errorasm.o 

optab.o: $(ASM_DIR)\optab.c $(INCLUDE)\$(ASM_INCLUDE_DIR)\optab.h
	$(CC) -I$(INCLUDE)\$(ASM_INCLUDE_DIR) $(CFLAGS) -c $(ASM_DIR)\optab.c -o$(OBJ_DIR)\optab.o

sicasm.o: $(ASM_DIR)\sicasm.c $(INCLUDE)\$(ASM_INCLUDE_DIR)\sicasm.h
	$(CC) -I$(INCLUDE)\$(ASM_INCLUDE_DIR) $(CFLAGS) -c $(ASM_DIR)\sicasm.c -o$(OBJ_DIR)\sicasm.o

getline.o: $(ASM_DIR)\getline.c $(INCLUDE)\$(ASM_INCLUDE_DIR)\getline.h
	$(CC) -I$(INCLUDE)\$(ASM_INCLUDE_DIR) $(CFLAGS) -c $(ASM_DIR)\getline.c -o$(OBJ_DIR)\getline.o

symtab.o: $(ASM_DIR)\symtab.c $(INCLUDE)\$(ASM_INCLUDE_DIR)\symtab.h
	$(CC) -I$(INCLUDE)\$(ASM_INCLUDE_DIR) $(CFLAGS) -c $(ASM_DIR)\symtab.c -o$(OBJ_DIR)\symtab.o

helpasm.o: $(ASM_DIR)\helpasm.c $(INCLUDE)\$(ASM_INCLUDE_DIR)\helpasm.h
	$(CC) -I$(INCLUDE)\$(ASM_INCLUDE_DIR) $(CFLAGS) -c $(ASM_DIR)\helpasm.c -o$(OBJ_DIR)\helpasm.o

errorasm.o: $(ASM_DIR)\errorasm.c $(INCLUDE)\$(ASM_INCLUDE_DIR)\errorasm.h
	$(CC) -I$(INCLUDE)\$(ASM_INCLUDE_DIR) $(CFLAGS) -c $(ASM_DIR)\errorasm.c -o$(OBJ_DIR)\errorasm.o

sicvm: sicvm.o helpvm.o dump.o bootstrap.o sic.o device.o errorvm.o interrupt.o delay.o
	$(CC) -o$(BIN_DIR)\sicvm.exe $(OBJ_DIR)\sicvm.o $(OBJ_DIR)\helpvm.o $(OBJ_DIR)\dump.o $(OBJ_DIR)\bootstrap.o $(OBJ_DIR)\sic.o  $(OBJ_DIR)\device.o $(OBJ_DIR)\errorvm.o $(OBJ_DIR)\interrupt.o $(OBJ_DIR)\delay.o

sicvm.o: $(VM_DIR)\sicvm.c $(INCLUDE)\$(VM_INCLUDE_DIR)\sicvm.h $(INCLUDE)\$(VM_INCLUDE_DIR)\helpvm.h $(INCLUDE)\$(VM_INCLUDE_DIR)\dump.h $(INCLUDE)\$(VM_INCLUDE_DIR)\interrupt.h
	$(CC) -I$(INCLUDE)\$(VM_INCLUDE_DIR) $(CFLAGS) -c $(VM_DIR)\sicvm.c -o$(OBJ_DIR)\sicvm.o

helpvm.o: $(VM_DIR)\helpvm.c 
	$(CC) -I$(INCLUDE)\$(VM_INCLUDE_DIR) $(CFLAGS) -c $(VM_DIR)\helpvm.c -o$(OBJ_DIR)\helpvm.o

dump.o: $(VM_DIR)\dump.c $(INCLUDE)\$(VM_INCLUDE_DIR)\sicvm.h
	$(CC) -I$(INCLUDE)\$(VM_INCLUDE_DIR) $(CFLAGS) -c $(VM_DIR)\dump.c -o$(OBJ_DIR)\dump.o

bootstrap.o: $(VM_DIR)\bootstrap.c $(INCLUDE)\$(VM_INCLUDE_DIR)\sicvm.h
	$(CC) -I$(INCLUDE)\$(VM_INCLUDE_DIR) $(CFLAGS) -c $(VM_DIR)\bootstrap.c -o$(OBJ_DIR)\bootstrap.o

sic.o: $(VM_DIR)\sic.c $(INCLUDE)\$(VM_INCLUDE_DIR)\sic.h $(INCLUDE)\$(VM_INCLUDE_DIR)\sicvm.h $(INCLUDE)\$(VM_INCLUDE_DIR)\interrupt.h
	$(CC) -I$(INCLUDE)\$(VM_INCLUDE_DIR) $(CFLAGS) -c $(VM_DIR)\sic.c -o$(OBJ_DIR)\sic.o

device.o: $(VM_DIR)\device.c $(INCLUDE)\$(VM_INCLUDE_DIR)\device.h $(INCLUDE)\$(VM_INCLUDE_DIR)\sicvm.h
	$(CC) -I$(INCLUDE)\$(VM_INCLUDE_DIR) $(CFLAGS) -c $(VM_DIR)\device.c -o$(OBJ_DIR)\device.o

errorvm.o: $(VM_DIR)\errorvm.c $(INCLUDE)\$(VM_INCLUDE_DIR)\errorvm.h $(INCLUDE)\$(VM_INCLUDE_DIR)\interrupt.h
	$(CC) -I$(INCLUDE)\$(VM_INCLUDE_DIR) $(CFLAGS) -c $(VM_DIR)\errorvm.c -o$(OBJ_DIR)\errorvm.o

interrupt.o: $(VM_DIR)\interrupt.c $(INCLUDE)\$(VM_INCLUDE_DIR)\interrupt.h
	$(CC) -I$(INCLUDE)\$(VM_INCLUDE_DIR) $(CFLAGS) -c $(VM_DIR)\interrupt.c -o$(OBJ_DIR)\interrupt.o

delay.o: $(VM_DIR)\delay.c $(INCLUDE)\$(VM_INCLUDE_DIR)\delay.h
	$(CC) -I$(INCLUDE)\$(VM_INCLUDE_DIR) $(CFLAGS) -c $(VM_DIR)\delay.c -o$(OBJ_DIR)\delay.o

macro: macro.o
	$(CC) -o$(BIN_DIR)\macro.exe $(OBJ_DIR)\macro.o

macro.o: $(MACRO_DIR)\macro.c $(INCLUDE)\macro\macro.h
	$(CC) -I$(INCLUDE)\$(MACRO_INCLUDE_DIR) $(CFLAGS) -c $(MACRO_DIR)\macro.c -o$(OBJ_DIR)\macro.o

clean:
	del /q $(OBJ_DIR)\* 2> NUL
	del /q $(BIN_DIR)\* 2> NUL
#	del $(OBJ_DIR)\*.o 2> NUL
#	del $(BIN_DIR)\*.exe 2> NUL