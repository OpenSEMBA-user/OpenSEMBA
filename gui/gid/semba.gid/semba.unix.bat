#!/bin/bash
# OutputFile: "$1.log"
# Errorfile: "$1.err"
# WarningFile: "$1.wrn"

SEMBA=$3/bin/semba
echo "Path to command: " > $1.log
echo $SEMBA >> $1.log

echo "Project name: " $1 >> $1.log
echo "Project path: " $2 >> $1.log
echo "Problem type path: " $3 >> $1.log

SEMBA_COMMAND="$SEMBA -i $2/$1.dat"
echo $SEMBA_COMMAND >> $1.log
$SEMBA_COMMAND >> $1.log
