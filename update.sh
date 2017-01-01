#!/bin/bash

echo "Checking for update..."

rm -f $0.running

mv $0 $0.running
cp $0.running $0

result=$(git pull)

if [[ $result != *"up-to-date"* ]]
then
	echo "Update downloaded, compiling..."

	./compile.sh

	echo "Compilation done."
else
	echo "Up-to-date."
fi

rm $0.running

