#!/usr/bin/env bash

source header.sh

function callCsvHasherOverAllColumns() {
    failCond "! [[ $# -eq 4 ]]" "Wrong number of arguments in $0 | $FUNCNAME."
    local hasherBinaryPathArg="$1"
    local csvFilePathArg="$2"

    declare -a columnNamesArray=$(./csvHeaderPrinter $csvFilePathArg)
    
    local saltToken
    read -s -p "Give password: " saltToken; printf "\n"

    local wordlistSizeArg="$3"
    local wordlistDumpPathArg="$4"

    for i in ${columnNamesArray[@]}; do
        $hasherBinaryPathArg "$csvFilePathArg" "$i" "$saltToken" \
                             --wordlist_size "$wordlistSizeArg" \
                             --wordlist_path "$wordlistDumpPathArg" 
    done
}
if ! (return 2>/dev/null); then
    function __init__ {
        readonly hasherBinaryPathSetting="./hasher"
        readonly csvFilePathSetting="./wordpass.csv"
        readonly wordlistSizeSetting="10000"
        declareExternalDrivePaths
        readonly extDriveWordlistsDir="$extDrive/.wordlists/"
    }
    function __main__ {
        local hasherCallCommandString="callCsvHasherOverAllColumns $hasherBinaryPathSetting \
$csvFilePathSetting $wordlistSizeSetting $extDriveWordlistsDir"
        mountDriveIfNeededEvalAction "$extDrivePart" "$extDrive" "mkdirSilently $extDriveWordlistsDir; $hasherCallCommandString"
    }
    __init__
    __main__ "$@"
fi

