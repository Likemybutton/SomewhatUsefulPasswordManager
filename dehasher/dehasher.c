#include "../settings.h"
#define SYSTEM_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION
#define ASCII_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION
#define CSV_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION
#include "../my_sfh.h"
#include "args.c"
#include "dehashing.c"
exitCode writeDehashedColumn(FILE* csvFilePointer, const CsvMap* csvMap,
                             DehashedCell* dehashCol, const UserInput* args){    
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
            if(dehashCol[i].encasedInSecondDegreeDelimeter == TRUE){
                fprintf(csvFilePointer, "\"%.*s\"", dehashCol[i].cellContentsSize, dehashCol[i].cellContents);
            }
            else{
                fprintf(csvFilePointer, "%.*s", dehashCol[i].cellContentsSize, dehashCol[i].cellContents);
            }
        }
        else{
            fprintfCellContents(csvFilePointer, &(csvMap->cells[i][0]), csvMap->csvBuffer);
        }
        for(MAX_CSV_WIDTH_ITER_TYPE j=1; j<csvMap->widestRowSize; j++){
            fprintfDelimitation(csvFilePointer, &(csvMap->cells[i][j]));
            if(args->hashedCsvColumnIndex == j){
                if(dehashCol[i].encasedInSecondDegreeDelimeter == TRUE){
                    fprintf(csvFilePointer, "\"%.*s\"",
                            dehashCol[i].cellContentsSize,
                          dehashCol[i].cellContents);
                }
                else{
                    fprintf(csvFilePointer, "%.*s", dehashCol[i].cellContentsSize,
                            dehashCol[i].cellContents);
                }
            }
            else{
                fprintfCellContents(csvFilePointer, &(csvMap->cells[i][j]), csvMap->csvBuffer);
            }
        }
        fprintf(csvFilePointer, "\n");
    }
    return 0;
}
exitCode printDehashedColumn(const CsvMap* csvMap, DehashedCell* dehashCol,
                             const UserInput* args){    
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
            if(dehashCol[i].encasedInSecondDegreeDelimeter == TRUE){
                printf("\"%.*s\"", dehashCol[i].cellContentsSize, dehashCol[i].cellContents);
            }
            else{
                printf("%.*s", dehashCol[i].cellContentsSize, dehashCol[i].cellContents);
            }
        }
        /* else{ */
        /*     printfCellContents(&(csvMap->cells[i][0]), csvMap->csvBuffer); */
        /* } */
        for(MAX_CSV_WIDTH_ITER_TYPE j=1; j<csvMap->widestRowSize; j++){
            //printfDelimitation(&(csvMap->cells[i][j]));
            if(args->hashedCsvColumnIndex == j){
                if(dehashCol[i].encasedInSecondDegreeDelimeter == TRUE){
                    printf("\"%.*s\"", dehashCol[i].cellContentsSize, dehashCol[i].cellContents);
                }
                else{
                  printf("%.*s", dehashCol[i].cellContentsSize, dehashCol[i].cellContents);
                }
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

    DehashedCell* dehashedColumnTemp = (DehashedCell*)malloc(csvMap.rowsNumber*sizeof(DehashedCell));
    for(MAX_CSV_LENGTH_ITER_TYPE i=0; i<csvMap.rowsNumber; i++){
        DehashedCellDefaultConstructor(&dehashedColumnTemp[i]);
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
        printDehashedColumn(&csvMap, dehashedColumnTemp, &args);
    }
    
    CsvMapDestructor(&csvMap);
    UserInputDestructor(&args);
    return 0;
}

