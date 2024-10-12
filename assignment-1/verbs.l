%{
#include "token.h"
#include <string.h>
%}

VERB (is|am|are|were|was|be|being|been|do|does|did|will|would|should|can|could|has|have|had|go)

%%
(" "|\t|\n) /* skip whitespace */
{VERB} { return TOKEN_VERB; }
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
        fclose(yyin);
        return 1;
    }
    
    fclose(yyin);

    return 0;
}

int yywrap() { return 1; }