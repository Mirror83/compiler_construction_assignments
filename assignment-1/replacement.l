%{
#include "token.h"
#include <string.h>
%}

%%
abc { printf("ABC"); }
. { printf(yytext); }
%%

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ./replacement file_name\n");
        printf("Example: ./replacement sample.txt\n");
        return 1;
    }

    char *file_name = argv[1];

    yyin = fopen(file_name, "r");
    if (!yyin)
    {
        printf("Could not open file: %s!\n", file_name);
        return 1;
    }

    yylex();
    
    fclose(yyin);

    return 0;
}

int yywrap() { return 1; }