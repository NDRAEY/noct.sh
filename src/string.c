#include <stdlib.h>
#include <string.h>
#include "noctsh_string.h"

void noctsh_string_from_charptr(noctsh_string_t *string, const char *str) {
    size_t length = strlen(str);

    char* buf = malloc(length + 1);

    memcpy(buf, str, length);

    buf[length] = 0;

    string->_m_buffer = buf;
    string->_m_length = length;
}

void noctsh_string_append(noctsh_string_t *string, const char *str) {
    size_t raw_str_len = strlen(str);

    char* new_buf = realloc(string->_m_buffer, string->_m_length + raw_str_len + 1);

    memcpy(new_buf + string->_m_length, str, raw_str_len);

    string->_m_buffer = new_buf;
    string->_m_length += raw_str_len;

    new_buf[string->_m_length] = 0;
}

void noctsh_string_push_char(noctsh_string_t* string, char ch) {
    char* new_buf = realloc(string->_m_buffer, (string->_m_length + 1) + 1);

    new_buf[string->_m_length++] = ch;
    new_buf[string->_m_length] = 0;

    string->_m_buffer = new_buf;
}

void noctsh_string_clone(noctsh_string_t* in_string, noctsh_string_t* out_string) {
    char* cloned_buf = malloc(in_string->_m_length + 1);

    memcpy(cloned_buf, in_string->_m_buffer, in_string->_m_length);

    cloned_buf[in_string->_m_length] = 0;

    out_string->_m_buffer = cloned_buf;
    out_string->_m_length = in_string->_m_length;
}

void noctsh_string_clear(noctsh_string_t* string) {
    noctsh_string_free(string);
}

void noctsh_string_free(noctsh_string_t* string) {
    free(string->_m_buffer);

    string->_m_buffer = NULL;
    string->_m_length = 0;
}