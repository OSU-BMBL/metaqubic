#pragma once
#include "fileProcessing.h"

// main function for step1
void step1(const int argc, const char ** argv);
int checkParameter(const int argc, const char ** argv);
void step1_pipeline(const char * fileName, const char * referenceDatabase, const char * DNA_file1, const char * DNA_file2, const char * RNA_file1, const char * RNA_file2, const char * outputDirectory, const char * bowtie2_command, const char * bedtools_command, const char * samtools_command);

// main function for step2
void step2(const int argc, const char ** argv);

// main function for step3
void step3(const int argc, const char ** argv);

// main function for step4
void step4(const int argc, const char ** argv);

// main function for step5
void step5(const int argc, const char ** argv);