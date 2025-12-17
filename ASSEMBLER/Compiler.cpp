#include "assembler.hpp"
#include "AsmInstrSet.cpp"


AsmErr_t Compilation(byte_t *code, line_with_num_t *lines, size_t count_lines, label_context *lbl_ctx, size_t *pc)
{
    ON_DEBUG( if (IS_BAD_PTR(code) || IS_BAD_PTR(lines) || IS_BAD_PTR(lbl_ctx) || IS_BAD_PTR(pc)) { return ASM_ERROR; } )

    (*pc) = 0;
    bool is_first_pass = !(bool)(lbl_ctx->count_labels);
    asm_context write_params = {code, NULL, MASK_EMP, *pc, asm_instr_set[0].cmd, lbl_ctx, is_first_pass};

    for (size_t line = 0; line < count_lines; ++line)
    {
        if (strchr(lines[line].ptr, ':') != NULL)
        {
            if (is_first_pass)
            {
                if (strlen(lines[line].ptr) >= MAX_LEN_CMD + MAX_LEN_LABEL) { return ASM_LINE_SIZE_EXCEED; }
    
                AsmErr_t add_label_verd = AddLabel(&(lines[line]), lbl_ctx, write_params.pc);
                if (add_label_verd != ASM_SUCCESS) { return add_label_verd; }
            }
        }
        // else
        // {
        //     hash_t hash_func = GetHash(lines[line].ptr);

        //     size_t *index = SearchCmdByHash(hash_func);
        //     if(IS_BAD_PTR(index)) { return ASM_UNKNOWN_CMD; }
            
        //     write_params.ptr = lines[line].ptr;
        //     write_params.cmd = asm_instr_set[index].cmd;
        //     write_params.mask = asm_instr_set[index].mask;
            
        //     AsmErr_t write_verd = (asm_instr_set[index].func)(&write_params);
        //     if (write_verd != ASM_SUCCESS) { return write_verd; }
        // }

    }

    code = write_params.code;
    *pc = write_params.pc;
    lbl_ctx->arr_lables = write_params.lbl_ctx->arr_lables;
    lbl_ctx->count_labels = write_params.lbl_ctx->count_labels;

    return ASM_SUCCESS;
}


AsmErr_t AddLabel(line_with_num_t *line, label_context *lbl_ctx, size_t pc)
{
    ON_DEBUG( if (IS_BAD_PTR(line) || IS_BAD_PTR(lbl_ctx)) { return ASM_ERROR; } )

    label_t *new_label = (label_t*)calloc(1, sizeof(label_t));
    if (IS_BAD_PTR(new_label)) { return ASM_ERROR; }

    new_label->name = line->ptr;
    new_label->pc   = pc;

    if (htInsert(lbl_ctx->ht_labels, new_label->name, LabelComparator) != HT_SUCCESS) { return ASM_RE_LABEL; }

    lbl_ctx->arr_lables[lbl_ctx->count_labels].name = line->ptr;
    lbl_ctx->arr_lables[lbl_ctx->count_labels].pc   = pc + 3;

    (lbl_ctx->count_labels)++;

    return ASM_SUCCESS;
}


AsmErr_t FindLabel(const char *label_name, ht_t<label_t*> *ht_labels, size_t *pc)
{
    return ASM_SUCCESS;
}


int LabelComparator(const void *ht_elem, const char *item)
{
    const char *str_in_table = ((label_t*)ht_elem)->name;
    
    if (str_in_table == NULL && item == NULL) return 1;
    if (str_in_table == NULL || item == NULL) return 0;
    
    return strcmp(str_in_table, item) == 0;
}


size_t SearchCmdByHash(hash_t hash_cmd)
{
    wrap_t *found = (wrap_t*)bsearch(&hash_cmd, asm_instr_set, LEN_INSTR_SET, sizeof(asm_instr_set[0]), CmpForBinSearch);
    if (IS_BAD_PTR(found)) { return 0; }
    
    return (size_t)(found - asm_instr_set);
}


int CmpForBinSearch(const void *a, const void *b)
{
    const hash_t *hash_a = (const hash_t*)a;
    const wrap_t *wrap_b = (const wrap_t*)b;
    if (*hash_a - wrap_b->hash > 0)
        return 1;
    if (*hash_a - wrap_b->hash < 0)
        return -1;
    return 0;
}