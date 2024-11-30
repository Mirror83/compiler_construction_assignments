%{
#include <stdlib.h>
#include "token.h"
int yyerror(char *);
%}

%%
exit    { return TOKEN_EXIT; }
[0-9]+  { /**Convert value to number*/ yylval.num = atoi(yytext); return TOKEN_INT; }
;   { return TOKEN_SEMI; }
[+] { return TOKEN_PLUS; }
[-] { return TOKEN_MINUS; }
[ \t\n]  ;
.   { 
    fprintf(stderr, "Parse error: Unexpected token: %s.\n", yytext);
    exit(EXIT_FAILURE); 
}
%%

int yywrap() {
    return 1;
}