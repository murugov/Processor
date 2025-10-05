#include "proc.h"
#include "colors.h"

void CalcErrPrint(ProcErr_t verd)
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

        case BUFFER_FAIL:
            perror(ANSI_COLOR_RED "Error creating buffer!\n" ANSI_COLOR_RESET);
            break;

        case BAD_ARR_PTR:
            perror(ANSI_COLOR_RED "Bad arr_ptr's pointer!\n" ANSI_COLOR_RESET);
            break;

        case UNKNOWN_CMD:
            perror(ANSI_COLOR_RED "Unknown command in source.asm!\n" ANSI_COLOR_RESET); // return line
            break;

        case WRONG_STK:
            printf(ANSI_COLOR_RED "Error occurred while creating the stack\n" ANSI_COLOR_RESET);
            break;

        case SIGNVERSVER:
            printf(ANSI_COLOR_RED "The signature or version of the source file does not match!\n" ANSI_COLOR_RESET);
            break;     

        case DIV_BY_ZERO:
            printf(ANSI_COLOR_RED "Division by zero!\n" ANSI_COLOR_RESET);
            break;   
            
        case STOP_BY_HLT:
            printf(ANSI_COLOR_GREEN "Stop by HLT: SUCCESS\n" ANSI_COLOR_RESET);
            break;

        case TASK_SUCCESS:
            printf(ANSI_COLOR_GREEN "SUCSESS\n" ANSI_COLOR_RESET);
            break;

        case TASK_ERROR:
            break;

        default:
            break;
    }
}