#include "spu.h"

FILE *LogFile = fopen(LOG_FILE, "wb");


int main()
{
    FILE *ByteCode = fopen(BYTE_CODE, "rb");

    spu_t spu = {};

    spuErr_t ctor_verd = spuCtor(&spu, ByteCode);
    if(ctor_verd)
        spuErrPrint(ctor_verd);

    spuErr_t execut_verd = spuExecutor(&spu);
    if(execut_verd)
        spuErrPrint(execut_verd);

    spuDtor(&spu);
    fclose(ByteCode);
    fclose(LogFile);

    return 0;
}