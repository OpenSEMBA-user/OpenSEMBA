#!/bin/bash
problemTypeDir=$1
projectDir=$2
smbFilename=$2$3.smb

echo "Problem Type Directory: $problemTypeDir"
echo "Project Directory:      $projectDir"
echo "Semba filename:         $smbFilename"
# Sources aliases for Allclean and Allrun.
source /opt/openfoam230/etc/bashrc
export PATH=$problemTypeDir/bin/:$PATH
# Generates OpenFoam inputs.
cd $projectDir
smbExtract -n $smbFilename 
# Runs openfoam scripts. 
cd $projectDir/openfoam
./Allclean
./Allrun
cd ..
# Generates inputs from semba file and openFoam mesh.
smbToNFDE -n $smbFilename

