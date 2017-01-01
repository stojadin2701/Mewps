#!/bin/bash

cd Microcontroller
make clean && make all && make up
cd ..

cd Debug
make clean && make all
cd ..

cd Release
make clean && make all
cd ..
