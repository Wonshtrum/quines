#define NOB_IMPLEMENTATION
#include "nob.h"
#define STB_C_LEXER_IMPLEMENTATION
#include "stb_c_lexer.h"

typedef struct Slice {
	char *str;
	int len;
} Slice;

typedef struct FileInfo {
	bool success;
	char *content;
	bool escaped;
	bool skip_post;
	bool no_comments;
	bool mergeable_nl;
	int width;
	int pad;
	int pre;
	int start;
	int end;
	Slice data;
	Slice post;
} FileInfo;
const FileInfo FILE_INFO_ERROR = {0};

bool build_bin(Nob_Cmd *cmd, const char *input_path, const char *output_path) {
	cmd->count = 0;
	// nob_cmd_append(cmd, "gcc", "-Wall", "-Wextra");
	nob_cmd_append(cmd, "gcc", "-w", "-lm", "-std=c11");
	nob_cmd_append(cmd, "-o", output_path);
	nob_cmd_append(cmd, input_path);
	return nob_cmd_run_sync(*cmd);
}

void wrapped_output(FILE *output, int *col, char *string, int length, int wrap, bool escaped) {
	for (int i = 0; i < length; i++) {
		if (string[i] == '\n')
			continue;
		fprintf(output, "%c", string[i]);
		*col += 1;
		if (escaped && *col >= wrap - 1) {
			*col = 1;
			fprintf(output, "\"\n\"");
		} else if (*col >= wrap) {
			*col = 0;
			fprintf(output, "\n");
		}
	}
}

bool build_pck(FileInfo info, const char *input_path, const char *output_path) {
	Nob_String_Builder sb = {0};
	if (!nob_read_entire_file(input_path, &sb)) return false;

	FILE *output = fopen(output_path, "wb");
	if (output == NULL) {
		nob_log(NOB_ERROR, "Could not read file %s: %s", output_path, strerror(errno));
		return false;
	}

	int W = info.width;
	int col = 0;
	wrapped_output(output, &col, sb.items, info.pre, W, false);
	wrapped_output(output, &col, "\"", 1, W, false);
	wrapped_output(output, &col, info.data.str, info.data.len, W, true);
	wrapped_output(output, &col, sb.items, info.pre, W, true);
	if (info.escaped) {
		wrapped_output(output, &col, "\\\"#\\\";", 6, W, true);
	} else {
		wrapped_output(output, &col, "$#$;", 4, W, true);
	}

	bool success = true;
	for (int i = info.start; i < info.end; ++i) {
		char c = sb.items[i];
		switch (c) {
		case '\n':
			break;
		case '\\':
		case '"': {
			if (info.escaped) {
				fprintf(output, "\\%c", c);
				col += 2;
			} else {
				fprintf(output, "%c", c + 2);
				col += 1;
			}
			break;
		}
		default:
			fprintf(output, "%c", c);
			col += 1;
		}
		if (col >= W - 1) {
			if (col > W - 1) {
				nob_log(NOB_ERROR, "Escape character just at the end of line, can not handle this case properly");
				success = false;
			}
			fprintf(output, "\"\n\"");
			col = 1;
		}
	}

	int auto_pad = 0;
	if (info.pad == 0) {
		auto_pad = W - (2 + col + info.post.len) % W;
		// FIXME: auto pad is broken if it wraps
		for (int i = 0; i < auto_pad; i++)
			wrapped_output(output, &col, "/", 1, W, true);
	}
	wrapped_output(output, &col, info.post.str, info.post.len, W, true);
	wrapped_output(output, &col, "\";", 2, W, false);
	fprintf(output, "%.*s", sb.count - info.start, sb.items + info.start);
	for (int i = 0; i < sb.count; i++) {
		col++;
		if (sb.items[i] == '\n') {
			col = 0;
		}
	}
	if (!info.skip_post) {
		for (int i = 0; i < auto_pad; i++)
			wrapped_output(output, &col, "/", 1, W, false);
		wrapped_output(output, &col, info.post.str, info.post.len, W, false);
	}
	fprintf(output, "\n");

	fclose(output);
	//nob_log(NOB_INFO, "Generated %s", output_path);
	return success;
}

bool is_unconcatable(long token) {
	return (token == CLEX_id || token == CLEX_intlit);
}

bool cmp_str(Slice s, char *p) {
	int i = 0;
	for (; i <= s.len && p[i] == s.str[i]; i++);
	return i == s.len && p[i] == '\0';
}

Slice slice_from_lexer(stb_lexer *l) {
	if (l->token == 261) {
		return (Slice){
			.str = l->where_firstchar + 1,
			.len = l->where_lastchar - l->where_firstchar - 1,
		};
	} else {
		return (Slice){
			.str = l->where_firstchar,
			.len = l->where_lastchar - l->where_firstchar + 1,
		};
	}
}

FileInfo unexpected_token(Slice token, char *expected) {
	nob_log(NOB_ERROR, "Unexpected token: `%.*s`, expected: %s", token.len, token.str, expected);
	return FILE_INFO_ERROR;
}

FileInfo build_fmt(const char *input_path, const char *output_path) {
	Nob_String_Builder sb = {0};
	if (!nob_read_entire_file(input_path, &sb)) return FILE_INFO_ERROR;

	FILE *output = fopen(output_path, "wb");
	if (output == NULL) {
		nob_log(NOB_ERROR, "Could not read file %s: %s", output_path, strerror(errno));
		return FILE_INFO_ERROR;
	}

	FileInfo info = {0};
	info.width = 80;
	info.success = true;
	stb_lexer l = {0};
	static char string_store[8192];
	stb_c_lexer_init(&l, sb.items, sb.items + sb.count, string_store, NOB_ARRAY_LEN(string_store));

	while (stb_c_lexer_get_token(&l)) {
		if (l.token != 260 || *l.where_firstchar != '_') break;
		Slice key = slice_from_lexer(&l);
		if (!stb_c_lexer_get_token(&l)) return FILE_INFO_ERROR;
		if (l.token != 61) return unexpected_token(slice_from_lexer(&l), "`=`");
		if (!stb_c_lexer_get_token(&l)) return FILE_INFO_ERROR;
		Slice val = slice_from_lexer(&l);

		if (cmp_str(key, "_escaped")) {
			if (l.token != 258) return unexpected_token(val, "integer");
			info.escaped = l.int_number != 0;
		} else if (cmp_str(key, "_skip_post")) {
			if (l.token != 258) return unexpected_token(val, "integer");
			info.skip_post = l.int_number != 0;
		} else if (cmp_str(key, "_no_comments")) {
			if (l.token != 258) return unexpected_token(val, "integer");
			info.no_comments = l.int_number != 0;
		} else if (cmp_str(key, "_mergeable_nl")) {
			if (l.token != 258) return unexpected_token(val, "integer");
			info.mergeable_nl = l.int_number != 0;
		} else if (cmp_str(key, "_width")) {
			if (l.token != 258) return unexpected_token(val, "integer");
			info.width = l.int_number;
		} else if (cmp_str(key, "_pad")) {
			if (l.token != 258) return unexpected_token(val, "integer");
			info.pad = l.int_number;
		} else if (cmp_str(key, "_data")) {
			if (l.token != 261) return unexpected_token(val, "string");
			info.data = val;
		} else if (cmp_str(key, "_post")) {
			if (l.token != 261) return unexpected_token(val, "string");
			info.post = val;
		} else {
			return unexpected_token(key, "FileInfo field");
		}
		if (!stb_c_lexer_get_token(&l)) return FILE_INFO_ERROR;
		if (l.token != 59) return unexpected_token(slice_from_lexer(&l), "`;`");
		//nob_log(NOB_INFO, "FileInfo::%.*s = %.*s", key.len, key.str, val.len, val.str);
	}

	int W = info.width;
	int cursor = 0;
	int x = 0;
	int sep = 0;
	int prev_token = 0;
	Slice token;
	while (cursor == 0 || stb_c_lexer_get_token(&l)) {
		int n = l.where_lastchar - l.where_firstchar + 1;
		sep = (is_unconcatable(prev_token) && is_unconcatable(l.token) && (x || info.mergeable_nl)) ? 1 : 0;
		token = slice_from_lexer(&l);
		if (l.token == 261 && token.str[0] == '#') {
			info.pre = cursor;
			fprintf(output, "\"#\";\n");
			if (!stb_c_lexer_get_token(&l)) return FILE_INFO_ERROR;
			if (l.token != 59) return unexpected_token(slice_from_lexer(&l), "`;`");
			for (x = 0; x < info.pad; x++) fprintf(output, " ");
			cursor += 5 + info.pad;
			info.start = cursor;
			continue;
		} else if (cmp_str(token, "_stop")) {
			info.end = cursor;
			continue;
		}
		if (x + sep + n > W) {
			cursor += W - x + 1;
			if (x < W - 1) {
				for (; x < W; x++) {
					if (info.no_comments) {
						fprintf(output, " ");
					} else {
						fprintf(output, "/");
					}
				}
			} else {
				fprintf(output, " ");
			}
			fprintf(output, "\n");
			x = 0;
		} else if (sep) {
			fprintf(output, " ");
			cursor += 1;
			x += 1;
		}
		prev_token = l.token;
		fprintf(output, "%.*s", n, l.where_firstchar);
		cursor += n;
		x += n;
		if (x >= W) {
			fprintf(output, "\n");
			cursor += 1;
			x = 0;
		}
	}

	if (!info.end) info.end = cursor;

	fclose(output);
	//nob_log(NOB_INFO, "FileInfo::_pre = %d", info.pre);
	//nob_log(NOB_INFO, "FileInfo::_start = %d", info.start);
	//nob_log(NOB_INFO, "Generated %s: %d bytes", output_path, cursor);
	info.content = sb.items;
	return info;
}

bool compile(Nob_Cmd *cmd, const char *src, char *pck, char *bin) {
	FileInfo info = build_fmt(src, "./build/fmt.c");
	if (!info.success) return false;
	if (!build_pck(info, "./build/fmt.c", pck)) return false;
	if (!build_bin(cmd, pck, bin)) return false;
	//nob_log(NOB_INFO, "Generated %s", bin);
	return true;
}

bool is_src(int len, const char *name, char *src, char *pck, char *bin) {
	int i;
	for (i = 0; i < 1024-len && name[i]; i++) {
		if (i < 4) {
			if (name[i] != "src_"[i]) return false;
		} else {
			src[len+i] = name[i];
			pck[len+i] = name[i];
			bin[len+i] = name[i];
		}
	}
	if (i < 4 || i >= 1024-len) return false;
	if (name[i-1] != 'c' || name[i-2] != '.') return false;
	src[len+i] = pck[len+i] = bin[len+i-2] = 0;
	return true;
}

int main(int argc, char **argv) {
	NOB_GO_REBUILD_URSELF(argc, argv);

	if (argc < 2) {
		nob_log(NOB_ERROR, "Missing source file or directory");
		return 1;
	}
	if (!nob_mkdir_if_not_exists("./build/")) return 1;

	Nob_Cmd cmd = {0};

	if (nob_get_file_type(argv[1]) == NOB_FILE_DIRECTORY) {
		static char src[1024], pck[1024], bin[1024];
		int i;
		for (i = 0; i < 1024 && argv[1][i]; i++) {
			src[i] = pck[i] = bin[i] = argv[1][i];
		}
		if (i >= 1000) return 1;
		if (argv[1][i-1] != '/') {
			src[i] = pck[i] = bin[i] = '/';
			i++;
		}
		int len = i;
		memcpy(src+i, "src_", 4);
		memcpy(pck+i, "pck_", 4);
		memcpy(bin+i, "bin_", 4);
		Nob_File_Paths children = {0};
		if (!nob_read_entire_dir(argv[1], &children)) return 1;
		for (i = 0; i < children.count; i++) {
			if (is_src(len, children.items[i], src, pck, bin)) {
				nob_log(NOB_INFO, "Compiling: %s", src);
				if (!compile(&cmd, src, pck, bin)) return 1;
			}
		}
	} else {
		if (compile(&cmd, argv[1], "./build/pck.c", "./build/bin")) return 1;
	}

	return 0;
}
