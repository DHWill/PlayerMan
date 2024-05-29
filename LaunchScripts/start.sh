#!/bin/bash

#This is For Backlight
gpioset 2 9=0 &
#This is Debug Light
gpioset 2 9=0 &
/home/root/player/launcher.sh &>/dev/null



#until "$(($(/home/root/player/launcher.sh >/dev/null) + 10))"; do
#	    echo "Artwork closed with exit code $?.  Respawning.." >&2
#	    sleep 5
#done

