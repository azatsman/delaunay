#!/bin/sh


## Build the name of the log file by concatenating the rootname of the executable
## with all its arguments:

exname=$1
rootname=${exname%%.*}
shift
for a in $*
do
  rootname=${rootname}-$a
done
logname=${rootname}.log

time stdbuf -o 0 -e 0 $exname $* > $logname 2>&1

echo RC = $? " <====== " $exname $*  >> $logname
