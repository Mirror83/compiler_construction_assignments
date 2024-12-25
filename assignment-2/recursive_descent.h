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
 *
 * The routines are named according to the production they parse.
 * They move through the token list as needed to do so.
 */

#include "token.h"

/** `ParseVal` is the return type for productions that can produce ε return a.
 * Whether or not ε was produced*/
typedef struct ParseVal
{
    boolean is_epsilon;
    int val; // If `is_epsilon` is TRUE, the value here does not matter
} ParseVal;

int parse_P(TokenList *token_list);

int parse_E(TokenList *token_list);

ParseVal parse_E_prime(TokenList *token_list);

int parse_T(TokenList *token_list);

ParseVal parse_T_prime(TokenList *token_list);

int parse_F(TokenList *token_list);

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
