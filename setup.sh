# set tool path
sed -i "11i strncpy(install_path, \"$(pwd)\", 1000);" main.c
gcc *.c -o runmeta

# install anaconda
cd tool && wget https://repo.continuum.io/archive/Anaconda2-5.3.0-Linux-x86_64.sh && bash Anaconda2-5.3.0-Linux-x86_64.sh << ANSWERS

yes
./python
no
no
ANSWERS
./python/bin/pip install --upgrade pip
./python/bin/conda install -c conda-forge rpy2 
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