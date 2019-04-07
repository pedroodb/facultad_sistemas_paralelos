#!/bin/bash

TIME_SEQ="$(./$1 $2 1)"
TIME_PAR="$(./$1 $2 4)"
SPEED_UP=$(echo "scale=6; $TIME_SEQ / $TIME_PAR" | bc)
EFFICIENCY=$(echo "scale=6; $SPEED_UP / 4" | bc)


echo "Tiempo secuencial:" $TIME_SEQ
echo "Tiempo paralelo:" $TIME_PAR
echo "Speed-Up:" $SPEED_UP
echo "Eficiencia:" $EFFICIENCY