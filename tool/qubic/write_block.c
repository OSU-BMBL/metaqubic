/******************************************************************/
/* Author: Qin Ma <maqin@csbl.bmb.uga.edu>, Jan. 25, 2010
 * Output the identified bicluster block.
 */

#include "write_block.h"

/******************************************************************/
/* scan through all columns and identify the set within threshold,
 * "fuzziness" of the block is controlled by TOLERANCE (-c)
 */
void scan_block (struct dyStack *gene_set, Block *b_ptr)
{
	int i, j;
	int block_rows, cur_rows;
	block_rows = cur_rows = dsSize(gene_set);
	
	int k;
	for (j = 0; j < cols; j++)
		for (k=0; k<sigma; k++) 
			profile[j][k] = 0;
	for (j = 0; j< cur_rows ; j++)
		seed_update(arr_c[dsItem(gene_set,j)]);

	int btolerance = ceil(po->TOLERANCE* block_rows);
	for (j = 0; j < cols; j++)
	{
		/* See if this column satisfies tolerance */
		/* here i start from 1 because symbols[0]=0 */
		for (i = 1; i < sigma; i++)
		{
			if ((profile[j][i] >= btolerance))
			{
				dsPush(b_ptr->conds, j); break;
			}
		}		
	}
	b_ptr->block_cols = dsSize(b_ptr->conds);
}

/*************************************************************************/

/* Identified clusters are backtraced to the original data, by
 * putting the clustered vectors together, identify common column
 */
void print_bc (FILE* fw, Block* b, int num)
{	
	int i, j;
	int block_rows, block_cols;
	int num_1=0,num_2=0;	
	/* block height (genes) */
	block_rows = b->block_rows;
	/* block_width (conditions) */
	block_cols = b->block_cols;

	fprintf(fw, "BC%03d\tS=%d\tPvalue:%LG \n", num, block_rows * block_cols, b->pvalue);

	fprintf(fw, " Genes [%d]: ", block_rows);
	for (i=0; i<dsSize(b->genes); i++)
		fprintf(fw, "%s ", genes[dsItem(b->genes, i)]);
	fprintf(fw, "\n");

	fprintf(fw, " Conds [%d]: ", block_cols);
	for (i=0; i<dsSize(b->conds); i++)
		fprintf(fw, "%s ", conds[dsItem(b->conds, i)]);
	fprintf(fw, "\n");	
	/* the complete block data output */
	for (i=0; i<dsSize(b->genes); i++)
	{
		fprintf(fw,"%10s:",genes[dsItem(b->genes, i)]);
		for (j=0; j<dsSize(b->conds); j++)
		{
			fprintf(fw, "\t%d", symbols[arr_c[dsItem(b->genes, i)][dsItem(b->conds, j)]]);
			if (i==0)
			{
				if (symbols[arr_c[dsItem(b->genes, i)][dsItem(b->conds, j)]] == 1) num_1++;
				if (symbols[arr_c[dsItem(b->genes, i)][dsItem(b->conds, j)]] == -1) num_2++;
			}
		}
		fputc('\n', fw);
		if (i == b->block_rows_pre -1) 
			fputc('\n',fw);
	}
	/*printf ("BC%03d: #of 1 and -1 are:\t%d\t%d\n",num,num_1,num_2);
	fputc('\n', fw);*/
}

/******************************************************************/
