%{
#include "token.h"
#include <string.h>
%}

VERB (is|am|are|were|was|be|being|been|do|does|did|will|would|should|can|could|has|have|had|go)
WHITESPACE ([ |\t|\n])
LETTER [a-zA-Z]

%%
(^{VERB}{WHITESPACE}|{WHITESPACE}{VERB}{WHITESPACE}) { return TOKEN_VERB; }
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
    
    while (1) {
        token_t token = yylex();
        if (token == TOKEN_EOF)
            break;
        
        if (token == TOKEN_VERB) {
            printf("verb: %s, lineno: %d\n", yytext, yylineno);
        }
    }
    fclose(yyin);

    return 0;
}

int yywrap() { return 1; }