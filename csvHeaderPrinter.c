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
/* #define CSV_NEWLINE_DELIMETER_STRING "\n" */
/* #define CSV_END_OF_FILE '\0' */

/* #define CSV_HEADER_ROW_POSITION 0 */
/* #define CSV_FIRST_DATA_ROW_POSITION 1 + CSV_HEADER_ROW_POSITION */

#include "./settings.h"
#define SYSTEM_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION
#define CSV_UTILITY_FUNCTIONS_MY_SFH_IMPLEMENTATION
#include "./my_sfh.h"

enum helpArguments{
BINARY_NAME_HELP_CASE,
HELP_FLAG_ARG_INDEX,
NUMBER_OF_REQUIRED_ARGUMENTS_HELP_CASE };
enum requiredArguments{
BINARY_NAME,
CSV_FILE_ARG_INDEX,
NUMBER_OF_REQUIRED_ARGUMENTS };
typedef struct{
    char* csvFilePath; LINUX_MAX_PATH_SIZE_ITER_TYPE csvFilePathSize;
}UserInput;

exitCode printHelpMessage(){
    printf("%s\n", "Prints header of a csv file.");
    printf("\nUsage: ./csvHeaderPrinter [Path to a .csv file] \n");
    printf("\n");
    return 0;
}
exitCode assignCsvFilePathArgument(const int argc, char** argv, UserInput* args){
    args->csvFilePath = (char*)malloc(LINUX_MAX_PATH_SIZE*sizeof(char));
    args->csvFilePathSize = strlen(argv[CSV_FILE_ARG_INDEX]);
    strncpy(args->csvFilePath, argv[CSV_FILE_ARG_INDEX], args->csvFilePathSize);
    return 0;
}
exitCode UserInputValidatingConstructor(
    const int argc, char** argv, UserInput* args){
    failCond(argc < NUMBER_OF_REQUIRED_ARGUMENTS_HELP_CASE,
             "No arguments, see --help for help!");
    if(strcmp(argv[HELP_FLAG_ARG_INDEX], "-h") == 0 ||
       strcmp(argv[HELP_FLAG_ARG_INDEX], "--help") == 0){
        printHelpMessage();
        exit(0);
    }

    failCond(!(whichOS() == LINUX || whichOS() == UNIX),
             "Hasher is Linux only, I don't know how to write cross-platform C. Bleh...");
    failCond(argc < NUMBER_OF_REQUIRED_ARGUMENTS,
             "Give required arguments fool!");

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
    const MAX_CSV_BUFFER_SIZE_ITER_TYPE userCsvBufferSize = fread(
        userCsvBuffer, sizeof(char), MAX_CSV_BUFFER_SIZE, csvFilePointer);
    char* headerToken = strtok(userCsvBuffer, CSV_NEWLINE_DELIMETER_STRING);
    char* userCsvHeader = (char*)malloc(MAX_CSV_BUFFER_SIZE*(sizeof(char)));
    strcpy(userCsvHeader, headerToken);
    fclose(csvFilePointer);
    
    CsvMap csvMap;
    const exitCode CsvMapConstructorErr = CsvMapConstructor(
        &csvMap, userCsvHeader, strlen(userCsvHeader));
    free(userCsvBuffer);
    free(userCsvHeader);
    failCond(CsvMapConstructorErr,
             CsvMapConstructorErrorMessages[CsvMapConstructorErr]);
    
    assignCsvFilePathArgument(argc, argv, args);

    CsvMapDestructor(&csvMap);
    return 0;
}
int main (const int argc, char** argv) {
    UserInput args;
    UserInputValidatingConstructor(argc, argv, &args);

    FILE* csvFilePointer = fopen(args.csvFilePath, "r");
    failCond(csvFilePointer == NULL, "Failed to open the csv file.");
    char* userCsvBuffer = (char *)malloc(MAX_CSV_BUFFER_SIZE * (sizeof(char)));
    const MAX_CSV_BUFFER_SIZE_ITER_TYPE userCsvBufferSize = fread(
        userCsvBuffer, sizeof(char), MAX_CSV_BUFFER_SIZE, csvFilePointer);
    char* headerToken = strtok(userCsvBuffer, "\n");
    char* userCsvHeader = (char*)malloc(MAX_CSV_BUFFER_SIZE*(sizeof(char)));
    strcpy(userCsvHeader, headerToken);
    fclose(csvFilePointer);

    CsvMap csvMap;
    const exitCode CsvMapConstructorErr = CsvMapConstructor(
        &csvMap, userCsvHeader, strlen(userCsvHeader));
    free(userCsvBuffer);
    free(userCsvHeader);
    failCond(CsvMapConstructorErr,
             CsvMapConstructorErrorMessages[CsvMapConstructorErr]);

    printCsvMap(&csvMap);

    return 0;
}
