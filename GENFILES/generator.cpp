#include <stdio.h> // добавить в makefile

void EnGenTor(FILE *EnumsFile)
{
    fprinf();
}


int main()
{
    FILE *EnumsFile = fopen("files/CmdCodesEnums.h", "w");
    FILE *AsmInstrSetFile = fopen("files/AsmInstrSet.cpp", "w");
    FILE *SpuInstrSetFile = fopen("files/SpuInstrSet.cpp", "w");

    

    EnGenTor(EnumsFile);
    
    return 0;
}