#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "noctsh_string.h"

bool noctsh_string_from_charptr(noctsh_string_t *string, const char *str) {
    if(str == NULL) {
        return false;
    }

    size_t length = strlen(str);

    char* buf = malloc(length + 1);
    if(buf == NULL) {
        return false;
    }

    memcpy(buf, str, length);

    buf[length] = 0;

    string->_m_buffer = buf;
    string->_m_length = length;

    return true;
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

bool noctsh_string_clone(noctsh_string_t* in_string, noctsh_string_t* out_string) {
    if(in_string == NULL || out_string == NULL) {
        return false;
    }
    
    char* cloned_buf = malloc(in_string->_m_length + 1);

    if(cloned_buf == NULL) {
        return false;
    }

    memcpy(cloned_buf, in_string->_m_buffer, in_string->_m_length);

    cloned_buf[in_string->_m_length] = 0;

    out_string->_m_buffer = cloned_buf;
    out_string->_m_length = in_string->_m_length;

    return true;
}

char noctsh_string_pop_char(noctsh_string_t* string) {
    if(string->_m_length == 0) {
        return 0xFF;
    }

    char ch = string->_m_buffer[string->_m_length - 1];

    string->_m_length--;
    string->_m_buffer[string->_m_length] = 0;

    char* new_buf = realloc(string->_m_buffer, string->_m_length + 1);
    if(new_buf != NULL) {
        string->_m_buffer = new_buf;
    }

    return ch;
}

void noctsh_string_insert_char(noctsh_string_t* string, size_t position, char ch) {
    size_t pos = position > string->_m_length ? string->_m_length : position;

    char* buf = realloc(string->_m_buffer, string->_m_length + 2);
    if(!buf) return;

    string->_m_buffer = buf;

    // Move only the characters after the insertion point; the terminator is
    // written explicitly below so we never depend on an existing one.
    memmove(buf + pos + 1, buf + pos, string->_m_length - pos);

    buf[pos] = ch;
    string->_m_length++;
    buf[string->_m_length] = 0;
}

void noctsh_string_clear(noctsh_string_t* string) {
    noctsh_string_free(string);
}

void noctsh_string_free(noctsh_string_t* string) {
    if(string->_m_buffer!=NULL) {
        free(string->_m_buffer);
    }

    string->_m_buffer = NULL;
    string->_m_length = 0;
}