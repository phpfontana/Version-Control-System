#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "data_structures.h"
#include "utils.h"
#include "commands.h"

void vcs_iniciar()
{
    vcs_init();
    printInfo("Inicialized successfully!");
}

void vcs_adiciona(int argc, const char *argv[])
{
    for (int i = 2; i < argc; i++)
    {
        if (file_exists(argv[i]))
        {
            vcs_add(argv[i]);
            printInfo("File add to the Stage Area.");
        }
        else
        {
            printf("Arquivo não encontrado: %s", argv[i]);
        }
    }
}


int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("\n------ WELCOME TO THE VERSIONATOR.EXE 2.0------\nfor help, use './versionador help'\nFormat: versionador.exe <comando> [argumentos]\n\n");
        return 1;
    }

    // Command to be executed
    const char *command = argv[1];

    // Call the function that correspond to the command
    if (strcmp(command, "help") == 0)
    {
        printf("----------List of functions avaliable----------\n");
        printf("iniciar\n");
        printf("adicionar [File1] [File2] ... [FileN]\n");
        printf("registra [texto\n");
    }
    else if (strcmp(command, "iniciar") == 0)
    {
        vcs_iniciar();
    }
    else if (strcmp(command, "adiciona") == 0)
    {
        if (argc < 3)
        {
            printInfo("\nThis function expect at least one more argments [nameFile].\n");
            return 1;
        }
        vcs_adiciona(argc, argv);
    }
    else if (strcmp(command, "registra") == 0)
    {
        if (argc < 3)
        {
            printInfo("\nThis function expect at least one more argments with [Commit text message].\n");
            return 1;
        }
        const char *text = argv[2];
        vcs_commit(text);
    }
    else if (strcmp(command, "log") == 0)
    {
        if (argc == 2)
        {
            vcs_log();
        }
        else
        {
            for (int i = 2; i < argc; i++)
            {
                if (strcmp(argv[i], "--conteudo") == 0)
                {
                    vcs_log_content();
                }
                else
                {
                    printAlert("Invalid command!\n");
                }
            }
        }
    }

    else if (strcmp(command, "mostrar") == 0)
    {
        vcs_show(argv[2]);
    }
    else if (strcmp(command, "mudar") == 0)
    {
        if (argc == 2)
        {
            vcs_checkout(argv[2]);
        }
        else
        {
            for (int i = 2; i < argc; i++)
            {
                if (strcmp(argv[i], "--atual") == 0)
                {
                    vcs_checkout_current();
                }
                else
                {
                    printAlert("Invalid command!\n");
                }
            }
        }


        
    }
    else
    {
        printWarning("-----INVALID COMMAND!-----\n Try using 'help' for more information.\n");
    }

    return 0;
}



