#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "noctsh.h"
#include "noctsh_option.h"
#include "noctsh_string.h"
#include "noctsh_vector.h"
#include "run.h"

#include "noctsh_builtins.h"

__attribute__((nonnull(1)))
bool request_input_string(noctsh_string_t* str) {
  size_t cursor = 0;

  while (1) {
    int character = fgetc(stdin);

    if (character == -1) {
      fputc('\n', stdout);
      return true;
    }

    if (character == '\n') {
      break;
    }

    if (character == '\b') {
      if (cursor > 0) {
        noctsh_string_pop_char(str);
        
        fputc(' ', stdout);
        fputc('\b', stdout);
        
        fflush(stdout);
        
        cursor--;
      }

      continue;
    }

    // if (character == '^[[D') {
    //   cursor = cursor > 0 ? cursor - 1 : 0;
    // }

    // FIXME: Insert a character instead of overwriting it.
    // out_buffer[cursor++] = character
    noctsh_string_insert_char(str, cursor++, character);
  }

  // out_buffer[cursor] = 0;

  return false;
}

noctsh_vector_t parse_cmdline(const noctsh_string_t *cmdline) {
  noctsh_vector_t vec = {};

  if(cmdline == NULL) {
    return vec;
  }

  if (noctsh_string_is_empty(cmdline)) {
    return vec;
  }

  noctsh_string_t stem = {};

  for (size_t i = 0, len = noctsh_string_length(cmdline); i < len; i++) {
    char ch = noctsh_string_nth_char(cmdline, i);

    if (ch == ' ') {
      if (noctsh_string_is_empty(&stem)) {
        continue;
      }

      noctsh_vector_push(&vec, noctsh_string_t, noctsh_string_clone_fast(stem));

      noctsh_string_clear(&stem);

      continue;
    } else if (ch == '\"') {
      i++;

      while (i < len) {
        ch = noctsh_string_nth_char(cmdline, i);

        if (ch != '\"') {
          noctsh_string_push_char(&stem, ch);
        } else {
          break;
        }

        i++;
      }

      noctsh_vector_push(&vec, noctsh_string_t, noctsh_string_clone_fast(stem));

      noctsh_string_clear(&stem);

      continue;
    }

    noctsh_string_push_char(&stem, ch);
  }

  if (!noctsh_string_is_empty(&stem)) {
    noctsh_vector_push(&vec, noctsh_string_t, stem);
  }

  return vec;
}

void print_greeting() {
  printf("Welcome to noct.sh v%d.%d!\n\n", NOCTSH_VERSION_MAJOR,
         NOCTSH_VERSION_MINOR);
}

void print_prompt() {
  char dirname[256] = {0};

  getcwd(dirname, 255);

  printf("%s > ", dirname);

  fflush(stdout);
}

int main(int argc, char **argv) {
  print_greeting();

  noctsh_string_t str = {};

  while (1) {
    print_prompt();

    bool eof = request_input_string(&str);

    if (eof) {
      return 0;
    }

    noctsh_vector_t argv = parse_cmdline(&str);

    if (noctsh_vector_len(argv) == 0) {
      goto end;
    }

    char *argv0 = noctsh_string_c_str(NOCTSH_OPTION_UNWRAP(
        noctsh_vector_get(&argv, noctsh_string_t, 0), noctsh_string_t));

    bool is_matched_builtin = false;

    // Look for the builtin, if builtin matches, use it.
    for (const struct __noctsh_builtin *cur = __start_noctsh_builtins;
         cur < __stop_noctsh_builtins; cur++) {
      if (strcmp(cur->_name, argv0) == 0) {
        int result = cur->_func(argv);

        if (result < 0) {
          fprintf(stderr, "noctsh: %s: %s\n", noctsh_string_c_str(&str), strerror(result));
        }

        is_matched_builtin = true;
        break;
      }
    }

    if (is_matched_builtin)
      goto end;

    // Run a program if no builtin there.
    int res = run_command(argv);

    printf("exitcode => %d\n", res);

    if (res > 0) {
      fprintf(stderr, "noctsh: %s: %s\n", noctsh_string_c_str(&str), strerror(res));
    }

    end:

    noctsh_vector_iter(&argv, noctsh_string_t) {
      noctsh_string_free(__iter.__element);
    }

    noctsh_vector_free(&argv);

    noctsh_string_clear(&str);
  }

  return 0;
}
