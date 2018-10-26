#include "fileProcessing.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int isFileExist(const char * fileName)
{
    if (0 == access(fileName, 0))
    { 
        return 1;
    } 
    else 
    { 
        return 0;
    }
}

void getFilelist(const char * DirectoryPath, struct dataSet dataSetList[])
{
    DIR *dir;
    struct dirent *ent;

    if (strlen(DirectoryPath) == 0)
    {
        printf("invalid directory path.\n");
        system("exit");
    }

    if ((dir = opendir (DirectoryPath)) != NULL) 
    {
        /* get all the files and directories within directory */
        int fileCounter = 0;
        int dataSetCounter = 0;
        char fileName[1000+1];
        char temp[1000+1];
        while ((ent = readdir (dir)) != NULL) 
        {
            if (fileCounter > 1)
            {
                strncpy(fileName, ent->d_name, 1000);
                snprintf( temp, sizeof( temp ), "%s%s%s", DirectoryPath, "/", fileName);
                strncpy(fileName, temp, 1000);
                strncpy(dataSetList[dataSetCounter].fileName, fileName, 1000);
                dataSetCounter++;
            }
            fileCounter++;
        }
        closedir (dir);

        // check if DNA OR RNa file exist
        int i = 0;
        char temp_DNA_fileName[1000+1];
        char temp_RNA_fileName[1000+1];

        for (i = 0; i < dataSetCounter; i++)
        {
            temp_DNA_fileName[0] = '\0';
            temp_RNA_fileName[0] = '\0';
            strncpy(temp_DNA_fileName, dataSetList[i].fileName, 1000);
            strcat(temp_DNA_fileName, "/DNA");
            dataSetList[i].DNA = isDirExist(temp_DNA_fileName);
            if(dataSetList[i].DNA == 1)
            {
                dir = opendir(temp_DNA_fileName);
                fileCounter = 0;
                fileName[0] = '\0';
                temp[0] = '\0';
                while ((ent = readdir (dir)) != NULL) 
                {
                    if (strstr(ent->d_name, "2.") == NULL)
                    {
                        strncpy(dataSetList[i].DNA_1, ent->d_name, 1000);
                    }
                    if (strstr(ent->d_name, "2."))
                    {
                        strncpy(dataSetList[i].DNA_2, ent->d_name, 1000);
                    }
                    fileCounter++;
                }
                closedir (dir);
            }

            strncpy(temp_RNA_fileName, dataSetList[i].fileName, 1000);
            strcat(temp_RNA_fileName, "/RNA");
            dataSetList[i].RNA = isDirExist(temp_RNA_fileName);
            if(dataSetList[i].RNA == 1)
            {
                dir = opendir(temp_RNA_fileName);
                fileCounter = 0;
                fileName[0] = '\0';
                temp[0] = '\0';
                while ((ent = readdir (dir)) != NULL) 
                {
                    if (strstr(ent->d_name, "2.") == NULL)
                    {
                        strncpy(dataSetList[i].RNA_1, ent->d_name, 1000);
                    }
                    if (strstr(ent->d_name, "2."))
                    {
                        strncpy(dataSetList[i].RNA_2, ent->d_name, 1000);
                    }
                    fileCounter++;
                }
                closedir (dir);
            }
        }
    } 
    else 
    {
        /* could not open directory */
        printf("directory: not exist: %s\n", DirectoryPath);
        system("exit");
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

int getNumberOfDir(const char * DirectoryPath)
{
    DIR *dir;
    struct dirent *ent;

    if (strlen(DirectoryPath) == 0)
    {
        printf("invalid directory path.\n");
        system("exit");
    }

    if ((dir = opendir (DirectoryPath)) != NULL) 
    {
        int fileCounter = 0;
        while ((ent = readdir (dir)) != NULL) 
        {
            fileCounter++;
        }
        closedir (dir);

        return fileCounter - 2;
    } 
    else 
    {
        /* could not open directory */
        printf("directory: not exist: %s\n", DirectoryPath);
        system("exit");
        return 0;
    }
}

int getRow(const char *filename)
{
   int ch = 0;
   int count = 0;    
   FILE *fileHandle;

   if ((fileHandle = fopen(filename, "r")) == NULL) {
      return -1;
   }

   do {
      ch = fgetc(fileHandle);
      if ( ch == '\n')
         count++;
   } while (ch != EOF);

   fclose(fileHandle);

   return count;
}