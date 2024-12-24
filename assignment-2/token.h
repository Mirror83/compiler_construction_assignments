#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_TOKENS 256

/** Can be either TRUE (1) or FALSE (0) */
typedef int boolean;

enum TokenType
{
    TOKEN_EOF = 0,
    TOKEN_PLUS,
    TOKEN_MULTIPLY,
    TOKEN_L_BRACKET,
    TOKEN_R_BRACKET,
    TOKEN_INT
};

union TokenVal
{
    int int_val;
    char char_val;
};

typedef struct Token
{
    enum TokenType token_type;
    union TokenVal token_val;
} Token;

typedef struct TokenList
{
    Token tokens[MAX_TOKENS];
    int current_position;
    size_t size;
} TokenList;

void add_token(Token token, TokenList *token_list)
{
    token_list->tokens[token_list->size] = token;
    token_list->size += 1;
}

Token current_token(TokenList *token_list)
{
    size_t next_index = token_list->current_position + 1;
    if (next_index <= token_list->size)
    {
        Token token = token_list->tokens[token_list->current_position];
        token_list->current_position += 1;
        return token;
    }
    else
    {
        printf("Consumed all collected tokens.\n");
        exit(EXIT_FAILURE);
    }
}

/** Makes the current token be reconsidered in the next call to `current_token`, */
void putback_token(TokenList *token_list)
{
    token_list->current_position -= 1;
}

/** Calls `current_token` to retrieve the next token, and check if it matches the type given.
 * Returns `TRUE` if it matches, and `FALSE` otherwise.
 */
boolean expect_token(enum TokenType token_type, TokenList *token_list)
{
    struct Token other = current_token(token_list);
    if (token_type == other.token_type)
    {
        return TRUE;
    }
    else
    {
        putback_token(token_list);
        return FALSE;
    }
}