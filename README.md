# MetaQUBIC: a computational pipeline for gene-level functional profiling of metagenome and metatranscriptome 

Metagenomic and metatranscriptomic analyses can provide an abundance of information related to microbial communities. However, straightforward analysis of this data does not provide optimal results, with a required integration of data types being needed to thoroughly investigate these microbiomes and their environmental interactions. Here, we present MetaQUBIC, an integrated biclustering-based computational pipeline for gene module detection that integrates both metagenomic and metatranscriptomic data.  Additionally, we used this pipeline to investigate 735 paired DNA and RNA human gut microbiome samples, resulting in a comprehensive hybrid gene expression matrix of 2.3 million cross-species genes in the 735 human faecal samples and 155 functional enriched gene modules. We believe both the MetaQUBIC pipeline and the generated comprehensive human gut hybrid expression matrix will facilitate further investigations into multiple levels of microbiome studies.

## A detailed tutorial with example codes can be found in [wiki](https://github.com/OSU-BMBL/metaqubic/wiki). 

### 1.2 Installation
The source code of MetaQUBIC is freely available at: https://github.com/OSU-BMBL/metaqubic. 

To install the MetaQUBIC, first, download the zip file manually from github, or use the code below in Unix:
```{r,engine='bash',eval=FALSE, download}
cd /your/working/path/
wget https://github.com/OSU-BMBL/metaqubic/archive/master.zip
```
Unzip the file:
```{r,engine='bash',eval=FALSE, unzip}
unzip master.zip && rm -rf master.zip
```

Then, execute "setup.sh" to compile the source code:
```{r,engine='bash',eval=FALSE, sh}
cd ./metaqubic-master
sh setup.sh
```
Now the MetaQUBIC is successfully installed and ready for use. \
<b>Note:</b> the installation of scipy package may ask you to agree with a user agreement. Simply click Enter button to continue.

### 2 Data download
To run the tutorial, a zip package can be downloaded [HERE]() (), or use wget code in Linux system:
```{r,engine='bash',eval=FALSE}
wget http://bmbl.sdstate.edu/metaqubic/meta_data.gz
```
Unzip the package to obtain data including: 
(1) the IGC reference fasta file, 
(2) three raw hGEM files generated from 735 human gut microbiome samples, 
(3) an KEGG functional annotation list provided by IGC, and 
(4) one sample of paired DNA and RNA fastq files

Extra data can be downloaded by clicking the links below:
* IGC_index.gz  See usage in 4.1 Execution-1 (using index files)
* RDRPK_735_hGEM_filt.txt  See usage in 4.3 Execution-3 (starting from biclustering)
* 735_sam.gz  See usage in 4.5 Execution-5 (link to HUManN2)
* bicluster.blocks  Biclustering results mentioned in our paper.

