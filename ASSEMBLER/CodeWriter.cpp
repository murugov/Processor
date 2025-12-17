#include "assembler.hpp"


AsmErr_t CodeWriter(FILE *ByteCode, line_with_num_t *lines, size_t count_lines)
{    
    ON_DEBUG( if (IS_BAD_PTR(ByteCode)) return ASM_BAD_OUTPUT_FILE_PTR; )
    ON_DEBUG( if (IS_BAD_PTR(lines)) return ASM_BAD_ARR_CMD_PTR; )
        
    byte_t *code = (byte_t*)calloc(count_lines * (1 + sizeof(arg_t)) + 1, sizeof(byte_t));
    if (IS_BAD_PTR(code)) return ASM_BAD_CODE_PTR;
    
    size_t count_labels = 0;
    label_t *arr_labels = (label_t*)calloc(count_lines, sizeof(label_t));
    if (IS_BAD_PTR(arr_labels)) { free(code); return ASM_BAD_ARR_LABELS_PTR;}

    ht_t<label_t*> *ht_labels = (ht_t<label_t*>*)calloc(1, sizeof(ht_t<label_t*>));
    if (IS_BAD_PTR(ht_labels)) { free(code); free(arr_labels); return ASM_BAD_HT_LABELS_PTR; }
    HT_CTOR(ht_labels);

    label_context lbl_ctx = {arr_labels, count_labels, ht_labels};

    size_t pc = 0;
    
    AsmErr_t first_pass = Compilation(code, lines, count_lines, &lbl_ctx, &pc);
    if (first_pass != ASM_SUCCESS) { free(code); free(arr_labels); return first_pass; }

    // if (count_labels > 0)
    // {
    //     AsmErr_t second_pass = Compilation(code, lines, count_lines, &lbl_ctx, &pc);
    //     if (second_pass != ASM_SUCCESS) { free(code); free(arr_labels); return second_pass; }
    // }

    sign_t signature = {SIGNATURE, VERSION};
    fwrite(&signature, sizeof(sign_t), 1, ByteCode); 
    fwrite(code, sizeof(byte_t), pc, ByteCode);

    HT_DTOR(ht_labels);
    free(ht_labels);
    free(code);
    free(arr_labels);
        
    return ASM_SUCCESS;
}