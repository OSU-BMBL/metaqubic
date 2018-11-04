#ifndef _STRUCT_H
#define _STRUCT_H

#ifndef _GNU_SOURCE 
#define _GNU_SOURCE 
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#include <sys/time.h>
#include <time.h>
#include "fib.h"

/***** Useful macros *****/

/* Compatibility of __attribute__ with non-GNU */
#ifndef __GNUC__
#  define __attribute__(x) /* Nothing */
#endif

/* Pretend that C has boolean type */
#define TRUE 1
#define FALSE 0
#define boolean unsigned char
#ifndef __cplusplus
#ifndef bool
#define bool unsigned char
#endif
#endif

#define MAX(a,b)  ((a)>(b)?(a):(b))
#define MIN(a,b)  ((a)<(b)?(a):(b))
#define ABS(x)    ((x)>0?(x):-(x))

/* Variable and array allocation */
#define AllocVar(pt) (pt = xmalloc(sizeof(*pt)))
#define AllocArray(pt, size) (pt = xmalloc(sizeof(*pt) * (size)))
#define ReAllocArray(pt, size) (pt = xrealloc(pt, sizeof(*pt) * (size)))

/* Strings */
/* strcmp: a zero value indicates that both strings are equal.
 * a value greater than zero indicates that the first character that does not match has a greater value in str1 than in str2; 
 * And a value less than zero indicates the opposite.
 */
#define sameString(a, b) (strcmp((a), (b))==0)
/* Returns TRUE if two strings are same */

/* Debugging */
#define internalErr() errAbort("Internal error %s %d", __FILE__, __LINE__)

/* Constants */
#define LABEL_LEN 64 

#ifndef NULL
#define NULL 0
#endif

/* Two major data types */
typedef float continuous;
typedef short discrete;

/* global data */
continuous **arr;
discrete **arr_c;
discrete *symbols;
char **genes;
char **conds;
char **sub_genes;
bool *sublist;
int rows, cols, sigma;
int TFindex;
int sub_genes_row;
/***** Structures *****/

struct dyStack
/* dynamically allocated stack */
  {
  int top;             /* top element index */
  int items[];		   /* data storage */
  };

/* edge between two genes */
typedef struct Edge{
	int gene_one;
	int gene_two;
	int score;
} Edge;

/* biclustering block */
typedef struct Block{
	struct dyStack *genes;
	struct dyStack *conds;
	int score;
	int block_rows;
	int block_cols;
	int block_rows_pre;
	int cond_low_bound;
	double significance;
	long double pvalue;
} Block;

/* holds running options */
typedef struct Prog_options{
	char FN[LABEL_LEN];
	char BN[LABEL_LEN];
	char LN[LABEL_LEN];
	bool IS_SWITCH;
	bool IS_DISCRETE;
	bool IS_TFname;
	bool IS_pvalue;
	bool IS_area;
	bool IS_cond;
	bool IS_list;
	int COL_WIDTH;
	int DIVIDED;
	int SCH_BLOCK;
	int RPT_BLOCK;
	double FILTER;
	double QUANTILE;
	double TOLERANCE;
	char TFname[LABEL_LEN];
	FILE* FP;
	FILE* FB;
	FILE* FL;
} Prog_options;

typedef unsigned short int bits16;
enum {UP=1, DOWN=2, IGNORE=3};

Prog_options* po;
/***** Helper functions *****/

void progress(char *format, ...)
/* Print progress message */
     __attribute__((format(printf, 1, 2)));

void verboseDot();
/* Print "i-am-alive" dot */

void err(char *format, ...)
/* Print error message but do not exit */
     __attribute__((format(printf, 1, 2)));

void errAbort(char *format, ...)
/* Print error message to stderr and exit */
     __attribute__((noreturn, format(printf, 1, 2)));

void uglyTime(char *label, ...);
/* Print label and how long it's been since last call.  Call with 
 * a NULL label to initialize. */

void *xmalloc ( int size );
/* Wrapper for memory allocations */

void *xrealloc ( void* ptr, int size );
/* Wrapper for memory re-allocations */

/* Stack-related operations */
struct dyStack *dsNew(int size);
void dsPrint(struct dyStack *ds);
void dsPush(struct dyStack *ds,int element);

#define dsFree free
/* Release the stack data structure */

#define dsClear(pds) ((pds)->top = -1)
/* Remove all the data in the stack */

#define dsSize(pds) ((pds)->top + 1)
/* Return the size of the stack */

#define dsItem(pds, j) ((pds)->items[j])
/* Return the j-th item in the stack */

bool isInStack(struct dyStack *ds, int element);
int dsIntersect(struct dyStack *ds1, struct dyStack *ds2);

/* File-related operations */
void *addSuffix();
/* Return a string containing headsuffix */

FILE *mustOpen(const char *fileName, char *mode);
/* Open a file or die */

#endif
