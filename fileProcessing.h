#pragma once
#include <dirent.h>
#include <stdio.h>
#include <libgen.h>


struct dataSet
{
    char fileName[1000+1];
    int DNA;
    char DNA_1[1000+1];
    char DNA_2[1000+1];
    int RNA;
    char RNA_1[1000+1];
    char RNA_2[1000+1];
};

int isFileExist(const char * fileName); // check if the file exist, if the file path is provided
void getFilelist(const char * DirectoryPath, struct dataSet dataSetList[]); // get file names list under given directory path
int isDirExist(const char * DirectoryPath); // check if the directory exist, if the directory path is provided
int getNumberOfDir(const char * DirectoryPath);  // get the number of directory under the given directory path
int getRow(const char *filename); // get the number of rows from a file