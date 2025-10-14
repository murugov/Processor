#include "compile.h"

#define SOURCE "./CompileFiles/source.asm"
#define BYTE_CODE "./CompileFiles/bytecode.asm" // спрятать в макросе


//tokenizations

int main() // argc argv
{
    FILE *SourceFile = fopen(SOURCE, "rb");
    FILE *ByteCode = fopen(BYTE_CODE, "wb");

    size_t count_line = 0;
    char **arr_ptr = ArrPtrCtor(SourceFile, &count_line);

    AsmErr_t asm_verd = Assembler(arr_ptr, count_line, ByteCode);
    AsmErrPrint(asm_verd);

    free(arr_ptr);
    fclose(SourceFile);
    fclose(ByteCode);

    return 0;
}