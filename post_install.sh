#!/bin/sh
sed -i.bak -e 's/.\/MayaMathNodes/\/Applications\/MayaMathNodes/g' /Users/Shared/Autodesk/modules/maya/MayaMathNodes.mod
rm /Users/Shared/Autodesk/modules/maya/MayaMathNodes.mod.bak
