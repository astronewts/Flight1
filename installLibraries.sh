#!/bin/sh -e

ARDUINO_LIB_PATH=~/Documents/Arduino/libraries
if [ -e lib ] && [ -e $ARDUINO_LIB_PATH ]
then
    cp -R lib/ $ARDUINO_LIB_PATH
fi
