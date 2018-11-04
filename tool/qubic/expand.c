/************************************************************************/
/* Author: Qin Ma <maqin@csbl.bmb.uga.edu>, Jan. 25, 2010
 * Biclustering expansion, greedy add the possible genes and negatively regulated
 * genes from outside of the current bicluster in a given background
 */

#include "expand.h"
/**********************************************************************************/
static char *atom = NULL;
static char delims[] = " \t\r\n";

static int intersect_row(const bool *colcand, discrete *g1, discrete *g2, const int cols)
{
	int i, cnt = 0;
	for(i=0; i< cols; i++)
		if( colcand[i] && (g1[i] == g2[i]) && g1[i] != 0 ) 
			cnt++;
	return cnt;
}
static int reverse_row(const bool *colcand, discrete *g1, discrete *g2, const int cols)
{
	int i, cnt = 0;
	for(i=0; i< cols; i++)
		if( colcand[i] && (symbols[g1[i]] == -symbols[g2[i]])) 
			cnt++;
	return cnt;
}
/************* DUPLICATE CODE *****************/
#if 0
void another_print_bc(FILE* fw, Block* b, int num)
{	
	int i, j;
	int block_rows, block_cols;
	
	/* block height (genes) */
	block_rows = b->block_rows;
	/* block_width (conditions) */
	block_cols = b->block_cols;

    /*if (block_rows==0 || block_cols==0) return;*/
	fprintf(fw, "BC%03d\tS=%d\n", num, block_rows * block_cols);

	fprintf(fw, "Genes [%d]:", block_rows);
	for (i=0; i<dsSize(b->genes); i++)
		fprintf(fw, "%s ", another_genes[dsItem(b->genes, i)]);
	fprintf(fw, "\n");

	fprintf(fw, "Conds [%d]:", block_cols);
	for (i=0; i<dsSize(b->conds); i++)
		fprintf(fw, "%s ", another_conds[dsItem(b->conds, i)]);
	fprintf(fw, "\n");	
	/* the complete block data output */
	for (i=0; i<dsSize(b->genes); i++)
	{
		fprintf(fw,"%10s:",another_genes[dsItem(b->genes, i)]);
		for (j=0; j<dsSize(b->conds); j++)
		{
            fprintf(fw, "\t%d", symbols[arr_c[dsItem(b->genes,i)]][dsItem(b->cons, j)]);
		}
		fputc('\n', fw);
		
		if (i == b->block_rows_pre -1) fputc('\n',fw); /* add a blank line for negative */
	}
	fputc('\n', fw);
}
void get_chars_size(FILE *fp)
{
	size_t n = 0;
	char *line;
	
	getline(&line, &n ,fp);
	
	another_cols = 0;
	atom = strtok(line, delims);
	while (strtok(NULL, delims) != NULL)/* not to the end of line */
	{
		++another_cols;
	}
	another_rows = 0;
	while (getline(&line, &n, fp) >= 0)
	{		++another_rows;
	}
	fseek(fp, 0, 0);
    progress("File contains %d rows and %d cols", another_rows, another_cols);
}

void read_array(FILE *fp)
{
	size_t n = 0;
	char *line;
	int row = 0;
	int col = 0;

	/* initialization */	
	another_genes = alloc2c(another_rows, LABEL_LEN);
	another_conds = alloc2c(another_cols, LABEL_LEN);	
    another_arr_c = alloc2c(another_rows, another_cols+1);
	for ( row = 0; row < another_rows; row++)
		another_arr_c[row][another_cols] = '\0';
	
	/* read condition names */
	getline(&line, &n, fp);
	atom = strtok(line, delims);
	while (atom != NULL)
	{
		strcpy(another_conds[col], atom);
		atom = strtok(NULL, delims);
		if (++col == another_cols) break;
	}
	/* read array .chars */
	row = 0;	
	while (getline(&line, &n, fp) >=0)
	{			
		atom = strtok(line, delims);
		strcpy(another_genes[row], atom);
		atom = strtok(NULL, delims);
		for ( col=0; col < another_cols; col++)
			another_arr_c[row][col] = atom[col];
		if (++row == another_rows) break;
	}
	fclose(fp);
}	
#endif
/************* DUPLICATE CODE *****************/

void store_block(Block *b_ptr, struct dyStack *ge, struct dyStack *co)
{
	int row, col;
	row = dsSize(ge);
	col = dsSize(co);
	b_ptr->genes = ge;
	b_ptr->conds = co;
	b_ptr->block_rows = row;
	b_ptr->block_cols = col;
}
static void init_expand()
{
	another_genes = genes;
	another_conds = conds;
	another_arr_c = arr_c;
	another_rows = rows;
	another_cols = cols;
}
/* Read the .block file, get components and colcand */
void read_and_solve_blocks(FILE *fb, const char *fn)
{
	init_expand();
	size_t n;
	int col;
	char *line = NULL;
	int bnumber = 0;
	struct dyStack *ge, *co;
	int i, components, m_cnt;
	bool *colcand;
	bool *candidates;
	Block *b;
	AllocVar(b);
	AllocArray(colcand, another_cols);
	AllocArray(candidates, another_rows);
	ge = dsNew(another_rows);
	co = dsNew(another_cols);
	FILE *fo = mustOpen(fn, "w");

	/* main course starts here */
	while (getline(&line, &n, fb) != -1)
	{
	        /* fast forward to a line that contains BC*/
		/* strncmp compares up to num characters of the C string str1 to those of the C string str2
		 * strncmp ( const char * str1, const char * str2, size_t num )*/
		while (strncmp(line, "BC", 2)!=0) 
		{
			if (getline(&line, &n, fb)==-1) 
			{
				uglyTime("expanded biclusters are written to %s", fn);
				exit(0);
			}
		}
		components = 0;
		col = 0;
		dsClear(ge);
		dsClear(co);
		for (i=0; i< another_cols; i++)
			colcand[i] = FALSE;
		for (i=0; i< another_rows; i++)
			candidates[i] = TRUE;
		/* read genes from block */		
		getline(&line, &n, fb);
		atom = strtok(line, delims);
		atom = strtok(NULL, delims);
		while((atom = strtok(NULL, delims)) != NULL)
		{
			/* look up for genes number */
			if (strlen(atom) == 0) continue;			
			for(i=0; i<another_rows; i++)
			{
				if (strcmp(atom ,another_genes[i]) == 0) break;
			}
			candidates[i] = FALSE;			
			dsPush(ge, i);
			components++;
		}
		/* read conditions from block */
		getline(&line, &n, fb);
		atom = strtok(line, delims);
		atom = strtok(NULL, delims);
		while((atom = strtok(NULL, delims)) != NULL)
		{
			/*if (strlen(atom) < 5) break;*/			
			if (strlen(atom) == 0) continue;			
			for(i=0; i<another_cols; i++)
				if (strcmp(atom, another_conds[i]) == 0) break;
			colcand[i] = TRUE;
			dsPush(co, i);
			col++;
		}
		
		b->block_rows_pre = components;
		/* add some possible genes */
		for( i = 0; i < another_rows; i++)
		{
			m_cnt = intersect_row(colcand, another_arr_c[dsItem(ge,0)], another_arr_c[i], another_cols);
			/*printf ("%d\n",m_cnt);*/
			if( candidates[i] && (m_cnt >= (int)floor( (double)col * po->TOLERANCE)) )
			{
				dsPush(ge,i);
				components++;
				candidates[i] = FALSE;
			}
		}
		/* add genes that negative regulated to the consensus */
		for( i = 0; i < another_rows; i++)
		{
			m_cnt = reverse_row(colcand, another_arr_c[dsItem(ge,0)], another_arr_c[i], another_cols);
			if( candidates[i] && (m_cnt >= (int)floor( (double)col * po->TOLERANCE)) )
			{
				dsPush(ge,i);
				components++;
				candidates[i] = FALSE;
			}
		}
		if(dsSize(ge) > 1)
		{		
			store_block(b, ge, co);
			/*another_print_bc(fo, b, bnumber);*/
			print_bc(fo, b, bnumber++);
		}
	}
			printf ("1111\n");
}
