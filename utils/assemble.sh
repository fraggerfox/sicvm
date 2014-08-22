#!/bin/bash

echo ---Converting all asm files from dos format to Unix--
dos2unix *.asm

echo ---Now SIC Assmebler will assemble the System Programs---
echo ---These are Mandatory Requred by the SICvm to Function Properly---
read

echo "*********************************************************"
echo ---The Bootstrap Loader---
./sicasm -b boot.asm
echo "********************************************************"
echo
echo "********************************************************"
echo ---The Absolute Loader---
./sicasm -l ldr.asm
echo "********************************************************"
echo
echo "********************************************************"
echo ---The SIC Shell---
./sicasm  shell.asm
echo
cat shell.obj > DEVF2
echo "********************************************************"
echo

echo "********************************************************"
echo ---Now SIC Assmebler will assemble the Sample Programs---
echo "********************************************************"
read

echo
echo "********************************************************"
echo ---A Sample Program Demostrating Standard I/O---
./sicasm  stdio.asm
echo
cat stdio.obj > DEVF3
echo "********************************************************"
echo
echo "********************************************************"
echo ---A Sample Program Demostrating a Simple Add Program---
./sicasm add.asm
echo
cat add.obj > DEVF4
echo "********************************************************"
echo All Assembly Completed
