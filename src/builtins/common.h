#pragma once

#include "noctsh_vector.h"

struct __noctsh_builtin {
  char *_name;
  int (*_func)(noctsh_vector_t);
};

#define NOCTSH_INTERNAL_BUILTIN(name, func)                                    \
  static const struct __noctsh_builtin __noctsh_builtin_##name __attribute__(( \
      section("noctsh_builtins"), used, aligned(sizeof(void *)))) = {          \
      ._name = #name,                                                          \
      ._func = func,                                                           \
  };
