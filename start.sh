#!/bin/bash

if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

./update.sh

if [[ $1 == "--debug" ]]
then
	cd Debug
else
	cd Release
fi

#./Mewps
