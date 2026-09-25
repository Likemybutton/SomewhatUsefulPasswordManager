enum helpArguments{
BINARY_NAME_HELP_CASE,
HELP_FLAG_ARG_INDEX,
NUMBER_OF_REQUIRED_ARGUMENTS_HELP_CASE };
enum requiredArguments{
BINARY_NAME, HASHED_CSV_FILE_PATH_ARG_INDEX,
WORDLIST_PATH_ARG_INDEX, USER_SALT_ARG_INDEX,
NUMBER_OF_REQUIRED_ARGUMENTS};
enum optionalArguments{
VIEWING_MODE_FLAG,
NUMBER_OF_OPTIONAL_ARGUMENTS
};
#define MAX_ARGUMENTS_NUMBER NUMBER_OF_REQUIRED_ARGUMENTS + NUMBER_OF_OPTIONAL_ARGUMENTS
#define MAX_ARGUMENTS_NUMBER_ITER_TYPE u8
exitCode printHelpMessage(){
    printf("%s\n", "Dehasher for dehashing of .csv columns. Dehashes the column \
when given a matching wordlist and salt-password. For proper security: the .csv file,\
wordlist and binaries/source should never be placed in the same physical device!");
    printf("\nUsage: ./dehasher [Path to a .csv file] [Path to a wordlist file] [Salt-password] \n");
    printf("\nOptional arguments:");
    printf("\n\t%s\n", "-v, --view_mode\tPrints dehashed .csv file into the terminal \
output instead of writing it into the file");
    printf("\n");
    return 0;
}
typedef struct{
    char* hashedCsvFilePath; LINUX_MAX_PATH_SIZE_ITER_TYPE hashedCsvFilePathSize;
    char* wordlistPath; LINUX_MAX_PATH_SIZE_ITER_TYPE wordlistPathSize;
    char* userSaltPassword; MAX_USER_SALT_LENGTH_ITER_TYPE userSaltPasswordSize;

    MAX_CSV_WIDTH_ITER_TYPE hashedCsvColumnIndex;
    char* hashedCsvColumnHeaderName; MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE hashedCsvColumnHeaderNameSize;
    char* dehashedCsvColumnHeaderName; MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE dehashedCsvColumnHeaderNameSize;
    char* timestampString; TIME_T_CONVERTED_TO_STRING_POSSIBLE_BUFFER_SIZE_ITER_TYPE timestampStringLen;
    char* salt; SALT_STRING_SIZE_ITER_TYPE saltLength;
    
    bool8 viewModeActive;
}UserInput;
exitCode assignHashedCsvFilePathArgument(const int argc, char** argv, UserInput* args){
    args->hashedCsvFilePath = (char*)malloc(LINUX_MAX_PATH_SIZE*sizeof(char));
    args->hashedCsvFilePathSize = strlen(argv[HASHED_CSV_FILE_PATH_ARG_INDEX]);
    strncpy(args->hashedCsvFilePath, argv[HASHED_CSV_FILE_PATH_ARG_INDEX], args->hashedCsvFilePathSize);
    return 0;
}
exitCode assignWordlistPathArgument(const int argc, char** argv, UserInput* args){
    args->wordlistPath = (char*)malloc(LINUX_MAX_PATH_SIZE*sizeof(char));
    args->wordlistPathSize = strlen(argv[WORDLIST_PATH_ARG_INDEX]);
    strncpy(args->wordlistPath, argv[WORDLIST_PATH_ARG_INDEX], args->wordlistPathSize);
    return 0;
}
exitCode handleUserSaltPasswordArgument(const int argc, char** argv, UserInput* args){
    failCond(strlen(argv[USER_SALT_ARG_INDEX]) >= MAX_USER_SALT_LENGTH,
             "Salt password argument is too big.");
    args->userSaltPassword = (char*)malloc(MAX_USER_SALT_LENGTH*sizeof(char));
    args->userSaltPasswordSize = strlen(argv[USER_SALT_ARG_INDEX]);
    strncpy(args->userSaltPassword, argv[USER_SALT_ARG_INDEX], args->userSaltPasswordSize);
    return 0;
}
exitCode assignHashedCsvColumnHeaderNameAttribute(const int argc, char** argv, UserInput* args){
    char* strdupTemp = strdup(basename(args->wordlistPath));
    char* tempExtension = strchr(strdupTemp, '.');
    if(tempExtension != NULL && strcmp(tempExtension, WORDLIST_FILE_EXTENSION) == 0){
        *tempExtension = '\0';
    }
    args->hashedCsvColumnHeaderName = (char*)malloc(MAX_CSV_CELL_CONTENT_SIZE*sizeof(char));
    args->hashedCsvColumnHeaderNameSize = strlen(strdupTemp);
    strncpy(args->hashedCsvColumnHeaderName,
            strdupTemp,
            args->hashedCsvColumnHeaderNameSize);
    return 0;
}
exitCode assignDehashedCsvColumnHeaderNameAttribute(const int argc, char** argv, UserInput* args){
    strtok(strdup(args->hashedCsvColumnHeaderName), OPEN_FILE_SIGNATURE_DELIMETER);
    strtok(NULL, OPEN_FILE_SIGNATURE_DELIMETER);
    char* tempDehashedCsvColumnHeaderName = strtok(NULL, OPEN_FILE_SIGNATURE_DELIMETER);
    args->dehashedCsvColumnHeaderName = (char*)malloc(MAX_CSV_CELL_CONTENT_SIZE*sizeof(char));
    args->dehashedCsvColumnHeaderNameSize = strlen(tempDehashedCsvColumnHeaderName);
    strncpy(args->dehashedCsvColumnHeaderName,
            tempDehashedCsvColumnHeaderName,
            args->dehashedCsvColumnHeaderNameSize);
    return 0;
}
exitCode assignTimestampStringAttribute(const int argc, char** argv, UserInput* args){
    char* tempTimestampString = strtok(strdup(args->hashedCsvColumnHeaderName),
                                       OPEN_FILE_SIGNATURE_DELIMETER);
    failCond(strlen(tempTimestampString)
             > TIME_T_CONVERTED_TO_STRING_POSSIBLE_BUFFER_SIZE,
             "Timestamp is out of bounds.");
    args->timestampString = (char*)malloc(TIME_T_CONVERTED_TO_STRING_POSSIBLE_BUFFER_SIZE*sizeof(char));
    args->timestampStringLen = strlen(tempTimestampString);
    strncpy(args->timestampString, tempTimestampString,
            args->timestampStringLen);    
    return 0;
}
exitCode assignSaltAttribute(const int argc, char** argv, UserInput* args){
    args->salt = (char*)malloc(SALT_STRING_SIZE*sizeof(char));
    sprintf(args->salt, "%s%s%s", args->timestampString,
            OPEN_FILE_SIGNATURE_DELIMETER, args->userSaltPassword);
    args->saltLength = strlen(args->salt);
    return 0;
}
exitCode handleOptionalArguments(const int argc, char** argv, UserInput* args){
    args->viewModeActive = FALSE;
    for(MAX_ARGUMENTS_NUMBER_ITER_TYPE i=NUMBER_OF_REQUIRED_ARGUMENTS;
        i<argc; i++){
        if(strcmp(argv[i],"-v") == 0 || strcmp(argv[i],"--view_mode") == 0){
            args->viewModeActive = TRUE;
        }
    }
    return 0;
}
exitCode UserInputValidatingConstructor(const int argc, char** argv, UserInput* args){
    failCond(argc < NUMBER_OF_REQUIRED_ARGUMENTS_HELP_CASE,
             "No arguments, see --help for help!");
    if(strcmp(argv[HELP_FLAG_ARG_INDEX],"-h") == 0 ||
       strcmp(argv[HELP_FLAG_ARG_INDEX],"--help") == 0){
        printHelpMessage();
        exit(0);
    }
    failCond(!(whichOS() == LINUX || whichOS() == UNIX),
             "Hasher is Linux only, I don't know how to write cross-platform C. Bleh...");
    failCond(argc < NUMBER_OF_REQUIRED_ARGUMENTS,
             "Give required arguments fool!");
    failCond(argc > MAX_ARGUMENTS_NUMBER,
             "Too much arguments fool");
    
    failCond(strlen(argv[WORDLIST_PATH_ARG_INDEX]) >= LINUX_MAX_PATH_SIZE,
             "CSV path argument is too big.");
    failCond(!isValidFilePath(argv[WORDLIST_PATH_ARG_INDEX],
                              strlen(argv[WORDLIST_PATH_ARG_INDEX])),
             "Bad naming for the path of csv file.");
    FILE* wordlistFilePointer = fopen(argv[WORDLIST_PATH_ARG_INDEX], "r");
    failCond(wordlistFilePointer == NULL,"Failed to open the wordlist file.");
    fseek(wordlistFilePointer, 0, SEEK_END);
    failCond(ftell(wordlistFilePointer) >= MAX_WORDLIST_FILE_BUFFER_SIZE,
             "Given wordlist file is too big");
    rewind(wordlistFilePointer);

    
    failCond(strlen(argv[HASHED_CSV_FILE_PATH_ARG_INDEX]) >= LINUX_MAX_PATH_SIZE,
             "CSV path argument is too big.");
    failCond(!isValidFilePath(argv[HASHED_CSV_FILE_PATH_ARG_INDEX],
                              strlen(argv[HASHED_CSV_FILE_PATH_ARG_INDEX])),
             "Bad naming for the path of csv file.");
    FILE* hashedCsvFilePointer = fopen(argv[HASHED_CSV_FILE_PATH_ARG_INDEX], "r");
    failCond(hashedCsvFilePointer == NULL,"Failed to open the csv file.");
    char* userHashedCsvBuffer = (char*)malloc(MAX_CSV_BUFFER_SIZE*(sizeof(char)));
    fseek(hashedCsvFilePointer, 0, SEEK_END);
    failCond(ftell(hashedCsvFilePointer) >= MAX_CSV_BUFFER_SIZE,
             "Given CSV file is too big.");
    rewind(hashedCsvFilePointer);
    const MAX_CSV_BUFFER_SIZE_ITER_TYPE userHashedCsvBufferSize = fread(
        userHashedCsvBuffer, sizeof(char), MAX_CSV_BUFFER_SIZE, hashedCsvFilePointer);    
    fclose(hashedCsvFilePointer);

    CsvMap csvMap;
    const exitCode CsvMapConstructorErr = CsvMapConstructor(
        &csvMap, userHashedCsvBuffer, userHashedCsvBufferSize);
    free(userHashedCsvBuffer);
    failCond(CsvMapConstructorErr,CsvMapConstructorErrorMessages[CsvMapConstructorErr]);

    bool8 wordlistBasenameInHashedCsvHeader = 0;
    for(MAX_CSV_WIDTH_ITER_TYPE i=0; i<csvMap.widestRowSize; i++){
        if(strncmp(
               &csvMap.csvBuffer[csvMap.cells[0][i].cellStringStartPosition],
               basename(argv[WORDLIST_PATH_ARG_INDEX]),
               csvMap.cells[0][i].cellStringEndPosition
               - csvMap.cells[0][i].cellStringStartPosition) == 0){
            wordlistBasenameInHashedCsvHeader = 1;
            args->hashedCsvColumnIndex = i;
        }
    }
    failCond(wordlistBasenameInHashedCsvHeader == 0,
             "Have not found compatible hashed column.");

    assignHashedCsvFilePathArgument(argc, argv, args);
    assignWordlistPathArgument(argc, argv, args);    
    handleUserSaltPasswordArgument(argc, argv, args);
    assignHashedCsvColumnHeaderNameAttribute(argc, argv, args);
    assignDehashedCsvColumnHeaderNameAttribute(argc, argv, args);
    assignTimestampStringAttribute(argc, argv, args);
    assignSaltAttribute(argc, argv, args);
    handleOptionalArguments(argc, argv, args);
        
    CsvMapDestructor(&csvMap);
    return 0;
}
exitCode UserInputDestructor(UserInput* args){
    free(args->hashedCsvFilePath);         args->hashedCsvFilePathSize=0;
    free(args->wordlistPath);              args->wordlistPathSize=0;
    free(args->userSaltPassword);          args->userSaltPasswordSize=0;

    args->hashedCsvColumnIndex=0;
    free(args->hashedCsvColumnHeaderName); args->hashedCsvColumnHeaderNameSize=0;
    free(args->timestampString);           args->timestampStringLen=0;
    free(args->salt);                      args->saltLength=0;
    args->viewModeActive=0;
    return 0;
}
exitCode printArguments(const UserInput* args){
    
    printf("Required args: %.*s\t", args->hashedCsvFilePathSize, args->hashedCsvFilePath);
    printf("%.*s\t", args->wordlistPathSize, args->wordlistPath);
    printf("%.*s\n", args->userSaltPasswordSize, args->userSaltPassword);

    printf("Derivatives: %d\t", args->hashedCsvColumnIndex);
    printf("%.*s\t", args->hashedCsvColumnHeaderNameSize, args->hashedCsvColumnHeaderName);
    printf("%.*s\t", args->dehashedCsvColumnHeaderNameSize, args->dehashedCsvColumnHeaderName);
    printf("%.*s\t", args->timestampStringLen, args->timestampString);
    printf("%.*s\t", args->saltLength, args->salt);
    printf("%d\n", args->viewModeActive);
    return 0;
}

