win32: SHELL=%SYSTEMROOT%\system32\cmd.exe
win32:
	if not exist "bin" mkdir bin
	if not exist "obj" mkdir obj
	make -C .\src -f ming.mak
	make -C .\utils win32
linux:
	if [ ! -e bin ]; then mkdir bin; fi
	if [ ! -e obj ]; then mkdir obj; fi
	make -C ./src -f linux.mak
	make -C ./utils linux
clean_win32:	
	make -k clean_win32 -C .\utils
	make -k clean -C .\src -f ming.mak
	rmdir bin obj
clean_linux:
	make -k clean_linux -C ./utils
	make -k clean -C ./src -f linux.mak
	rmdir bin obj

