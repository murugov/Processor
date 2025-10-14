#include <stdio.h> // добавить в makefile

void generator()
{
    
}


int main()
{
    FILE* EnumsFile = fopen("files/CmdCodesEnums.h", "w");
    FILE* AsmInstrSetFile = fopen("files/AsmInstrSet", "w");
    FILE* SpuInstrSetFile = fopen("files/SpuInstrSet", "w");
    return 0;
}