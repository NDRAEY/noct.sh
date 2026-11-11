#pragma once

#include <stddef.h>
#include <stdint.h>

#define noctsh_string_length(st) ((&st)->_m_length)
#define noctsh_string_c_str(st) ((&st)->_m_buffer)

#define NOCTSH_STRING(str) ({ noctsh_string_t st; noctsh_string_from_charptr(&st, str); st; })

typedef struct {
    char* _m_buffer;
    size_t _m_length;
} noctsh_string_t;

void noctsh_string_from_charptr(noctsh_string_t* string, char* str);
void noctsh_string_append(noctsh_string_t* string, char* str);
void noctsh_string_push_char(noctsh_string_t* string, char ch);
void noctsh_string_free(noctsh_string_t* string);
