#!/bin/sh

SPATH=$HOME/WORKSPACE/works-emu-wow/servers/server_bin/hw2_system
CORE=`grep -c ^processor /proc/cpuinfo`+1

cd build
cmake ../ -DPREFIX=$SPATH -DPCH=1 -DDEBUG=1
make -j $CORE
make install

cp $SPATH/etc/mangosd.conf.dist $SPATH/etc/mangosd.conf
cp $SPATH/etc/realmd.conf.dist $SPATH/etc/realmd.conf
cp $SPATH/etc/scriptdev2.conf.dist $SPATH/etc/scriptdev2.conf

