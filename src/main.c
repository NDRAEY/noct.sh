#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

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

noctsh_vector_t parse_cmdline(const char *cmdline) {
	noctsh_vector_t vec = {};

	noctsh_string_t stem = {};

	for (size_t i = 0, len = strlen(cmdline); i < len; i++) {
		char ch = cmdline[i];

		if(ch == ' ') {
			if(noctsh_string_empty(&stem)) {
				continue;
			}

			noctsh_vector_push(&vec, noctsh_string_t, noctsh_string_clone_fast(stem));

			noctsh_string_clear(&stem);

			continue;
		} else if(ch == '\"') {
			i++;
			
			while(i < len) {
				ch = cmdline[i];

				if(ch != '\"') {
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

	if(!noctsh_string_empty(&stem)) {
		noctsh_vector_push(&vec, noctsh_string_t, stem);
	}

	return vec;
}

int run_command(noctsh_vector_t parsed_cmdline) {
	noctsh_vector_t argv = {};

	noctsh_vector_iter(&parsed_cmdline, noctsh_string_t) {
		noctsh_vector_push(&argv, char*, noctsh_string_c_str(__iter.__element));
	}

	noctsh_vector_push(&argv, char*, NULL);

	char** raw_argv = argv._m_buffer;

	int procwd = fork();

	if(procwd > 0) {
		int status;
		
		waitpid(procwd, &status, 0);

		noctsh_vector_free(&argv);

		return status;
	} else if(procwd == 0) {
		int result = execvp(raw_argv[0], raw_argv);

		exit(result);
	}

	__builtin_unreachable();
}

void print_greeting() {
	printf("Welcome to noct.sh v%d.%d!\n\n", NOCTSH_VERSION_MAJOR,
				 NOCTSH_VERSION_MINOR);
}

void print_prompt() { printf("> "); }

int main(int argc, char** argv) {
	print_greeting();

	char buffer[512] = {0};

	// if(argc == 2) {
	// 	char* cmd = argv[1];

	// 	strcpy(buffer, cmd);

	// 	noctsh_vector_t argv = parse_cmdline(buffer);

	// 	noctsh_vector_iter(&argv, noctsh_string_t) {
	// 		printf("[%zu]: `%s`\n", __iter.__index, noctsh_string_c_str(__iter.__element));
	// 	}

	// 	noctsh_vector_iter(&argv, noctsh_string_t) {
	// 		noctsh_string_free(__iter.__element);
	// 	}

	// 	noctsh_vector_free(&argv);

	// 	return 0;
	// }

	while (1) {
		print_prompt();

		size_t cursor = request_input_string(buffer, 511);

		if (cursor == (size_t)(-1)) {
			return 0;
		}

		printf("String is: `%s` (cursor = %zu)\n", buffer, cursor);

		noctsh_vector_t argv = parse_cmdline(buffer);

		noctsh_vector_iter(&argv, noctsh_string_t) {
			printf("[%zu]: `%s`\n", __iter.__index, noctsh_string_c_str(__iter.__element));
		}

		int res = run_command(argv);

		printf("=> %d\n", res);

		noctsh_vector_iter(&argv, noctsh_string_t) {
			noctsh_string_free(__iter.__element);
		}

		noctsh_vector_free(&argv);

		memset(buffer, 0, 512);
	}

	return 0;
}
