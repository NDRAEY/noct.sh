#include <stdio.h>
#include "noctsh_vector.h"

noctsh_option_t _noctsh_vector_get(noctsh_vector_t* vec, size_t _element_size, size_t index) {
    if(!vec) {
        return NOCTSH_OPTION_NONE;
    }

    if(vec->_m_buffer == NULL) {
        return NOCTSH_OPTION_NONE;
    }

    size_t byte_offset = _element_size * index;
    size_t byte_offset_end = _element_size * (index + 1);

    size_t vec_size = vec->_m_length * _element_size;
    
    if(byte_offset < vec_size && byte_offset_end <= vec_size) {
        size_t addr = (size_t)vec->_m_buffer;

        void* element = (void*)(addr + byte_offset);

        return NOCTSH_OPTION_SOME(element);
    }

    return NOCTSH_OPTION_NONE;
}

void _noctsh_vector_push(noctsh_vector_t* vec, size_t _element_size, void* element) {
    assert(vec != NULL);

    size_t vec_size = vec->_m_length * _element_size;
    size_t new_vec_size = vec_size + _element_size;

    void* new_buffer = realloc(vec->_m_buffer, new_vec_size);

    assert(new_buffer != NULL);

    memcpy((void*)((size_t)new_buffer + vec_size), element, _element_size);

    vec->_m_buffer = new_buffer;
    vec->_m_length += 1;
}

void noctsh_vector_free(noctsh_vector_t* vec) {
    vec->_m_length = 0;
    
    free(vec->_m_buffer);
}