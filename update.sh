#!/bin/bash

echo "Checking for update..."

rm -f $0.running

owner=$(ls -l $0 | awk '{print $3}')
group=$(ls -l $0 | awk '{print $4}')

mv $0 $0.running
cp $0.running $0
chown $owner:$group $0

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

