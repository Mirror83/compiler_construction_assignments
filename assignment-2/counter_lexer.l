%{
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
%}

%%
a { return TOKEN_LETTER_A; }
b { return TOKEN_LETTER_B; }
. { 
    fprintf(stderr, "Parse error: Unexpected token: %s. Allowed tokens are only a's and b's\n", yytext);
    exit(EXIT_FAILURE); 
}
%%

int yywrap() {
    return 1;
}

