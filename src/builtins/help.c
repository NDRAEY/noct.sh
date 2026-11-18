#include "noctsh_builtins.h"

#include "noctsh_option.h"
#include "noctsh_string.h"
#include "noctsh_vector.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int help_impl(noctsh_vector_t args) {
  char* program = NULL;

  if(noctsh_vector_len(args) == 2) {
    program = noctsh_string_c_str(NOCTSH_OPTION_UNWRAP(
        noctsh_vector_get(&args, noctsh_string_t, 1), noctsh_string_t));
  }

  if(program != 0) {
    printf("No help info is available for the command `%s`\n", program);
    return 1;
  }

  printf("Available commands:\n");
  for(const struct __noctsh_builtin* cur = __start_noctsh_builtins; cur < __stop_noctsh_builtins; cur++) {
    printf("\t%s\n", cur->_name);
  }

  return 0;
}

NOCTSH_INTERNAL_BUILTIN(help, help_impl);
