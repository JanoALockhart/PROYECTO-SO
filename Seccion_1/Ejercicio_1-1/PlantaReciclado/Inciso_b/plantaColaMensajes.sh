#!/bin/bash
echo "Compilando archivos"
gcc plantaColaMensajes.c -o plantaColaMensajes
gcc recolector.c -o recolector
gcc clasificador.c -o clasificador
gcc reciclador.c -o reciclador
echo "Archivos compilados"

