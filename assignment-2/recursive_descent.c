#include "recursive_descent.h"

extern struct Token tokens[MAX_TOKENS];

void evaluate(char *cmdline)
{
    if (strcmp(cmdline, "quit\n") == 0)
    {
        exit(EXIT_SUCCESS);
    }

    tokenize(cmdline, tokens);
    int result = parse_P();
    printf("%d\n", result);
}

void print_header()
{
    printf("Recusive descent calculator. (+, and *)\n");
    printf("---------------------------------------\n");
    printf("Enter 'quit' or 'CTR+D' and then press enter to exit.\n\n");
}

int main(int argc, char **argv)
{
    char cmdline[MAX_TOKENS];

    print_header();
    while (TRUE)
    {
        printf(">> ");
        fgets(cmdline, MAX_TOKENS, stdin);
        if (feof(stdin))
        {
            exit(EXIT_SUCCESS);
        }
        evaluate(cmdline);
    }

    exit(EXIT_SUCCESS);
}
