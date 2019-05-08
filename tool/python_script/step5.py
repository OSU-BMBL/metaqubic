import os
import sys
import copy
import scipy.stats as origin
from rpy2.robjects.packages import importr
from rpy2.robjects.vectors import FloatVector
stats = importr('stats')

# function definition
def get_Pvalue(x1, x2, x3, x4):
    value = origin.hypergeom.cdf(int(x1), int(x2), int(x3), int(x4))
    return 1 - value

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



# variable definition
block_file_path = ""
output_path = ""
gene_annotation = []
sample_annotation = []

count = 0
# parsing arguments
while count < len(sys.argv):
    if sys.argv[count] == "-i":
        block_file_path = sys.argv[count + 1]
    elif sys.argv[count] == "-o":
        output_path = sys.argv[count + 1]
    elif sys.argv[count] == '-g':
        for parameter in sys.argv[count + 1:]:
            if parameter[0] != '-':
                gene_annotation.append(parameter)
            else:
                break
            count = count + 1
    elif sys.argv[count] == '-c':
        for parameter in sys.argv[count + 1:]:
            if parameter[0] != '-':
                sample_annotation.append(parameter)
            else:
                break
            count = count + 1
    count = count + 1

# check arguments
if os.path.exists(block_file_path) == False:
    print "ERROR - the path of .blocks file is invalid: " + block_file_path
    sys.exit() 
if os.path.exists(output_path) == False:
    print "ERROR - the output path is invalid: " + output_path
    sys.exit() 
for element in gene_annotation:
    if os.path.exists(element) == False:
        print "ERROR - the path is invalid: " + output_path
        sys.exit() 
for element in sample_annotation:
    if os.path.exists(element) == False:
        print "ERROR - the path is invalid: " + output_path
        sys.exit()


# print arugment
print "The .blocks is in " + block_file_path
print "The output path is in " + output_path
print "The gene annotation file " + str(gene_annotation)
print "The sample annotation file " + str(sample_annotation)  


# gather info from .blocks
block_file = open(block_file_path, "r")
block_data = block_file.readlines()
biclusters = []
count = 0
bicluster_count = 0
while count < len(block_data):
    if block_data[count].find("Pvalue") != -1: # line with BCXXX
        biclusters.append([])
        biclusters[bicluster_count].append(block_data[count][:-1]) # p_value
        biclusters[bicluster_count].append(block_data[count + 1].split()[2:]) # number of bicluster
        biclusters[bicluster_count].append(block_data[count + 2].split()[2:]) # number of condition
        bicluster_count = bicluster_count + 1
    count = count + 1

# handle gene annotation
if len(gene_annotation) != 0:
    for fileName in gene_annotation:
        annotation_file = open(fileName)
        annotation_data = annotation_file.readlines()

        # convert it to a matrix
        count = 0
        while count < len(annotation_data):
            annotation_data[count] = [annotation_data[count].split()[0], annotation_data[count].split()[1]]
            count = count + 1

        annotation_data = sorted(annotation_data, key=lambda annotation_data : annotation_data[1])
        # classification
        count = 0
        dictionary = {}
        while count < len(annotation_data):
            dictionary[annotation_data[count][1]] = []
            count = count + 1

        count = 0
        while count < len(annotation_data):
            dictionary[annotation_data[count][1]].append(annotation_data[count][0])
            count = count + 1

        # mageyaode, a gene list stored the annotated genes
        # gene_list = {}
        # for line in annotation_data:
        #     gene_list[line[0]] = "1"

        print "get total number of elements in dictionary"
        # get total number of elements in dictionary
        count = 0
        for key in dictionary:
            count = count + len(dictionary[key])
        x2 = count

        # temp_outFile = open(output_path + "/mageyaode.txt", "w") # mageyaode
        # start getting pvalue
        print "start getting pvalue"
        count = 0
        for bicluster in biclusters:
            key_list = []
            pvalue_list = []
            minimum = 0.0
            for key in dictionary:
                #mageyaode filter non-annotated gene
                # annotated_gene_list = []
                # for gene in bicluster[1]:
                #     if gene in gene_list:
                #         annotated_gene_list.append(gene)
                # x1 = len(set(annotated_gene_list).intersection(dictionary[key])) - 1
                # # x2
                # x3 = len(dictionary[key])
                # x4 = len(annotated_gene_list)

                x1 = len(set(bicluster[1]).intersection(dictionary[key])) - 1
                # x2 already exists
                x3 = len(dictionary[key])
                x4 = len(bicluster[1])
                pValue = get_Pvalue(x1, x2, x3, x4)
                key_list.append(key)
                pvalue_list.append(pValue)
            BH_adjust = stats.p_adjust(FloatVector(pvalue_list), method = 'BH')
            minimum_pvalue = min(pvalue_list)
            x = 0

            #mage yaode
            # b = 0
            # for a in key_list:
            #     temp_outFile.write(str(count + 1) + "\t" + str(BH_adjust[b]) + "\t" + str(a) + "\n") 
            #     b = b + 1
            #mageyaode
            
            # find min p value
            for value in pvalue_list:
                if value == minimum_pvalue:
                    biclusters[count][0] = biclusters[count][0] + "\tFunction:" + str(key_list[x]) + "\tPvalue: " + str(value) + "\tBH_adjust_Pvalue: " + str(BH_adjust[x])
                x = x + 1
            count = count + 1
            print count
            

        # pvalue 0.05 threshold
        count = 0
        while count < len(biclusters):
            if float(biclusters[count][0].split(':')[-1]) > 0.05:  # use bh pvalue
                biclusters[count][0] = ""
            count = count + 1

# handle sample annotation
if len(sample_annotation) != 0:
    for fileName in gene_annotation:
        annotation_file = open(fileName)
        annotation_data = annotation_file.readlines()

        # convert it to a matrix
        count = 0
        while count < len(annotation_data):
            annotation_data[count] = [annotation_data[count].split()[0], annotation_data[count].split()[1]]
            count = count + 1

        annotation_data = sorted(annotation_data, key=lambda annotation_data : annotation_data[1])
        # classification
        count = 0
        dictionary = {}
        while count < len(annotation_data):
            dictionary[annotation_data[count][1]] = []
            count = count + 1

        count = 0
        while count < len(annotation_data):
            dictionary[annotation_data[count][1]].append(annotation_data[count][0])
            count = count + 1

        # # mageyaode create gene list stored the annotated gene
        # gene_list = {}
        # for line in annotation_data:
        #     gene_list[line[0]] = "1"

        print "get total number of elements in dictionary"
        # get total number of elements in dictionary
        count = 0
        for key in dictionary:
            count = count + len(dictionary[key])
        x2 = count

        # temp_outFile = open(output_path + "/mageyaode.txt", "w") # mageyaode
        # start getting pvalue
        print "start getting pvalue"
        count = 0
        for bicluster in biclusters:
            key_list = []
            pvalue_list = []
            minimum = 0.0
            for key in dictionary:
                # #mageyaode filter non-annotated genes
                # annotated_gene_list = []
                # for gene in bicluster[1]:
                #     if gene in gene_list:
                #         annotated_gene_list.append(gene)
                # x1 = len(set(annotated_gene_list).intersection(dictionary[key])) - 1
                # # x2
                # x3 = len(dictionary[key])
                # x4 = len(annotated_gene_list)

                x1 = len(set(bicluster[2]).intersection(dictionary[key])) - 1
                # x2 already exists
                x3 = len(dictionary[key])
                x4 = len(bicluster[1])
                pValue = get_Pvalue(x1, x2, x3, x4)
                key_list.append(key)
                pvalue_list.append(pValue)
            BH_adjust = stats.p_adjust(FloatVector(pvalue_list), method = 'BH')
            minimum_pvalue = min(pvalue_list)
            x = 0

            #mage yaode
            # b = 0
            # for a in key_list:
            #     temp_outFile.write(str(count + 1) + "\t" + str(BH_adjust[b]) + "\t" + str(a) + "\n") 
            #     b = b + 1
            # find min p value
            for value in pvalue_list:
                if value == minimum_pvalue:
                    biclusters[count][0] = biclusters[count][0] + "\tFunction:" + str(key_list[x]) + "\tPvalue: " + str(value) + "\tBH_adjust_Pvalue: " + str(BH_adjust[x])
                x = x + 1
            count = count + 1
            # print count

        # pvalue 0.05 threshold
        count = 0
        while count < len(biclusters):
            if float(biclusters[count][0].split(':')[-1]) > 0.05:
                biclusters[count][0] = ""
            count = count + 1


# output to the file
outFile = open(output_path + "/enrichment_report.txt", "w")
for bicluster in biclusters:
    if len(bicluster[0]) != 0:
        outFile.write(bicluster[0].split('\t')[0] + "\t")
        for element in  bicluster[0].split('\t')[2:]:
            outFile.write(element + "\t")
        outFile.write("\n")
    
# calculate purity
# total_purity = 0
# purity_list = []
# number_of_enrich_bicluster = 0
# for bicluster in biclusters:
#     each_bicluster_purity_list = []
#     if len(bicluster[0]) != 0:
#         count = 0
#         while count < len(bicluster[0].split('\t')):
#             if bicluster[0].split('\t')[count].find('Function') != -1:
#                 kegg = bicluster[0].split('\t')[count].split(':')[1]
#                 number_of_enrich_gene = len(set(bicluster[1]).intersection(dictionary[kegg]))
#                 total_gene = len(bicluster[1])
#                 each_bicluster_purity_list.append(number_of_enrich_gene / float(total_gene))
#             count = count + 1
#         total_purity += max(each_bicluster_purity_list)
#         print each_bicluster_purity_list
#         number_of_enrich_bicluster = number_of_enrich_bicluster + 1
# print "number of enrich bicluster: " + str(number_of_enrich_bicluster)
# print "average purity: " +  str(total_purity / float(number_of_enrich_bicluster))

# total = []
# for bicluster in biclusters:
#     total.append(len(bicluster[1]))

# print max(total)
# print min(total)
# print sum(total) / float(len(total))