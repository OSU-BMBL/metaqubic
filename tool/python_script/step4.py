import os
import sys

# set and check argument
tool_path = sys.argv[1]
rdln_matrix = ""
output_path = ""
argument = ""

count = 0
while count < len(sys.argv):
    if sys.argv[count] == "-i":
        rdln_matrix = sys.argv[count + 1]
    elif sys.argv[count] == "-o":
        output_path = sys.argv[count + 1]
    elif sys.argv[count] == '--QUBIC':
        for parameter in sys.argv[count + 1:]:
            argument = argument + " " + parameter
            count = count + 1
    count = count + 1
if os.path.exists(rdln_matrix) == False:
    print "ERROR - the path of RNA/DNA matrix is invalid: " + rdln_matrix
if os.path.isdir(output_path) == False:
    print "Creating output directory: " + output_path
    os.system("mkdir " + output_path) 

 # set default value to qubic
if argument.find("-f") == -1:
    argument = argument + " -f 0.8"
if argument.find("-c") == -1:
    argument = argument + " -c 1"
if argument.find("-o") == -1:
    argument = argument + " -o 200"
if argument.find("-k") == -1:
    argument = argument + " -k 20"

# print out the argument
print "The path of the RNA/DNA matrix is " + rdln_matrix
print "The output path is " + output_path
print "The parameters that will pass to QUBIC is " + argument


# run qubic for biclustering
qubic_execute = tool_path + "/qubic/qubic -i "
os.system(qubic_execute + rdln_matrix + argument)
os.system("mv " +  rdln_matrix + ".blocks " + output_path)
os.system("rm " +  rdln_matrix + ".rules " + output_path)
os.system("mv " +  rdln_matrix + ".chars " + output_path)

# ouptut genes and samples lists
# os.system("cd " + output_path + " for file in *blocks; do grep Conds $file |cut -d \':\' -f2 >\"condition.txt\"; done")
# os.system("cd " + output_path + " for file in *blocks; do grep Genes $file |cut -d \':\' -f2 >\"gene.txt\"; done\"")