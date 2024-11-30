%{
#include <stdio.h>  
#include <stdlib.h>

int count_of_letter_a = 0; 
extern int yylex();
int yyerror();

%}

%token TOKEN_LETTER_A
%token TOKEN_LETTER_B

%%
stmnt: TOKEN_LETTER_A stmnt {count_of_letter_a++;}
     | TOKEN_LETTER_B
     ;
%%

int main() {
    if (yyparse() == 0) {
        printf("No. of a's encountered: %d\n", count_of_letter_a);
    } else {
        printf("Parse failed.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int yyerror(char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
    return EXIT_FAILURE;
}