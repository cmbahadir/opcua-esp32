#!/bin/bash

#This script processes the XML nodeset to create 
#source and header files those are going to be used 
#during server instantiation with defined nodeset.

#It uses the "Open62541" projects nodeset compiler. 

function setVariables() {
    open62541Dir=$1
    #Nodeset Compiler Location
    nodesetCompiler="$open62541Dir/tools/nodeset_compiler/nodeset_compiler.py"
    #Server nodeset XML file location.
    nodesetXML="myNS.xml"
    #Nodeset schema location
    nodetSchema="$open62541Dir/deps/ua-nodeset/Schema/Opc.Ua.NodeSet2.xml"
}

function usage() {
    printf "Usage: CreateNodeSet.sh path filename\n"
    printf "path: open62541 Path\n"
    printf "filename: Output File Name\n"
}

if [ $# -ne 2 ]
then
    usage
else
    outputFileName=$2
    setVariables $1
    python $nodesetCompiler --types-array=UA_TYPES --existing $nodetSchema --xml $nodesetXML $outputFileName
fi

