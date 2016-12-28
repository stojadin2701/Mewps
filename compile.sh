#!/bin/bash

cd Microcontroller
make clean && make && make up
cd ..

cd Debug
make clean && make
cd ..

cd Release
make clean && make
cd ..
