#!/bin/sh
export LD_LIBRARY_PATH="./lib":LD_LIBRARY_PATH
#export SDL_VIDEODRIVER=omapdss
#export SDL_OMAP_VSYNC=1
#export SDL_OMAP_FORCE_DOUBLEBUF=1
#export SDL_OMAP_FORCE_DIRECTBUF=0

NUB0=`cat /proc/pandora/nub0/mode`
NUB1=`cat /proc/pandora/nub1/mode`
/usr/pandora/scripts/op_nubchange.sh absolute absolute

./Project4

echo $NUB0 > /proc/pandora/nub0/mode
echo $NUB1 > /proc/pandora/nub1/mode
