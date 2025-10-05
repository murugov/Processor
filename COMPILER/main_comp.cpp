#include "compile.h"

#define SOURCE "./CompileFiles/source.asm"
#define BYTE_CODE "./CompileFiles/bytecode.asm"

int main()
{
    FILE *SourceFile = fopen(SOURCE, "rb");
    FILE *ByteCode = fopen(BYTE_CODE, "wb");

    AsmErr_t compile_verd = Compiler(SourceFile, ByteCode);

    AsmErrPrint(compile_verd);

    fclose(SourceFile);
    fclose(ByteCode);

    return 0;
}