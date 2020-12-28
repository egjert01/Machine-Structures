/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              memory.c
 *      by Branch Seidenman (bseide01) and Emily Gjertsson (egjert01)
 *                            11/23/2020
 *
 *                            UM (hw6)
 *
 *                Implementation of registers and segments
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "memory.h"

// uint32_t registers[8] = {0};

const uint64_t HINT = 1000;

/* 
*              Struct representing segment memory
*  Seq_T mapped holds Hanson UArrays which represent the segments themselves
*  Seq_T unmapped holds ids of unmapped segments which can be reused
*  Next_index is the index of length + 1 of the mapped Sequence
*  p_counter is the index which keeps track of which instruction in seg0 
*       should be executed next
*/
struct segments {
    Seq_T mapped;
    Seq_T unmapped;
    uint32_t registers[8];
    uint32_t next_index;
    uint32_t p_counter;
};

void set_register(uint8_t reg_i, uint32_t value, segments memory)
{
    assert(reg_i < 8);
    memory->registers[reg_i] = value;
}

uint32_t get_register(uint8_t reg_i, segments memory)
{
    assert(reg_i < 8);
    return memory->registers[reg_i];
}

uint32_t get_next_inst(segments memory)
{
    assert(memory != NULL && memory->unmapped != NULL 
                          && memory->mapped   != NULL);

    UArray_T segment0 = Seq_get(memory->mapped, 0);
    uint32_t next_inst = *(uint32_t *)UArray_at(segment0, memory->p_counter);
    memory->p_counter++;
    return next_inst;
}

segments init_segs() 
{
    segments memory = malloc(sizeof(struct segments));
    assert(memory);

    memory->mapped     = Seq_new(HINT);
    memory->unmapped   = Seq_new(HINT);
    for (int i = 0; i < 8; i++) {
        memory->registers[i] = 0;
    }
    memory->next_index = 0;
    memory->p_counter  = 0;
    
    return memory;
}

uint32_t map_seg(segments memory, UArray_T new)
{
    assert(memory != NULL && memory->unmapped != NULL 
                          && memory->mapped   != NULL);

    Seq_T unmapped = memory->unmapped;
    Seq_T mapped = memory->mapped;
    
    if (Seq_length(unmapped) > 0) {
        uint32_t index = (uintptr_t)Seq_remhi(unmapped);
        Seq_put(mapped, index, new);
        return index;
        
    } else { /* no previously unmapped ids to use */
        uint32_t next_index = memory->next_index;
        Seq_addhi(mapped, new);
        memory->next_index++;
        return next_index;
        
    }
}

void unmap_seg(segments memory, uint32_t seg_id)
{
    assert(memory != NULL && memory->unmapped != NULL 
                          && memory->mapped   != NULL);

    UArray_T segment = (UArray_T)Seq_get(memory->mapped, seg_id);
    UArray_free(&segment);
    Seq_put(memory->mapped, seg_id, NULL);
    Seq_addhi(memory->unmapped, (void *)(uintptr_t)seg_id);
}

uint32_t get_seg_val(segments memory, uint32_t seg_id, uint32_t index)
{
    assert(memory != NULL && memory->unmapped != NULL 
                          && memory->mapped   != NULL);

    UArray_T segment = (UArray_T)Seq_get(memory->mapped, seg_id);
    uint32_t val = *(uint32_t *)UArray_at(segment, index);
    return val;
}

void set_seg_val(segments memory, uint32_t seg_id, uint32_t index, uint32_t val)
{
    assert(memory != NULL && memory->unmapped != NULL 
                          && memory->mapped   != NULL);
    
    Seq_T mapped = memory->mapped;
    UArray_T segment = Seq_get(mapped, seg_id);
    *((uint32_t *)UArray_at(segment, index)) = val;
}

void load_prog(uint8_t rb, uint8_t rc, segments memory)
{
    assert(memory != NULL && memory->unmapped != NULL 
                          && memory->mapped   != NULL);

    if (get_register(rb, memory) != 0) {
        UArray_T to_dup = Seq_get(memory->mapped, get_register(rb, memory));
        UArray_T duplicate_seg = UArray_copy(to_dup, UArray_length(to_dup));
        UArray_T old_seg0 = Seq_put(memory->mapped, 0, duplicate_seg);
        
        UArray_free(&old_seg0);
    }

    memory->p_counter = get_register(rc, memory);
}

void free_segs(segments memory)
{
    assert(memory != NULL && memory->unmapped != NULL 
                          && memory->mapped   != NULL);
    
    for (uint32_t i = 0; i < memory->next_index; i++) {
        UArray_T curr = Seq_get(memory->mapped, i);
        if (curr != NULL) {
            UArray_free(&curr);
        }
    }

    Seq_free(&memory->mapped);
    Seq_free(&memory->unmapped);
    free(memory);
}