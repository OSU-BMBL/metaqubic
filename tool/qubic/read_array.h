#ifndef _READ_ARRAY_H
#define _READ_ARRAY_H

/************************************************************************/

#include "struct.h"

/***********************************************************************/

/* prototypes  */
continuous **alloc2d (int rr, int cc);
discrete **alloc2c (int rr, int cc);
void write_imported (const char *stream_nm);
void get_matrix_size (FILE *fp);
void read_labels (FILE *fp);
void read_discrete (FILE *fp);
void read_continuous (FILE *fp);
void discretize (const char *stream_nm);
discrete dis_value(float current, int divided, float *small, int cntl, float *big, int cntu);
void read_list (FILE* fp);
/*static continuous quantile_from_sorted_data_value(const continuous sorted_data[], size_t n, double f);*/
/***********************************************************************/

#endif
