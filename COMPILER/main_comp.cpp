#include "compile.h"
#include "colors.h"

//tokenizations

int main(int argc, char *argv[])
{
    if (argc >= 3)
    {
        FILE *SourceFile = fopen(argv[1], "rb");
        FILE *ByteCode = fopen(argv[2], "wb");
    
        size_t count_line = 0;
        char **arr_ptr = ArrPtrCtor(SourceFile, &count_line);
    
        AsmErr_t asm_verd = Assembler(ByteCode, arr_ptr, count_line);
        AsmErrPrint(SourceFile, ByteCode, asm_verd);
    
        free(arr_ptr);
        fclose(SourceFile);
        fclose(ByteCode);
    }
    else
        printf(ANSI_COLOR_RED "Incorrect transfer of input files\n" ANSI_COLOR_RESET);

    return 0;
}