#pragma once

#include "noctsh_option.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define noctsh_vector_get(vec, T, index) _noctsh_vector_get(vec, sizeof(T), index)
#define noctsh_vector_push(vec, T, element) _noctsh_vector_push(vec, sizeof(T), (void*)(&element))
#define noctsh_vector_len(vec) ((&vec)->_m_length)

typedef struct {
    void* _m_buffer;
    size_t _m_length;
} noctsh_vector_t;

noctsh_option_t _noctsh_vector_get(noctsh_vector_t* vec, size_t _element_size, size_t index);
void _noctsh_vector_push(noctsh_vector_t* vec, size_t _element_size, void* element);