/* 
 * Author:Qin Ma <maqin@csbl.bmb.uga.edu>, Jan. 24, 2010 
 * Usage: This is part of the bicluster package. Use, redistribute, modify without limitations.
 *
 * Include two procedures for file input:
 * o read_continuous() would read a file with this format:
 * ----------------------------------------
 * 	 	cond1	 cond2	 cond3
 * 	gene1    3.14	  -1.2     0.0
 * 	gene2      nd      2.8     4.5
 * ----------------------------------------
 *   values may possibly be any continuous value, e.g. log-ratio of 
 *   lumin intensity for two channels. The procedure then, for each 
 *   row, produces a distribution using method similar to outlier algorithm, 
 *   base on two tails of values (6%),
 *   the middle part, is regarded as insignificant. This would discretize
 *   the continuous value into classes. (If you want divide the data into 
 *   more levels, you can adjust the parameter r and q) See below.
 * 
 * o read_discrete() would read a file with format like:
 * ----------------------------------------
 *  		cond1	 cond2	 cond3
 *  	gene1	    1        1       0
 *  	gene2       1       -1       0
 * ----------------------------------------
 *   the symbols could be any integers (-32768~+32767) and represent distinct classes.
 *   '0', however, will be ignored, and uncounted in the alter algorithms.
 *   since they would represent a no-change class.
 */

#include "read_array.h"

/************************************************************************/
/* Helper variables for tokenizer function */

static char *atom = NULL;
static char delims[] = "\t\r\n";
#define MAXC 100000
/* record the position of each discretized symbol in _symbols_ */
/* an unsigned short can hold all the values between 0 and  USHRT_MAX inclusive. USHRT_MAX must be at least 65535*/
static int bb[USHRT_MAX];

/***********************************************************************/

/* Comparison function for GNU qsort */
static int compare_continuous (const void *a, const void *b)
{
    const continuous *da = a;
    const continuous *db = b;
    /*make qsort in the increasing order*/
    return (*da < *db)?-1:(*da != *db);
}
/* emulate gnu gsl quantile function */
/*divide by the number of the data*/
static continuous quantile_from_sorted_data(const continuous sorted_data[], size_t n, double f)
{
	/*floor function returns the largest integral value less than or equal to x*/
	int i = floor((n-1)*f);
	continuous delta = (n-1)*f-i;
	return (1-delta)*sorted_data[i]+delta*sorted_data[i+1];
}
/*divide by the value of the data*/
/*static continuous quantile_from_sorted_data_value(const continuous sorted_data[],size_t n, double f)
{
        return sorted_data[0]+f*(sorted_data[n-1]-sorted_data[0]);
}*/
/***********************************************************************/

static int charset_add(discrete *ar, discrete s)
{
	/*A signed short can hold all the values between SHRT_MIN  and SHRT_MAX inclusive.SHRT_MIN is required to be -32767 or less,SHRT_MAX must be at least 32767*/
	int ps = s + SHRT_MAX;
	if (bb[ps]<0)
	{
		bb[ps] = sigma; 
		ar[sigma++] = s;
	}
	return bb[ps];
}

/***********************************************************************/

/* Matrix allocations (continuous and discrete 2d array) */

continuous** alloc2d(int rr, int cc)
{
	continuous** result;
	int i;
	AllocArray(result, rr);
	for (i = 0; i < rr; i++)
		AllocArray(result[i], cc);
	return result;
}

discrete** alloc2c(int rr, int cc)
{
	discrete** result;
	int i;
	AllocArray(result, rr);
	for (i = 0; i < rr; i++)
		AllocArray(result[i], cc);
	return result;
}

/***********************************************************************/

/* Pre-read the datafile, retrieve gene labels and condition labels
 * as well as determine the matrix size
 */
void get_matrix_size (FILE* fp)
{
	/*size_t is the best type to use if you want to represent sizes of objects. 
	 * Using int to represent object sizes is likely to work on most modern systems, but it isn't guaranteed.
	 */
	size_t n = 0;
	char *line;
	/*getline() reads an entire line, storing the address of the buffer containing the text into *line.
	 *the buffer is null-terminated and includes the newline character, if a newline delimiter was found.
	 */
	if (getline(&line, &n, fp)>=0)
	{
		/*strtok function returns a pointer to the next token in str1, where str2 contains the delimiters that determine the token*/
		atom = strtok(line, delims);
		/*delete the first element in atom because the first element corresponding to description column*/
		atom = strtok(NULL, delims);
		while (atom != NULL)
		{
			/*if we do not set atom = strtok(NULL, delims), here is a infinite loop*/
			atom = strtok(NULL, delims);
			cols++;
		}			
	}
	while (getline(&line, &n, fp)>=0)
	{
		atom = strtok(line, delims);
		rows++;
	}
	/*fseed sets the position indicator associated with the stream to a new position defined by adding offset to a reference position specified by origin*/
	fseek(fp, 0, 0);
}

/* Read in the labels on x and y, in microarray terms, genes(rows) and conditions(cols)*/
void read_labels (FILE* fp)
{
	int row = 0;
	int col;
	size_t n = 0;
	char *line;
	while (getline(&line, &n, fp)>=0)
	{
		atom = strtok(line, delims);
		/*currently the first element in atom is the gene name of each row when row>=1, the 0 row corresponding to the line of condition names*/
		if (row >= 1) 
		{
			strcpy(genes[row-1], atom);
			/*check if there exist a gene name equals to TFname by -T*/
			if (strcmp (atom, po->TFname)==0)
			{
				TFindex = row-1;
				printf ("%d\n",TFindex);
			}
		}
		/*delete the first element in atom because the first element corresponding to description column*/
		atom = strtok(NULL, delims);
		col = 0;
		while (atom != NULL)
		{
			if (row == 0) 
				strcpy(conds[col], atom);
			atom = strtok(NULL, delims);
			if (++col == cols) break;
		}
		if (++row == rows+1) break;
	}
	fseek(fp, 0, 0);
}

/*read in the sub-gene list*/
void read_list (FILE* fp)
{
	int i=0, j=0;
	sub_genes_row = 0;
	char line[MAXC];
	while(fgets(line,MAXC,fp)!=NULL)
	{
		atom = strtok(line, delims);
		strcpy(sub_genes[sub_genes_row], atom);	
		sub_genes_row++;
	}

	/*update the sub_list*/
	AllocArray(sublist,rows);
	for (i = 0; i<rows; i++)
		sublist[i] = FALSE;
	for (i=0; i<sub_genes_row; i++)
		for (j=0; j<rows; j++)
			if (strcmp (sub_genes[i], genes[j])==0)
				sublist[j] = TRUE;
}


/* initialize data for discretization */
void init_dis()
{
	int row, col;
	/* store discretized values */
	AllocArray(symbols, USHRT_MAX);
	/* memset sets the first num bytes of the block of memory pointed by ptr to the specified value
	 * memset ( void * ptr, int value, size_t num )*/
	memset(bb, -1, USHRT_MAX*sizeof(*bb));
	/* always add an 'ignore' index so that symbols[0]==0*/
	charset_add(symbols, 0); 
	/*initialize for arr_c*/
	arr_c = alloc2c(rows,cols);
	for (row = 0; row < rows; row++)
        	for (col = 0; col < cols; col++) 
			arr_c[row][col] = 0;
}

void read_discrete (FILE* fp)
{
	int row, col, i;
	init_dis();
	/* import data */
	size_t n = 0;
	char *line;
	row = 1;
	/* Skip first line with condition labels */
	getline(&line, &n, fp);
	/* read the discrete data from the second line */
	while (getline(&line, &n, fp)>=0)
	{
		atom = strtok(line, delims);
		/*skip the first column*/
		atom = strtok(NULL, delims);
		col = 0;
		while (atom != NULL)
		{			
			arr_c[row-1][col] = charset_add(symbols, atoi(atom));
			atom = strtok(NULL, delims);
			if (++col == cols) break;
		}
		if (++row == rows+1) break;
	}
	/* trim the leading spaceholder */
	printf("Discretized data contains %d classes with charset [ ", sigma);
	for (i=0;i<sigma;i++) 
		printf("%d ", symbols[i]); printf("]\n");
	fseek(fp, 0, 0);
}

void read_continuous (FILE* fp)
{
	int row, col;
	arr = alloc2d(rows,cols);
	/* import data */
	size_t n = 0;
	char *line;
	row = 1;
	/* ignore header line */
	getline(&line, &n, fp);
	while (getline(&line, &n, fp)>=0)
	{
		atom = strtok(line, delims);
		/*skip the first column*/
		atom = strtok(NULL, delims);
		col = 0;
		while (atom != NULL)
		{
			/*we set all the aplha to ignore value 0*/
			/*Checks if parameter atom is either an uppercase or a lowercase alphabetic letter*/
			if (isalpha(*atom)) 
				arr[row-1][col] = 0.0;
			else 
				arr[row-1][col] = atof(atom);
			atom = strtok(NULL, delims);
			if (++col == cols) break;
		}
		if (++row == rows+1) break;
	}
	fseek(fp, 0, 0);
}

/***********************************************************************/

/* Discretize continuous values by revised outlier detection algorithm
 * see details in Algorithm Design section in paper
 */
discrete dis_value(float current, int divided, float *small, int cntl, float *big, int cntu)
{
	int i;
	float d_space = 1.0 / divided;	
	for(i=0; i < divided; i++)
	{		
            if ((cntl > 0) && (current <= quantile_from_sorted_data(small, cntl, d_space * (i+1)))) 
		    return -i-1;
            if ((cntu > 0) && (current >= quantile_from_sorted_data(big, cntu, 1.0 - d_space * (i+1)))) 
		    return i+1;
	}
	return 0;
}

void discretize (const char* stream_nm)
{
	int row, col;
	continuous rowdata[cols];
	float big[cols], small[cols];
	int i,cntu,cntl;
	float f1,f2,f3, upper, lower;
	FILE *fw;
	fw = mustOpen(stream_nm, "w");
	init_dis();
	for (row = 0; row < rows; row++)
	{
		for (col = 0; col < cols; col++) 
			rowdata[col] = arr[row][col];
		qsort(rowdata, cols, sizeof *rowdata, compare_continuous);
		f1 = quantile_from_sorted_data(rowdata,cols,1-po->QUANTILE); 
		f2 = quantile_from_sorted_data(rowdata,cols,po->QUANTILE);
		f3 = quantile_from_sorted_data(rowdata,cols,0.5);
		if ((f1-f3)>=(f3-f2))
		{
			upper = 2*f3-f2; 
			lower = f2;
		}
		else
		{
			upper = f1; 
			lower = 2*f3-f1;
		}
		cntu = 0; cntl = 0;
		for (i = 0; i < cols; i++)
		{
			if (rowdata[i] < lower) 
			{ 
				small[cntl] = rowdata[i]; 
				cntl++; 
			}
			if (rowdata[i] > upper) 
			{ 
				big[cntu] = rowdata[i]; 
				cntu++; 
			}
		}		
		for (col = 0; col < cols; col++)
	    		arr_c[row][col] = charset_add(symbols, dis_value(arr[row][col],po->DIVIDED, small, cntl, big, cntu));
		fprintf(fw, "row %s :low=%2.5f, up=%2.5f; %d down-regulated,%d up-regulated\n", genes[row], lower, upper,cntl, cntu);
    	}
        progress ("Discretization rules are written to %s", stream_nm);
	fclose(fw);
}

/* output the formatted matrix */
void write_imported (const char* stream_nm)
{
	int row, col;
	FILE *fw;
	fw = mustOpen(stream_nm, "w"); 
	fprintf(fw, "o");
	for (col = 0; col < cols; col++)
		fprintf(fw,"\t%s",conds[col]);
	fputc('\n',fw);
	for (row = 0; row < rows; row++)
	{
		fprintf(fw, "%s", genes[row]);
		for (col = 0; col < cols; col++)
			fprintf(fw, "\t%d", symbols[arr_c[row][col]]);
		fputc('\n', fw);
	}
	progress("Formatted data are written to %s", stream_nm);
	fclose(fw);
}

/***********************************************************************/
