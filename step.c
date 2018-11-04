#include <stdio.h>
#include "step.h"
#include <string.h>

void step1(const int argc, const char ** argv)
{
    char arguments[1000+1];
    strncpy(arguments, install_path, 1000);
    strcat(arguments, " ");
    int count = 0;
    while (count < argc - 3)
    {
        strcat(arguments, argv[count]);
        strcat(arguments, " ");
        count++;
    }

    char command[1000+1];
    snprintf( command, sizeof( command ), "%s%s%s%s%s", install_path, "/python/bin/python ", install_path, "/python_script/step1.py ", arguments);
    system(command);
}
void step2(const int argc, const char ** argv)
{
    char arguments[1000+1];
    arguments[0] = '\0';
    int count = 0;
    while (count < argc - 3)
    {
        strcat(arguments, argv[count]);
        strcat(arguments, " ");
        count++;
    }

    char command[1000+1];
    snprintf( command, sizeof( command ), "%s%s%s%s%s", install_path, "/python/bin/python ", install_path, "/python_script/update_matrix.py ", arguments);
    system(command);
}

void step3(const int argc, const char ** argv)
{
    char arguments[1000+1];
    arguments[0] = '\0';
    int count = 0;
    while (count < argc - 3)
    {
        strcat(arguments, argv[count]);
        strcat(arguments, " ");
        count++;
    }

    char command[1000+1];
    snprintf( command, sizeof( command ), "%s%s%s%s%s", install_path, "/python/bin/python ", install_path, "/python_script/calc_TPM.py ", arguments);
    system(command);
}

void step4(const int argc, const char ** argv)
{
    char arguments[1000+1];
    arguments[0] = '\0';
    int count = 0;
    while (count < argc - 3)
    {
        strcat(arguments, argv[count]);
        strcat(arguments, " ");
        count++;
    }

    char command[1000+1];
    snprintf( command, sizeof( command ), "%s%s%s%s%s", install_path, "/python/bin/python ", install_path, "/python_script/biclustering.py ", arguments);
    system(command);
}

void step5(const int argc, const char ** argv)
{
    char arguments[1000+1];
    arguments[0] = '\0';
    int count = 0;
    while (count < argc - 3)
    {
        strcat(arguments, argv[count]);
        strcat(arguments, " ");
        count++;
    }

    char command[1000+1];
    snprintf( command, sizeof( command ), "%s%s%s%s%s", install_path, "/python/bin/python ", install_path, "/python_script/gene_enrichment.py ", arguments);
    system(command);
}