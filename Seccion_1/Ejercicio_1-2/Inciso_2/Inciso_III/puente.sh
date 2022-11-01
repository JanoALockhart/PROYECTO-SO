#!/bin/bash
echo "Compilando archivos"
gcc puenteProcesosColaMsg.c -o puenteProcesosColaMsg
gcc generadorAutos.c -o generadorAutos
gcc auto.c -o auto
gcc policia.c -o policia
gcc timer.c -o timer
echo "Archivos compilados"
echo "Removiendo colas de mensaje basura"
ipcrm -a
echo "Colas de mensaje eliminadas"
echo "Ejeutando..."
echo ""
./puenteProcesosColaMsg

