#pragma once

#include <stddef.h>
#include <stdint.h>

#define noctsh_vector_get(vec, T, index) (T*)_noctsh_vector_get(vec, sizeof(T), index)
#define noctsh_vector_push(vec, T, element) _noctsh_vector_push(vec, sizeof(T), (void*)(&element))

typedef struct {
    void* _m_buffer;
    size_t length;
} noctsh_vector_t;

void* _noctsh_vector_get(noctsh_vector_t* vec, size_t _element_size, size_t index) {
    
}

void _noctsh_vector_push(noctsh_vector_t* vec, size_t _element_size, void* element) {

}