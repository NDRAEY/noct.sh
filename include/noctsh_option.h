#pragma once

#include <stdbool.h>

typedef struct {
    bool _m_is_present;
    void* _m_element_ref;
} noctsh_option_t;

#define NOCTSH_OPTION_SOME(e) ((noctsh_option_t) { ._m_is_present = true, ._m_element_ref = e })
#define NOCTSH_OPTION_NONE ((noctsh_option_t) { ._m_is_present = false })