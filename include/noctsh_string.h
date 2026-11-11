#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct {
    char* _m_buffer;
    size_t _m_length;
} noctsh_string_t;

void noctsh_string_from_charptr(noctsh_string_t* string, char* str);
void noctsh_string_append(noctsh_string_t* string, char* str);
