BUILD_FOLDER := build

all: ${BUILD_FOLDER}/word_count ${BUILD_FOLDER}/replacement ${BUILD_FOLDER}/verbs

${BUILD_FOLDER}/word_count: word_count.l
	lex -o word_count.c word_count.l
	gcc word_count.c -o $@
	rm word_count.c

${BUILD_FOLDER}/replacement: replacement.l
	lex -o replacement.c replacement.l
	gcc replacement.c -o $@
	rm replacement.c

${BUILD_FOLDER}/verbs: verbs.l
	lex -o verbs.c verbs.l
	gcc verbs.c -o $@
	rm verbs.c

# Runs the executable built from the word_count.l LEX specification
# on the word_count.txt sample
run_word_count: ${BUILD_FOLDER}/word_count
	${BUILD_FOLDER}/word_count text-samples/word_count.txt

