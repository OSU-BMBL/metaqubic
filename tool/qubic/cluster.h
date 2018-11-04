#ifndef _CLUSTER_H
#define _CLUSTER_H

#include "struct.h"

/* this structure holds the matching score for each row */
struct rowMatch
{
    int row_id;
    int matches;
};

/* struct */
extern void verboseDot();

/* make_graph */
extern void seed_update (const discrete *s);

/* write_block */
extern void scan_block (struct dyStack *gene_set, Block *b_ptr);
extern void print_bc (FILE *fw, Block *b, int num);

/* prototypes */
static int compare_int (const void *a, const void *b);
static void update_colcand(bool *colcand, discrete *g1, discrete *g2);
static int intersect_row(const bool *colcand, discrete *g1, discrete *g2);
static int reverse_row(const bool *colcand, discrete *g1, discrete *g2);
static void seed_current_modify (const discrete *s, bool *colcand, int* cnt, int components);
static bool check_seed(Edge *e, Block **bb, const int block_id);
static void print_params(FILE *fw);
int cluster (FILE *fw, Edge **el, int n);
static int report_blocks(FILE *fw, Block **bb, int num);
static void sort_block_list(Block **el, int n);
static void block_init(Edge *e, Block *b, struct dyStack *genes, struct dyStack *scores,
bool *candidates, const int cand_threshold,int *components, struct dyStack *allincluster, long double *pvalues);
long double get_pvalue (continuous a, int b);

bits16 **profile;

#endif
