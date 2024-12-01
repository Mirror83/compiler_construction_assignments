#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

#define TRUE 1
#define FALSE 0
#define MAX_TOKENS 256
#define MAX_INT_LENGTH 10

typedef enum
{
    TOKEN_EOF = 0,
    TOKEN_PLUS,
    TOKEN_MULTIPLY,
    TOKEN_L_BRACKET,
    TOKEN_R_BRACKET,
    TOKEN_INT
} token_t;

union TokenVal
{
    int intVal; // Store the numbers of an integer as
    char charVal;
};

struct Token
{
    token_t tokenType;
    union TokenVal tokenVal;
};

int parse_P();
int parse_E();
int parse_E_prime();
int parse_T();
int parse_T_prime();
int parse_F();

FILE *input;

struct Token tokens[MAX_TOKENS];

/** The current position in the tokens list. */
int current_pos = -1;
size_t tokens_size = 0;

void tokenize(char *source, struct Token tokens[])
{
    int i = 0;
    size_t source_length = strlen(source);

    while (i < source_length)
    {
        struct Token token;

        char c = source[i];
        if (c == '(')
        {
            token.tokenType = TOKEN_L_BRACKET;
            token.tokenVal.charVal = c;
            tokens[tokens_size] = token;
            tokens_size += 1;
            i += 1;
            continue;
        }
        if (c == ')')
        {
            token.tokenType = TOKEN_R_BRACKET;
            token.tokenVal.charVal = c;
            tokens[tokens_size] = token;

            tokens_size += 1;

            i += 1;
            continue;
        }
        if (c == '+')
        {
            token.tokenType = TOKEN_PLUS;
            token.tokenVal.charVal = c;
            tokens[tokens_size] = token;

            tokens_size += 1;
            i += 1;
            continue;
        }
        if (c == '*')
        {
            token.tokenType = TOKEN_MULTIPLY;
            token.tokenVal.charVal = c;
            tokens[tokens_size] = token;

            tokens_size += 1;
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
            token.tokenType = TOKEN_INT;
            token.tokenVal.intVal = atoi(intChars);
            tokens[tokens_size] = token;

            tokens_size += 1;
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

    struct Token token;
    token.tokenType = TOKEN_EOF;
    token.tokenVal.charVal = '\0';
    tokens[tokens_size] = token;

    tokens_size += 1;
    printf("Token size: %d\n", tokens_size);
}

struct Token current_token()
{
    size_t next_index = current_pos + 1;
    if (next_index < tokens_size)
    {
        struct Token token = tokens[++current_pos];
        return token;
    }
    else
    {
        printf("Consumed all collected tokens.\n");
        exit(EXIT_FAILURE);
    }
}

/** Makes the current token be reconsidered in the next call to `current_token`, */
void putback_token()
{
    current_pos -= 1;
}

/** Calls `current_token` to retrieve the next token, and check if it matches the type given.
 * Returns `TRUE` if it matches, and `FALSE` otherwise.
 */
int expect_token(token_t token_type)
{
    struct Token other = current_token();
    if (token_type == other.tokenType)
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
    int e = parse_E();
    if (expect_token(TOKEN_EOF) == TRUE)
    {
        return e;
    }
    else
    {
        fprintf(stderr, "Parse error: Expected EOF, got something different.\n");
        exit(EXIT_FAILURE);
    }
}

int parse_E()
{
    int t = parse_T();
    int e_prime = parse_E_prime();
    if (e_prime != FALSE)
    {
        return t + e_prime;
    }
    else
    {
        return t;
    }
}

int parse_E_prime()
{
    struct Token t = current_token();
    if (t.tokenType == TOKEN_PLUS)
    {
        int t = parse_T();
        int e_prime = parse_E_prime();

        if (e_prime != FALSE)
        {
            return t + e_prime;
        }
        else
        {
            return t;
        }
    }
    else
    {
        putback_token(t);
        return FALSE;
    }
}

int parse_T()
{
    int f = parse_F();
    int t_prime = parse_T_prime();
    if (t_prime != FALSE)
    {
        return f * t_prime;
    }
    else
    {
        return f;
    }
}

int parse_T_prime()
{
    struct Token t = current_token();
    if (t.tokenType == TOKEN_MULTIPLY)
    {
        int f = parse_F();
        int t_prime = parse_T_prime();
        if (t_prime != FALSE)
        {
            return f * t_prime;
        }
        else
        {
            return f;
        }
    }
    else
    {
        putback_token(t);
        return FALSE;
    }
}

int parse_F()
{
    struct Token t = current_token();
    if (t.tokenType == TOKEN_L_BRACKET)
    {
        int e = parse_E();
        if (expect_token(TOKEN_R_BRACKET))
        {
            return e;
        }
        else
        {
            fprintf(stderr, "Parse Error: Expected ')'\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (t.tokenType == TOKEN_INT)
    {
        int value = t.tokenVal.intVal;
        return value;
    }
    else
    {
        printf("Parse error: unexpected token %c\n", t.tokenVal);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    char *program;
    if (argc == 2)
    {
        program = argv[1];
    }
    else
    {
        program = "(10+2)*5";
    }

    tokenize(program, tokens);
    int result = parse_P();
    printf("Result: %d\n", result);

    exit(EXIT_SUCCESS);
}
