#include "noctsh_builtins.h"

#include "noctsh_option.h"
#include "noctsh_string.h"
#include "noctsh_vector.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int cd_impl(noctsh_vector_t args) {
  if (noctsh_vector_len(args) < 2) {
    fprintf(stderr, "Usage: cd <path>\n");
    return 1;
  }

  char *dir = noctsh_string_c_str(NOCTSH_OPTION_UNWRAP(
      noctsh_vector_get(&args, noctsh_string_t, 1), noctsh_string_t));

  int result = chdir(dir);

  if(result == -1) {
    return -errno;
  }

  return 0;
}

NOCTSH_INTERNAL_BUILTIN(cd, cd_impl);
