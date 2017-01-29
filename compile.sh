#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "This script must be run as root."
  exit
fi

cd Microcontroller
rm -f bin/code.*
make all && make up
cd ..

cd Debug
rm -f Mewps
make all
cd ..

cd Release
rm -f Mewps
make all
cd ..
