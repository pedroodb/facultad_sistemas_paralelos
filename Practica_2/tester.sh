#!/bin/bash

TIME_SEQ="$(./$1 1 $2)"
TIME_PAR_2="$(./$1 2 $2)"
TIME_PAR_4="$(./$1 4 $2)"
SPEED_UP=$(echo "scale=6; $TIME_SEQ / $TIME_PAR_4" | bc)
EFFICIENCY=$(echo "scale=6; $SPEED_UP / 4" | bc)


echo "Tiempo secuencial:" $TIME_SEQ
echo "Tiempo paralelo(2 procesadores):" $TIME_PAR_2
echo "Tiempo paralelo(4 procesadores):" $TIME_PAR_4
echo "-----------------------------------------------------"
echo "Speed-Up:" $SPEED_UP
echo "Eficiencia:" $EFFICIENCY
echo "Speed-Up y eficiencia se calculan entre 1 hilo y 4"