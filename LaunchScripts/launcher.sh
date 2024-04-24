#!/bin/sh

#if test -z "$XDG_RUNTIME_DIR"; then
# export XDG_RUNTIME_DIR=/run/user/`id -u`
#   if ! test -d "$XDG_RUNTIME_DIR"; then
#     mkdir --parents $XDG_RUNTIME_DIR
#      chmod 0700 $XDG_RUNTIME_DIR
#  fi
#i

# wait for weston
#while [ ! -e  $XDG_RUNTIME_DIR/wayland-0 ] ; do sleep 0.1; done


#export XDG_RUNTIME_DIR=/run/user/0
export DISPLAY=:0.0

/home/root/player/manager3 >/dev/null

