#include "stack.h"
#include "proc.h"

#define BYTE_CODE "./CompileFiles/bytecode.asm"
FILE *LogFile = fopen("./ReportFiles/LogFile.log", "wb");

int main()
{
    FILE *ByteCode = fopen(BYTE_CODE, "rb");

    ProcErr_t calc_verd = Calculator(ByteCode);

    CalcErrPrint(calc_verd);

    fclose(ByteCode);
    fclose(LogFile);

    return 0;
}