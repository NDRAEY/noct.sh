#include "noctsh_string.h"
#include "noctsh_vector.h"

#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int run_command(noctsh_vector_t parsed_cmdline) {
  noctsh_vector_t argv = {};

  noctsh_vector_iter(&parsed_cmdline, noctsh_string_t) {
    noctsh_vector_push(&argv, char *, noctsh_string_c_str(__iter.__element));
  }

  noctsh_vector_push(&argv, char *, NULL);

  char **raw_argv = argv._m_buffer;

  int procwd = fork();

  if (procwd > 0) {
    int status;

    waitpid(procwd, &status, 0);

    noctsh_vector_free(&argv);

    return WEXITSTATUS(status);
  } else if (procwd == 0) {
    int result = execvp(raw_argv[0], raw_argv);

    // printf("Execvp: %d\n", result);

    if(result == -1) {
      exit(errno);
    }

    exit(0);
  }

  __builtin_unreachable();
}
