BIN_DIR_WIN = ..\bin
BIN_DIR_LIN = ../bin

#Override The Default Shell for Windows Compile
win32: SHELL=%SYSTEMROOT%\system32\cmd.exe
win32: 

	copy *.asm $(BIN_DIR_WIN)
#	copy *.obj $(BIN_DIR_WIN)
#	copy *.txt $(BIN_DIR_WIN)
	copy DEV?? $(BIN_DIR_WIN)
	copy *.bat $(BIN_DIR_WIN)
linux:
	cp *.asm $(BIN_DIR_LIN)
#	cp *.obj $(BIN_DIR_LIN)
#	cp *.txt $(BIN_DIR_LIN)
	cp DEV?? $(BIN_DIR_LIN)
	cp *.sh $(BIN_DIR_LIN)

clean_win32:
#	del $(BIN_DIR_WIN)\*.asm 2> NUL
#	del $(BIN_DIR_WIN)\*.txt 2> NUL
#	del $(BIN_DIR_WIN)\DEV?? 2> NUL
#	del $(BIN_DIR_WIN)\*.bat 2> NUL
#	del $(BIN_DIR_WIN)\*.obj 2> NUL

	del /q $(BIN_DIR_WIN)\* 2> NUL

clean_linux:
	rm -f $(BIN_DIR_LIN)/*
