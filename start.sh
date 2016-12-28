#!/bin/bash

./update.sh

if [[ $1 == "--debug" ]]
then
	cd Debug
else
	cd Release
fi

#./Mewps
