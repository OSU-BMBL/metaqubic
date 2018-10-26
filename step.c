#include <stdio.h>
#include "step.h"
#include <string.h>
#include "install_tools.h"

void step1(const int argc, const char ** argv)
{
    
    // check error for parameter 
    int paramenterStatus = checkParameter(argc, argv);
    if(paramenterStatus == 1) // paramenter is valid, start processing
    {
        // variable intialization
        char dataset_path[1000+1];
        strcpy(dataset_path, argv[0]);
        char IGC_database_path[1000+1];
        strcpy(IGC_database_path, argv[1]);
        char IGC_annotation_table[1000+1];
        strcpy(IGC_annotation_table, argv[2]);
        char output_path[1000+1];
        strcpy(output_path, argv[3]);
        char DNA_file1[1000+1];
        char DNA_file2[1000+1];
        char RNA_file1[1000+1];
        char RNA_file2[1000+1];


        int numberOfDataset = getNumberOfDir(dataset_path);
        struct dataSet dataSetList[numberOfDataset];
        getFilelist(dataset_path, dataSetList); // the first two elements are system files

        int i = 0; // loop counter
        for(i = 0; i < numberOfDataset; i++)
        {
            // get DNA_file1
            strncpy(DNA_file1, dataSetList[i].fileName, 1000);
            strcat(DNA_file1, "/DNA/");
            strcat(DNA_file1, dataSetList[i].DNA_1);
            
            // get DNA_file2
            if(strlen(dataSetList[i].DNA_2) != 1) // if pair-end
            {
                strncpy(DNA_file2, dataSetList[i].fileName, 1000);
                strcat(DNA_file2, "/DNA/");
                strcat(DNA_file2, dataSetList[i].DNA_2);
            }
            else // if single-end
            {
                DNA_file2[0] = '\0';
            }

            // get RNA_file1
            strncpy(RNA_file1, dataSetList[i].fileName, 1000);
            strcat(RNA_file1, "/RNA/");
            strcat(RNA_file1, dataSetList[i].RNA_1);

            // get RNA_file2
            if(strlen(dataSetList[i].RNA_2) != 1) // if pair-end
            {
                strncpy(RNA_file2, dataSetList[i].fileName, 1000);
                strcat(RNA_file2, "/RNA/");
                strcat(RNA_file2, dataSetList[i].RNA_2);
            }
            else // if single-end
            {
                RNA_file2[0] = '\0';
            }

            // call the pipeline to do gene mapping
            step1_pipeline( dataSetList[i].fileName, IGC_database_path, DNA_file1, DNA_file2, RNA_file1, RNA_file2, output_path); 
        }
        // call python script to add annotation to the new catalogs
        char command[1000+1];
        snprintf( command, sizeof( command ), "%s%s%s%s%s%s%s", install_path, "/python/python ", install_path, "/python_script/add_annotation_to_new_catalog.py ", IGC_annotation_table, " ", output_path);
        system(command);
    }
}

int checkParameter(const int argc, const char ** argv)
{
    // check new paired MG MT directory argv[0]
    // check IGC database file argv[1]
    // check IGC database annotation table argv[2]
    // check output file argv[3]
    if(isDirExist(argv[0]) == 1 && isFileExist(argv[1]) && isFileExist(argv[2]) && isDirExist(argv[3]))
    {
        return 1;
    }
    else
    {
        if (isDirExist(argv[0]) == 0)
        {
            printf("ERROR - The path of MG MT directory is invalid: %s\n", argv[0]);
        }
        if (isDirExist(argv[1]) == 0)
        {
            printf("ERROR - The path of IGC reference database is invalid: %s\n", argv[1]);
        }
        if (isDirExist(argv[2]) == 0)
        {
            printf("ERROR - The path of IGC annotation table is invalid: %s\n", argv[2]);
        }
        if (isDirExist(argv[3]) == 0)
        {
            printf("ERROR - The path of output Directory is invalid: %s\n", argv[3]);
        }
        return 0;
    }
}

void step1_pipeline(const char * fileName, const char * referenceDatabase, const char * DNA_file1, const char * DNA_file2, const char * RNA_file1, const char * RNA_file2, const char * outputDirectory)
{
    char command[1000+1];  // initialize the command string for linux command
    char DNA_outputName[1000+1] = "metagenome";
    char RNA_outputName[1000+1] = "metatranscriptome";
    
    // get bowtie directory path
    char Bowtie2_dir[1000+1];
    strncpy(Bowtie2_dir, install_path, 1000);
    strcat(Bowtie2_dir, "/bowtie2");

    // get bedtools directory path
    char Bedtools_dir[1000+1];
    strncpy(Bedtools_dir, install_path, 1000);
    strcat(Bedtools_dir, "/bedtools/bin");

    // get samtools directory path
    char Samtools_dir[1000+1];
    strncpy(Samtools_dir, install_path, 1000);
    strcat(Samtools_dir, "/samtools");

    char true_fileName[1000+1]; // the variable only store the filename, not the whole path
    char true_output_path[1000+1];

    if(isFileExist(DNA_file1) && isFileExist(RNA_file1)) // the MG and MT dataset should be both exist
    {
        // create directory for dataset in the output directory
        strncpy(true_fileName, fileName, 1000);
        strncpy(true_fileName, basename(true_fileName), 1000);
        snprintf( command, sizeof( command ), "%s%s%s", "mkdir ", outputDirectory, "/alignment_result" );
        system(command);
        snprintf( true_output_path, sizeof( true_output_path ), "%s%s%s", outputDirectory, "/alignment_result/", true_fileName );
        snprintf( command, sizeof( command ), "%s%s", "mkdir ", true_output_path );
        system(command);
    
        // create intermediate directory for the intermeidate files
        char referenceDatabase_dir[1000+1];
        strncpy(referenceDatabase_dir, referenceDatabase, 1000);
        dirname(referenceDatabase_dir);
        char build_index_directory[1000+1];
        strncpy(build_index_directory, referenceDatabase_dir, 1000);
        strcat(build_index_directory, "/build_index");

        if (isDirExist(build_index_directory) == 0) // create directory to store the index files generated by Bowtie2
        {
            snprintf( command, sizeof( command ), "%s%s", "mkdir ", build_index_directory );
            system(command);

            //Build Bowtie index from a set of DNA sequence
            snprintf( command, sizeof( command ), "%s%s%s%s%s%s", Bowtie2_dir, "/bowtie2-build --large-index ", referenceDatabase, " ",  build_index_directory, "/Model" );
            system(command);
        }

        // processing metagenome dataset

        // bowtie2
        if(strlen(DNA_file2) == 0) // if metagenome is single-end 
        {
            snprintf( command, sizeof( command ), "%s%s%s%s%s%s%s%s%s%s", Bowtie2_dir, "/bowtie2 -x ", build_index_directory, "/Model -U ",  DNA_file1, " -S ", true_output_path, "/", DNA_outputName, ".sam");
            system(command);
        }
        else // if metagenome is pair-end
        {
            snprintf( command, sizeof( command ), "%s%s%s%s%s%s%s%s%s%s%s%s", Bowtie2_dir, "/bowtie2 -x ", build_index_directory, "/Model -1 ",  DNA_file1, " -2 ", DNA_file2, " -S ", true_output_path, "/", DNA_outputName, ".sam");
            system(command);
        }

        // samtools
        snprintf( command, sizeof( command ), "%s%s%s%s%s%s%s%s%s%s", Samtools_dir, "/samtools view -bS ", true_output_path, "/", DNA_outputName, ".sam > ", true_output_path, "/", DNA_outputName, ".bam");
        system(command);
        snprintf( command, sizeof( command ), "%s%s", referenceDatabase, ".bed");
        if(isFileExist(command) == 0) // create IGC.fa.bed
        {
            printf("hello\n");
            snprintf( command, sizeof( command ), "%s%s%s%s%s", "awk '/^>/ {if (seqlen){print \"0\t\"seqlen}; gsub(/^>/,\"\",$1);printf(\"%s\t\",$1) ;seqlen=0;next; } { seqlen += length($0)}END{print \"0\t\"seqlen}' ", referenceDatabase, " > ", referenceDatabase, ".bed");
            system(command);
        }

        // bedtools
        snprintf( command, sizeof( command ), "%s%s%s%s%s%s%s%s%s%s%s%s", Bedtools_dir, "/bedtools coverage -abam ", true_output_path, "/", DNA_outputName, ".bam -b ", referenceDatabase, ".bed > ", true_output_path, "/", DNA_outputName, ".coverage");
        system(command);
        
        // // processing metatranscriptome dataset

        // bowtie2
        if(strlen(RNA_file2) == 0) // if metatranscriptome is single-end
        {
            snprintf( command, sizeof( command ), "%s%s%s%s%s%s%s%s%s%s", Bowtie2_dir, "/bowtie2 -x ", build_index_directory, "/Model -U ",  RNA_file1, " -S ", true_output_path, "/", RNA_outputName, ".sam");
            system(command);
        }
        else
        {
            snprintf( command, sizeof( command ), "%s%s%s%s%s%s%s%s%s%s%s%s", Bowtie2_dir, "/bowtie2 -x ", build_index_directory, "/Model -1 ",  RNA_file1, " -2 ", RNA_file2, " -S ", true_output_path, "/", RNA_outputName, ".sam");
            system(command);
        }

        //samtools
        snprintf( command, sizeof( command ), "%s%s%s%s%s%s%s%s%s%s", Samtools_dir, "/samtools view -bS ", true_output_path, "/", RNA_outputName, ".sam > ", true_output_path, "/", RNA_outputName, ".bam");
        system(command);
        snprintf( command, sizeof( command ), "%s%s", referenceDatabase, ".bed");
        if(isFileExist(command) == 0) // create IGC.fa.bed
        {
            snprintf( command, sizeof( command ), "%s%s%s%s%s", "awk '/^>/ {if (seqlen){print \"0\t\"seqlen}; gsub(/^>/,\"\",$1);printf(\"%s\t\",$1) ;seqlen=0;next; } { seqlen += length($0)}END{print \"0\t\"seqlen}' ", referenceDatabase, " > ", referenceDatabase, ".bed");
            system(command);
        }
        
        // bedtools
        snprintf( command, sizeof( command ), "%s%s%s%s%s%s%s%s%s%s%s%s", Bedtools_dir, "/bedtools coverage -abam ", true_output_path, "/", RNA_outputName, ".bam -b ", referenceDatabase, ".bed > ", true_output_path, "/", RNA_outputName, ".coverage");
        system(command);
        
        // merge dna and rna mapping result
        snprintf( command, sizeof( command ), "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", "bash -c \"", "join -j 1 -o 1.1,1.2,1.3,1.4,1.5,1.7,2.4,2.5,2.7 ", " <(sort -k1 ", true_output_path, "/", DNA_outputName, ".coverage) <(sort -k1 ", true_output_path, "/", RNA_outputName, ".coverage) > ", true_output_path, "/", DNA_outputName, "_", RNA_outputName, ".coverage", "\"");
        system(command);

        snprintf( command, sizeof( command ), "%s%s%s%s%s%s%s%s%s%s%s%s%s", "awk '{if ($3 + 0 != 0 && $6 + 0 != 0 && $9 + 0 != 0) print $1 \"\t\" $2 \"\t\" $3 \"\t\" $4 \"\t\" $5 \"\t\" $6 \"\t\" $7 \"\t\" $8 \"\t\" $9 \"\t\" (($7 / $3) / ($4 / $3)) \"\t\" (($7 / $9) / ($4 / $6))}' ", true_output_path, "/", DNA_outputName, "_", RNA_outputName, ".coverage > ", true_output_path, "/", DNA_outputName, "_", RNA_outputName, "_integrative_coverage");
        system(command);
        
        snprintf( command, sizeof( command ), "%s%s%s%s%s%s%s", "rm ", true_output_path, "/", DNA_outputName, "_", RNA_outputName, ".coverage");
        system(command);
    }

}

void step2(const int argc, const char ** argv)
{
    char command[1000+1];
    snprintf( command, sizeof( command ), "%s%s%s%s %s %s %s %s %s", install_path, "/python/python ", install_path, "/python_script/update_matrix.py ", argv[0], argv[1], argv[2], argv[3], argv[4]);
    system(command);
}

void step3(const int argc, const char ** argv)
{
    char command[1000+1];
    snprintf( command, sizeof( command ), "%s%s%s%s %s %s %s %s %s %s ", install_path, "/python/python ", install_path, "/python_script/calc_TPM.py ", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
    system(command);
}

void step4(const int argc, const char ** argv)
{
    char arguments[1000+1];
    arguments[0] = '\0';
    int count = 0;
    while (count < argc - 2)
    {
        strcat(arguments, argv[count]);
        strcat(arguments, " ");
        count++;
    }

    char command[1000+1];
    snprintf( command, sizeof( command ), "%s%s%s%s%s", install_path, "/python/python ", install_path, "/python_script/biclustering.py ", arguments);
    system(command);
}

void step5(const int argc, const char ** argv)
{
    char arguments[1000+1];
    arguments[0] = '\0';
    int count = 0;
    while (count < argc - 2)
    {
        strcat(arguments, argv[count]);
        strcat(arguments, " ");
        count++;
    }

    char command[1000+1];
    snprintf( command, sizeof( command ), "%s%s%s%s%s", install_path, "/python/python ", install_path, "/gene_enrichment.py ", arguments);
    system(command);
}