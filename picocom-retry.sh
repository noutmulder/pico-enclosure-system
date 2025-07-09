#!/bin/bash
PORT="/dev/ttyACM0"
BAUD="115200"

while true; do
    echo "Wachten op $PORT..."
    while [ ! -e "$PORT" ]; do sleep 1; done
    echo "Verbonden met $PORT"
    picocom "$PORT" -b "$BAUD"
    echo "Connectie verbroken. Herstart over 2 seconden..."
    sleep 2
done

