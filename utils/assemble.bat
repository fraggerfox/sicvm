@ECHO OFF
echo ---Now SIC Assmebler will assemble the System Programs---
echo ---These are Mandatory Requred by the SICvm to Function Properly---
pause
echo *********************************************************
echo ---The Bootstrap Loader---
sicasm -b boot.asm
echo *********************************************************
echo.

echo *********************************************************
echo ---The Absolute Loader---
sicasm -l ldr.asm
echo *********************************************************
echo.

echo *********************************************************
echo ---The SIC Shell---
sicasm  shell.asm
echo.
type shell.obj > DEVF2
echo Loaded SIC Shell in DEVF2
echo *********************************************************
echo.

echo *********************************************************
echo ---Now SIC Assmebler will assemble the Sample Programs---
echo *********************************************************
echo.
pause

echo *********************************************************
echo ---A Sample Program Demostrating Standard I/O---
sicasm  stdio.asm
echo.
type stdio.obj > DEVF3
echo Loaded uCat for SIC in DEVF3
echo *********************************************************
echo.
echo *********************************************************
echo ---A Sample Program Demostrating a Simple Add Program---
sicasm  add.asm
echo.
type stdio.obj > DEVF3
echo Loaded Simple Addition program in DEVF4
echo *********************************************************
echo.
echo All Assembly Completed