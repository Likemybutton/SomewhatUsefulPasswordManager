#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>
#include <sys/stat.h>
#include <time.h>

#pragma once
#ifndef MY_SFH_H_
#define MY_SFH_H_

// TODO Just make my own C string that is paired with length ffs

// GENERAL UTILITY STUFF DEFINITIONS _____________________________________________________ <-- 90th col

#define TRUE 1
#define FALSE 0

#define SMOL_TEXT_BUFF_SIZE 1024           // 2^10
#define SMOL_TEXT_BUFF_SIZE_ITER_TYPE u16
#define MID_TEXT_BUFF_SIZE 8192            // 2^13
#define MID_TEXT_BUFF_SIZE_ITER_TYPE u16
#define BIG_TEXT_BUFF_SIZE 65535           // 2^16-1 
#define BIG_TEXT_BUFF_SIZE_ITER_TYPE u16

#define TIME_T_CONVERTED_TO_STRING_POSSIBLE_BUFFER_SIZE 20+1
#define TIME_T_CONVERTED_TO_STRING_POSSIBLE_BUFFER_SIZE_ITER_TYPE u8

typedef int8_t  i8;  // -127                       to 127
typedef int16_t i16; // -32,867                    to 32,867
typedef int32_t i32; // -2,147,483,647             to 2,147,483,647
typedef int64_t i64; // -9,223,372,036,854,775,807 to 9,223,372,036,854,775,807

typedef uint8_t  u8;  // 255                          ~31.875 Bytes
typedef uint16_t u16; // 65,535                       ~8.191875 Kilobytes
typedef uint32_t u32; // 4,294,967,295                ~536.8709 Megabytes
typedef uint64_t u64; // 18,446,744,073,709,551,615   ~2,305,843.01 Terabytes

typedef u8 bool8;
typedef i8 exitCode;

void failCond(const bool8 condition, const char* msg);
exitCode printfBigAssLine();
int qsortMode(const void *x_void, const void *y_void);
enum printStringPartErrorCodes{
NO_ERROR_PRINT_STRING_PART,
SUBSTRING_OUT_OF_SCOPE_PRINT_STRING_PART,
START_FURTHER_THAN_END_PRINT_STRING_PART
};
exitCode printStringPart(const char *string, const u64 stringSize,
                         const u64 start, const u64 end);

// SYSTEM STUFF DEFINITIONS ______________________________________________________________
// File/Directory manipulation etc.

#define LINUX_MAX_PATH_SIZE 4095  // 2^12-1
#define LINUX_MAX_PATH_SIZE_ITER_TYPE u16
#define WINDOWS_MAX_PATH_SIZE 260
#define WINDOWS_MAX_PATH_SIZE_ITER_TYPE u8
#define MAC_MAX_PATH_SIZE 255
#define MAC_MAX_PATH_SIZE_ITER_TYPE u8

bool8 isValidFilePath(const char* path, const u64 len);
bool8 isValidDirPath(const char* path, const u64 len);

enum pathState{
FILE_EXISTS, DIRECTORY_EXISTS,
FILE_DOES_NOT_EXIST, DIRECTORY_DOES_NOT_EXIST
};
enum pathState isFileOrDirExists(const char* path, const u64 len);

enum OSType{ WIN32, WIN64, APPLE, LINUX, UNIX, BSD };
enum OSType whichOS();

#if defined(SYSTEM_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION) // ___________________________
#define GENERAL_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION

#define INVALID_CHARS_FOR_LINUX_UNIX_FILE_PATHS {}
#define INVALID_CHARS_FOR_LINUX_UNIX_FILE_PATHS_ARRAY_SIZE 0
#define INVALID_CHARS_FOR_LINUX_UNIX_FILE_PATHS_ARRAY_SIZE_ITER_TYPE u8

#define INVALID_CHARS_FOR_MAC_FILE_PATHS {':'}
#define INVALID_CHARS_FOR_MAC_FILE_PATHS_ARRAY_SIZE 1
#define INVALID_CHARS_FOR_MAC_FILE_PATHS_ARRAY_SIZE_ITER_TYPE u8

#define INVALID_CHARS_FOR_WINDOWS_FILE_PATHS {'<', '>', '\"', '/', '|', '?', '*', \
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, \
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F}
#define INVALID_CHARS_FOR_WINDOWS_FILE_PATHS_ARRAY_SIZE 38
#define INVALID_CHARS_FOR_WINDOWS_FILE_PATHS_ARRAY_SIZE_ITER_TYPE u8
#define INVALID_CHARS_FOR_WINDOWS_FILE_NAMES {':'}
#define INVALID_CHARS_FOR_WINDOWS_FILE_NAMES_ARRAY_SIZE 1
#define INVALID_CHARS_FOR_WINDOWS_FILE_NAMES_ARRAY_SIZE_ITER_TYPE u8
#define INVALID_ENDINGS_FOR_WINDOWS_FILE_NAMES {' ', '.'}
#define INVALID_ENDINGS_FOR_WINDOWS_FILE_NAMES_ARRAY_SIZE 2
#define INVALID_ENDINGS_FOR_WINDOWS_FILE_NAMES_ARRAY_SIZE_ITER_TYPE u8
#define INVALID_FILE_NAMES_FOR_WINDOWS {"CON", "PRN", "AUX", "NUL", "CONIN$", "CONOUT$" \
        "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", \
        "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9" }
#define INVALID_FILE_NAMES_FOR_WINDOWS_ARRAY_SIZE 24
#define INVALID_FILE_NAMES_FOR_WINDOWS_ARRAY_SIZE_ITER_TYPE u8

bool8 isValidFilePathLinuxUnix(const char* path,
                               const LINUX_MAX_PATH_SIZE_ITER_TYPE len){
    char badChars[]=INVALID_CHARS_FOR_LINUX_UNIX_FILE_PATHS;
    for (INVALID_CHARS_FOR_LINUX_UNIX_FILE_PATHS_ARRAY_SIZE_ITER_TYPE i=0;
         i<INVALID_CHARS_FOR_LINUX_UNIX_FILE_PATHS_ARRAY_SIZE; i++){
        if(strchr(path, badChars[i])!=NULL){
            return 0;
        }
    }
    return 1;
}
bool8 isValidFilePathWindows(const char* path, const
                             WINDOWS_MAX_PATH_SIZE_ITER_TYPE len){
    // TODO FIX THE WINDOWS FUNCTION
    char badChars[]=INVALID_CHARS_FOR_WINDOWS_FILE_PATHS;
    for (INVALID_CHARS_FOR_WINDOWS_FILE_PATHS_ARRAY_SIZE_ITER_TYPE i=0;
         i<INVALID_CHARS_FOR_WINDOWS_FILE_PATHS_ARRAY_SIZE; i++){
        if(strchr(path, badChars[i])!=NULL){
            return 0;
        }
    }
    // TODO Rewrite and test for windows (basename() does not work
    // and I need to check the character for any subdirectory not just the basename)
    char badCharsForNames[]=INVALID_CHARS_FOR_WINDOWS_FILE_NAMES;
    for (INVALID_CHARS_FOR_WINDOWS_FILE_NAMES_ARRAY_SIZE_ITER_TYPE i=0;
         i<INVALID_CHARS_FOR_WINDOWS_FILE_NAMES_ARRAY_SIZE; i++){
        if (strchr(basename(strdup(path)), badCharsForNames[i])!=NULL){
            return 0;
        }
    }
    char badEndings[]=INVALID_ENDINGS_FOR_WINDOWS_FILE_NAMES;
    for (INVALID_ENDINGS_FOR_WINDOWS_FILE_NAMES_ARRAY_SIZE_ITER_TYPE i=0;
         i<INVALID_ENDINGS_FOR_WINDOWS_FILE_NAMES_ARRAY_SIZE; i++){
        // TODO THIS IS WRONG
        if(path[len-1] == badEndings[i]){
            return 0;
        }
    }
    // TODO Rewrite and test for windows (basename() does not work)
    char* badNames[]=INVALID_FILE_NAMES_FOR_WINDOWS;
    for (INVALID_FILE_NAMES_FOR_WINDOWS_ARRAY_SIZE_ITER_TYPE i=0;
         i<INVALID_FILE_NAMES_FOR_WINDOWS_ARRAY_SIZE; i++){
        if(strstr(basename(strdup(path)), badNames[i])!=NULL){
            return 0;
        }
    }
    return 1;
}
bool8 isValidFilePathMac(const char* path, const
                         MAC_MAX_PATH_SIZE_ITER_TYPE len){
    char badChars[]=INVALID_CHARS_FOR_MAC_FILE_PATHS;
    for (INVALID_CHARS_FOR_MAC_FILE_PATHS_ARRAY_SIZE_ITER_TYPE i=0;
         i<INVALID_CHARS_FOR_MAC_FILE_PATHS_ARRAY_SIZE; i++){
        if(strchr(path, badChars[i])!=NULL){
            return 0;
        }
    } 
    return 1;
}
bool8 isValidFilePath(const char* path, const u64 len){
    switch(whichOS()){
        /* case WIN32: */
        /*     if(len > WINDOWS_MAX_PATH_SIZE) { return 0; } */
        /*     return isValidFilePathWindows(path, len); */
        /* case WIN64: */
        /*     if(len > WINDOWS_MAX_PATH_SIZE) { return 0; } */
        /*     return isValidFilePathWindows(path, len); */
        case APPLE:
            if(len > MAC_MAX_PATH_SIZE) { return 0; }
            return isValidFilePathMac(path, len);
        case LINUX:
            if(len > LINUX_MAX_PATH_SIZE) { return 0; }
            return isValidFilePathLinuxUnix(path, len);
        case UNIX:
            if(len > LINUX_MAX_PATH_SIZE) { return 0; }
            return isValidFilePathLinuxUnix(path, len);
        default: return 0;
    }
}

bool8 isValidDirPathLinuxUnix(const char* path,
                              const LINUX_MAX_PATH_SIZE_ITER_TYPE len){
    char badChars[]=INVALID_CHARS_FOR_LINUX_UNIX_FILE_PATHS;
    for (INVALID_CHARS_FOR_LINUX_UNIX_FILE_PATHS_ARRAY_SIZE_ITER_TYPE i=0;
         i<INVALID_CHARS_FOR_LINUX_UNIX_FILE_PATHS_ARRAY_SIZE; i++){
        if(strchr(path, badChars[i])!=NULL){
            return 0;
        }
    }
    return 1;
}
bool8 isValidDirPathWindows(const char* path,
                            const WINDOWS_MAX_PATH_SIZE_ITER_TYPE len){
    // TODO FIX THE WINDOWS FUNCTION
    char badChars[]=INVALID_CHARS_FOR_WINDOWS_FILE_PATHS;
    for (INVALID_CHARS_FOR_WINDOWS_FILE_PATHS_ARRAY_SIZE_ITER_TYPE i=0;
         i<INVALID_CHARS_FOR_WINDOWS_FILE_PATHS_ARRAY_SIZE; i++){
        if(strchr(path, badChars[i])!=NULL){
            return 0;
        }
    }
    // TODO Rewrite and test for windows (basename() does not work
    // and I need to check the character for any subdirectory not just the basename)
    char badCharsForNames[]=INVALID_CHARS_FOR_WINDOWS_FILE_NAMES;
    for (INVALID_CHARS_FOR_WINDOWS_FILE_NAMES_ARRAY_SIZE_ITER_TYPE i=0;
         i<INVALID_CHARS_FOR_WINDOWS_FILE_NAMES_ARRAY_SIZE; i++){
        if(strchr(basename(strdup(path)), badCharsForNames[i])!=NULL){
            return 0;
        }
    }
    return 1;
}
bool8 isValidDirPathMac(const char* path, const
                        MAC_MAX_PATH_SIZE_ITER_TYPE len){
    char badChars[]=INVALID_CHARS_FOR_MAC_FILE_PATHS;
    for (INVALID_CHARS_FOR_MAC_FILE_PATHS_ARRAY_SIZE_ITER_TYPE i=0;
         i<INVALID_CHARS_FOR_MAC_FILE_PATHS_ARRAY_SIZE; i++){
        if(strchr(path, badChars[i])!=NULL){
            return 0;
        }
    } 
    return 1;
}
bool8 isValidDirPath(const char* path, const u64 len){
    switch(whichOS()){
        /* case WIN32: */
        /*     if(len > WINDOWS_MAX_PATH_SIZE) { return 0; } */
        /*     return isValidDirPathWindows(path, len); */
        /* case WIN64: */
        /*     if(len > WINDOWS_MAX_PATH_SIZE) { return 0; } */
        /*     return isValidDirPathWindows(path, len); */
        case APPLE:
            if(len > MAC_MAX_PATH_SIZE) { return 0; }
            return isValidDirPathMac(path, len);
        case LINUX:
            if(len > LINUX_MAX_PATH_SIZE) { return 0; }
            return isValidDirPathLinuxUnix(path, len);
        case UNIX:
            if(len > LINUX_MAX_PATH_SIZE) { return 0; }
            return isValidDirPathLinuxUnix(path, len);
        default: return 0;
    }
}

enum pathState isFileOrDirExists(const char* path, const u64 len){
    struct stat st;
    if(stat(path, &st) == 0){
        //if (st.st_mode & S_IFREG){ return FILE_EXISTS; }
        if(st.st_mode & S_IFDIR){ return DIRECTORY_EXISTS; }
        else{ return FILE_EXISTS; }
    }else{
        if(path[len-1] == '/'){ return DIRECTORY_DOES_NOT_EXIST; }
        else{ return FILE_DOES_NOT_EXIST; }
    }
    return 0;
}
enum OSType whichOS(){
#ifdef _WIN32
    return WIN32;
#elif _WIN64
    return WIN64;
#elif __APPLE__
    return APPLE;
#elif __linux__
    return LINUX;
#elif __unix__
x    return UNIX;
#elif BSD
    return BSD;
#endif    
}
#endif // SYSTEM_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION

// ASCII STUFF DEFINITIONS _______________________________________________________________
extern const char SPECIALS_LIST[];
char getRandomSymbol(const char* symCase);
bool8 isUpper(const char sym);
bool8 isLower(const char sym);
bool8 isDigit(const char sym);
bool8 isSpecial(const char sym);
bool8 areSimpleStringContents(char* str, const u64 len);
bool8 areNullsInTheString(char* str, const u64 len);
exitCode fillStringRandom(char* str, const u64 len);
exitCode toLowerString(char* string, const u64 size);
bool8 hasUpper(const char* string, const u64 len);
bool8 hasLower(const char* string, const u64 len);
bool8 hasDigit(const char* string, const u64 len);
bool8 hasSpecial(const char* string, const u64 len);
#if defined(ASCII_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION) // ____________________________
#define GENERAL_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION

#define NUMBER_ALPHABET 26
#define NUMBER_DIGITS 10
#define NUMBER_SPECIALS 30
#define ASCII_START_LOWER 97
#define ASCII_START_UPPER 65
#define ASCII_START_DIGITS 48
#define ASCII_END_LOWER 122
#define ASCII_END_UPPER 90
#define ASCII_END_DIGITS 57
#define ASCII_SPECIALS_BOUNDARIES {33, 46, 58, 64, 91, 95, 123, 126}
#define ASCII_SPECIALS_BOUNDARIES_ARRAY_SIZE 8
const char SPECIALS_LIST[] = {'~','`','!','@','#','$','%','^','&','*','(',
                              ')','-','_','+','=','{','}','[',']','|','\\',
                              ';',':','\"','<','>',',','.','?'};
char getRandomSymbol(const char* symCase){
    if(strcmp(symCase, "lower") == 0){
        return rand() % NUMBER_ALPHABET + ASCII_START_LOWER;
    }else if(strcmp(symCase, "upper") == 0){
        return rand() % NUMBER_ALPHABET + ASCII_START_UPPER;
    }else if(strcmp(symCase, "digit") == 0){
        return rand() % NUMBER_DIGITS + ASCII_START_DIGITS;
    }else if(strcmp(symCase, "special") == 0){
        return SPECIALS_LIST[rand() % NUMBER_SPECIALS];
    }
    failCond(1, "Wrong option given in getRandomSymbol()!");
}
bool8 isUpper(const char sym){
    if(sym >= ASCII_START_UPPER && sym <= ASCII_END_UPPER){
        return 1;
    }
    return 0;
}
bool8 isLower(const char sym){
    if(sym >= ASCII_START_LOWER && sym <= ASCII_END_LOWER){
        return 1;
    }
    return 0;
}
bool8 isDigit(const char sym){
    if(sym >= ASCII_START_DIGITS && sym <= ASCII_END_DIGITS){
        return 1;
    }
    return 0;
}
bool8 isSpecial(const char sym){
    u8 bounds[ASCII_SPECIALS_BOUNDARIES_ARRAY_SIZE] =   \
        ASCII_SPECIALS_BOUNDARIES;
    if((sym >= bounds[0] && sym <= bounds[1]) ||
       (sym >= bounds[2] && sym <= bounds[3]) ||
       (sym >= bounds[4] && sym <= bounds[5]) ||
       (sym >= bounds[6] && sym <= bounds[7])){
        return 1;
    }
    return 0;
}
bool8 areSimpleStringContents(char* str, const u64 len){
    for(u64 i=0; i<len; i++){
        if(!isUpper(str[i]) && !isLower(str[i])
           && !isDigit(str[i]) && !isSpecial(str[i])){
            return 0;
        }
    }
    return 1;
}
bool8 areNullsInTheString(char* str, const u64 len){
    for(u64 i=0; i<len; i++){
        if(str[i] == '\0'){
            return 1;
        }
    }
    return 0;
}
exitCode fillStringRandom(char* str, const u64 len){
    for(u64 i=0; i<len; i++){
        u8 opt = rand() % 3+1;
        if(opt == 1){
            str[i] = getRandomSymbol("lower");
        }
        else if(opt == 2){
            str[i] = getRandomSymbol("upper");
        }
        else if(opt == 3){
            str[i] = getRandomSymbol("digit");
        }
    }
    str[len] = '\0';
    return 0;
}
exitCode toLowerString(char* string, const u64 size){
    for(u64 i=0; i<size; i++){
        string[i] = (char)tolower((int)string[i]);
    }
    return 0;
}
bool8 hasUpper(const char* string, const u64 len){
    for(u64 i=0; i<len; i++){
        if(isUpper(string[i])){ return 1; }
    }
    return 0;
}
bool8 hasLower(const char* string, const u64 len){
    for(u64 i=0; i<len; i++){
        if(isLower(string[i])){ return 1; }
    }
    return 0;
}
bool8 hasDigit(const char* string, const u64 len){
    for(u64 i=0; i<len; i++){
        if(isDigit(string[i])){ return 1; }
    }
    return 0;
}
bool8 hasSpecial(const char* string, const u64 len){
    for(u64 i=0; i<len; i++){
        if(isSpecial(string[i])){ return 1; }
    }
    return 0;
}
#endif // ASCII_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION

// CSV UTILITY DEFINITIONS _______________________________________________________________

// Default settings for use in applications
/* #define MAX_CSV_WIDTH 10 */
/* #define MAX_CSV_WIDTH_ITER_TYPE u8 */
/* #define MAX_CSV_LENGTH 500 */
/* #define MAX_CSV_LENGTH_ITER_TYPE u16 */
/* #define MAX_CSV_CELLS MAX_CSV_LENGTH * MAX_CSV_WIDTH // 5000 */
/* #define MAX_CSV_CELLS_ITER_TYPE u16 */
/* #define MAX_CSV_CELL_CONTENT_SIZE SMOL_TEXT_BUFF_SIZE // 1024 */
/* #define MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE SMOL_TEXT_BUFF_SIZE_ITER_TYPE */

/* #define MAX_CSV_BUFFER_SIZE MAX_CSV_CELLS*MAX_CSV_CELL_CONTENT_SIZE * 1.1 // 5000 * 1024 * 1.1 = 5,632,000 */
/* #define MAX_CSV_BUFFER_SIZE_ITER_TYPE u32 */

/* #define CSV_FIRST_DEGREE_DELIMETER ',' */
/* #define CSV_SECOND_DEGREE_DELIMETER '\"' */
/* #define CSV_NEWLINE_DELIMETER '\n' */
/* #define CSV_END_OF_FILE '\0' */

/* #define CSV_HEADER_ROW_POSITION 0 */
/* #define CSV_FIRST_DATA_ROW_POSITION CSV_HEADER_ROW_POSITION + 1 */

// Custom setting for use. Comment the defaults and fill the fields for any personal preference
#include "settings.h"

typedef struct{
    MAX_CSV_LENGTH_ITER_TYPE lineIndex;
    MAX_CSV_BUFFER_SIZE_ITER_TYPE lineStringStartPosition;
    MAX_CSV_BUFFER_SIZE_ITER_TYPE lineStringEndPosition;
}CsvLine;
typedef struct{
    bool8 encasedInSecondDegreeDelimeter;
    bool8 notEmpty;
    MAX_CSV_WIDTH_ITER_TYPE cellColumnPosition;
    MAX_CSV_LENGTH_ITER_TYPE cellRowPosition;
    MAX_CSV_BUFFER_SIZE_ITER_TYPE cellStringStartPosition;
    MAX_CSV_BUFFER_SIZE_ITER_TYPE cellStringEndPosition;
}CsvCell;
// TODO Implement wider mapping functonality
//      TODO Store respective cells to CsvLines
//      TODO Store the number of stored cells in the line
//      TODO Define a new struct CsvColumn
//           TODO Store respective cells, columnIndex and number of stored cells in the column
typedef struct{
    MAX_CSV_LENGTH_ITER_TYPE rowsNumber; MAX_CSV_WIDTH_ITER_TYPE widestRowSize;
    char* csvBuffer; MAX_CSV_BUFFER_SIZE_ITER_TYPE csvBufferSize;  
    CsvLine* rows; CsvCell** cells; //cell[column][row]
}CsvMap;
enum getStringInCsvHeaderErrorCodes{
NO_ERROR_GET_STRING_IN_CSV_HEADER,
HEADER_NOT_FOUND_GET_STRING_IN_CSV_HEADER
};
exitCode getStringInCsvHeader(
    MAX_CSV_WIDTH_ITER_TYPE* dest, const CsvMap* csvMap,
    const char* string, const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE stringSize);
bool8 isStringInCsvHeader(const CsvMap* csvMap, const char* string,
                          const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE stringSize);
exitCode copyCellFromCsvMap(char* dest, const CsvMap* csvMap,
                            const MAX_CSV_LENGTH_ITER_TYPE lengthSheetPosition,
                            const MAX_CSV_WIDTH_ITER_TYPE widthSheetPosition);
enum CsvMapConstructorErrorCodes{
NO_ERROR_CSV_MAP_CONSTRUCTOR,
ROWS_EXCEED_LIMIT_CSV_MAP_CONSTRUCTOR,
COLUMNS_EXCEED_LIMIT_CSV_MAP_CONSTRUCTOR};
extern const char* CsvMapConstructorErrorMessages[];
exitCode CsvMapConstructor(CsvMap* csvMap, const char* csvBuffer,
                           const MAX_CSV_BUFFER_SIZE_ITER_TYPE csvBufferSize);
bool8 cellContentsShouldBeEncasedInSecondDegreeDelimitation(
    const char* cellContents, const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE cellContentsSize);
exitCode CsvMapDestructor(CsvMap* csvMap);
exitCode fprintfCsvMap(FILE* csvFilePointer, const CsvMap* csvMap);
exitCode printCsvMapDebug(const CsvMap* csvMap);
exitCode printCsvMap(const CsvMap* csvMap);
#if defined(CSV_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION) // ______________________________
#define GENERAL_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION

exitCode copyCellFromCsvMap(char* dest, const CsvMap* csvMap,
                            const MAX_CSV_LENGTH_ITER_TYPE lengthSheetPosition,
                            const MAX_CSV_WIDTH_ITER_TYPE widthSheetPosition){
    char temp[MAX_CSV_CELL_CONTENT_SIZE];
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE i=0;
    while(i<(csvMap->cells[lengthSheetPosition][widthSheetPosition].cellStringEndPosition-
             csvMap->cells[lengthSheetPosition][widthSheetPosition].cellStringStartPosition)){
        temp[i] = csvMap->csvBuffer[i+csvMap->cells[lengthSheetPosition][widthSheetPosition].cellStringStartPosition];
        i++;
    }
    strncpy(dest, temp, i);
    return 0;
}
exitCode getStringInCsvHeader(
    MAX_CSV_WIDTH_ITER_TYPE* dest, const CsvMap* csvMap,
    const char* string, const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE stringSize){
    char temp[MAX_CSV_CELL_CONTENT_SIZE];
    for(MAX_CSV_WIDTH_ITER_TYPE i=0; i<csvMap->widestRowSize; i++){
        copyCellFromCsvMap(temp, csvMap, CSV_HEADER_ROW_POSITION, i);
        if(strncmp(temp, string, stringSize) == 0){
            *dest = i;
            return NO_ERROR_GET_STRING_IN_CSV_HEADER;
        }
    }
    return HEADER_NOT_FOUND_GET_STRING_IN_CSV_HEADER;
}
bool8 isStringInCsvHeader(const CsvMap* csvMap, const char* string,
                          const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE stringSize){
    char temp[MAX_CSV_CELL_CONTENT_SIZE];
    for(MAX_CSV_WIDTH_ITER_TYPE i=0; i<csvMap->widestRowSize; i++){
        copyCellFromCsvMap(temp, csvMap, CSV_HEADER_ROW_POSITION, i);
        if(strncmp(temp, string, stringSize) == 0){
            return 1;
        }
    }
    return 0;
}
const char* CsvMapConstructorErrorMessages[] = {"NO_ERROR",
"Number of rows in .csv file exceeds the limit of MAX_CSV_LENGTH, please check the \"settings.h\".",
"Number of columns in .csv file exceeds the limit of MAX_CSV_LENGTH, please check the \"settings.h\"."};
exitCode finishMarkingPrevCell_CsvMapConstructor(
    CsvMap* csvMap, const MAX_CSV_BUFFER_SIZE_ITER_TYPE lineCharacterCountAtDelimeter,
    const MAX_CSV_LENGTH_ITER_TYPE lineCount, const MAX_CSV_WIDTH_ITER_TYPE widthCount){
    if(csvMap->cells[lineCount][widthCount].encasedInSecondDegreeDelimeter == TRUE){
        csvMap->cells[lineCount][widthCount].cellStringEndPosition = lineCharacterCountAtDelimeter-1;
    }
    else{
        csvMap->cells[lineCount][widthCount].cellStringEndPosition = lineCharacterCountAtDelimeter;
    }
    csvMap->cells[lineCount][widthCount].cellRowPosition = lineCount;
    csvMap->cells[lineCount][widthCount].cellColumnPosition = widthCount;
    return 0;
}
exitCode startMarkingNextCell_CsvMapConstructor(
    CsvMap* csvMap, const MAX_CSV_BUFFER_SIZE_ITER_TYPE lineCharacterCountAfterDelimeter,
    const MAX_CSV_LENGTH_ITER_TYPE lineCount, const MAX_CSV_WIDTH_ITER_TYPE widthCount,
    const bool8 encasedInSecondDegreeDelimeter){
    csvMap->cells[lineCount][widthCount].cellStringStartPosition = lineCharacterCountAfterDelimeter;
    csvMap->cells[lineCount][widthCount].encasedInSecondDegreeDelimeter = encasedInSecondDegreeDelimeter;
    csvMap->cells[lineCount][widthCount].notEmpty = TRUE;
    return 0;
}
exitCode fillUpLines_CsvMapConstructor(CsvMap* csvMap){
    csvMap->rows = (CsvLine*)malloc(MAX_CSV_LENGTH*(sizeof(CsvLine)));
    csvMap->rowsNumber = 0;
    csvMap->rows[csvMap->rowsNumber].lineStringStartPosition = 0;
    for(MAX_CSV_BUFFER_SIZE_ITER_TYPE i=csvMap->rows[csvMap->rowsNumber].lineStringStartPosition;
        i<csvMap->csvBufferSize; i++){
        if(csvMap->rowsNumber+1 > MAX_CSV_LENGTH){
            return ROWS_EXCEED_LIMIT_CSV_MAP_CONSTRUCTOR;
        }
        if(csvMap->csvBuffer[i] == CSV_FIRST_DEGREE_DELIMETER &&
           csvMap->csvBuffer[i+1] == CSV_SECOND_DEGREE_DELIMETER){
            while(csvMap->csvBuffer[i] == CSV_SECOND_DEGREE_DELIMETER &&
                  csvMap->csvBuffer[i+1] == CSV_FIRST_DEGREE_DELIMETER){ i++; }
        }
        else if(csvMap->csvBuffer[i+1] == CSV_END_OF_FILE){
            if(csvMap->csvBuffer[i] == CSV_NEWLINE_DELIMETER){
                csvMap->rows[csvMap->rowsNumber].lineStringEndPosition = i;
            }
            else{
                csvMap->rows[csvMap->rowsNumber].lineStringEndPosition = i + 1;
            }
            csvMap->rows[csvMap->rowsNumber].lineIndex = csvMap->rowsNumber;
            csvMap->rowsNumber++;
            return 0;
        }
        else if(csvMap->csvBuffer[i] == CSV_NEWLINE_DELIMETER){
            csvMap->rows[csvMap->rowsNumber].lineStringEndPosition = i;
            csvMap->rows[csvMap->rowsNumber].lineIndex = csvMap->rowsNumber;
            csvMap->rowsNumber++;
            csvMap->rows[csvMap->rowsNumber].lineStringStartPosition = i+1;
        }
    }
    return 0;
}
exitCode fillUpCells_CsvMapConstructor(CsvMap* csvMap){
    csvMap->cells = (CsvCell**)malloc(MAX_CSV_LENGTH*(sizeof(CsvCell*)));
    for(MAX_CSV_LENGTH_ITER_TYPE i=0; i<MAX_CSV_LENGTH; i++){
        csvMap->cells[i] = (CsvCell*)malloc(MAX_CSV_WIDTH*(sizeof(CsvCell)));
    }
    for(MAX_CSV_LENGTH_ITER_TYPE j=0; j<MAX_CSV_LENGTH; j++){
        for(MAX_CSV_WIDTH_ITER_TYPE i=0; i<MAX_CSV_WIDTH; i++){
            csvMap->cells[j][i].cellStringStartPosition = 0;
            csvMap->cells[j][i].cellStringEndPosition = 0;
            csvMap->cells[j][i].cellColumnPosition = i;
            csvMap->cells[j][i].cellRowPosition = j;
            csvMap->cells[j][i].encasedInSecondDegreeDelimeter = 0;
            csvMap->cells[j][i].notEmpty = FALSE;
        }
    }
    csvMap->widestRowSize = 0;
    MAX_CSV_WIDTH_ITER_TYPE tempWidth = 0;
    for(MAX_CSV_LENGTH_ITER_TYPE i=0; i<csvMap->rowsNumber; i++){
        if(csvMap->csvBuffer[csvMap->rows[i].lineStringStartPosition] == CSV_SECOND_DEGREE_DELIMETER){
            startMarkingNextCell_CsvMapConstructor(
                csvMap, csvMap->rows[i].lineStringStartPosition, i, tempWidth, TRUE);
        }
        else{
            startMarkingNextCell_CsvMapConstructor(
                csvMap, csvMap->rows[i].lineStringStartPosition, i, tempWidth, FALSE);
        }
        if(csvMap->rows[i].lineStringEndPosition - csvMap->rows[i].lineStringStartPosition == 0){
            finishMarkingPrevCell_CsvMapConstructor(
                csvMap, csvMap->rows[i].lineStringEndPosition, i, tempWidth);
            tempWidth++;
            if(tempWidth > csvMap->widestRowSize) { csvMap->widestRowSize = tempWidth; }
        }
        else{
            for(MAX_CSV_BUFFER_SIZE_ITER_TYPE chrIter=csvMap->rows[i].lineStringStartPosition;
                chrIter<csvMap->rows[i].lineStringEndPosition; chrIter++){
                if(tempWidth+1 > MAX_CSV_WIDTH){
                    return COLUMNS_EXCEED_LIMIT_CSV_MAP_CONSTRUCTOR;
                }
                if(csvMap->csvBuffer[chrIter] == CSV_FIRST_DEGREE_DELIMETER &&
                   csvMap->csvBuffer[chrIter+1] == CSV_SECOND_DEGREE_DELIMETER){
                    finishMarkingPrevCell_CsvMapConstructor(csvMap, chrIter, i, tempWidth);
                    chrIter++;chrIter++;
                    tempWidth++;
                    if(tempWidth > csvMap->widestRowSize) { csvMap->widestRowSize = tempWidth; }
                    startMarkingNextCell_CsvMapConstructor(csvMap, chrIter, i, tempWidth, TRUE);
                    while((csvMap->csvBuffer[chrIter] != CSV_SECOND_DEGREE_DELIMETER &&
                           csvMap->csvBuffer[chrIter+1] != CSV_FIRST_DEGREE_DELIMETER) ||
                          (csvMap->csvBuffer[chrIter] != CSV_SECOND_DEGREE_DELIMETER &&
                           chrIter+1 != csvMap->rows[i].lineStringEndPosition)){
                        chrIter++;
                    }
                }
                else if(csvMap->csvBuffer[chrIter] == CSV_FIRST_DEGREE_DELIMETER){
                    finishMarkingPrevCell_CsvMapConstructor(csvMap, chrIter, i, tempWidth);
                    chrIter++;
                    tempWidth++;
                    if(tempWidth > csvMap->widestRowSize) { csvMap->widestRowSize = tempWidth; }
                    startMarkingNextCell_CsvMapConstructor(csvMap, chrIter, i, tempWidth, FALSE);
                }
                if(chrIter+1 == csvMap->rows[i].lineStringEndPosition ||
                   chrIter == csvMap->rows[i].lineStringEndPosition){
                    finishMarkingPrevCell_CsvMapConstructor(csvMap, chrIter+1, i, tempWidth);
                    tempWidth++;
                    if(tempWidth > csvMap->widestRowSize) { csvMap->widestRowSize = tempWidth; }
                }
            }
        }
        tempWidth = 0;
    }
    return 0;
}
exitCode CsvMapConstructor(CsvMap* csvMap, const char* csvBuffer,
                           const MAX_CSV_BUFFER_SIZE_ITER_TYPE csvBufferSize){
    csvMap->csvBuffer = (char*)malloc(MAX_CSV_BUFFER_SIZE*(sizeof(char)));
    strncpy(csvMap->csvBuffer, csvBuffer, csvBufferSize);
    csvMap->csvBufferSize = csvBufferSize;
    if(fillUpLines_CsvMapConstructor(csvMap) == ROWS_EXCEED_LIMIT_CSV_MAP_CONSTRUCTOR){
        return ROWS_EXCEED_LIMIT_CSV_MAP_CONSTRUCTOR;
    }
    if(fillUpCells_CsvMapConstructor(csvMap) == COLUMNS_EXCEED_LIMIT_CSV_MAP_CONSTRUCTOR){
        return COLUMNS_EXCEED_LIMIT_CSV_MAP_CONSTRUCTOR;
    }
    return NO_ERROR_CSV_MAP_CONSTRUCTOR;
}
bool8 cellContentsShouldBeEncasedInSecondDegreeDelimitation(
    const char* cellContents, const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE cellContentsSize){
    if(strchr(cellContents, CSV_FIRST_DEGREE_DELIMETER) != NULL){
        return TRUE;
    }
    if(strchr(cellContents, CSV_SECOND_DEGREE_DELIMETER) != NULL){
        return TRUE;
    }
    if(strchr(cellContents, CSV_NEWLINE_DELIMETER) != NULL){
        return TRUE;
    }
    return FALSE;
}
exitCode CsvMapDestructor(CsvMap* csvMap){
    free(csvMap->csvBuffer);
    csvMap->csvBufferSize = 0;
    free(csvMap->rows);
    free(csvMap->cells);
    csvMap->rowsNumber = 0;
    csvMap->widestRowSize = 0;
    return 0;
}
exitCode fprintfCellContents(FILE* csvFilePointer, const CsvCell* cell,
                             const char* csvBuffer){
    if(cell->notEmpty == FALSE){
        return 0;
    }
    for(MAX_CSV_BUFFER_SIZE_ITER_TYPE i=cell->cellStringStartPosition;
        i<cell->cellStringEndPosition; i++){
        fprintf(csvFilePointer, "%c", csvBuffer[i]);
    }
    if(cell->encasedInSecondDegreeDelimeter == TRUE){
        fprintf(csvFilePointer, "\"");
    }
    return 0;
}
exitCode fprintfDelimitation(FILE* csvFilePointer, const CsvCell* cell){
    if(cell->notEmpty == FALSE){
        return 0;
    }
    if(cell->encasedInSecondDegreeDelimeter == TRUE){
        fprintf(csvFilePointer, "%s", ",\"");
    }
    else{
        fprintf(csvFilePointer, "%s", ",");
    }
    return 0;
}
exitCode fprintfCsvMap(FILE* csvFilePointer, const CsvMap* csvMap){    
    for(MAX_CSV_LENGTH_ITER_TYPE i=0; i<csvMap->rowsNumber; i++){
        fprintfCellContents(csvFilePointer, &(csvMap->cells[i][0]), csvMap->csvBuffer);
        for(MAX_CSV_WIDTH_ITER_TYPE j=1; j<csvMap->widestRowSize; j++){
            fprintfDelimitation(csvFilePointer, &(csvMap->cells[i][j]));
            fprintfCellContents(csvFilePointer, &(csvMap->cells[i][j]), csvMap->csvBuffer);
        }
        fprintf(csvFilePointer, "\n");
    }
    return 0;
}
exitCode printfCellContents(const CsvCell* cell, const char* csvBuffer){
    if(cell->notEmpty == FALSE){
        return 0;
    }
    for(MAX_CSV_BUFFER_SIZE_ITER_TYPE i=cell->cellStringStartPosition;
        i<cell->cellStringEndPosition; i++){
        printf("%c", csvBuffer[i]);
    }
    if(cell->encasedInSecondDegreeDelimeter == TRUE){
        printf("\"");
    }
    return 0;
}
exitCode printfDelimitation(const CsvCell* cell){
    if(cell->notEmpty == FALSE){
        return 0;
    }
    if(cell->encasedInSecondDegreeDelimeter == TRUE){
        printf("%s", ",\"");
    }
    else{
        printf("%s", ",");
    }
    return 0;
}
exitCode printCsvMapDebug(const CsvMap* csvMap){
    //printf("%.*s\n", csvMap->csvBufferSize, csvMap->csvBuffer);
    printfBigAssLine();
    
    for(MAX_CSV_LENGTH_ITER_TYPE i=0; i<csvMap->rowsNumber; i++){
        printf("Line %d: %d-%d:\t", i,
               csvMap->rows[i].lineStringStartPosition,
               csvMap->rows[i].lineStringEndPosition);
        if(printStringPart(csvMap->csvBuffer,
                           csvMap->csvBufferSize,
                           csvMap->rows[i].lineStringStartPosition,
                           csvMap->rows[i].lineStringEndPosition) != NO_ERROR_PRINT_STRING_PART){
            return 1;
        }
        printf("\n");
    }
    printfBigAssLine();
    
    for(MAX_CSV_LENGTH_ITER_TYPE j=0; j<csvMap->rowsNumber; j++){
        for(MAX_CSV_WIDTH_ITER_TYPE i=0; i<csvMap->widestRowSize; i++){
            printf("Cell %d.%d, %d; %d: %d-%d:  ",
                   csvMap->cells[j][i].cellColumnPosition,
                   csvMap->cells[j][i].cellRowPosition,
                   csvMap->cells[j][i].encasedInSecondDegreeDelimeter,
                   csvMap->cells[j][i].notEmpty,
                   csvMap->cells[j][i].cellStringStartPosition,
                   csvMap->cells[j][i].cellStringEndPosition);
            if(printStringPart(csvMap->csvBuffer,
                               csvMap->csvBufferSize,
                               csvMap->cells[j][i].cellStringStartPosition,
                               csvMap->cells[j][i].cellStringEndPosition) != NO_ERROR_PRINT_STRING_PART){
                return 1;
            }
            printf("\t");
        }
        printf("\n");
    }
    printfBigAssLine();
    return 0;
}
exitCode printCsvMap(const CsvMap* csvMap){    
    for(MAX_CSV_LENGTH_ITER_TYPE j=0; j<csvMap->rowsNumber; j++){
        for(MAX_CSV_WIDTH_ITER_TYPE i=0; i<csvMap->widestRowSize; i++){
            if(printStringPart(csvMap->csvBuffer,
                               csvMap->csvBufferSize,
                               csvMap->cells[j][i].cellStringStartPosition,
                               csvMap->cells[j][i].cellStringEndPosition) != NO_ERROR_PRINT_STRING_PART){
                return 1;
            }
            printf(" ");
        }
        printf("\n");
    }
    return 0;
}
#endif //CSV_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION

// GENERAL UTILITY IMPLEMENTATIONS _______________________________________________________

// These are split cause I am using things in implementations above
// #if defined clause won't run in case I am using subsections with them
// automatically pulling depending definitions/implementations by themselves
#if defined(GENERAL_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION) // __________________________
void failCond(const bool8 condition, const char* msg){
    if(condition){
        printf("Error: %s Exitting...\n",msg);
        exit(1);
    }
}
exitCode printfBigAssLine(){
    printf ("\n\n____________________________________________________________\n");
}
int qsortMode(const void *x_void, const void *y_void){
    int x = *(int *)x_void;
    int y = *(int *)y_void;
    return x-y;
}
exitCode printStringPart(const char *string, const u64 stringSize,
                         const u64 start, const u64 end){
    if(end > stringSize || end < 0 || start > stringSize || start < 0) {
        return SUBSTRING_OUT_OF_SCOPE_PRINT_STRING_PART;
    }
    if(start > end) {
        return START_FURTHER_THAN_END_PRINT_STRING_PART;
    }
    for(u64 i=start; i<end; i++){
        printf("%c", string[i]);
    }
    return NO_ERROR_PRINT_STRING_PART; 
}
#endif // GENERAL_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION

//________________________________________________________________________________________

#endif // MY_SFH_H_
