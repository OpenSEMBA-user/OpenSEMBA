#!/bin/bash
# OutputFile: "$1.log"
# Errorfile: "$1.err"
# WarningFile: "$1.wrn"

PROJECT_NAME=$1
PROJECT_PATH=$2
PROBLEMTYPE_PATH=$3

SEMBA="$PROBLEMTYPE_PATH/bin/semba"
echo "Path to command: " > "$PROJECT_NAME.log"
echo $SEMBA >> "$PROJECT_NAME.log"

echo "Project name: " $PROJECT_NAME >> "$PROJECT_NAME.log"
echo "Project path: " $PROJECT_PATH >> "$PROJECT_NAME.log"
echo "Problem type path: " $PROBLEMTYPE_PATH >> "$PROJECT_NAME.log"

SEMBA_COMMAND="$SEMBA -i $PROJECT_PATH/$PROJECT_NAME.dat"
echo "$SEMBA_COMMAND"
echo "$SEMBA_COMMAND" >> "$PROJECT_NAME.log"
$SEMBA -i "$PROJECT_PATH/$PROJECT_NAME.dat"
