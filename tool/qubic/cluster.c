/************************************************************************/
/* Author: Qin Ma <maqin@csbl.bmb.uga.edu>, Jan. 25, 2010
 * Biclustering procedure, greedy heuristic by picking an edge with highest
 * score and then dynamically adding vertices into the block and see if 
 * the block score can be improved.
 */

#include "cluster.h"

/************************************************************************/
/* Initialize seed */
static int compare_int (const void *a, const void *b)
{
	const int *da = a;
	const int *db = b;    
	return (*da < *db)?-1:(*da != *db);
}
static void update_colcand(bool *colcand, discrete *g1, discrete *g2)
{
	int i;
	for (i=0; i< cols; i++)
		if (colcand[i] && (g1[i] != g2[i])) 
			colcand[i] = FALSE;
}
static int intersect_row(const bool *colcand, discrete *g1, discrete *g2)
/*caculate the weight of the edge with two vertices g1 and g2*/
{
	int i;
	int cnt = 0;
	for (i=0; i< cols; i++)
		if (colcand[i] && (g1[i] == g2[i]) && (g1[i]!=0)) 
			cnt++;
	return cnt;
}
static int reverse_row(const bool *colcand, discrete *g1, discrete *g2)
{
/*caculate the negative correlation between g1 and g2*/	
	int i;
	int cnt = 0;
	for (i = 0; i < cols; i++)
	{
		if (colcand[i] && (symbols[g1[i]] == -symbols[g2[i]])) 
			cnt++;
	}
	return cnt;
} 
static void seed_current_modify (const discrete *s, bool *colcand, int* cnt, int components)
/* calculate the coverage of any row to the current consensus
 * cnt = # of valid consensus columns
 */
{
	int i, k, flag, n;
	int threshold = ceil(components * po->TOLERANCE);
	discrete ss;
	*cnt = 0;
	for (i=0; i<cols; i++)
	{
		flag = 0; ss = s[i];
		for (k=1; k<sigma; k++)
		{
			n = profile[i][k];
			if (k == ss) 
				n++;
			if (n >= threshold)
			{
				flag = k; 
				break;
			}
		}
		if (flag)	
		{
			(*cnt)++;
			colcand[i] = TRUE;
		}
	}
}

static bool check_seed(Edge *e, Block **bb, const int block_id)
/*check whether current edge can be treat as a seed*/
{
	int profiles[rows];
	int i,b1,b2,b3;
	bool fg = FALSE;
	b1 = b2 = -1;
	for (i = 0; i < block_id; i++)
		if ( isInStack(bb[i]->genes,e->gene_one) && isInStack(bb[i]->genes, e->gene_two) ) 
			return FALSE; 

	for ( i = 0; i < rows; i++) profiles[i] = 0;
	fg = FALSE;	
	for ( i = 0; i < block_id; i++)
		if ( isInStack(bb[i]->genes, e->gene_one) ) 
		{ 
			fg = TRUE;
		       	break; 
		}
	if (fg) 
		b1 = i;
	fg = FALSE;	
	for ( i = 0; i < block_id; i++)
		if ( isInStack(bb[i]->genes, e->gene_two) ) 
		{ 
			fg = TRUE; 
			break; 
		}
	if (fg) 
		b2 = i;
	if ( (b1 == -1)||(b2 == -1) ) 
		return TRUE;
	else
	{
		for ( i = 0; i < bb[b1]->block_rows; i++)
			profiles[dsItem(bb[b1]->genes,i)]++;
		for ( i = 0; i < bb[b2]->block_rows; i++)
			profiles[dsItem(bb[b2]->genes,i)]++;
		for ( i = 0; i < rows; i++)
 			if (profiles[i] > 1) 
				return FALSE;
		b3 = MAX(bb[b1]->block_cols, bb[b2]->block_cols);
		if ( e->score <b3/* (bb[b1]->block_cols + bb[b2]->block_cols) / 2*/ ) 
			return FALSE;
		else 
			return TRUE;
	}
	err("never see this message\n");
	return FALSE;
}

static void block_init(Edge *e, Block *b, 
                     struct dyStack *genes, struct dyStack *scores,
                     bool *candidates, const int cand_threshold,
                     int *components, struct dyStack *allincluster, long double *pvalues)
{
	int i,score,top;
	int cnt = 0, cnt_all=0, pid=0;
	continuous cnt_ave=0, row_all = rows;
	long double pvalue;
	int max_cnt, max_i;
	int *arr_rows, *arr_rows_b;
	AllocArray(arr_rows, rows);
	AllocArray(arr_rows_b, rows);	
	bool *colcand;
	AllocArray(colcand, cols);
	for (i=0; i< cols; i++) 
		colcand[i] = FALSE;
	discrete *g1, *g2;
	g1 = arr_c[dsItem(genes,0)];
	g2 = arr_c[dsItem(genes,1)];
	for (i=0; i< cols; i++)
		if ((g1[i] == g2[i])&&(g1[i]!=0)) 
			colcand[i] = TRUE;

	for (i = 0; i < rows; i++)
	{
		arr_rows[i] = intersect_row(colcand, arr_c[dsItem(genes,0)], arr_c[i]);
		arr_rows_b[i] = arr_rows[i];
	}
	/*we just get the largest 100 rows when we initial a bicluster because we believe that 
	 * the 100 rows can characterize the structure of the bicluster 
	 * btw, it can reduce the time complexity*/
	if (rows > 100)
	{		
		qsort(arr_rows_b, rows, sizeof *arr_rows, compare_int);
		top = arr_rows_b[rows -100];
		for (i = 0; i < rows; i++)
			if (arr_rows[i] < top) 
				candidates[i] = FALSE;
	}
	/*calculate the condition low bound for current seed*/
	int cutoff = floor (0.05*rows);
	b->cond_low_bound = arr_rows_b[rows-cutoff];


	while (*components < rows)
	{
		max_cnt = -1;
		max_i = -1;
		(*components)++;
		cnt_all =0;
		cnt_ave = 0;
		/******************************************************/
		/*add a function of controling the bicluster by pvalue*/
		/******************************************************/
		for (i=0; i< rows; i++)
		{
			if (!candidates[i]) continue;
			if (po->IS_list && !sublist[i]) continue;
			cnt = intersect_row(colcand,arr_c[dsItem(genes,0)],arr_c[i]);
			cnt_all += cnt;
			if (cnt < cand_threshold) 
				candidates[i] = FALSE;
			if (cnt > max_cnt)
			{
				max_cnt = cnt;
				max_i = i;
			}
		}
		cnt_ave = cnt_all/row_all;
		pvalue = get_pvalue (cnt_ave, max_cnt);
		if (po->IS_cond)
		{
			if (max_cnt < po->COL_WIDTH || max_i < 0|| max_cnt < b->cond_low_bound) break;
		}
		else
		{
			if (max_cnt < po->COL_WIDTH || max_i < 0) break;
		}
		if (po->IS_area)
			score = *components*max_cnt;
		else
			score = MIN(*components, max_cnt);
		if (score > b->score) 
			b->score = score;
		if (pvalue < b->pvalue)
			b->pvalue = pvalue;
		dsPush(genes, max_i);
		dsPush(scores,score);
		pvalues[pid++] = pvalue;
		update_colcand(colcand,arr_c[dsItem(genes,0)], arr_c[max_i]);
		candidates[max_i] = FALSE;
	}
	/*be sure to free a pointer when you finish using it*/
	free(colcand);
}
/************************************************************************/
/* Core algorithm */
int cluster (FILE *fw, Edge **el, int n)
{
	int block_id = 0;
	Block **bb;
	int allocated = po->SCH_BLOCK;
	AllocArray(bb, allocated);

	Edge *e;
	Block *b;
	struct dyStack *genes, *scores, *b_genes, *allincluster;
	
	int i, j, k, components;
	AllocArray(profile, cols);
	for (j = 0; j < cols; j++) 
		AllocArray(profile[j], sigma);

	genes = dsNew(rows);
	scores = dsNew(rows);
	allincluster = dsNew(rows);

    
	long double *pvalues;
	AllocArray(pvalues, rows);

	bool *candidates;
	AllocArray(candidates, rows);

	e = *el; 
	i = 0;
	while (i++ < n)
	{	
		e = *el++;
		/* check if both genes already enumerated in previous blocks */
		bool flag = TRUE;
		/* speed up the program if the rows bigger than 200 */
	        if (rows > 250)
		{ 
			if ( isInStack(allincluster,e->gene_one) && isInStack(allincluster,e->gene_two) )
				flag = FALSE;
			else if ((po->IS_TFname)&&(e->gene_one!= TFindex)&&(e->gene_two!=TFindex))
				flag = FALSE;
			else if ((po->IS_list)&&(!sublist[e->gene_one] || !sublist[e->gene_two]))
				flag =FALSE;
		}
		else   
		{
			flag = check_seed(e, bb, block_id);
			if ((po->IS_TFname)&&(e->gene_one!= TFindex)&&(e->gene_two!=TFindex))
				flag = FALSE;
			if ((po->IS_list)&&(!sublist[e->gene_one] || !sublist[e->gene_two]))
				flag = FALSE;
		}
		if (!flag) continue;

		for (j = 0; j < cols; j++)
			for (k = 0; k < sigma; k++) 
				profile[j][k] = 0;

		/*you must allocate a struct if you want to use the pointers related to it*/
		AllocVar(b);
		/*initial the b->score*/
                b->score = MIN(2, e->score);
		/*initial the b->pvalue*/
		b->pvalue = 1;
	
		/* initialize the stacks genes and scores */		
		int ii;		
		dsClear(genes);
		dsClear(scores);		
		for(ii = 0; ii < rows; ii ++)
		{
			dsPush(genes,-1);
			dsPush(scores,-1);
		}		
		dsClear(genes);
		dsClear(scores);
		
		dsPush(genes, e->gene_one);
		dsPush(genes, e->gene_two);
		dsPush(scores, 1);
		dsPush(scores, b->score);

		/* branch-and-cut condition for seed expansion */
		int cand_threshold = floor(po->COL_WIDTH * po->TOLERANCE);
                if (cand_threshold < 2) 
			cand_threshold = 2;

		/* maintain a candidate list to avoid looping through all rows */		
		for (j = 0; j < rows; j++) 
			candidates[j] = TRUE;
		candidates[e->gene_one] = candidates[e->gene_two] = FALSE;
		components = 2;

		/* expansion step, generate a bicluster without noise */
		block_init(e, b, genes, scores, candidates, cand_threshold, &components, allincluster, pvalues);

		/* track back to find the genes by which we get the best score*/
		for(k = 0; k < components; k++)
		{
			if (po->IS_pvalue)
				if ((pvalues[k] == b->pvalue) &&(k >= 2) &&(dsItem(scores,k)!=dsItem(scores,k+1))) break;
			if ((dsItem(scores,k) == b->score)&&(dsItem(scores,k+1)!= b->score)) break;
		}
		components = k + 1;
		int ki;
		for (ki=0; ki < rows; ki++)
			candidates[ki] = TRUE;

		for (ki=0; ki < components - 1 ; ki++)
		{
			seed_update(arr_c[dsItem(genes,ki)]);
			candidates[dsItem(genes,ki)] = FALSE;
		}
		candidates[dsItem(genes,k)] = FALSE;
		genes->top = k ;
		int cnt = 0;
		bool *colcand;
		AllocArray(colcand, cols);
		for(ki = 0; ki < cols; ki++) 
			colcand[ki] = FALSE;             
    
		/* add columns satisfy the conservative r */ 
		seed_current_modify(arr_c[dsItem(genes,k)], colcand, &cnt, components);
		
		/* add some new possible genes */
		int m_cnt;
		for ( ki = 0; ki < rows; ki++)
		{
			if (po->IS_list && !sublist[ki]) continue;
			m_cnt = intersect_row(colcand, arr_c[dsItem(genes,0)], arr_c[ki]);
			if ( candidates[ki] && (m_cnt >= floor(cnt* po->TOLERANCE)) )
			{
				dsPush(genes,ki);
				components++;
				candidates[ki] = FALSE;
			}
		}
                b->block_rows_pre = components;
		
		/* add genes that negative regulated to the consensus */
		for ( ki = 0; ki < rows; ki++)
		{
			if (po->IS_list && !sublist[ki]) continue;
			m_cnt = reverse_row(colcand, arr_c[dsItem(genes,0)], arr_c[ki]);
			if ( candidates[ki] && (m_cnt >= floor(cnt * po->TOLERANCE)) )
			{
				dsPush(genes,ki);
				components++;
				candidates[ki] = FALSE;
			}
		}
		free(colcand);

		/* save the current cluster*/
		b_genes = dsNew(b->block_rows_pre);
		for (ki = 0; ki < b->block_rows_pre; ki++)
			dsPush(b_genes, dsItem(genes,ki));

		/* store gene arrays inside block */
		b->genes = dsNew(components);
		b->conds = dsNew(cols);
	
		scan_block(b_genes, b);
		if (b->block_cols == 0) continue;
		b->block_rows = components;
                if (po->IS_pvalue)
			b->score = -(100*log(b->pvalue));
		else
			b->score = b->block_rows * b->block_cols;		

		dsClear(b->genes);
		for ( ki=0; ki < components; ki++)
			dsPush(b->genes,dsItem(genes,ki));
		for(ki = 0; ki < components; ki++)
			if(!isInStack(allincluster, dsItem(genes,ki))) 
				dsPush(allincluster,dsItem(genes,ki));	
		/*save the current block b to the block list bb so that we can sort the blocks by their score*/
		bb[block_id++] = b;

		/* reaching the results number limit */
		if (block_id == po->SCH_BLOCK) break;
		verboseDot();	
	}
	/* writes character to the current position in the standard output (stdout) and advances the internal file position indicator to the next position.
	 * It is equivalent to putc(character,stdout).*/
	putchar('\n');
	/* free-up the candidate list */
	free(candidates);
	free(allincluster);
	free (pvalues);
	return report_blocks(fw, bb, block_id);
}

/************************************************************************/
static void print_params(FILE *fw)
{
	char filedesc[LABEL_LEN];
	strcpy(filedesc, "continuous");
	if (po->IS_DISCRETE) 
		strcpy(filedesc, "discrete");
	fprintf(fw, "# QUBIC version %.1f output\n", VER);
	fprintf(fw, "# Datafile %s: %s type\n", po->FN, filedesc);
	fprintf(fw, "# Parameters: -k %d -f %.2f -c %.2f -o %d",
			po->COL_WIDTH, po->FILTER, po->TOLERANCE, po->RPT_BLOCK);
	if (!po->IS_DISCRETE) 
		fprintf(fw, " -q %.2f -r %d", po->QUANTILE, po->DIVIDED);
	fprintf(fw, "\n\n");
}

/************************************************************************/
static int report_blocks(FILE* fw, Block** bb, int num)
{
	print_params(fw);
	sort_block_list(bb, num);
	
	int i, j,k;
	/*MIN MAX et al functions can be accessed in struct.h*/
        int n = MIN(num, po->RPT_BLOCK);
	bool flag;

	Block **output;
	AllocArray(output, n);

	Block **bb_ptr = output;
	Block *b_ptr;
	double cur_rows, cur_cols;
	double inter_rows, inter_cols;
        /*double proportion;*/
	
	/* the major post-processing here, filter overlapping blocks*/
	i = 0; j = 0;
	while (i < num && j < n)
	{
		b_ptr = bb[i];
		cur_rows = b_ptr->block_rows;
		cur_cols = b_ptr->block_cols;

		flag = TRUE;
		k = 0;
		while (k < j)
		{
			inter_rows = dsIntersect(output[k]->genes, b_ptr->genes);
			inter_cols = dsIntersect(output[k]->conds, b_ptr->conds);
			
			if (inter_rows*inter_cols > po->FILTER*cur_rows*cur_cols)
			{
				flag = FALSE; 
				break;
			}
                        k++;
		}
	        i++;
		if (flag)
		{
			print_bc(fw, b_ptr, j++);
			*bb_ptr++ = b_ptr;
		}
	}
	return j;
}
/************************************************************************/

static int block_cmpr(const void *a, const void *b)
/* compare function for qsort, descending by score */
{
	return ((*(Block **)b)->score - (*(Block **)a)->score);
}

static void sort_block_list(Block **el, int n)
{
	qsort(el, n, sizeof *el, block_cmpr);
}
/************************************************************************/

long double get_pvalue (continuous a, int b)
{
	int i =0;
	long double one = 1, pvalue=0;
	long double poisson=one/exp(a);
	for (i=0;i<b+300;i++)
	{
		if (i>(b-1)) 
			pvalue=pvalue+poisson;
		else 
			poisson=poisson*a/(i+1);
	}
	return pvalue;
}
