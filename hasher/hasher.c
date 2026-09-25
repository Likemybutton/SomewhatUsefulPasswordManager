#include "../settings.h"
#define SYSTEM_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION
#define ASCII_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION
#define CSV_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION
#include "../my_sfh.h"
#include "args.c"
#include "hashing.c"
#include "wordlist.c"
exitCode writeHashedColumn(FILE* csvFilePointer, const CsvMap* csvMap,
                           const CsvCellForHashing* hashCol, const UserInput* args){    
    if(args->userCsvColumnIndex == 0){
        fprintf(csvFilePointer, "%.*s", args->openFileSignatureLength, args->openFileSignature);
    }
    else{
        fprintfCellContents(csvFilePointer, &(csvMap->cells[0][0]), csvMap->csvBuffer);
    }
    for(MAX_CSV_WIDTH_ITER_TYPE headIter=1; headIter<csvMap->widestRowSize; headIter++){
        fprintfDelimitation(csvFilePointer, &(csvMap->cells[0][headIter]));
        if(args->userCsvColumnIndex == headIter){
            fprintf(csvFilePointer, "%.*s", args->openFileSignatureLength, args->openFileSignature);
        }
        else{
            fprintfCellContents(csvFilePointer, &(csvMap->cells[0][headIter]), csvMap->csvBuffer);
        }
    }
    fprintf(csvFilePointer, "\n");
    
    for(MAX_CSV_LENGTH_ITER_TYPE i=1; i<csvMap->rowsNumber; i++){
        if(args->userCsvColumnIndex == 0){
            fprintf(csvFilePointer, "%.*s",
                    hashCol[i-CSV_FIRST_DATA_ROW_POSITION].csvCellContentsHashedSize,
                    hashCol[i-CSV_FIRST_DATA_ROW_POSITION].csvCellContentsHashed);
        }
        else{
            fprintfCellContents(csvFilePointer, &(csvMap->cells[i][0]), csvMap->csvBuffer);
        }
        for(MAX_CSV_WIDTH_ITER_TYPE j=1; j<csvMap->widestRowSize; j++){
            fprintfDelimitation(csvFilePointer, &(csvMap->cells[i][j]));
            if(args->userCsvColumnIndex == j){
                fprintf(csvFilePointer, "%.*s",
                        hashCol[i-CSV_FIRST_DATA_ROW_POSITION].csvCellContentsHashedSize,
                        hashCol[i-CSV_FIRST_DATA_ROW_POSITION].csvCellContentsHashed);
            }
            else{
                fprintfCellContents(csvFilePointer, &(csvMap->cells[i][j]), csvMap->csvBuffer);
            }
        }
        fprintf(csvFilePointer, "\n");
    }
    return 0;
}

// TODO BONUS Rewrite args constructor thingy, to more explicitly show required and optional flags

int main (const int argc, char** argv) {
    const time_t timestamp = time(NULL);
    UserInput args;
    UserInputValidatingConstructor(argc, argv, &args, timestamp);

    FILE* csvFilePointer = fopen(args.csvFilePath, "r");
    failCond(csvFilePointer == NULL,"Failed to open the csv file.");
    char* userCsvBuffer = (char*)malloc(MAX_CSV_BUFFER_SIZE*(sizeof(char)));
    const MAX_CSV_BUFFER_SIZE_ITER_TYPE userCsvBufferSize = fread(userCsvBuffer, sizeof(char),
                                                                  MAX_CSV_BUFFER_SIZE,
                                                                  csvFilePointer);
    fclose(csvFilePointer);
    CsvMap csvMap;
    const exitCode CsvMapConstructorErr = CsvMapConstructor(&csvMap, userCsvBuffer, userCsvBufferSize);
    free(userCsvBuffer);
    failCond(CsvMapConstructorErr,CsvMapConstructorErrorMessages[CsvMapConstructorErr]);

    srand(clock());
    
    CsvCellForHashing* userColumn = (CsvCellForHashing*)malloc(csvMap.rowsNumber
                                                               * sizeof(CsvCellForHashing));
    ColumnForHashingDefaultConstructor(userColumn, csvMap.rowsNumber);
    MAX_CSV_LENGTH_ITER_TYPE userColumnSize = fillUserColumnForHashing(userColumn, &csvMap, &args);
    
    FILE* wordlistFilePointer = fopen(args.wordlistFilePath, "w");
    fillWordlistFile(wordlistFilePointer,userColumn,userColumnSize,&args);
    fclose(wordlistFilePointer);
    
    csvFilePointer = fopen(args.csvFilePath, "w");
    chmod(args.csvFilePath, 0777);
    failCond(csvFilePointer == NULL,"Failed to open the csv file.");
    writeHashedColumn(csvFilePointer, &csvMap, userColumn, &args);
    fclose(csvFilePointer);

    CsvMapDestructor(&csvMap);
    UserInputDestructor(&args);
    return 0;
}

