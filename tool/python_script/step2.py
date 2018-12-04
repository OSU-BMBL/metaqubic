# This script is used for matrix generation based on the catalogs

import os
import sys
import ntpath

# function definition
def mergeUserCatalog():
    # variable definiton
    count = 0
    matrix_folder = ""
    catalog_new = ""
    output_path = ""
    DNA_matrix = ""
    RNA_matrix = ""
    RDLN_matrix = ""

    # parsing arguments
    while count < len(sys.argv):
        if sys.argv[count] == '-m' and os.path.isdir(sys.argv[count + 1]):
            matrix_folder = sys.argv[count + 1]
        elif sys.argv[count] == '-i' and os.path.isdir(sys.argv[count + 1]):
            catalog_new = sys.argv[count + 1]
        elif  sys.argv[count] == '-o':
            output_path = sys.argv[count + 1]
            if os.path.isdir(output_path) ==False:
                os.system("mkdir " + output_path)
        count = count + 1

    # get 3 matrices names
    fileNames = os.listdir(matrix_folder)
    if len(fileNames) != 3:
        print "ERROR - only DNA, RNA, RDRPK matrix in the folder:" + matrix_folder
        sys.exit()
    else:
        for fileName in fileNames:
            if fileName.find("DNA") != -1:
                DNA_matrix = fileName
            elif fileName.find("RNA") != -1:
                RNA_matrix = fileName
            elif fileName.find("RDRPK") != -1:
                RDLN_matrix = fileName
            else:
                print "ERROR - can't recognize this file: " + fileName
    
    if os.path.exists(matrix_folder + "/" + DNA_matrix) == False:
        print "ERROR - missing DNA expression matrix"
        sys.exit()
    if os.path.exists(matrix_folder + "/" + RNA_matrix) == False:
        print "ERROR - missing RNA expression matrix"
        sys.exit()
    if os.path.exists(matrix_folder + "/" + RDLN_matrix) == False:
        print "ERROR - missing RDRPK expression matrix"
        sys.exit()


    # check argument
    # check if the file under catalog directory is valid
    fileNames_new = []
    if len(catalog_new) > 0:
        fileNames_new = os.listdir(catalog_new)
        if len(fileNames_new) == 0: # if no file under the folder
            print "ERROR - Catalog directory is empty: " + catalog_path
            sys.exit()

    if os.path.isdir(matrix_folder) == False:
        print "ERROR - the path of matrix folder is invalid: " + matrix_folder
        sys.exit()
    if os.path.isdir(matrix_folder) == False:
        print "ERROR - the path of matrix folder is invalid: " + matrix_folder
        sys.exit()

    # print out arguments for users
    print "The path of matrix folder is " + matrix_folder
    print "The path of new catalogs is " + catalog_new
    print "The output path is " + output_path

    full_path_fileNames = []
    # get full path of all the files
    for fileName in  fileNames_new:
        full_path_fileNames.append(catalog_new + "/" + fileName)

    # start forming DNA, RNA, R/D matrices
    count_sample = 0
    DNA_dictionary = {}
    RNA_dictionary = {}
    RNA_DNA_dictionary = {}
    # get data from three matrices
    # DNA
    DNA_matrix_file = open(matrix_folder + "/" + DNA_matrix)
    DNA_data = DNA_matrix_file.readlines()
    count = 0
    while count < len(DNA_data):
        if count == 0:
            DNA_dictionary["sampleList"] = DNA_data[count][:-1] + "\t"
            count = count + 1
        else:
            geneName = DNA_data[count].split('\t')[0]
            DNA_dictionary[geneName] = DNA_data[count][:-1] + "\t"
            count = count + 1
    print "number of gene in DNA " + str(len(DNA_dictionary))
    # RNA
    RNA_matrix_file = open(matrix_folder + "/" + RNA_matrix)
    RNA_data = RNA_matrix_file.readlines()
    count = 0
    while count < len(RNA_data):
        if count == 0:
            RNA_dictionary["sampleList"] = RNA_data[count][:-1] + "\t"
            count = count + 1
        else:
            geneName = RNA_data[count].split('\t')[0]
            RNA_dictionary[geneName] = RNA_data[count][:-1] + "\t"
            count = count + 1
    print "number of gene in RNA " + str(len(RNA_dictionary))
    # RDLN
    RDLN_matrix_file = open(matrix_folder + "/" + RDLN_matrix)
    RDLN_data = RDLN_matrix_file.readlines()
    count = 0
    while count < len(RDLN_data):
        if count == 0:
            RNA_DNA_dictionary["sampleList"] = RDLN_data[count][:-1] + "\t"
            count = count + 1
        else:
            geneName = RDLN_data[count].split('\t')[0]
            RNA_DNA_dictionary[geneName] = RDLN_data[count][:-1] + "\t"
            count = count + 1
    print "number of gene in RDRPK " + str(len(RNA_DNA_dictionary))


    # getting data from new catalog
    sample_count = 0
    for fileName in full_path_fileNames:
        inFile = open(fileName)
        data = inFile.readlines()
        for line in data:
            geneName = line.split('\t')[0]
            DNA_readCounts = int(line.split('\t')[3])
            RNA_readCounts = int(line.split('\t')[6])
            RDLN_readCounts = float(line.split('\t')[9])
            if geneName in DNA_dictionary and geneName in RNA_dictionary and geneName in RNA_DNA_dictionary: # in gene in the matrix, append the readcounts
                DNA_dictionary[geneName] = DNA_dictionary[geneName] + str(DNA_readCounts) + "\t"
                RNA_dictionary[geneName] = RNA_dictionary[geneName] + str(RNA_readCounts) + "\t"
                RNA_DNA_dictionary[geneName] = RNA_DNA_dictionary[geneName] + str(RDLN_readCounts) + "\t"
            else:   # if gene not in the matrix, make new line
                DNA_dictionary[geneName] = "0\t" * sample_count + str(DNA_readCounts) + "\t"
                RNA_dictionary[geneName] = "0\t" * sample_count + str(RNA_readCounts) + "\t"
                RNA_DNA_dictionary[geneName] = "0\t" * sample_count + str(RDLN_readCounts) + "\t"
        sample_count = sample_count + 1

    # writing file
    # DNA
    outFile_DNA = open(output_path + "/" + "DNA_" + str(735 + sample_count) + "_hGEM.txt", "w")
    outFile_DNA.write(DNA_dictionary["sampleList"] + "\n")
    for key in DNA_dictionary:
        if key != "sampleList":
            outFile_DNA.write(DNA_dictionary[key] + "\n")

    # RNA
    outFile_RNA = open(output_path + "/" + "RNA_" + str(735 + sample_count) + "_hGEM.txt", "w")
    outFile_RNA.write(RNA_dictionary["sampleList"] + "\n")
    for key in RNA_dictionary:
        if key != "sampleList":
            outFile_RNA.write(RNA_dictionary[key] + "\n")
    # RdLN
    outFile_RDLN = open(output_path + "/" + "RDRPK_" + str(735 + sample_count) + "_hGEM.txt", "w")
    outFile_RDLN.write(RNA_DNA_dictionary["sampleList"] + "\n")
    for key in RNA_DNA_dictionary:
        if key != "sampleList":
            outFile_DNA.write(RNA_DNA_dictionary[key] + "\n")
def onlyUserCatalog():
    # variable definiton
    count = 0
    catalog_new = ""
    output_path = ""

    # parsing arguments
    while count < len(sys.argv):
        if sys.argv[count] == '-i' and os.path.isdir(sys.argv[count + 1]):
            catalog_new = sys.argv[count + 1]
        elif  sys.argv[count] == '-o':
            output_path = sys.argv[count + 1]
            if os.path.isdir(output_path) ==False:
                os.system("mkdir " + output_path)
        count = count + 1
    # check arguments
    if os.path.isdir(catalog_new) != True:
        print "ERROR - The path of catalog folder is invalid: " + catalog_new
        sys.exit()
    if os.path.isdir(output_path) != True:
        print "ERROR - The path of catalog folder is invalid: " + output_path
        sys.exit()
    

    # check if the file under catalog directory is valid
    fileNames_new = []
    if len(catalog_new) > 0:
        fileNames_new = os.listdir(catalog_new)
        if len(fileNames_new) == 0: # if no file under the folder
            print "ERROR - Catalog directory is empty: " + catalog_path
            sys.exit()
        
    # print out arguments for users
    print "The path of new catalogs is " + catalog_new
    print "The output path is " + output_path

    # get full path of all the files
    full_path_fileNames = []
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
        DNA_dictionary["geneName"] = DNA_dictionary["geneName"] + "\t" + os.path.basename(fileName)[:-4]
        RNA_dictionary["geneName"] = RNA_dictionary["geneName"] + "\t" + os.path.basename(fileName)[:-4]
        RNA_DNA_dictionary["geneName"] = RNA_DNA_dictionary["geneName"] + "\t" + os.path.basename(fileName)[:-4]
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
    outFile_DNA = open(output_path + "/" + "DNA_hGEM.txt", "w")
    outFile_DNA.write(DNA_dictionary["geneName"] + "\n")
    for key in DNA_dictionary:
        if key != "geneName":
            outFile_DNA.write(DNA_dictionary[key] + "\n")
    # RNA
    outFile_RNA = open(output_path + "/" + "RNA_hGEM.txt", "w")
    outFile_RNA.write(RNA_dictionary["geneName"] + "\n")
    for key in RNA_dictionary:
        if key != "geneName":
            outFile_RNA.write(RNA_dictionary[key] + "\n")
    # RNA/DNA
    outFile_RNA_DNA = open(output_path + "/" + "RDRPK_hGEM.txt", "w")
    outFile_RNA_DNA.write(RNA_DNA_dictionary["geneName"] + "\n")
    for key in RNA_DNA_dictionary:
        if key != "geneName":
            outFile_RNA_DNA.write(RNA_DNA_dictionary[key] + "\n")


arguments = ""
for x in sys.argv:
    arguments = arguments + x + " "

if arguments.find("-m") != -1: # user provide 3 matrix folder path
    mergeUserCatalog()
else:                          # user want to make new matrices
    onlyUserCatalog()