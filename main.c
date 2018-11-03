#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "install_tools.h"
#include "fileProcessing.h"
#include "step.h"

int main(const int argc, const char ** argv)
{
    if(strcmp(argv[1], "--mapping") == 0)
    {
        printf("step 1 (Gene mapping and pairing) is processing.\n");
        
        // install mapping tools but don't need it right now
        // install_tools(argv[6]);
        step1(argc, &argv[2]);
    }
    else if(strcmp(argv[1], "--mergeMatrix") == 0)
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
        printf("step 5 (enrichment) is processing.\n");
        step5(argc, &argv[2]);
    }
    else
    {
        printf("wrong arguments\n");
    }
}