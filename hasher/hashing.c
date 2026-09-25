#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#define ITERATION_PKCS5_PBKDF2_HMAC_SHA1 1
#define DEFAULT_HASH_STRING_LENGTH_PKCS5_PBKDF2_HMAC_SHA1 DEFAULT_HASH_OCTET_LENGTH_PKCS5_PBKDF2_HMAC_SHA1 * 2
typedef struct{
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE length;
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE numUppers;
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE numDigits;
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE numSpecials;
}Options;
typedef struct{
    char* csvCellContents; MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE csvCellContentsSize;
    char* csvCellContentsHashed; MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE csvCellContentsHashedSize;
    Options csvCellContentsCharacteristics;
}CsvCellForHashing;
exitCode ColumnForHashingDefaultConstructor(CsvCellForHashing* col,
                                            const MAX_CSV_LENGTH_ITER_TYPE colLen){
    for(MAX_CSV_LENGTH_ITER_TYPE i=0; i<colLen; i++){
        col[i].csvCellContents = (char*)malloc(MAX_CSV_CELL_CONTENT_SIZE * sizeof(char));
        col[i].csvCellContentsSize = 0;
        col[i].csvCellContentsHashed = (char*)malloc(DEFAULT_HASH_STRING_LENGTH_PKCS5_PBKDF2_HMAC_SHA1
                                                     * sizeof(char));
        col[i].csvCellContentsHashedSize = DEFAULT_HASH_STRING_LENGTH_PKCS5_PBKDF2_HMAC_SHA1;
        col[i].csvCellContentsCharacteristics.length = 0;
        col[i].csvCellContentsCharacteristics.numUppers = 0;
        col[i].csvCellContentsCharacteristics.numDigits = 0;
        col[i].csvCellContentsCharacteristics.numSpecials = 0;
    }
    return 0;
}
MAX_CSV_LENGTH_ITER_TYPE fillUserColumnForHashing(CsvCellForHashing* userColumnForHashing,
                                                  const CsvMap *csvMap,
                                                  const UserInput* args){
    MAX_CSV_LENGTH_ITER_TYPE i=CSV_FIRST_DATA_ROW_POSITION;
    MAX_CSV_LENGTH_ITER_TYPE indentedIter = i-CSV_FIRST_DATA_ROW_POSITION;
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE hashtempSize = DEFAULT_HASH_OCTET_LENGTH_PKCS5_PBKDF2_HMAC_SHA1;
    unsigned char* hashtemp = (unsigned char*)malloc(hashtempSize * sizeof(unsigned char));
    while(indentedIter < csvMap->rowsNumber){
        indentedIter = i-CSV_FIRST_DATA_ROW_POSITION;
        if(csvMap->cells[i][args->userCsvColumnIndex].notEmpty == FALSE){
            return indentedIter;
        }
        while(userColumnForHashing[indentedIter].csvCellContentsSize <
              (csvMap->cells[i][args->userCsvColumnIndex].cellStringEndPosition -
               csvMap->cells[i][args->userCsvColumnIndex].cellStringStartPosition)){
            userColumnForHashing[indentedIter].csvCellContents[userColumnForHashing[indentedIter].csvCellContentsSize]
                = csvMap->csvBuffer[csvMap->cells[i][args->userCsvColumnIndex]
                                    .cellStringStartPosition + userColumnForHashing[indentedIter].csvCellContentsSize];
            if(isUpper(userColumnForHashing[indentedIter]
                       .csvCellContents[userColumnForHashing[indentedIter].csvCellContentsSize])){
                userColumnForHashing[indentedIter].csvCellContentsCharacteristics.numUppers++;
            }
            else if(isDigit(userColumnForHashing[indentedIter]
                            .csvCellContents[userColumnForHashing[indentedIter].csvCellContentsSize])){
                userColumnForHashing[indentedIter].csvCellContentsCharacteristics.numDigits++;
            }
            else if(isSpecial(userColumnForHashing[indentedIter]
                              .csvCellContents[userColumnForHashing[indentedIter].csvCellContentsSize])){
                userColumnForHashing[indentedIter].csvCellContentsCharacteristics.numSpecials++;
            }
            userColumnForHashing[indentedIter].csvCellContentsSize++;
        }
        userColumnForHashing[indentedIter].csvCellContentsCharacteristics.length
            = userColumnForHashing[indentedIter].csvCellContentsSize;

        hashtemp = (unsigned char*)calloc(hashtempSize, sizeof(unsigned char));
        failCond(PKCS5_PBKDF2_HMAC_SHA1(userColumnForHashing[indentedIter].csvCellContents,
                                        userColumnForHashing[indentedIter].csvCellContentsSize,
                                        (unsigned char*)args->salt, args->saltLength,
                                        ITERATION_PKCS5_PBKDF2_HMAC_SHA1,
                                        hashtempSize,
                                        hashtemp) == 0,
                 "Failed to hash a string!");
        
        for(size_t k=0; k<hashtempSize; k++){
            snprintf((userColumnForHashing[indentedIter].csvCellContentsHashed+(k*2)),
                     userColumnForHashing[indentedIter].csvCellContentsHashedSize,
                     "%02x", hashtemp[k]);
        }
        userColumnForHashing[indentedIter].csvCellContentsHashed[userColumnForHashing[indentedIter]
                                                                 .csvCellContentsHashedSize] = '\0';
        i++;
    }
    return indentedIter;
}
exitCode printCsvCellForHashing(const CsvCellForHashing* userColumnForHashing,
                                const MAX_CSV_LENGTH_ITER_TYPE colLen){
    for(MAX_CSV_LENGTH_ITER_TYPE i=0; i<colLen; i++){
        printf("%.*s, %.*s, %d, %d, %d\n",
               userColumnForHashing[i].csvCellContentsSize,
               userColumnForHashing[i].csvCellContents,
               userColumnForHashing[i].csvCellContentsHashedSize,
               userColumnForHashing[i].csvCellContentsHashed,
               userColumnForHashing[i].csvCellContentsCharacteristics.numUppers,
               userColumnForHashing[i].csvCellContentsCharacteristics.numDigits,
               userColumnForHashing[i].csvCellContentsCharacteristics.numSpecials);
    }
    return 0;
}
