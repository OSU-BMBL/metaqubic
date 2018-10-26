#include "install_tools.h"
#include "fileProcessing.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>


int install_tools(const char * tools_path)
{
    strncpy(install_path, tools_path, 1000);
    if (isDirExist(install_path) == 0)
    {
        printf("ERROR - tools path is invalid. \n");
        system("exist");
    }

    
    printf("tool dir: %s\n", install_path);
    if(install_bowtie2() == 1 && install_bedtools() == 1 && install_samtools() == 1 && install_python() == 1 && install_qubic() == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int install_bowtie2()
{
    // get bowtie2 path
    char bowtie2_path[1000+1];
    strcpy(bowtie2_path, install_path);
    strcat(bowtie2_path, "/bowtie2");

    DIR* dir = opendir(bowtie2_path);
    if (dir)
    {
        /* Directory exists. */
        closedir(dir);
        printf("installing bowtie2. \n");

        // start use make to compile bowtie2
        char install_command[1000+1];
        snprintf( install_command, sizeof( install_command ), "%s%s%s", "cd ", bowtie2_path, " && make NO_TBB=1" );
        system(install_command);

        return 1;
    }
    else
    {
        printf("ERROR - couldn't find the bowtie2 directory, exit installing bowtie2\n");
        system("exist");
        return 0;
    }
}

int install_samtools()
{
    // get bowtie2 path
    char samtools_path[1000+1];
    strcpy(samtools_path, install_path);
    strcat(samtools_path, "/samtools");

    DIR* dir = opendir(samtools_path);
    if (dir)
    {
        /* Directory exists. */
        closedir(dir);
        printf("installing samtools.\n");

        // start use make to compile bowtie2
        char install_command[1000+1];
        snprintf( install_command, sizeof( install_command ), "%s%s%s", "cd ", samtools_path, " && make" );
        system(install_command);

        return 1;
    }
    else
    {
        printf("ERROR - couldn't find the samtools directory, exit installing samtools\n");
        system("exist");
        return 0;
    }
}

int install_bedtools()
{
    // get bedtools path
    char bedtools_path[1000+1];
    strcpy(bedtools_path, install_path);
    strcat(bedtools_path, "/bedtools");

    DIR* dir = opendir(bedtools_path);
    if (dir)
    {
        /* Directory exists. */
        closedir(dir);
        printf("installing bedtools. \n");

        // start use make to compile bedtools
        char install_command[1000+1];
        snprintf( install_command, sizeof( install_command ), "%s%s%s", "cd ", bedtools_path, " && make" );
        system(install_command);

        return 1;
    }
    else
    {
        printf("ERROR - couldn't find the bedtools2 directory, exit installing bedtools\n");
        system("exist");
        return 0;
    }
}

int install_python()
{
    // get bedtools path
    char python_path[1000+1];
    strcpy(python_path, install_path);
    strcat(python_path, "/python");
    char python_exe[1000+1];
    strncpy(python_exe, python_path, 1000);
    strcat(python_exe, "/python");

    DIR* dir = opendir(python_path);
    if (dir)
    {
        if(isFileExist(python_exe) == 0)
        {
            /* Directory exists. */
            closedir(dir);
            printf("installing bedtools. \n");

            // start use make to compile bedtools
            char install_command[1000+1];
            snprintf( install_command, sizeof( install_command ), "%s%s%s", "cd ", python_path, " && ./configure && make" );
            system(install_command);
        }

        return 1;
    }
    else
    {
        printf("ERROR - couldn't find the python directory, exit installing python\n");
        system("exist");
        return 0;
    }
}

int install_qubic()
{
    // get bedtools path
    char qubic_path[1000+1];
    strcpy(qubic_path, install_path);
    strcat(qubic_path, "/qubic");

    DIR* dir = opendir(qubic_path);
    if (dir)
    {
        /* Directory exists. */
        closedir(dir);
        printf("installing bedtools. \n");

        // start use make to compile bedtools
        char install_command[1000+1];
        snprintf( install_command, sizeof( install_command ), "%s%s%s", "cd ", qubic_path, " && make" );
        system(install_command);

        return 1;
    }
    else
    {
        printf("ERROR - couldn't find the qubic directory, exit installing qubic\n");
        system("exist");
        return 0;
    }
}