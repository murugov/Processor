#include "compile.h"
#include "colors.h"

//tokenizations

int main(int argc, char *argv[])
{
    if (argc >= 3)
    {
        FILE *SourceFile = fopen(argv[1], "rb");
        FILE *ByteCode = fopen(argv[2], "wb");

        if (IS_BAD_PTR(SourceFile))
        {
            perror(ANSI_COLOR_RED "Bad pointer SourceFile" ANSI_COLOR_RESET);
            return 1;
        }
        if (IS_BAD_PTR(ByteCode))
        {
            perror(ANSI_COLOR_RED "Bad pointer ByteCode" ANSI_COLOR_RESET);
            return 1;
        }

        if (VerifyAsmInstrSetSort() != SUCCESS)
        {
            printf(ANSI_COLOR_RED "Not sorted asm_instr_set!\n" ANSI_COLOR_RESET);
            printf(ANSI_COLOR_RED "TODO: generation AsmInstrSet.cpp with using \"make run-gen\"!\n" ANSI_COLOR_RESET);
            return 1;
        }
    
        char *buffer = NULL;
        size_t count_line = 0;
        char **arr_ptr = ArrPtrCtor(SourceFile, buffer, &count_line);
    
        AsmErr_t asm_verd = CodeCtor(ByteCode, arr_ptr, count_line);
        AsmErrPrint(argv[1], argv[2], asm_verd);
    
        AsmDtor(buffer, arr_ptr);

        fclose(SourceFile);
        fclose(ByteCode);
    }
    else
        printf(ANSI_COLOR_RED "Incorrect transfer of input files\n" ANSI_COLOR_RESET);

    return 0;
}