#ifndef _MAKE_GRAPH_H
#define _MAKE_GRAPH_H

#include "struct.h"

/* global data */
int col_width;
Edge **edge_list;
Edge *edge_ptr;

/* from cluster */
extern bits16 **profile;

/* global functions */
extern int cluster (FILE *fw, Edge **el, int n);

/* prototypes */
void seed_update (const discrete *s);
static int str_intersect_r (const discrete *s1, const discrete *s2);
void seed_deduct (const discrete *s);
void make_graph (const char *fn);
static int edge_cmpr(void *a, void *b);
static void fh_insert_fixed(struct fibheap *a, Edge *i, Edge **cur_min);
static void fh_dump(struct fibheap *a, Edge **res);
static int too_many_zeros(const discrete *s);
void find_too_many_zero_genes(int too_many_zero_array[], discrete **arr_c);

#endif
