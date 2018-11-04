#ifndef _EXPAND_H
#define _EXPAND_H

/************************************************************************/

#include "struct.h"

/***********************************************************************/

discrete** another_arr_c;
char** another_genes;
char** another_conds;
int another_rows;
int another_cols;

extern continuous **arr;
extern discrete **arr_c;
extern discrete *symbols;
extern char **genes;
extern char **conds;
extern int rows, cols, sigma;

extern char **alloc2c (int rr, int cc);
extern void print_bc (FILE* fw, Block *b, int num);

static int intersect_row(const bool *colcand, discrete *g1, discrete *g2, const int cols);
static int reverse_row(const bool *colcand, discrete *g1, discrete *g2, const int cols);
void get_chars_size(FILE *fp);
void store_block(Block *b_ptr, struct dyStack *ge, struct dyStack *co);
static void init_expand();
void read_array(FILE *fp);
void read_and_solve_blocks(FILE *fb, const char *fn);
#endif
