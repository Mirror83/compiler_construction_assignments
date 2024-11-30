%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
void exit_success();

int yyerror();
int parse_result = 0;
%}

%union { int num; }

%start program
%token <num> TOKEN_INT
%token TOKEN_PLUS
%token TOKEN_MINUS
%token TOKEN_SEMI
%token TOKEN_EXIT

%type <num> program expr term

%%
program : expr TOKEN_SEMI { printf("Result: %d\n", $1); }
        | program expr TOKEN_SEMI { printf("Result: %d\n", $2); }
        | TOKEN_EXIT { exit_success(); }
        | TOKEN_EXIT TOKEN_SEMI { exit_success(); }
        | program TOKEN_EXIT { exit_success(); }
        | program TOKEN_EXIT TOKEN_SEMI { exit_success(); }
        ; 
        
expr : expr TOKEN_PLUS term { $$ = $1 + $3; }
     | expr TOKEN_MINUS term { $$ = $1 - $3; }
     | term { $$ = $1; }
     ;

term : TOKEN_INT { $$ = $1; }
     ;
%%

void exit_success() {
    exit(EXIT_SUCCESS);
}

int yyerror( char *s ) {
  printf("parse error: %s\n",s);
  return 1;
}

int main() {
    printf("Simple LR(1) grammar based calculator.\n");
    yyparse();
    return EXIT_SUCCESS;
}
