%{
#include "token.h"
#include <ctype.h>
#include <string.h>

// The longest verb in the list is "should" with 6 characters
#define MAX_VERB_LENGTH 6

/** Stores current matched token after removal of whitespace characters by `clean_verb`
    The array is defined to have length MAX_VERB_LENGTH + 1, 
    to allow for the string termination character '\0'.
*/
char current_result[MAX_VERB_LENGTH + 1];

void clean_verb(char *s, char *output);
void print_header();

int verb_count = 0;
%}

VERB (is|am|are|were|was|be|being|been|do|does|did|will|would|should|can|could|has|have|had|go)
WHITESPACE ([ |\t|\n])
%%
(^{VERB}{WHITESPACE}|{WHITESPACE}{VERB}{WHITESPACE}) { 
    clean_verb(yytext, current_result);

    int at_end_of_line = yytext[strlen(yytext) - 1] == '\n';

    printf("%-d\t%-s\n", at_end_of_line ? yylineno - 1 : yylineno, current_result);
    verb_count++; 
}
\n ;
. ;
%%

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ./verbs file_name\n");
        printf("Example: ./verbs sample.txt\n");
        return 1;
    }

    char *file_name = argv[1];

    yyin = fopen(file_name, "r");
    if (!yyin)
    {
        printf("Could not open file: %s!\n", file_name);
        return 1;
    }

    print_header();

    yylex();
    printf("\nNumber of verbs recognized: %d\n", verb_count);
    fclose(yyin);

    return 0;
}

void print_header() {
    printf("Verb recognizer\n");
    printf("---------------\n\n");
    printf("line\tverb\n");
    printf("----\t----\n");
}

/** Copies the letters of the verb in s into output without whitespace characters e.g '\t' or '\n'. */
void clean_verb(char *s, char *output) {
    char *copy = s;

    int i = 0;

    while (*copy != '\0') {
        if (!isspace(*copy)) {
            output[i] = *copy;
            i += 1;
        }
        copy += 1;
    }

    output[i] = '\0';
}

int yywrap() { return 1; }