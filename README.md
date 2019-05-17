## Environment

MetaQUBIC is an integrated C package requires a basic <b>UNIX/Linux environment</b>. The gcc compiler with <b>version 4.8.5 or higher</b> is required to be pririor installed. More details can be found <a href="https://gcc.gnu.org/wiki/InstallingGCC">here</a>. <b>Currently, MetaQUBIC does not support Mac or Windows system.</b>
\
\
Our experimental test performed for mapping 735 datasets to the [IGC reference database](http://meta.genomics.cn/meta/dataTools) were proceeded on the [XSEDE](https://www.xsede.org) PSC cluster which used an overall RAM of **128GB**. Higher RAM showed less effect to the running time, while it is still required for larger dataset (more samples or genes) analysis to prevent memory overflow. 
**Due to the large memory requirement, we recommend users run MetaQUBIC on a high-performance computer (HPC), such as XSEDE, rather than local computers.**

## Installation
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

***


1. To run the tutorial, first download the [meta_data package HERE](http://bmbl.sdstate.edu/metaqubic/meta_data.gz) (3.7GB), or use wget code in Linux system:

```{r,engine='bash',eval=FALSE}
cd /pylon5/cc5fpcp/anjunma/metaqubic/   
wget http://bmbl.sdstate.edu/metaqubic/meta_data.gz
```

<div style="text-indent:3em">
<p>Then unzip the file and relocate to the the MetaQUBIC folder.</p>
</div>

```{r,engine='bash',eval=FALSE}
tar -xvzf meta_data.gz && rm -rf meta_data.gz
```

<div style="text-indent:3em">
<p>After unzip the file, you may get several sub-folders prepared for inputs and outputs files.</p>
</div>
<table width="100%">
<tr>
<td>&emsp;./meta_data/data/<b>ref</b>/</td>
<td><i>Store reference fasta file.</i></td>
</tr>
<tr>
<td>&emsp;./meta_data/data/<b>ref_index</b>/</td>
<td><i>(Empty) Store reference index files.</i></td>
</tr>
<tr>
<td>&emsp;./meta_data/data/<b>samples</b>/</td>
<td><i>Store DNA and RNA files. </i></td>
</tr>
<tr>
<td>&emsp;./meta_data/data/<b>annotation</b>/</td>
<td><i>Store annotation file(s) for functional and conditional enrichment.</td></i>
</tr>
<tr>
<td>&emsp;./meta_data/data/<b>735_hGEM</b>/</td>
<td><i>Store preprocessed three hGEM of 735 samples.</i></td>
</tr>
<tr>
<td>&emsp;</td>
</tr>
<tr>
<td>&emsp;./meta_data/results/<b>align_out</b>/</td>
<td><i>(Empty) Output folder for sam and bam files. One sample per sub-folder.</i></td>
</tr>
<tr>
<td>&emsp;./meta_data/results/<b>cat</b>/</td>
<td><i>(Empty) Output folder for generated sample catalogs. One sample per sub-folder (txt format delimited by TAB).</i></td>
</tr>
<tr>
<td>&emsp;./meta_data/results/<b>hGEM</b>/</td>
<td><i>(Empty) Output folder for the three hGEM (txt format delimited by TAB).</i></td>
</tr>
<tr>
<td>&emsp;./meta_data/results/<b>hGEM_filt</b>/</td>
<td><i>(Empty) Output folder for filtered three hGEM.(txt format delimited by TAB).</i></td>
</tr>
<tr>
<td>&emsp;./meta_data/results/<b>bic</b>/</td>
<td><i>(Empty) Output folder for biclustering result (.block file and a summary.txt file).</i></td>
</tr>
<tr>
<tr>
<td>&emsp;./meta_data/results/<b>enrich_out</b>/</td>
<td><i>(Empty) Output folder for enrichment analysis results. (.blocks file and a summary.txt file).</i></td>
</tr>
<tr>
</table>

&emsp;<b>Note 1:</b> Here we use the IGC pan-metagenome for reference.

&emsp;<b>Note 2:</b> one sample folder should have two sub-folders naming "DNA" and "RNA", respectively.For this tutorial, the path is like:

&emsp;&emsp;<i>./meta_data/data/samples/Sample001/<b>DNA</b>/Sample001_DNA.fastq</i>
&emsp;&emsp;<i>./meta_data/data/samples/Sample001/<b>RNA</b>/Sample001_RNA.fastq</i>


2. <b>To add your own data,</b> simply to the previous instructions:
* Put any reference <b>fasta file</b>, <b>annotation files</b> and <b>phenotype information file</b> in the "data/ref_data" folder;
* Put your raw fastq files in the "data/genome_data" folder. <b>Delete or remove any sample folders that you don't want to be included in the analysis</b>;
* You can have customed folder name for each sample and the fastq files, but keep "DNA" and "RNA" names as they are the key words for MetaQUBIC to read data.


3. Extra data can be downloaded by clicking the links below:
Extra data can be downloaded by clicking the links below:
* [IGC_index.tar.gz](http://bmbl.sdstate.edu/metaqubic/IGC_index.tar.gz)(10GB)  Reference index files resulted from bowtie2. See usage in 4.1 Execution-1 (using index files)
* [RDRPK_735_hGEM_filt.txt](http://bmbl.sdstate.edu/metaqubic/RDRPK_735_hGEM_filt.txt.tar.gz)(160MB, unzip to 2.7GB)  The filtered hGEM matrix. See usage in 4.3 Execution-3 (starting from biclustering)
* [bicluster.blocks](http://bmbl.sdstate.edu/metaqubic/biclusters.blocks.tar.gz)  Biclustering results resulted in our paper.
* [735 sample catlogs](http://bmbl.sdstate.edu/metaqubic/735_catalogs.tar.gz)(1GB)    All 735 catalog files.

The raw fastq files of 735 samples can be downloaded from [NCBI SRA](https://www.ncbi.nlm.nih.gov/sra/) using the respecitve ID [<b>here</b>](http://bmbl.sdstate.edu/metaqubic/735_fastq_SRA_ID.txt)(52KB).
4. The raw fastq files of 735 samples can be downloaded from [NCBI SRA](https://www.ncbi.nlm.nih.gov/sra/) using the respecitve ID <a href="http://bmbl.sdstate.edu/metaqubic/Tutorial/735_fastq_SRA_ID.txt"><b>here</b></a>.

***  


The general pipeline of MetaQUBIC includes three parts (five codes) to process the raw sample fastq files and results in a functional enriched blocks file contains bicluster information.\
Five more advanced executions can be reached in [4. Advance executions] section in user's preferance.


First, go to the folder where you just unpacked the zip data:
```{r,engine='bash',eval=FALSE}
cd /your/folder/path/metaqubic/ 
```
Here, we put all three folders in the same root folder, so that we can easily use the same relative path in the following example. The three folders are:

&emsp;master - the metaQUBIC package folder.\
&emsp;data - contains input data files like  reference, annotation, fastq, et al. (See Input data preparation)\
&emsp;results - where all output files will be stored.(See Input data preparation)

### 3.1 Part 1 - Gene mapping and catalog construction
<i><b>Description:</i></b>
\
In the first part, the DNA and RNA sequencing data (fastq) of Sample001 are loaded and mapped to the IGC pan_metagenome reference (IGC_ref.fa). We integrated a parallel structure so that 10 mapping processes will be carried on at the same time to save running time. Three files of alignment result files (.sam, .bam, coverage.txt) will be stored in the ./results/align_out/Sample001/ folder, and one sample catalog in the ./results/cat/ folder.
\
<i><b>Input:</i></b>
\
Folder containing fastq files, reference fasta file (IGC_ref.fa)
\
<i><b>Output:</i></b>
\
sam file (Sample001_DNA.sam,Sample001_RNA.sam), bam file (Sample001_DNA.bam, Sample001_RNA.bam), coverage summary (Sample001_DNA.coverage, Sample001_RNA.coverage), sample catalog (Sample001.cat)
\
<i><b>Argument:</i></b>
```{r,engine='bash',eval=FALSE}
./master/runmeta --mapping -s ./meta_data/data/samples/ -ref ./meta_data/data/ref/IGC_ref.fa -o1 ./meta_data/results/align_out/ -o2 ./meta_data/results/cat/
```


### 3.2 Part 2 - hGEM generation and filtering
<b>Part 2.1</b>\
<i><b>hGEM generation (matrix update):</i></b>
\
<i><b>Input:</i></b>
\
sample catalog (Sample001.cat), three hGEM of 735 sample (DNA_735_hGEM.txt, RNA_735_hGEM.txt, RDRPK_735_hGEM.txt)
\
<i><b>Output:</i></b>
\
three updated hGEM (DNA_hGEM.txt, RNA_hGEM.txt, RDRPK_hGEM.txt)
\
<i><b>Argument:</i></b>
```{r,engine='bash',eval=FALSE}
./master/runmeta --CatToMat -i ./meta_data/results/cat/ -m ./meta_data/data/735_hGEM/ -o ./meta_data/results/hGEM/
```
<b>Part 2.2</b>\
<i><b>gene normalization and filtering:</i></b>
\
<i><b>Input:</i></b>
\
three updated hGEM (DNA_hGEM.txt, RNA_hGEM.txt, RDRPK_hGEM.txt)
\
<i><b>Output:</i></b>
\
three filtered hGEM (DNA_hGEM_filt.txt, RNA_hGEM_filt.txt, RDRPK_hGEM_filt.txt), summary file (hGEM_filt.summary)
\
<i><b>Argument:</i></b>
```{r,engine='bash',eval=FALSE}
./master/runmeta --filtering -i ./meta_data/results/hGEM/ -minTPM 1 -NominTPM 2 -o ./meta_data/results/hGEM_filt/
```

### 3.3 Part 3 - Biclustering and enrichment analyses
<b>Part 3.1</b>\
<i><b>Biclustering</i></b>
\
<i><b>Input:</i></b>
\
The filtered RD-RPK hGEM (RDRPK_hGEM_filt.txt)
\
<i><b>Output:</i></b>
\
biclusters (.blocks)
\
<i><b>Argument:</i></b>
```{r,engine='bash',eval=FALSE}
./master/runmeta --biclustering -i ./meta_data/results/hGEM_filt/ -o ./meta_data/results/bic/ --QUBIC -c 1 -f 0.8 -o 100 
```
<b>Part 3.2</b>\
<i><b>functional enrichment:</i></b>
\
<i><b>Input:</i></b>
\
.blocks file, annotation file (KEGG_ann.txt)
\
<i><b>Output:</i></b>
\
enriched blocks file (.blocks), summary file (hGEM_enrichment.summary)
\
<i><b>Argument:</i></b>
```{r,engine='bash',eval=FALSE}
./master/runmeta --enrichment -i ./meta_data/results/bic -g ./meta_data/data/annotation/KEGG_ann.txt -o ./meta_data/results/enrich_out/
```
Note: the conditional annotations have not been provided by the author, thus, we omit the conditional enrichment here. 

***

We also offer several executions to ease the way to use MetaQUBIC.

### 4.1 Execution-1 (using index files)
MetaQUBIC supports self-provided reference databases by editing the pathway in the  configuration to broaden its application to metatranscriptomic researchers focusing on environments other than human gut. 
\
To increase the processing speed, we recommend users use reference index files instead of .fa file. Then, use the -ind setting to indicate the index path and omit the -ref:
```{r,engine='bash',eval=FALSE}
./master/runmeta --mapping -s ./meta_data/data/samples/ -ind ./meta_data/data/ref_index/ -o ./meta_data/results/align_out/ 
```


### 4.2 Execution-2 (update hGEM vs new hGEM)
MetaQUBIC allows the user to add new samples directly to the three 735_sample_hGEM we provided to increase the sample pool for a better study of the human gut microbiome (Step 3.2). However, if users are discovering metagenome data from other environment or simply only want to use their own data, they need to generate three new hGEM alone, using the code below without the provided three hGEM matrices. 
\
To generate new matrix, turn off the -m flag in --CatToMat command:
```{r,engine='bash',eval=FALSE}
./master/runmeta --CatToMat -i ./meta_data/results/cat/ -o ./meta_data/results/hGEM/
```


### 4.3 Execution-3 (starting from biclustering)

MetaQUBIC can perform biclustering analysis on any given expression matrix as the user's preference. Simply use our provided filtered RD-RPK hGEM with 2.4 million genes X 735 samples, or any user's expression matrix, and start from Part 3 to omit the first two parts.

### 4.4 Execution-4 (various enrichment analyses)
If more gene annotation files are provided or a conditional annotation file exist, the --enrichment function can take over them all at a time:
```{r,engine='bash',eval=FALSE}
./master/runmeta --enrichment -i ./meta_data/results/bic -g ./meta_data/data/annotation/KEGG_ann1.txt ./meta_data/data/annotation/KEGG_ann2.txt -c ./meta_data/data/annotation/condition_ann.txt -o ./meta_data/results/enrich_out/
```

### 4.5 Execution-5 (link to HUManN2)
MetaQUBIC is also an open-access tool that can be easily modified, enhanced, and facilitated to other tools or analyses as users need. Here, we regard HUManN2 as an effective novel species-level functional profiling tool for microbiome pathway reconstruction and diversity discovery from DNA and RNA reads and integrated an additional step in MetaQUBIC to connect the outcome with HUManN2. 

The command first selects conditional-specific functional biclusters resulted from MetaQUBIC, after performing both condition and gene annotation enrichments, with only the sample of interest (SI) contained. Since each bicluster is functionally enriched, extracting the gene list from biclusters with SI will result in a function-gene list with each column representing the group of genes in each SI-included bicluster corresponding to the same function. The sequences of genes are then retrieved by matching the function-gene list with the output sam file and used as the sequence input of HUManN2 pipeline. This process helps to accelerate the analysis of HUManN2 by narrowing a whole fastq file to specific function-related (pathway-related)  genes, to construct relationships among species, conditions, and pathways in the SI.
\
<i><b>Input:</i></b>
\
enriched blocks files, SI name, sam file of SI (Sample001_DNA.sam)
\
<i><b>Output:</i></b>
\
gene-bic list, sequence-bic list
\
<i><b>Argument:</i></b>
```{r,engine='bash',eval=FALSE}
../master/runmeta --bicToSeq -i ./meta_data/results/enrich_out/enriched.blocks -s. Sample001 -o ./meta_data/results/HUManN2

```

Several examples are shown below to help users better understand the files (inputs and outputs) used or produced in MetaQUBIC.
<b>Files using universal (or common) format:</b> fasta (or .fa), .fastq (or .fq), .sam (bowtie2 alignment output), .bam (converted from sam), .coverage (bedtools output), index files (bowtie2 reference indexing output)
\
<b>Other files:</b>
\
<center><img src="http://bmbl.sdstate.edu/metaqubic/Tutorial/Figure3.jpg" alt="The MetaQUBIC pipeline" width="85%"></center>

***

<center><img src="http://bmbl.sdstate.edu/metaqubic/Tutorial/Figure4.jpg" alt="The MetaQUBIC pipeline" width="100%"></center>

***

<center><img src="http://bmbl.sdstate.edu/metaqubic/Tutorial/Figure5.jpg" alt="The MetaQUBIC pipeline" width="100%"></center>

***

<center><img src="http://bmbl.sdstate.edu/metaqubic/Tutorial/Figure6.jpg" alt="The MetaQUBIC pipeline" width="100%"></center>

*** 
