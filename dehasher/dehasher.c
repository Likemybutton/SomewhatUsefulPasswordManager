#include "../settings.h"
#define SYSTEM_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION
#define ASCII_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION
#define CSV_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION
#include "../my_sfh.h"
#include "args.c"
#include "dehashing.c"
exitCode writeDehashedColumn(FILE* csvFilePointer, const CsvMap* csvMap,
                             char** dehashCol, const UserInput* args){    
    if(args->hashedCsvColumnIndex == 0){
        fprintf(csvFilePointer, "%.*s", args->dehashedCsvColumnHeaderNameSize, args->dehashedCsvColumnHeaderName);
    }
    else{
        fprintfCellContents(csvFilePointer, &(csvMap->cells[0][0]), csvMap->csvBuffer);
    }
    for(MAX_CSV_WIDTH_ITER_TYPE headIter=1; headIter<csvMap->widestRowSize; headIter++){
        fprintfDelimitation(csvFilePointer, &(csvMap->cells[0][headIter]));
        if(args->hashedCsvColumnIndex == headIter){
            fprintf(csvFilePointer, "%.*s", args->dehashedCsvColumnHeaderNameSize, args->dehashedCsvColumnHeaderName);
        }
        else{
            fprintfCellContents(csvFilePointer, &(csvMap->cells[0][headIter]), csvMap->csvBuffer);
        }
    }
    fprintf(csvFilePointer, "\n");
    
    for(MAX_CSV_LENGTH_ITER_TYPE i=1; i<csvMap->rowsNumber; i++){
        if(args->hashedCsvColumnIndex == 0){
            fprintf(csvFilePointer, "%s",
                    dehashCol[i]);
        }
        else{
            fprintfCellContents(csvFilePointer, &(csvMap->cells[i][0]), csvMap->csvBuffer);
        }
        for(MAX_CSV_WIDTH_ITER_TYPE j=1; j<csvMap->widestRowSize; j++){
            fprintfDelimitation(csvFilePointer, &(csvMap->cells[i][j]));
            if(args->hashedCsvColumnIndex == j){
                fprintf(csvFilePointer, "%s",
                        dehashCol[i]);
            }
            else{
                fprintfCellContents(csvFilePointer, &(csvMap->cells[i][j]), csvMap->csvBuffer);
            }
        }
        fprintf(csvFilePointer, "\n");
    }
    return 0;
}
exitCode printFileDehashedColumn(const CsvMap* csvMap, char** dehashCol, const UserInput* args){    
    if(args->hashedCsvColumnIndex == 0){
        printf("%.*s", args->dehashedCsvColumnHeaderNameSize, args->dehashedCsvColumnHeaderName);
    }
    /* else{ */
    /*     printfCellContents(&(csvMap->cells[0][0]), csvMap->csvBuffer); */
    /* } */
    for(MAX_CSV_WIDTH_ITER_TYPE headIter=1; headIter<csvMap->widestRowSize; headIter++){
        //printfDelimitation(&(csvMap->cells[0][headIter]));
        if(args->hashedCsvColumnIndex == headIter){
            printf("%.*s", args->dehashedCsvColumnHeaderNameSize, args->dehashedCsvColumnHeaderName);
        }
        /* else{ */
        /*     printfCellContents(&(csvMap->cells[0][headIter]), csvMap->csvBuffer); */
        /* } */
    }
    printf("\n");
    
    for(MAX_CSV_LENGTH_ITER_TYPE i=1; i<csvMap->rowsNumber; i++){
        if(args->hashedCsvColumnIndex == 0){
            printf("%s",
                    dehashCol[i]);
        }
        /* else{ */
        /*     printfCellContents(&(csvMap->cells[i][0]), csvMap->csvBuffer); */
        /* } */
        for(MAX_CSV_WIDTH_ITER_TYPE j=1; j<csvMap->widestRowSize; j++){
            //printfDelimitation(&(csvMap->cells[i][j]));
            if(args->hashedCsvColumnIndex == j){
                printf("%s",
                        dehashCol[i]);
            }
            /* else{ */
            /*     printfCellContents(&(csvMap->cells[i][j]), csvMap->csvBuffer); */
            /* } */
        }
        printf("\n");
    }
    return 0;
}
int main (const int argc, char** argv) {    
    UserInput args;
    UserInputValidatingConstructor(argc, argv, &args);
    //printArguments(&args);

    FILE* hashedCsvFilePointer = fopen(args.hashedCsvFilePath, "r");
    char* userHashedCsvBuffer = (char*)malloc(MAX_CSV_BUFFER_SIZE*(sizeof(char)));
    const MAX_CSV_BUFFER_SIZE_ITER_TYPE userCsvBufferSize = fread(userHashedCsvBuffer, sizeof(char),
                                                                  MAX_CSV_BUFFER_SIZE,
                                                                  hashedCsvFilePointer);
    fclose(hashedCsvFilePointer);
    CsvMap csvMap;
    const exitCode CsvMapConstructorErr = CsvMapConstructor(&csvMap, userHashedCsvBuffer, userCsvBufferSize);
    free(userHashedCsvBuffer);
    failCond(CsvMapConstructorErr,CsvMapConstructorErrorMessages[CsvMapConstructorErr]);

    char** dehashedColumnTemp = (char**)malloc(csvMap.rowsNumber*sizeof(char*));
    for(MAX_CSV_LENGTH_ITER_TYPE i=0; i<csvMap.rowsNumber; i++){
        dehashedColumnTemp[i] = (char*)malloc(MAX_CSV_CELL_CONTENT_SIZE*sizeof(char));
    }
    getDehashedValues(dehashedColumnTemp, &csvMap, &args);
    
    if(args.viewModeActive == FALSE){
        hashedCsvFilePointer = fopen(args.hashedCsvFilePath, "w");
        chmod(args.hashedCsvFilePath, 0777);
        failCond(hashedCsvFilePointer == NULL,"Failed to open the csv file.");
        writeDehashedColumn(hashedCsvFilePointer, &csvMap, dehashedColumnTemp, &args);
        fclose(hashedCsvFilePointer);
    }
    else{
        printFileDehashedColumn(&csvMap, dehashedColumnTemp, &args);
    }
    
    CsvMapDestructor(&csvMap);
    UserInputDestructor(&args);
    return 0;
}

