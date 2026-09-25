typedef struct{
    char* csvCellContents;
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE csvCellContentsSize;
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE* avaliablePositionsArr;
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE avaliablePositionsArrSize;
}RealFakeCellForWordlist;
exitCode RealFakeCellForWordlistDestructor(RealFakeCellForWordlist* cell){
    free(cell->csvCellContents);
    cell->csvCellContentsSize = 0;
    free(cell->avaliablePositionsArr);
    cell->avaliablePositionsArrSize = 0;
    return 0;
}
int qsortModeFor_MAX_CSV_LENGTH_ITER_TYPE(const void *x_void, const void *y_void){
    MAX_CSV_LENGTH_ITER_TYPE x = *(MAX_CSV_LENGTH_ITER_TYPE *)x_void;
    MAX_CSV_LENGTH_ITER_TYPE y = *(MAX_CSV_LENGTH_ITER_TYPE *)y_void;
    failCond(sizeof(MAX_CSV_LENGTH_ITER_TYPE) > sizeof(int),
             "qsortModeFor_MAX_CSV_LENGTH_ITER_TYPE got broken somehow.");
    return x-y;
}
exitCode isInCellContentsPositionsArray(const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE* array,
                                        const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE size,
                                        const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE value){
    for(MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE i=0; i<size; i++){
        if(value == array[i])
            return 1;
    }
    return 0;
}
MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE excludeItemInCellContentsPositionsArray(
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE* array,
    const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE size,
    const MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE item){
    for(MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE i=0; i<size; i++){
        if(array[i]==item){
            for(MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE j=i; j<size; j++){
                array[j] = array[j+1];
            }
            return size-1;
        }
    }
    return size;
}
exitCode fillRealFakeCsvCellContentsWithSymbolsRandomly(RealFakeCellForWordlist* cell,
                                                        const char* symCase, const int numFills){
    MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE selectPosition;
    for(MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE i=0; i<numFills; i++){
        selectPosition = rand() % cell->avaliablePositionsArrSize;
        for(FAULTY_RANDOM_COUNTER_LIMIT_ITER_TYPE faultyRandCounter=0;
            !isInCellContentsPositionsArray(cell->avaliablePositionsArr,
                                            cell->avaliablePositionsArrSize,
                                            selectPosition);
            faultyRandCounter++){
            selectPosition = rand() % cell->avaliablePositionsArrSize;
            if(faultyRandCounter==FAULTY_RANDOM_COUNTER_LIMIT){
                selectPosition = cell->avaliablePositionsArr[0];
                break;
            }
        }
        cell->csvCellContents[selectPosition] = getRandomSymbol(symCase);
        cell->avaliablePositionsArrSize
            = excludeItemInCellContentsPositionsArray(cell->avaliablePositionsArr,
                                                      cell->avaliablePositionsArrSize,
                                                      selectPosition);        
    }
    return 0;
}
exitCode generateRealFakeCell(const Options opt, RealFakeCellForWordlist* cell){
    if((opt.numUppers + opt.numDigits + opt.numSpecials) > opt.length){
        printf("Error: The number of optional symbols has to be less than lenght of the password.\n");
        return 1;
    }
    cell->csvCellContentsSize = opt.length;
    cell->csvCellContents = (char*)malloc(cell->csvCellContentsSize*sizeof(char));

    cell->avaliablePositionsArrSize = opt.length;
    cell->avaliablePositionsArr = (MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE*)malloc(
        cell->avaliablePositionsArrSize
        * sizeof(MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE));
    for(MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE i=0; i<cell->csvCellContentsSize; i++){
        cell->csvCellContents[i] = getRandomSymbol("lower");
        cell->avaliablePositionsArr[i] = i;
    }
    fillRealFakeCsvCellContentsWithSymbolsRandomly(cell, "upper", opt.numUppers);
    fillRealFakeCsvCellContentsWithSymbolsRandomly(cell, "digit", opt.numDigits);
    fillRealFakeCsvCellContentsWithSymbolsRandomly(cell, "special", opt.numSpecials);
    return 0;
}
exitCode fillWordlistFile(FILE* wordlistFilePointer,
                          const CsvCellForHashing* userColumn,
                          const MAX_CSV_LENGTH_ITER_TYPE userColumnSize,
                          const UserInput* args){
    MAX_CSV_LENGTH_ITER_TYPE randomWordlistPositions[userColumnSize];
    for(MAX_CSV_LENGTH_ITER_TYPE i=0; i<userColumnSize; i++){
        randomWordlistPositions[i] = rand()%args->wordlistSize;
    }
    qsort(randomWordlistPositions, userColumnSize,
          sizeof(MAX_CSV_LENGTH_ITER_TYPE),
          qsortModeFor_MAX_CSV_LENGTH_ITER_TYPE);
    MAX_CSV_LENGTH_ITER_TYPE randomWordlistPositionsCounter = 0;
    for(MAX_WORDLIST_SIZE_ITER_TYPE i=0; i<args->wordlistSize; i++){
        if(i == randomWordlistPositions[randomWordlistPositionsCounter]){
            fprintf(wordlistFilePointer, "%.*s\n",
                    userColumn[randomWordlistPositionsCounter].csvCellContentsSize,
                    userColumn[randomWordlistPositionsCounter].csvCellContents);
            randomWordlistPositionsCounter++;
        }
        else{
            RealFakeCellForWordlist realCell;
            generateRealFakeCell(userColumn[rand()%userColumnSize].csvCellContentsCharacteristics, &realCell);
            fprintf(wordlistFilePointer, "%.*s\n", realCell.csvCellContentsSize,realCell.csvCellContents);
            RealFakeCellForWordlistDestructor(&realCell);
        }
    }    
    return 0;
}
