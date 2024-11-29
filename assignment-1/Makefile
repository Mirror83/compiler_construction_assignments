BUILD_DIR := build

all: ${BUILD_DIR}/word_count ${BUILD_DIR}/replacement ${BUILD_DIR}/verbs

ensure_build_dir_exists:
	mkdir -p build

${BUILD_DIR}/word_count: word_count.l ensure_build_dir_exists
	lex -o word_count.c word_count.l
	gcc word_count.c -o $@
	rm word_count.c

${BUILD_DIR}/replacement: replacement.l ensure_build_dir_exists
	lex -o replacement.c replacement.l
	gcc replacement.c -o $@
	rm replacement.c

${BUILD_DIR}/verbs: verbs.l ensure_build_dir_exists
	lex -o verbs.c --yylineno verbs.l
	gcc verbs.c -o $@
	rm verbs.c

# Runs the executable built from the word_count.l LEX specification
# on the word_count.txt sample
run_word_count: ${BUILD_DIR}/word_count
	${BUILD_DIR}/word_count text-samples/word-count.txt

