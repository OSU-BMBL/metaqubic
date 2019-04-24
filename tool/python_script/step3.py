import os
import sys
import copy

# variable define
IGC_reference_database = ""
dna_matrix_path = ""
rna_matrix_path = ""
rna_dna_matrix_path = ""
matrix_folder = ""
output_path = ""
cutoff_value = 1.0
sample_value = 1

# parsing arguments
count = 0
while count < len(sys.argv):
    if sys.argv[count] == '-i':
        IGC_reference_database = sys.argv[count + 1]
    elif sys.argv[count] == '-m':
        matrix_folder = sys.argv[count + 1]
    elif sys.argv[count] == '-o':
        output_path = sys.argv[count + 1]
    elif sys.argv[count] == '-minTPM':
        cutoff_value = float(sys.argv[count + 1])
    elif sys.argv[count] == '-NominTPM':
        sample_value = int(sys.argv[count + 1])
    count = count + 1

# get 3 matrices names
fileNames = os.listdir(matrix_folder)
if len(fileNames) != 3:
    print "ERROR - only DNA, RNA, RDRPK matrix in the folder:" + matrix_folder
    sys.exit()
else:
    for fileName in fileNames:
        if fileName.find("DNA") != -1:
            dna_matrix_path = matrix_folder + "/" + fileName
        elif fileName.find("RNA") != -1:
            rna_matrix_path = matrix_folder + "/" + fileName
        elif fileName.find("RDRPK") != -1:
            rna_dna_matrix_path = matrix_folder + "/" + fileName
        else:
            print "ERROR - can't recognize this file: " + fileName

# check arguments
if os.path.exists(IGC_reference_database) == False:
    print "ERROR - the path of reference database is invalid: " + IGC_reference_database
    sys.exit()
if os.path.exists(dna_matrix_path) == False:
    print os.path.exists(dna_matrix_path)
    print "ERROR - the path of DNA matrix is invalid: " + dna_matrix_path
    sys.exit()
if os.path.exists(rna_matrix_path) == False:
    print "ERROR - the path of RNA matrix is invalid: " + rna_matrix_path
    sys.exit()
if os.path.exists(rna_dna_matrix_path) == False:
    print "ERROR - the path of RNA/DNA matrix is invalid: " + rna_dna_matrix_path
    sys.exit()
if os.path.exists(output_path) == False:
    os.system("mkdir " + output_path)
if cutoff_value < 0:
    print "ERROR - the cut off value for TPM normalization and filtering is invalid: " + cutoff_value + ", should be greater than 0"
    sys.exit()

# print arugments for user
print "The path of reference database is " + IGC_reference_database
print "The path of DNA matrix is " + dna_matrix_path
print "The path of RNA matrix is " + rna_matrix_path
print "The path of RNA/DNA matrix is " + rna_dna_matrix_path
print "THe output path is " + output_path
if cutoff_value == 1:
    print "The default TPM cut-off value is 1"
else:
    print "The TPM cut-off value is " + str(cutoff_value)

# return true if at least one element greater than cut off value
# return false if all element less than cut off value
def keep_this_line(aList):
    count = 0
    for element in aList:
        if float(element) >= cutoff_value:
            count = count + 1
    if count >= sample_value:
        return True
    else:
        return False

# get gene length
IGC_reference_database_file = open(IGC_reference_database)
IGC_reference = IGC_reference_database_file.readlines()
dictionary = {}
count = 0
while count < len(IGC_reference):
    if IGC_reference[count][0] == '>': # it is the header of the sequence
        gene_name = IGC_reference[count].split()[0][1:]
        dictionary[gene_name] = len(IGC_reference[count + 1]) - 1
        temp_count = count + 2
        while temp_count < len(IGC_reference) and IGC_reference[temp_count][0] != '>':
            dictionary[gene_name] = dictionary[gene_name] + len(IGC_reference[temp_count]) - 1
            temp_count = temp_count + 1
    
    count = count + 1


# ***************************************************************************************************************
# start processing DNA matrix:
# ***************************************************************************************************************
DNA_matrix_file = open(dna_matrix_path)
DNA_matrix_data = DNA_matrix_file.readlines()
DNA_matrix_length_before_filter = len(DNA_matrix_data) - 1
DNA_matrix_data_original = copy.deepcopy(DNA_matrix_data)
row = 0
column = 0

# convert DNA_matrix_data to the matrix
while row < len(DNA_matrix_data):
    DNA_matrix_data[row] = DNA_matrix_data[row].split('\t')
    row = row + 1

# start calculating TPM
row = 1
column = 1
# step 1 divide read counts by gene length in kilobase
while row < len(DNA_matrix_data):
    while column < len(DNA_matrix_data[0]):
        gene_name = DNA_matrix_data[row][0]
        gene_length = dictionary[gene_name]
        DNA_matrix_data[row][column] = str( float( DNA_matrix_data[row][column] ) /  ( float(gene_length) / 1000) )

        column = column + 1
    row = row + 1
# step 2 Count up all the RPK values in a sample and divide this number by 1,000,000
scaling_factor = []
scaling_factor.append(0)
row = 1
column = 1
while column < len(DNA_matrix_data[0]):
    total = 0.0
    while row < len(DNA_matrix_data):
        total = total + float(DNA_matrix_data[row][column])

        row = row + 1

    scaling_factor.append(total / 1000000)
    column = column + 1
# step 3
row = 1
column = 1
while column < len(DNA_matrix_data[0]):
    while row < len(DNA_matrix_data):
        DNA_matrix_data[row][column] = str( float( DNA_matrix_data[row][column] ) / scaling_factor[column] )

        row = row + 1
    column = column + 1

count = 0
for line in DNA_matrix_data[1:]:
    if keep_this_line(line[1:]) == True:
        count = count + 1
# write to the file
DNA_matrix_outFile = open(output_path + "/DNA_hGEM_filt.txt", "w")
DNA_matrix_outFile.write(DNA_matrix_data_original[0])
count = 1
DNA_matrix_length_after_filter = 0
while count < len(DNA_matrix_data_original):
    if keep_this_line(DNA_matrix_data[count][1:]) == True:
        DNA_matrix_outFile.write(DNA_matrix_data_original[count])
        DNA_matrix_length_after_filter = DNA_matrix_length_after_filter + 1

    count = count + 1

DNA_matrix_file.close()
DNA_matrix_outFile.close()
# ***************************************************************************************************************
# start processing RNA matrix:
# ***************************************************************************************************************
RNA_matrix_file = open(rna_matrix_path)
RNA_matrix_data = RNA_matrix_file.readlines()
RNA_matrix_length_before_filter = len(RNA_matrix_data) - 1
RNA_matrix_data_original = copy.deepcopy(RNA_matrix_data)
row = 0
column = 0

# convert DNA_matrix_data to the matrix
while row < len(RNA_matrix_data):
    RNA_matrix_data[row] = RNA_matrix_data[row].split('\t')
    row = row + 1

# start calculating TPM
row = 1
column = 1
# step 1 divide read counts by gene length in kilobase
while row < len(RNA_matrix_data):
    while column < len(RNA_matrix_data[0]):
        gene_name = RNA_matrix_data[row][0]
        gene_length = dictionary[gene_name]
        RNA_matrix_data[row][column] = str( float( RNA_matrix_data[row][column] ) /  ( float(gene_length) / 1000) )

        column = column + 1
    row = row + 1

# step 2 Count up all the RPK values in a sample and divide this number by 1,000,000
scaling_factor = []
scaling_factor.append(0)
row = 1
column = 1
while column < len(RNA_matrix_data[0]):
    total = 0.0
    while row < len(RNA_matrix_data):
        total = total + float(RNA_matrix_data[row][column])

        row = row + 1

    scaling_factor.append(total / 1000000)
    column = column + 1

# step 3
row = 1
column = 1
while column < len(RNA_matrix_data[0]):
    while row < len(RNA_matrix_data):
        RNA_matrix_data[row][column] = str( float( RNA_matrix_data[row][column] ) / scaling_factor[column] )

        row = row + 1
    column = column + 1

count = 0
for line in RNA_matrix_data[1:]:
    if keep_this_line(line[1:]) == True:
        count = count + 1
    
# write to the file
RNA_matrix_outFile = open(output_path + "/RNA_hGEM_filt.txt", "w")
RNA_matrix_outFile.write(RNA_matrix_data_original[0])
count = 1
RNA_matrix_length_after_filter = 0
while count < len(RNA_matrix_data_original):
    if keep_this_line(RNA_matrix_data[count][1:]) == True:
        RNA_matrix_outFile.write(RNA_matrix_data_original[count])
        RNA_matrix_length_after_filter = RNA_matrix_length_after_filter + 1

    count = count + 1

RNA_matrix_file.close()
RNA_matrix_outFile.close()

# ***************************************************************************************************************
# start looking for mutual gene list:
# ***************************************************************************************************************
TPM_DNA_matrix = open(output_path + "/DNA_hGEM_filt.txt")
TPM_RNA_matrix = open(output_path + "/RNA_hGEM_filt.txt")
TPM_DNA_matrix_data = TPM_DNA_matrix.readlines()
TPM_RNA_matrix_data = TPM_RNA_matrix.readlines()

# get DNA gene list
DNA_gene_list = []
for line in TPM_DNA_matrix_data[1:]:
    DNA_gene_list.append(line.split('\t')[0])
print "number of DNA gene: " + str(len(DNA_gene_list))

# get RNA gene list
RNA_gene_list = []
for line in TPM_RNA_matrix_data[1:]:
    RNA_gene_list.append(line.split('\t')[0])
print "number of RNA gene: " + str(len(RNA_gene_list))

# get mutual gene list
mutual_gene_list = list(set(DNA_gene_list).intersection(RNA_gene_list))
print "number of mutual gene: " + str(len(mutual_gene_list))

# output filtered RNA/DNA matrix
RNA_DNA_matrix_file = open(rna_dna_matrix_path)
RNA_DNA_matrix_data = RNA_DNA_matrix_file.readlines()
RNA_DNA_matrix_outFile = open(output_path + "/RDRPK_hGEM_filt.txt", "w")
RNA_DNA_matrix_outFile.write(RNA_DNA_matrix_data[0])
count = 0
for line in RNA_DNA_matrix_data[1:]:
    if line.split('\t')[0] in mutual_gene_list:
        RNA_DNA_matrix_outFile.write(line)



# write summary file
outFile_summary = open(output_path + "/hGEM_filt.summary", "w")
outFile_summary.write("DNA TPM filtering:\n")
rate = float((DNA_matrix_length_before_filter - DNA_matrix_length_after_filter) / float(DNA_matrix_length_before_filter)) * 100
outFile_summary.write("Total: " + str(DNA_matrix_length_before_filter) + " genes\tRemoval: " + str(DNA_matrix_length_before_filter - DNA_matrix_length_after_filter) + " genes\tRemaining: " + str(DNA_matrix_length_after_filter) + " genes\tFilter_rate: " + str(rate) + "\n\n")
outFile_summary.write("RNA TPM filtering:\n")
rate = float((RNA_matrix_length_before_filter - RNA_matrix_length_after_filter) / float(RNA_matrix_length_before_filter)) * 100
outFile_summary.write("Total: " + str(RNA_matrix_length_before_filter) + " genes\tRemoval: " + str(RNA_matrix_length_before_filter - RNA_matrix_length_after_filter) + " genes\tRemaining: " + str(RNA_matrix_length_after_filter) + " genes\tFilter_rate: " + str(rate) + "\n\n")
outFile_summary.write("DNA & RNA TPM intersection: " + str(len(mutual_gene_list)) + " genes\n\n")
outFile_summary.write("RD-RPK hGEM: " + str(len(mutual_gene_list)) + " X " + str(len(RNA_DNA_matrix_data[0].split('\t')) - 1) + "samples\n")

