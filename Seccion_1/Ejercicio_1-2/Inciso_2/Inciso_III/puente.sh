#!/bin/bash
echo "Compilando archivos"
gcc puenteProcesosColaMsg.c -o puenteProcesosColaMsg
gcc generadorAutos.c -o generadorAutos
gcc auto.c -o auto
gcc policia.c -o policia
gcc timer.c -o timer
echo "Archivos compilados"
ipcrm -a #BORRAR ESTA LINEA
#echo "Ejecutando..."
#echo ""
#./puenteProcesosColaMsg

