#include "noctsh_string.h"
#include "noctsh_vector.h"

#include <errno.h>
#include <proc.h>

int run_command(noctsh_vector_t parsed_cmdline) {
  noctsh_vector_t argv = {};

  noctsh_vector_iter(&parsed_cmdline, noctsh_string_t) {
    noctsh_vector_push(&argv, char *, noctsh_string_c_str(__iter.__element));
  }

  noctsh_vector_push(&argv, char *, NULL);

  char **raw_argv = argv._m_buffer;
  int argc = parsed_cmdline._m_length;

  int pid = proc_run(raw_argv[0], (const char **)raw_argv, argc, 0);

  if (pid == -1) {
    return errno;
  }

  int exitcode = waitpid(pid);

  return exitcode;
}
