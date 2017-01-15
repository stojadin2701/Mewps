#!/bin/bash

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
