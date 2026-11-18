#include "noctsh_builtins.h"

#include "noctsh_option.h"
#include "noctsh_string.h"
#include "noctsh_vector.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include <dirent.h>

int dir_impl(noctsh_vector_t args) {
  char *path = NULL;

  if (noctsh_vector_len(args) == 2) {
    path = noctsh_string_c_str(NOCTSH_OPTION_UNWRAP(
        noctsh_vector_get(&args, noctsh_string_t, 1), noctsh_string_t));
  }

  DIR* dir = opendir(path ?: ".");

  if(dir == NULL) {
    fprintf(stderr, "opendir failed: %s\n", strerror(errno));
    return 1;
  }

  struct dirent* entry;

  while((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
  }

  closedir(dir);

  return 0;
}

NOCTSH_INTERNAL_BUILTIN(dir, dir_impl);
