#!/usr/bin/env sh

source header.sh

function callCsvDehasherOverAllColumnsForPrinting() {
    failCond "! [[ $# -eq 3 ]]" "Wrong number of arguments in $0 | $FUNCNAME."
    local dehasherBinaryPathArg="$1"
    local csvFilePathArg="$2"
    local wordlistDirPathArg="$3"

    declare -a columnNamesArray=$(./csvHeaderPrinter $csvFilePathArg)
    
    local saltToken
    read -s -p "Give password: " saltToken; printf "\n"

    local columnOutputStorage=()
    for i in ${columnNamesArray[@]}; do
        local wordlistFile="$3$i.txt"
        columnOutputStorage+=( "$($dehasherBinaryPathArg "$csvFilePathArg" "$wordlistFile" "$saltToken" --view_mode)" )
    done
    local numCols=${#columnOutputStorage[@]}
    local printCsvCommand="paste -d ','"
    for k in $(seq 0 $(($numCols-1))); do
        printCsvCommand+=' <(printf "%s\n" "${columnOutputStorage['
        printCsvCommand+="$k"
        printCsvCommand+=']}")'
    done
    eval "$printCsvCommand"
}
function callCsvDehasherOverAllColumnsForWriting() {
    failCond "! [[ $# -eq 3 ]]" "Wrong number of arguments in $0 | $FUNCNAME."
    local dehasherBinaryPathArg="$1"
    local csvFilePathArg="$2"
    local wordlistDirPathArg="$3"

    declare -a columnNamesArray=$(./csvHeaderPrinter $csvFilePathArg)
    
    local saltToken
    read -s -p "Give password: " saltToken; printf "\n"

    local columnOutputStorage=()
    for i in ${columnNamesArray[@]}; do
        local wordlistFile="$3$i.txt"
        columnOutputStorage+=( "$($dehasherBinaryPathArg "$csvFilePathArg" "$wordlistFile" "$saltToken" --view_mode)" )
    done
    local numCols=${#columnOutputStorage[@]}
    local printCsvCommand="paste -d ','"
    for k in $(seq 0 $(($numCols-1))); do
        printCsvCommand+=' <(printf "%s\n" "${columnOutputStorage['
        printCsvCommand+="$k"
        printCsvCommand+=']}")'
    done
    eval "$printCsvCommand"
    
    askExitOnNoDialogue "Was dehashing output correct?" "Find a correct password then bruv."

    for i in ${columnNamesArray[@]}; do
        local wordlistFile="$3$i.txt"
        $dehasherBinaryPathArg "$csvFilePathArg" "$wordlistFile" "$saltToken"
        rm -f "$wordlistFile"
    done
}
if ! (return 2>/dev/null); then
    function __init__ {
        readonly dehasherBinaryPathSetting="./dehasher"
        readonly csvFilePathSetting="./wordpass.csv"
        declareExternalDrivePaths
        readonly extDriveWordlistsDir="$extDrive/.wordlists/"
    }
    function __main__ {
        local writeModeArg="$1"
        case "$writeModeArg" in
            "--read")
                local dehasherCallCommandString="callCsvDehasherOverAllColumnsForPrinting \
$dehasherBinaryPathSetting $csvFilePathSetting $extDriveWordlistsDir"
                mountDriveIfNeededEvalAction "$extDrivePart" "$extDrive" "$dehasherCallCommandString"
                ;;
            "--write")                
                local dehasherCallCommandString="callCsvDehasherOverAllColumnsForWriting \
$dehasherBinaryPathSetting $csvFilePathSetting $extDriveWordlistsDir"
                mountDriveIfNeededEvalAction "$extDrivePart" "$extDrive" "$dehasherCallCommandString"
                ;;
            *)
                echo "Wrong flag given."
                ;;
        esac
    }
    __init__
    __main__ "$@"
fi

