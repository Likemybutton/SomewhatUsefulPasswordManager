enum helpArguments{
BINARY_NAME_HELP_CASE,
HELP_FLAG_ARG_INDEX,
NUMBER_OF_REQUIRED_ARGUMENTS_HELP_CASE };
enum requiredArguments{
BINARY_NAME, CSV_FILE_ARG_INDEX,
CSV_COLUMN_ARG_INDEX, USER_SALT_ARG_INDEX,
NUMBER_OF_REQUIRED_ARGUMENTS};
enum optionalArguments{
WORDLIST_SIZE_FLAG_ARG_INDEX, WORDLIST_SIZE_ARGUMENT_ARG_INDEX,
WORDLIST_PATH_FLAG_ARG_INDEX, WORDLIST_PATH_ARGUMENT_ARG_INDEX,
NUMBER_OF_OPTIONAL_ARGUMENTS};
#define MAX_ARGUMENTS_NUMBER NUMBER_OF_REQUIRED_ARGUMENTS + NUMBER_OF_OPTIONAL_ARGUMENTS
#define MAX_ARGUMENTS_NUMBER_ITER_TYPE u8
typedef struct{
    char* csvFilePath; LINUX_MAX_PATH_SIZE_ITER_TYPE csvFilePathSize;
    char* userCsvColumnName; MAX_CSV_CELL_CONTENT_SIZE_ITER_TYPE userCsvColumnNameSize;
    MAX_CSV_WIDTH_ITER_TYPE userCsvColumnIndex; 
    char* userSaltPassword; MAX_USER_SALT_LENGTH_ITER_TYPE userSaltPasswordSize;
    MAX_WORDLIST_SIZE_ITER_TYPE wordlistSize; 
    char* wordlistPath; LINUX_MAX_PATH_SIZE_ITER_TYPE wordlistPathSize;
    char* timestampString; TIME_T_CONVERTED_TO_STRING_POSSIBLE_BUFFER_SIZE_ITER_TYPE timestampStringLen;
    char* salt; SALT_STRING_SIZE_ITER_TYPE saltLength;
    char* openFileSignature; OPEN_FILE_SIGNATURE_STRING_SIZE_ITER_TYPE openFileSignatureLength;
    char* wordlistFilePath; LINUX_MAX_PATH_SIZE_ITER_TYPE wordlistFilePathSize;
}UserInput;
exitCode printHelpMessage(){
    printf("%s\n", "Hasher for hashing of .csv columns. Hashes the column with \
SHA1 algorithm and produces special obfusing wordlist. For proper security: the .csv file, \
wordlist and binaries/source should never be placed in the same physical device!");
    printf("\nUsage: ./hasher [Path to a .csv file] [Name of a .csv column] [Salt-password] \n");
    printf("\nOptional arguments:");
    printf("\n\t%s\n", "-s, --wordlist_size [Number of lines in the produced wordlist]");
    printf("\n\t%s\n", "-w, --wordlist_path [Path where wordlist should go]");
    printf("\n");
    return 0;
}
exitCode assignCsvFilePathArgument(const int argc, char** argv, UserInput* args){
    args->csvFilePath = (char*)malloc(LINUX_MAX_PATH_SIZE*sizeof(char));
    args->csvFilePathSize = strlen(argv[CSV_FILE_ARG_INDEX]);
    strncpy(args->csvFilePath, argv[CSV_FILE_ARG_INDEX], args->csvFilePathSize);
    return 0;
}
exitCode handleColumnNameArgument(const int argc, char** argv,
                                  const CsvMap csvMap, UserInput* args){
    failCond(strlen(argv[CSV_COLUMN_ARG_INDEX]) >= MAX_CSV_CELL_CONTENT_SIZE,
             "CSV column argument is too big.");
    failCond(getStringInCsvHeader(&(args->userCsvColumnIndex), &csvMap,
                                  argv[CSV_COLUMN_ARG_INDEX],
                                  strlen(argv[CSV_COLUMN_ARG_INDEX]))
             == HEADER_NOT_FOUND_GET_STRING_IN_CSV_HEADER,
             "Could not find the given name of a CSV column in header.");
    args->userCsvColumnName = (char*)malloc(MAX_CSV_CELL_CONTENT_SIZE*sizeof(char));
    args->userCsvColumnNameSize = strlen(argv[CSV_COLUMN_ARG_INDEX]);
    strncpy(args->userCsvColumnName, argv[CSV_COLUMN_ARG_INDEX], args->userCsvColumnNameSize);
    return 0;
}
exitCode handleUserSaltPasswordArgument(const int argc, char** argv, UserInput* args){
    failCond(strlen(argv[USER_SALT_ARG_INDEX]) >= MAX_USER_SALT_LENGTH,
             "Salt password argument is too big.");

    failCond(strlen(argv[USER_SALT_ARG_INDEX]) < 5,
             "User salt password should have at least 5 characters!");
    failCond(!hasUpper(argv[USER_SALT_ARG_INDEX], strlen(argv[USER_SALT_ARG_INDEX])),
             "User salt password should have at least 1 upper character!");
    failCond(!hasDigit(argv[USER_SALT_ARG_INDEX], strlen(argv[USER_SALT_ARG_INDEX])),
             "User salt password should have at least 1 digit!");
    failCond(!hasLower(argv[USER_SALT_ARG_INDEX], strlen(argv[USER_SALT_ARG_INDEX])),
             "User salt password should have at least 1 lower character!");
    failCond(!hasSpecial(argv[USER_SALT_ARG_INDEX], strlen(argv[USER_SALT_ARG_INDEX])),
             "User salt password should have at least 1 special character!");

    args->userSaltPassword = (char*)malloc(MAX_USER_SALT_LENGTH*sizeof(char));
    args->userSaltPasswordSize = strlen(argv[USER_SALT_ARG_INDEX]);
    strncpy(args->userSaltPassword, argv[USER_SALT_ARG_INDEX], args->userSaltPasswordSize);

    return 0;
}
exitCode assignTimestampStringAttribute(const int argc, char** argv, UserInput* args, const time_t timestamp){
    args->timestampString = (char*)malloc(TIME_T_CONVERTED_TO_STRING_POSSIBLE_BUFFER_SIZE*sizeof(char));
    sprintf(args->timestampString, "%ld", timestamp);
    args->timestampStringLen = strlen(args->timestampString);
    return 0;
}
exitCode assignSaltAttribute(const int argc, char** argv, UserInput* args){
    args->salt = (char*)malloc(SALT_STRING_SIZE*sizeof(char));
    sprintf(args->salt, "%s%s%s", args->timestampString, OPEN_FILE_SIGNATURE_DELIMETER, args->userSaltPassword);
    args->saltLength = strlen(args->salt);
    return 0;
}
exitCode assignOpenFileSignatureAttribute(const int argc, char** argv, UserInput* args){
    failCond(args->timestampStringLen
             + strlen(OPEN_FILE_SIGNATURE_DELIMETER)
             + args->userCsvColumnNameSize >= OPEN_FILE_SIGNATURE_STRING_SIZE,
             "Open file signature is too big. Column name for hashing is too big!");
    args->openFileSignature = (char*)malloc(OPEN_FILE_SIGNATURE_STRING_SIZE*sizeof(char));
    sprintf(args->openFileSignature, "%s%s%d%s%s",
            args->timestampString,
            OPEN_FILE_SIGNATURE_DELIMETER,
            args->userCsvColumnIndex,
            OPEN_FILE_SIGNATURE_DELIMETER,
            args->userCsvColumnName);
    args->openFileSignatureLength = strlen(args->openFileSignature);
    return 0;
}
exitCode handleOptionalArguments(const int argc, char** argv,
                                 const CsvMap csvMap, UserInput* args){
    args->wordlistSize = DEFAULT_WORDLIST_SIZE;
    args->wordlistPath = (char*)malloc(LINUX_MAX_PATH_SIZE*sizeof(char));
    args->wordlistPathSize = strlen(DEFAULT_WORDLIST_PATH);
    strncpy(args->wordlistPath, DEFAULT_WORDLIST_PATH, args->wordlistPathSize);
    bool8 wordlist_pathOptionNotPresent = 1;
    if(argc > NUMBER_OF_REQUIRED_ARGUMENTS + 1){
        for(MAX_ARGUMENTS_NUMBER_ITER_TYPE i=NUMBER_OF_REQUIRED_ARGUMENTS;
            i<argc; i++){
            if(strcmp(argv[i],"-s") == 0 || strcmp(argv[i],"--wordlist_size") == 0){
                failCond(!atoi(argv[i+1]),
                         "--wordlist_size option or -s flag must be an integer!");
                failCond(atoi(argv[i+1])>MAX_WORDLIST_SIZE,
                         "Upper limit for the --wordlist_size is MAX_WORDLIST_SIZE (call --help for the settings)!");
                failCond(atoi(argv[i+1])<csvMap.rowsNumber,
                         "Lower limit for the --wordlist_size is the number of entries in the csv file!");
                args->wordlistSize = atoi(argv[i+1]);
            }
            if(strcmp(argv[i],"-w") == 0 || strcmp(argv[i],"--wordlist_path") == 0){
                wordlist_pathOptionNotPresent = 0;
                failCond(strlen(argv[i+1]) >= LINUX_MAX_PATH_SIZE,
                         "Wordlist file path argument is too big");
                if(isFileOrDirExists(argv[i+1], strlen(argv[i+1])) == DIRECTORY_DOES_NOT_EXIST){
                    const mode_t permissionsMode = 0777;
                    failCond(mkdir(argv[i+1],0) != 0,
                             "Failed to create the directory under the --wordlist_path option!");
                    chmod(argv[i+1], permissionsMode);
                }
                failCond(!(isFileOrDirExists(argv[i+1], strlen(argv[i+1])) == DIRECTORY_EXISTS),
                         "Path under --wordlist_path does not exist, probably a file given!");
                args->wordlistPath = (char*)malloc(LINUX_MAX_PATH_SIZE*sizeof(char));
                args->wordlistPathSize = strlen(argv[i+1]);
                strncpy(args->wordlistPath, argv[i+1], args->wordlistPathSize);
   
                args->wordlistFilePath = (char*)malloc(LINUX_MAX_PATH_SIZE*sizeof(char));
                failCond(args->wordlistPathSize + args->openFileSignatureLength >= LINUX_MAX_PATH_SIZE,
                         "Wordlist file path argument + Column name string size is too big");
                sprintf(args->wordlistFilePath, "%s%s%s", args->wordlistPath, args->openFileSignature,
                        WORDLIST_FILE_EXTENSION);
                args->wordlistFilePathSize = strlen(args->wordlistFilePath);
                failCond(isFileOrDirExists(args->wordlistFilePath, strlen(args->wordlistFilePath)) == FILE_EXISTS,
                         "Wordlist path already exists, abort to avoid overwriting!");
            }
        }
    }
    if(wordlist_pathOptionNotPresent){
        const char* defaultWordlistPath = DEFAULT_WORDLIST_PATH;
        args->wordlistFilePath = (char*)malloc(LINUX_MAX_PATH_SIZE*sizeof(char));
        failCond(strlen(defaultWordlistPath) + args->openFileSignatureLength >= LINUX_MAX_PATH_SIZE,
                 "Wordlist file path argument + Column name string size is too big");
        sprintf(args->wordlistFilePath, "%s%s%s", defaultWordlistPath, args->openFileSignature,
                WORDLIST_FILE_EXTENSION);
        args->wordlistFilePathSize = strlen(args->wordlistFilePath);
        failCond(isFileOrDirExists(args->wordlistFilePath, strlen(args->wordlistFilePath)) == FILE_EXISTS,
                 "Wordlist path already exists, abort to avoid overwriting!");
    }
    return 0;
}
exitCode UserInputValidatingConstructor(const int argc, char** argv,
                                        UserInput* args, const time_t timestamp){
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

    failCond(strlen(argv[CSV_FILE_ARG_INDEX]) >= LINUX_MAX_PATH_SIZE,
             "CSV path argument is too big.");
    failCond(!isValidFilePath(argv[CSV_FILE_ARG_INDEX], strlen(argv[CSV_FILE_ARG_INDEX])),
             "Bad naming for the path of csv file.");

    FILE* csvFilePointer = fopen(argv[CSV_FILE_ARG_INDEX], "r");
    failCond(csvFilePointer == NULL,"Failed to open the csv file.");
    char* userCsvBuffer = (char*)malloc(MAX_CSV_BUFFER_SIZE*(sizeof(char)));
    fseek(csvFilePointer, 0, SEEK_END);
    failCond(ftell(csvFilePointer) >= MAX_CSV_BUFFER_SIZE,
             "Given CSV file is too big.");
    rewind(csvFilePointer);
    const MAX_CSV_BUFFER_SIZE_ITER_TYPE userCsvBufferSize = fread(userCsvBuffer, sizeof(char),
                                                                  MAX_CSV_BUFFER_SIZE,
                                                                  csvFilePointer);
    fclose(csvFilePointer);
    CsvMap csvMap;
    const exitCode CsvMapConstructorErr = CsvMapConstructor(&csvMap, userCsvBuffer, userCsvBufferSize);
    free(userCsvBuffer);
    failCond(CsvMapConstructorErr,CsvMapConstructorErrorMessages[CsvMapConstructorErr]);
    
    assignCsvFilePathArgument(argc, argv, args);
    handleColumnNameArgument(argc, argv, csvMap, args);

    for(MAX_CSV_LENGTH_ITER_TYPE i=0; csvMap.cells[i][args->userCsvColumnIndex].notEmpty == TRUE; i++){
        failCond(
            areNullsInTheString(
                &(csvMap.csvBuffer[csvMap.cells[i][args->userCsvColumnIndex].cellStringStartPosition]),
                csvMap.cells[i][args->userCsvColumnIndex].cellStringEndPosition
                - csvMap.cells[i][args->userCsvColumnIndex].cellStringStartPosition),
            "Column for hashing contains NUll characters!");
    }
    
    handleUserSaltPasswordArgument(argc, argv, args);
    
    assignTimestampStringAttribute(argc, argv, args, timestamp);
    assignSaltAttribute(argc, argv, args);
    assignOpenFileSignatureAttribute(argc, argv, args);

    handleOptionalArguments(argc, argv, csvMap, args);

    CsvMapDestructor(&csvMap);
    return 0;
}
exitCode UserInputDestructor(UserInput* args){
    free(args->csvFilePath);       args->csvFilePathSize = 0;
    free(args->userCsvColumnName); args->userCsvColumnNameSize = 0;
    args->userCsvColumnIndex = 0;
    free(args->userSaltPassword);  args->userSaltPasswordSize = 0;
    args->wordlistSize = 0;
    free(args->wordlistPath);      args->wordlistPathSize = 0;
    free(args->timestampString);   args->timestampStringLen = 0;
    free(args->salt);              args->saltLength = 0;
    free(args->openFileSignature); args->openFileSignatureLength = 0;
    free(args->wordlistFilePath);  args->wordlistFilePathSize = 0;
    return 0;
}
exitCode printArguments(const UserInput* args){
    printf("Required args: %.*s\t", args->csvFilePathSize, args->csvFilePath);
    printf("%.*s\t", args->userCsvColumnNameSize, args->userCsvColumnName);
    printf("index: %d\t", args->userCsvColumnIndex);
    printf("%.*s\n", args->userSaltPasswordSize, args->userSaltPassword);

    printf("Optional arguments: %d\t", args->wordlistSize);
    printf("%.*s\n", args->wordlistPathSize, args->wordlistPath);

    printf("Derivatives: timestamp - %.*s\n", args->timestampStringLen,args->timestampString);
    printf("salt - %.*s\n", args->saltLength, args->salt);
    printf("openFileSignature - %.*s\n", args->openFileSignatureLength, args->openFileSignature);
    printf("wordlistFilePath - %.*s\n", args->wordlistFilePathSize, args->wordlistFilePath);
    return 0;
}
