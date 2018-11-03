gcc *.c -o metaqubic

# install anaconda
cd tool && bash Anaconda2-5.3.0-Linux-x86_64.sh
cd ..

# install bedtools
cd tool/bedtools && make
cd ..
cd ..

# install bowtie2
cd tool/bowtie2 && make NO_TBB=1
cd ..
cd ..

# install samtools
cd tool/samtools/ && make
cd ..
cd ..

# install qubic
cd tool/qubic && make
cd ..
cd ..