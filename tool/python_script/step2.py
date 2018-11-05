# This script is used for matrix generation based on the catalogs

import os
import sys

# variable definiton
count = 0
catalog_735 = ""
catalog_new = ""
prefix = ""
output_path = ""

# parsing arguments
while count < len(sys.argv):
    if sys.argv[count] == '-c1' and os.path.isdir(sys.argv[count + 1]):
        catalog_735 = sys.argv[count + 1]
    elif  sys.argv[count] == '-c2' and os.path.isdir(sys.argv[count + 1]):
        catalog_new = sys.argv[count + 1]
    elif  sys.argv[count] == '-prefix':
        prefix = sys.argv[count + 1]
    elif  sys.argv[count] == '-o':
        output_path = sys.argv[count + 1]
        if os.path.isdir(output_path) ==False:
            os.system("mkdir " + output_path)
    count = count + 1

# check if the file under catalog directory is valid
fileNames_735 = os.listdir(catalog_735)
if len(fileNames_735) == 0: # if no file under the folder
    print "ERROR - Catalog directory is empty: " + catalog_path
    sys.exit()

fileNames_new = []
if len(catalog_new) > 0:
    fileNames_new = os.listdir(catalog_new)
    if len(fileNames_new) == 0: # if no file under the folder
        print "ERROR - Catalog directory is empty: " + catalog_path
        sys.exit()
    
# print out arguments for users
print "The path of 735 catalogs is " + catalog_735
print "The path of new catalogs is " + catalog_new
print "The prefix of the name of the matrices is " + prefix
print "The output path is " + output_path

# get full path of all the files
full_path_fileNames = []
for fileName in fileNames_735:
    full_path_fileNames.append(catalog_735 + "/" + fileName)

for fileName in  fileNames_new:
    full_path_fileNames.append(catalog_new + "/" + fileName)

# start forming DNA, RNA, R/D matrices
count_sample = 0
DNA_dictionary = {}
DNA_dictionary["geneName"] = "gene_name"
RNA_dictionary = {}
RNA_dictionary["geneName"] = "gene_name"
RNA_DNA_dictionary = {}
RNA_DNA_dictionary["geneName"] = "gene_name"
for fileName in full_path_fileNames:
    inFile = open(fileName)
    data = inFile.readlines()
    # append sample names in the matrix
    DNA_dictionary["geneName"] = DNA_dictionary["geneName"] + "\t" + os.path.basename(fileName)[:-21]
    RNA_dictionary["geneName"] = RNA_dictionary["geneName"] + "\t" + os.path.basename(fileName)[:-21]
    RNA_DNA_dictionary["geneName"] = RNA_DNA_dictionary["geneName"] + "\t" + os.path.basename(fileName)[:-21]
    # append the exact value
    for line in data:
        gene_name = line.split('\t')[0]
        dna_readCount = line.split('\t')[3]
        rna_readCount = line.split('\t')[6]
        rna_dna_readCount = line.split('\t')[9]
        # DNA read counts
        if gene_name not in DNA_dictionary:
            DNA_dictionary[gene_name] = gene_name + "\t0" * count_sample
        DNA_dictionary[gene_name] = DNA_dictionary[gene_name] + '\t' + dna_readCount
        # RNA read counts
        if gene_name not in RNA_dictionary:
            RNA_dictionary[gene_name] = gene_name + "\t0" * count_sample
        RNA_dictionary[gene_name] = RNA_dictionary[gene_name] + '\t' + rna_readCount
        # RNA/DNA
        if gene_name not in RNA_DNA_dictionary:
            RNA_DNA_dictionary[gene_name] = gene_name + "\t0" * count_sample
        RNA_DNA_dictionary[gene_name] = RNA_DNA_dictionary[gene_name] + '\t' + rna_dna_readCount
    count_sample = count_sample + 1

    # append 0 to the gene not mapped
    # fix DNA matrix
    for key in DNA_dictionary:
        if len(DNA_dictionary[key].split('\t')) < count_sample + 1:
            DNA_dictionary[key] = DNA_dictionary[key] + '\t' + "0"
    # fix RNA matrix
    for key in RNA_dictionary:
        if len(RNA_dictionary[key].split('\t')) < count_sample + 1:
            RNA_dictionary[key] = RNA_dictionary[key] + '\t' + "0"
    # fix RNA/DNA matrix
    for key in RNA_DNA_dictionary:
        if len(RNA_DNA_dictionary[key].split('\t')) < count_sample + 1:
            RNA_DNA_dictionary[key] = RNA_DNA_dictionary[key] + '\t' + "0"

# write matrices
# DNA
outFile_DNA = open(output_path + "/" + prefix + "raw_DNA_counts.txt", "w")
outFile_DNA.write(DNA_dictionary["geneName"] + "\n")
for key in DNA_dictionary:
    if key != "geneName":
        outFile_DNA.write(DNA_dictionary[key] + "\n")
# RNA
outFile_RNA = open(output_path + "/" + prefix + "raw_RNA_counts.txt", "w")
outFile_RNA.write(RNA_dictionary["geneName"] + "\n")
for key in RNA_dictionary:
    if key != "geneName":
        outFile_RNA.write(RNA_dictionary[key] + "\n")
# RNA/DNA
outFile_RNA_DNA = open(output_path + "/" + prefix + "raw_RDLN_counts.txt", "w")
outFile_RNA_DNA.write(RNA_DNA_dictionary["geneName"] + "\n")
for key in RNA_DNA_dictionary:
    if key != "geneName":
        outFile_RNA_DNA.write(RNA_DNA_dictionary[key] + "\n")