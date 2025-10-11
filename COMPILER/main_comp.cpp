#include "compile.h"

#define SOURCE "./CompileFiles/source.asm"
#define BYTE_CODE "./CompileFiles/bytecode.asm" // спрятать в макросе для Compiler

//PUSH 80
//PUSH 70
//JMP metka


//metka:
//PUSH

//tokenizations

int main() // argc argv
{
    FILE *SourceFile = fopen(SOURCE, "rb");
    FILE *ByteCode = fopen(BYTE_CODE, "wb");

    AsmErr_t compile_verd = Compiler(SourceFile, ByteCode);

    AsmErrPrint(compile_verd);

    fclose(SourceFile);
    fclose(ByteCode);

    return 0;
}