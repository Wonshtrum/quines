#define NOB_IMPLEMENTATION
#include "nob.h"
#define STB_C_LEXER_IMPLEMENTATION
#include "stb_c_lexer.h"

bool build_exe(Nob_Cmd *cmd, const char *input_path, const char *output_path) {
	cmd->count = 0;
	nob_cmd_append(cmd, "gcc", "-Wall", "-Wextra");
	nob_cmd_append(cmd, "-o", output_path);
	nob_cmd_append(cmd, input_path);
	return nob_cmd_run_sync(*cmd);
}

bool compile_quine_blob(const char *input_path, const char *output_path) {
	Nob_String_Builder sb = {0};
	if (!nob_read_entire_file(input_path, &sb)) return false;

	FILE *output = fopen(output_path, "wb");
	if (output == NULL) {
		nob_log(NOB_ERROR, "Could not read file %s: %s", output_path, strerror(errno));
		return false;
	}

	bool success = true;
	fprintf(output, "char*ctime(),c,*t,*z,*s,*S=\"char*ctime(),c,*t,*z,*s,*S=\\\"#\\\";");
	int col = 61;
	for (int i = 0; i < sb.count; ++i) {
		switch (sb.items[i]) {
			case '\n': break;
			case '\\': fprintf(output, "\\\\"); col+=2; break;
			case '"':  fprintf(output, "\\\""); col+=2; break;
			default: fprintf(output, "%c", sb.items[i]); col+=1;
		}
		if (col >= 80-1) {
			if (col > 80-1) {
				nob_log(NOB_ERROR, "Escape character just at the end of line, can not handle this case properly");
				success = false;
			}
			fprintf(output, "\"\n\"");
			col = 1;
		}
	}
	int padding = 80-2-col;
	//fprintf(output, " //Copyright Wonshtrum //Inspired by Tsoding");
	for (int i = 0; i < padding; i++) fprintf(output, "/");
	fprintf(output, "\";\n%.*s", sb.count, sb.items);
	for (int i = 0; i < padding; i++) fprintf(output, "/");
	fprintf(output, "\n");

	fclose(output);
	nob_log(NOB_INFO, "Generated %s", output_path);

	return success;
}

bool is_unconcatable(long token) {
	return (token == CLEX_id || token == CLEX_intlit);
}

bool format_tokens(const char *input_path, const char *output_path) {
	Nob_String_Builder sb = {0};
	if (!nob_read_entire_file(input_path, &sb)) return false;

	FILE *output = fopen(output_path, "wb");
	if (output == NULL) {
		nob_log(NOB_ERROR, "Could not read file %s: %s", output_path, strerror(errno));
		return false;
	}

	stb_lexer l = {0};
	static char string_store[1024];
	stb_c_lexer_init(&l, sb.items, sb.items + sb.count, string_store, NOB_ARRAY_LEN(string_store));

	int x = 0;
	int prev_token = 0;
	int sep = 0;
	while (stb_c_lexer_get_token(&l)) {
		int n = l.where_lastchar - l.where_firstchar + 1;
		sep = (is_unconcatable(prev_token) && is_unconcatable(l.token) && x > 0) ? 1 : 0;
		if (x + sep + n > 80) {
			if (x < 79) {
				for (; x<80; x++) fprintf(output, "/");
			} else {
				fprintf(output, " ");
			}
			fprintf(output, "\n");
			x = 0;
		} else if (sep) {
			fprintf(output, " ");
			x += 1;
		}
		prev_token = l.token;
		fprintf(output, "%.*s", n, l.where_firstchar);
		x += n;
		if (x >= 80) {
			fprintf(output, "\n");
			x = 0;
		}
	}
	fclose(output);
	nob_log(NOB_INFO, "Generated %s", output_path);
	return true;
}

int main(int argc, char **argv) {
	NOB_GO_REBUILD_URSELF(argc, argv);

	if (argc < 2) {
		nob_log(NOB_ERROR, "Missing source file");
		return 1;
	}
	if (!nob_mkdir_if_not_exists("./build/")) return 1;

	Nob_Cmd cmd = {0};
	if (!format_tokens(argv[1], "./build/packed.c")) return 1;
	if (!compile_quine_blob("./build/packed.c", "./build/qlock.c")) return 1;
	if (!build_exe(&cmd, "./build/qlock.c", "./build/qlock")) return 1;

	return 0;
}
