/**
 * Here is an implementation of a recursive descent parser for the grammar
 * of a calculator that can perform simple arithmetic involving "+" and "*".
 * The follwing is the grammar to be parsed:
 * P -> E
 * E -> TE'
 * E' -> +TE'
 * E' -> ε
 * T -> FT'
 * T' -> *FT'
 * T' -> ε
 * F -> (E)
 * F -> int
 *
 * Key
 * ----
 * P - Program
 * E - Expression
 * T - Term
 * F - Factor
 *
 * The grammar above is an LL(1) grammar, suitable for recursive descent parsing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"

#define MAX_INT_LENGTH 10

typedef struct ParseVal
{
    boolean is_epsilon;
    int val; // If is_epsilon is TRUE, the value here does not matter
} ParseVal;

int parse_P(TokenList *token_list);
int parse_E(TokenList *token_list);
ParseVal parse_E_prime(TokenList *token_list);
int parse_T(TokenList *token_list);
ParseVal parse_T_prime(TokenList *token_list);
int parse_F(TokenList *token_list);

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
            // Should be handled differently as
            // a number can consist of more than one character
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

int parse_P(TokenList *token_list)
{
    int e = parse_E(token_list);
    if (expect_token(TOKEN_EOF, token_list) == TRUE)
    {
        return e;
    }
    else
    {
        fprintf(stderr, "Parse error: Expected EOF, got something different.\n");
        exit(EXIT_FAILURE);
    }
}

int parse_E(TokenList *token_list)
{
    int t = parse_T(token_list);
    ParseVal e_prime = parse_E_prime(token_list);
    if (e_prime.is_epsilon == FALSE)
    {
        return t + e_prime.val;
    }
    else
    {
        return t;
    }
}

ParseVal parse_E_prime(TokenList *token_list)
{
    struct Token t = current_token(token_list);
    if (t.token_type == TOKEN_PLUS)
    {
        int t = parse_T(token_list);
        ParseVal e_prime = parse_E_prime(token_list);

        if (e_prime.is_epsilon == FALSE)
        {
            return (ParseVal){FALSE, t + e_prime.val};
        }
        else
        {
            return (ParseVal){FALSE, t};
        }
    }
    else
    {
        putback_token(token_list);
        return (ParseVal){TRUE, 0};
    }
}

int parse_T(TokenList *token_list)
{
    int f = parse_F(token_list);
    ParseVal t_prime = parse_T_prime(token_list);
    if (t_prime.is_epsilon == FALSE)
    {
        return f * t_prime.val;
    }
    else
    {
        return f;
    }
}

ParseVal parse_T_prime(TokenList *token_list)
{
    struct Token t = current_token(token_list);
    if (t.token_type == TOKEN_MULTIPLY)
    {
        int f = parse_F(token_list);
        ParseVal t_prime = parse_T_prime(token_list);
        if (t_prime.is_epsilon == FALSE)
        {
            ParseVal parseVal = {FALSE, f * t_prime.val};
            return parseVal;
        }
        else
        {
            return (ParseVal){FALSE, f};
        }
    }
    else
    {
        putback_token(token_list);
        return (ParseVal){TRUE, 0};
    }
}

int parse_F(TokenList *token_list)
{
    struct Token t = current_token(token_list);
    if (t.token_type == TOKEN_L_BRACKET)
    {
        int e = parse_E(token_list);
        if (expect_token(TOKEN_R_BRACKET, token_list))
        {
            return e;
        }
        else
        {
            fprintf(stderr, "Parse Error: Expected ')'\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (t.token_type == TOKEN_INT)
    {
        int value = t.token_val.int_val;
        return value;
    }
    else
    {
        printf("Parse error: unexpected token %c\n", t.token_val);
        exit(EXIT_FAILURE);
    }
}

void evaluate(char *cmdline)
{
    if (strcmp(cmdline, "quit\n") == 0)
    {
        exit(EXIT_SUCCESS);
    }

    TokenList list;
    list.size = 0;
    list.current_position = 0;

    tokenize(cmdline, &list);
    int result = parse_P(&list);
    printf("%d\n", result);
}