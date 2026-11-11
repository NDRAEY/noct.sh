#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "noctsh.h"
#include "noctsh_option.h"
#include "noctsh_string.h"
#include "noctsh_vector.h"

size_t request_input_string(char *out_buffer, size_t maxlength) {
  size_t cursor = 0;

  while (1) {
    if (cursor > maxlength) {
      return cursor;
    }

    int character = fgetc(stdin);

    if (character == -1) {
      return (size_t)(-1);
    }

    if (character == '\n') {
      break;
    }

    if (character == '\b') {
      if (cursor > 0) {
        out_buffer[cursor--] = 0;
      }

      continue;
    }

    out_buffer[cursor++] = character;
  }

  out_buffer[cursor] = 0;

  return cursor;
}

void print_greeting() {
  printf("Welcome to noct.sh v%d.%d!\n\n", NOCTSH_VERSION_MAJOR,
         NOCTSH_VERSION_MINOR);
}

void print_prompt() { printf("> "); }

int main() {
  print_greeting();

  noctsh_vector_t vec = {};

  noctsh_vector_push(&vec, noctsh_string_t, NOCTSH_STRING("Hello"));
  noctsh_vector_push(&vec, noctsh_string_t, NOCTSH_STRING("World"));
  noctsh_vector_push(&vec, noctsh_string_t, NOCTSH_STRING("Computer"));

  noctsh_vector_iter(&vec, noctsh_string_t) {
	printf("%zu %zu %s\n", __iter.__index, __iter.__len, noctsh_string_c_str(*__iter.__element));
  }

  return 0;

  char buffer[512] = {0};

  while (1) {
    print_prompt();

    size_t cursor = request_input_string(buffer, 511);

    if (cursor == (size_t)(-1)) {
      return 0;
    }

    printf("String is: `%s` (cursor = %zu)\n", buffer, cursor);

    memset(buffer, 0, 512);
  }

  return 0;
}
