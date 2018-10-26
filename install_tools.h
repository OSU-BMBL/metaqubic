#pragma once

char install_path[1000+1];

int install_tools(const char * tools_path);
int install_bowtie2();
int install_bedtools();
int install_samtools();
int install_python();
int install_qubic();