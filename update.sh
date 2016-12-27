#!/bin/bash

if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

echo "Checking for update..."

result=$(git pull)

if [[ $result != *"up-to-date"* ]]
then
	echo "Update downloaded, compiling..."

	cd Microcontroller
	make clean && make && make up
	cd ..

	cd Debug
	make clean && make
	cd ..

	cd Release
	make clean && make
	cd ..
else
	echo "Up-to-date."
fi

