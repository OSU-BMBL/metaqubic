#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "step.h"

int isDirExist(const char * DirectoryPath);

int main(const int argc, const char ** argv)
{
    
    strcat(install_path, "/tool/");

    // insert tool path to the argv[1]


    if (isDirExist(install_path) == 0)
    {
        printf("ERROR - Tool path is invalid: %s\n", install_path);
        exit(0);
    }

    if(strcmp(argv[1], "--mapping") == 0)
    {
        printf("step 1 (Gene mapping and pairing) is processing.\n");
        step1(argc, &argv[2]);
    }
    else if(strcmp(argv[1], "--CatToMat") == 0)
    {
        printf("step 2 (merge catalog to the matrix) is processing.\n");
        step2(argc, &argv[2]);
    }
    else if(strcmp(argv[1], "--filtering") == 0)
    {
        printf("step 3 (TPM normalizationa dn filtering) is processing.\n");
        step3(argc, &argv[2]);
    }
    else if(strcmp(argv[1], "--biclustering") == 0)
    {
        printf("step 4 (biclustering) is processing.\n");
        step4(argc, &argv[2]);
    }
    else if(strcmp(argv[1], "--enrichment") == 0)
    {
        printf("step 5 (enrichment analysis) is processing.\n");
        step5(argc, &argv[2]);
    }
    else if(strcmp(argv[1], "--bicToSeq") == 0)
    {
        printf("Link to HUManN2 is processing.\n");
        step6(argc, &argv[2]);
    }
    else
    {
        printf("wrong arguments\n");
    }
}



int isDirExist(const char * DirectoryPath)
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (DirectoryPath)) != NULL) 
    {
        /* print all the files and directories within directory */
        closedir (dir);
        return 1;
    } 
    else 
    {
        /* could not open directory */
        return 0;
    }
}