#include "recursive_descent.h"

void print_header()
{
    printf("Recusive descent calculator. (+, and *)\n");
    printf("---------------------------------------\n");
    printf("Enter 'quit' or 'CTR+D' and then press Enter to exit.\n\n");
}

void read(char *cmdline)
{
    printf(">> ");
    fgets(cmdline, MAX_TOKENS, stdin);

    if (feof(stdin))
    {
        exit(EXIT_SUCCESS);
    }
}

int evaluate(char *cmdline)
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

    return result;
}

int main(int argc, char **argv)
{
    char cmdline[MAX_TOKENS];

    print_header();
    while (TRUE)
    {
        read(cmdline);
        int result = evaluate(cmdline);
        printf("%d\n", result);
    }

    exit(EXIT_SUCCESS);
}
