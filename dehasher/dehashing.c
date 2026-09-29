#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#define ITERATION_PKCS5_PBKDF2_HMAC_SHA1 1
#define DEFAULT_HASH_STRING_LENGTH_PKCS5_PBKDF2_HMAC_SHA1 DEFAULT_HASH_OCTET_LENGTH_PKCS5_PBKDF2_HMAC_SHA1 * 2
typedef struct{
    char* cellContents;
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE cellContentsSize;
    bool8 encasedInSecondDegreeDelimeter;
}DehashedCell;
exitCode DehashedCellDefaultConstructor(DehashedCell* cell){
    cell->cellContents = (char*)malloc(MAX_CSV_CELL_CONTENT_SIZE*sizeof(char));
    cell->cellContentsSize = 0;
    cell->encasedInSecondDegreeDelimeter = 0;
    return 0;
}
exitCode fillDehashedCell(
    DehashedCell* cell, const char* cellContentsFill,
    const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE cellContentsSizeFill,
    const bool8 encasedInSecondDegreeDelimeterFill){
    cell->cellContentsSize = cellContentsSizeFill;
    strncpy(cell->cellContents, cellContentsFill, cell->cellContentsSize);
    cell->encasedInSecondDegreeDelimeter = encasedInSecondDegreeDelimeterFill;
    return 0;
}
exitCode printDehashedCell(const DehashedCell* cell){
    printf("%.*s - %d\n", cell->cellContentsSize, cell->cellContents,
           cell->encasedInSecondDegreeDelimeter);
    return 0;
}
exitCode DehashedCellDestructor(DehashedCell* cell){
    free(cell->cellContents);
    cell->cellContentsSize = 0;
    cell->encasedInSecondDegreeDelimeter = 0;
    return 0;
}
MAX_WORDLIST_FILE_BUFFER_SIZE_ITER_TYPE findMatchingWordFromWordlistBufferAndFill(
    DehashedCell* ans, const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE maxAnsContentsSize,
    const char* src, const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE srcSize,
    const char* wordlistBuffer,
    const MAX_WORDLIST_FILE_BUFFER_SIZE_ITER_TYPE worlistIteration,
    const UserInput* args){
    MAX_WORDLIST_FILE_BUFFER_SIZE_ITER_TYPE bufferCounter = worlistIteration;
    char* strdupTemp = strdup(wordlistBuffer + (bufferCounter * sizeof(char)));
    
    char* lineString = strtok(strdupTemp, WORDLIST_DELIMETER);
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE hashtempSize = DEFAULT_HASH_OCTET_LENGTH_PKCS5_PBKDF2_HMAC_SHA1;
    unsigned char* hashtemp = (unsigned char*)malloc(hashtempSize * sizeof(unsigned char));
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE hashtempCharsSize = DEFAULT_HASH_STRING_LENGTH_PKCS5_PBKDF2_HMAC_SHA1;
    char* hashtempChars = (char*)malloc(hashtempCharsSize + 1 * sizeof(char));
    while(lineString){
        MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE lineStringSize = strlen(lineString);
        hashtemp = (unsigned char*)calloc(hashtempSize, sizeof(unsigned char));
        failCond(PKCS5_PBKDF2_HMAC_SHA1(lineString, lineStringSize,
                                        (unsigned char*)args->salt, args->saltLength,
                                        ITERATION_PKCS5_PBKDF2_HMAC_SHA1,
                                        hashtempSize,
                                        hashtemp) == 0,
                 "Failed to hash a string!");
        for(size_t i=0; i<hashtempSize; i++){
            snprintf((hashtempChars+(i*2)), hashtempCharsSize, "%02x", hashtemp[i]);
        }
        bufferCounter += (MAX_WORDLIST_FILE_BUFFER_SIZE_ITER_TYPE)lineStringSize + 1;
        if(strncmp(hashtempChars, src, hashtempCharsSize) == 0){
            failCond(lineStringSize >= maxAnsContentsSize,
                     "Failure on copying during dehashing");
            fillDehashedCell(
                ans, lineString, lineStringSize,
                cellContentsShouldBeEncasedInSecondDegreeDelimitation(
                    lineString, lineStringSize));
            return bufferCounter;
        }        
        lineString = strtok(NULL, WORDLIST_DELIMETER);
    }
    free(hashtemp);
    free(hashtempChars);
    free(strdupTemp);
    return bufferCounter;
}
exitCode getDehashedValues(DehashedCell* dehashedColumnTemp,
                           const CsvMap* csvMap, const UserInput* args){
    FILE* wordlistPointer = fopen(args->wordlistPath, "r");
    char* wordlistBuffer = (char*)malloc(MAX_WORDLIST_FILE_BUFFER_SIZE*sizeof(char));
    const MAX_WORDLIST_FILE_BUFFER_SIZE_ITER_TYPE   \
        wordlistBufferSize = fread(wordlistBuffer,
                                   sizeof(char),
                                   MAX_WORDLIST_FILE_BUFFER_SIZE,
                                   wordlistPointer);
    fclose(wordlistPointer);
    fillDehashedCell(&(dehashedColumnTemp[CSV_HEADER_ROW_POSITION]),
                     args->dehashedCsvColumnHeaderName,
                     args->dehashedCsvColumnHeaderNameSize,
                     cellContentsShouldBeEncasedInSecondDegreeDelimitation(
                         args->dehashedCsvColumnHeaderName,
                         args->dehashedCsvColumnHeaderNameSize));
    MAX_WORDLIST_FILE_BUFFER_SIZE_ITER_TYPE bufferCounter = 0;
    for(MAX_CSV_LENGTH_ITER_TYPE i=CSV_FIRST_DATA_ROW_POSITION; i<csvMap->rowsNumber; i++){
        bufferCounter = findMatchingWordFromWordlistBufferAndFill(
            &(dehashedColumnTemp[i]), MAX_CSV_CELL_CONTENT_SIZE,
            &(csvMap->csvBuffer[csvMap->cells[i][args->hashedCsvColumnIndex].cellStringStartPosition]),
            csvMap->cells[i][args->hashedCsvColumnIndex].cellStringEndPosition
            - csvMap->cells[i][args->hashedCsvColumnIndex].cellStringStartPosition,
            wordlistBuffer, bufferCounter, args);
        if(bufferCounter > wordlistBufferSize){
            break;
        }
    }
    free(wordlistBuffer);
    return 0;
}

