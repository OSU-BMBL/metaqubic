import os
import sys
import copy
import scipy.stats as stats

# define function
def get_Pvalue(x1, x2, x3, x4):
    value = stats.hypergeom.sf(int(x4) - 1, int(x1), int(x2), int(x3))
    return value

def is_minimum(bicluster_number, fileName, p):
    inFile = open(fileName)
    data = inFile.readlines()
    count = 0
    while count < len(data):
        if data[count].split(':')[0] == bicluster_number:
            pvalue = float(data[count + 1].split(':')[1])
            if pvalue < p:
                return False
        count = count + 1
    return True

# check arguments
count = 0
while count < len(sys.argv):
    if sys.argv[count] == "-b":
        blocks_list = sys.argv[count + 1]
    elif sys.argv[count] == "-a":
        gene_list = sys.argv[count + 1]
    elif sys.argv[count] == '-o':
        output_path = sys.argv[count + 1]
    count = count + 1

# get lists from the blocks
if os.path.exists(blocks_list) == False:
    print "ERROR - The path of bicluster matrix is invalid: " + blocks_list
    sys.exit()
blocks_list_file = open(blocks_list)
blocks_list_data = blocks_list_file.readlines()
count = 0
while count < len(blocks_list_data):
    blocks_list_data[count] = blocks_list_data[count][:-1]
    count = count + 1

# get lists from enrichments lists
if os.path.exists(gene_list) == False:
    print "ERROR - The path of phenotype file is invalid: " + gene_list
    sys.exit()
gene_list_file = open(gene_list)
gene_list_data = gene_list_file.readlines()

# convert it to a matrix
count = 0
while count < len(gene_list_data):
    gene_list_data[count] = [gene_list_data[count].split()[0], gene_list_data[count].split()[1]]
    count = count + 1

gene_list_data = sorted(gene_list_data, key=lambda gene_list_data : gene_list_data[1])

# classification
count = 0
dictionary = {}
while count < len(gene_list_data):
    dictionary[gene_list_data[count][1]] = []
    count = count + 1

count = 0
while count < len(gene_list_data):
    dictionary[gene_list_data[count][1]].append(gene_list_data[count][0])
    count = count + 1

# starting getting pvalue
outfile = open(dirname(output_path) + "/temp.txt", "w")
for key in dictionary:
    if key == "unknown":
            continue
    else:
        outfile.write("\nLabel:\t" + key + "\n")
        count = 0
        for line in blocks_list_data:
            if len(line) == 0:
                count = count + 1
                continue
            w = line
            line = line.split()
            x1 = len(line) + len(dictionary[key])
            x2 = len(line)
            x3 = len(dictionary[key])
            x4 = len(set(line).intersection(dictionary[key]))
            pValue = get_Pvalue(x1, x2, x3, x4)
            outfile.write("BC" + str(count) + ": " + w + "\n")
            outfile.write("Pvalue: " + str(pValue) + "\n")
            count = count + 1
outfile.close()


# start analyzing outFile
inFile = open(dirname(output_path) + "/temp.txt")
outFile = open(output_path, "w")
data = inFile.readlines()
count = 0
while count < len(data):
    data[count] = data[count][:-1]
    count = count + 1

count = 0
while count < len(data):
    if data[count].split(':')[0] == "Label":
        outFile.write("\n" + data[count] + "\n")
        temp = count
        temp = temp + 1
        while temp < len(data) and len(data[temp]) != 0:
            if is_minimum(data[temp].split(':')[0], dirname(output_path) + "/temp.txt", float(data[temp + 1].split(':')[1])) == True:
                outFile.write(data[temp] + "\n")
                outFile.write(data[temp + 1] + "\n")
            temp = temp + 2
        count = count + 1
    else:
        count = count + 1

os.system("rm -rf " + dirname(output_path) + "/temp" )




    


