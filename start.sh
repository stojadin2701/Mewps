#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "This script must be run as root."
  exit
fi

./update.sh

if [[ $1 == "--debug" ]]
then
	cd Debug
else
	cd Release
fi

./Mewps
