#include "compile.h"
#include "colors.h"

void AsmErrPrint(AsmErr_t verd)
{
    switch (verd)
    {
        case BAD_INPUT_FILE_PTR:
            perror(ANSI_COLOR_RED "Bad input_file's pointer!" ANSI_COLOR_RESET);
            break;

        case BAD_OUTPUT_FILE_PTR:
            perror(ANSI_COLOR_RED "Bad output_file's pointer!" ANSI_COLOR_RESET);
            break;

        case WRONG_FILE_INFO:
            perror(ANSI_COLOR_RED "Error getting file information!\n" ANSI_COLOR_RESET);
            break;

        case BAD_BUFFER_PTR:
            perror(ANSI_COLOR_RED "Bad buffer pointer!\n" ANSI_COLOR_RESET);
            break;

        case BAD_ARR_PTR:
            perror(ANSI_COLOR_RED "Bad pointer to array of pointers!\n" ANSI_COLOR_RESET);
            break;

        case CMD_NUM_FAIL:
            perror(ANSI_COLOR_RED "Incorrect number of commands!\n" ANSI_COLOR_RESET);
            break;

        case BAD_ARR_CMDS_PTR:
            perror(ANSI_COLOR_RED "Bad pointer to array of commands!" ANSI_COLOR_RESET);

        case UNKNOWN_CMD:
            break;

        case SUCCESS:
            printf(ANSI_COLOR_GREEN "SUCSESS\n" ANSI_COLOR_RESET);
            break;

        case ERROR:
            break;

        default:
            break;
    }
}