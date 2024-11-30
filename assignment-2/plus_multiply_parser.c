#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    TOKEN_EOF = 0,
    TOKEN_PLUS,
    TOKEN_MULTIPLY,
    TOKEN_L_BRACKET,
    TOKEN_R_BRACKET,
    TOKEN_INT
} token_t;

#define TRUE 1
#define FALSE 0

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

FILE *input;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: plus_multiply_parser file_name\n");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    input = fopen(filename, "r");

    if (!input)
    {
        fprintf(stderr, "Could not open file at %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int result = parse_P();
    printf("Parse results: %d", result);

    fclose(input);
    exit(EXIT_SUCCESS);
}

/** Returns the next token on the input stream. */
token_t *scan_token()
{
    char c = fgetc(input);
    if (c == "*")
    {
        return TOKEN_MULTIPLY;
    }
    else if (c == "+")
    {
        return TOKEN_PLUS;
    }
    else if (isalnum(c))
    {
        return TOKEN_INT;
    }
    else if (c == " ")
    {
    }
    else if (c == NULL)
    {
        return TOKEN_EOF;
    }
    else
    {
        printf("Encountered unexpected token: %s\n", c);
        exit(EXIT_FAILURE);
    }
}

/** Puts an unexpected token back on the input stream,
 *  where it will be read again by the next call to `scan_token` */
void putback_token(token_t t) {}

/** Calls `scan_token` to retrieve the next token.
 * It returns `true` (1) if the token matches the expected type. If not,
 * it puts the token back on the input stream and returns `false` (0).*/
int expect_token(token_t t)
{
    token_t other = scan_token();
    if (t == other)
    {
        return TRUE;
    }
    else
    {
        putback_token(other);
        return FALSE;
    }
}

int parse_P()
{
    return parse_E() && expect_token(TOKEN_EOF);
}

int parse_E()
{
    return parse_T() && parse_E_prime();
}

int parse_E_prime()
{
    token_t t = scan_token();
    if (t == TOKEN_PLUS)
    {
        return parse_T() && parse_E_prime();
    }
    else
    {
        putback_token(t);
        return TRUE;
    }
}

int parse_T()
{
    return parse_F() && parse_T_prime();
}

int parse_T_prime()
{
    token_t t = scan_token();
    if (t == TOKEN_MULTIPLY)
    {
        return parse_F() && parse_T_prime();
    }
    else
    {
        putback_token(t);
        return TRUE;
    }
}

int parse_F()
{
    token_t t = scan_token();
    if (t == TOKEN_L_BRACKET)
    {
        return parse_E() && expect_token(TOKEN_R_BRACKET);
    }
    else if (t == TOKEN_INT)
    {
        return TRUE;
    }
    else
    {
        printf("Parse error: unexpected token %s\n", token_string(t));
        return FALSE;
    }
}
