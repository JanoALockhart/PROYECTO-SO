#!/bin/bash
echo "Compilando archivos"
gcc miniShell.c -o miniShell
gcc createDir.c -o createDir
gcc createFile.c -o createFile
gcc deleteDir.c -o deleteDir
gcc help.c -o help
gcc listDir.c -o listDir
gcc viewFile.c -o viewFile
gcc setFilePer.c -o setFilePer
echo "Archivos Compilados"
echo "Ejecutando..."
echo ""
./miniShell

