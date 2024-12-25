#include "recursive_descent.h"

void print_header()
{
    printf("Recusive descent calculator. (+, and *)\n");
    printf("---------------------------------------\n");
    printf("Enter 'quit' or 'CTR+D' and then press Enter to exit.\n\n");
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
