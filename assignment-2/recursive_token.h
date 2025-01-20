#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define MAX_TOKENS 256
#define MAX_INT_LENGTH 10

/** `boolean` should be either TRUE (1) or FALSE (0).
 * This is the intended usage of this `int` alias,
 * but it is not enforced by the compiler. */
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

/** `TokenVal` stores the value of a token. The union will contain an
 * integer value (stored in int_val) if we encounter an integer token,
 * or a char (stored in char_val) when we encounter
 * an arithmetic sign, a bracket, or EOF (in wich case the value stored)
 * in char_val would be the null termination character '\0'
 */
union TokenVal
{
    int int_val;
    char char_val;
};

/** `Token` represents allowable symbols of our grammar
 * for arithmetic experssions.
 * (see [`recursive_descent.h`](recursive_descent.h) for the grammar)
 */
typedef struct Token
{
    enum TokenType token_type;
    union TokenVal token_val;
} Token;

/** `TokenList` brings together the list storing the token, together with
 * the number of elements in the token (`size`) and the position of the token
 * to be considered (`current_position`)
 */
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

/** `current_token` returns the current token under consideration, and
 * increments the `current_position` attribute of `token_list`, allowing
 * the next token to be considered.
 * It exits with failure if there are no more tokens to consider.
 */
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

/** Makes the previous current token be reconsidered in the next call to `current_token`, */
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

/** `tokenize` takes the string represented by `source`, derives `Token`s
 * from them and updates `token_list` as each token is derived.
 * The routine exits with failure if an invalid symbol is encountered
 */
void tokenize(char *source, TokenList *token_list)
{
    int i = 0;
    size_t source_length = strlen(source);

    while (i < source_length)
    {
        struct Token token;

        char c = source[i];
        if (c == '(')
        {
            token.token_type = TOKEN_L_BRACKET;
            token.token_val.char_val = c;
            add_token(token, token_list);
            i += 1;
            continue;
        }
        if (c == ')')
        {
            token.token_type = TOKEN_R_BRACKET;
            token.token_val.char_val = c;
            add_token(token, token_list);
            i += 1;
            continue;
        }
        if (c == '+')
        {
            token.token_type = TOKEN_PLUS;
            token.token_val.char_val = c;
            add_token(token, token_list);
            i += 1;
            continue;
        }
        if (c == '*')
        {
            token.token_type = TOKEN_MULTIPLY;
            token.token_val.char_val = c;
            add_token(token, token_list);
            i += 1;
            continue;
        }
        if (isdigit(c))
        {
            char intChars[MAX_INT_LENGTH];
            int j = 0;
            while (isdigit(c))
            {
                if (j < MAX_INT_LENGTH - 1)
                {
                    intChars[j] = c;
                    intChars[j + 1] = '\0';
                    j += 1;
                }
                else
                {
                    fprintf(stderr, "Tokenizer error: You have a number that is longer than %d\n", MAX_INT_LENGTH - 1);
                    exit(EXIT_FAILURE);
                }

                i += 1;
                if (i == source_length)
                    break;

                c = source[i];
            }
            token.token_type = TOKEN_INT;
            token.token_val.int_val = atoi(intChars);
            add_token(token, token_list);

            continue;
        }

        if (isspace(c))
        {
            i += 1;
            continue;
        }

        printf("Unexpected character: %c", c);
        exit(EXIT_FAILURE);
    }

    Token token;
    token.token_type = TOKEN_EOF;
    token.token_val.char_val = '\0';
    add_token(token, token_list);
}