# This script is used for link to human2

import os
import sys

# variable definition
block_file = ""
sample_name = ""
sam_file = ""
output_path = ""

# parsing arguments
count = 0
while count < len(sys.argv):
    if sys.argv[count] == '-i':
        block_file = sys.argv[count + 1]
    elif sys.argv[count] == '-s':
        sample_name = sys.argv[count + 1]
    elif sys.argv[count] == '-f':
        sam_file = sys.argv[count + 1]
    elif sys.argv[count] == '-o':
        output_path = sys.argv[count + 1]
    count = count + 1
if os.path.exists(block_file) == False:
        print "ERROR - the path of blocks file is invalid: " + block_file
        sys.exit()
if len(sample_name) == 0:
        print "ERROR - the sample name is invalid: " + sample_name
        sys.exit()
if os.path.exists(sam_file) == False:
        print "ERROR - the path of sam file is invalid: " + sam_file
        sys.exit()
if os.path.isdir(output_path) == False:
        os.system("mkdir " + output_path)

#output useful info
print "The path of block file is " + block_file
print "The path of sample name is " + sample_name
print "The path of sam file is " + sam_file
print "The output path is " + output_path + "/gene_sequence_list.txt"
# create fstream
sam_data = open(sam_file).readlines()
outFile = open(output_path + "/gene_sequence_list.txt", "w")

# reading and format sam file
count = 0
sam_dic = {}
for line in sam_data:
    line_data = line.split()
    if len(line_data) >= 10 and line_data[2] != "*":
        if line_data[2] not in sam_dic:
            sam_dic[line_data[2]] = []
        sam_dic[line_data[2]].append(line_data[9])
    count +=1

# reading and format block file
gene_list = []
sample_list = []
block_data = open(block_file).readlines()
count = 0
while count < len(block_data):
    if block_data[count] not in ('\n', '\r\n') and block_data[count].split()[0].find("BC") != -1:
        gene_list = block_data[count + 1].split()[2:]
        sample_list = block_data[count + 2].split()[2:]
        if sample_name in sample_list:
            for gene in gene_list:
                if gene in sam_dic:
                    for sequence in sam_dic[gene]:
                        outFile.write(gene + "\t" + sequence + "\n")
    count += 1