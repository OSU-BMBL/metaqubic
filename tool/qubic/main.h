/* #ifndef checks whether the given token has been #defined earlier in the file or in an included file; if not, it includes the code between it and the closing #else or, if no #else is present, #endif statement. 
 * #ifndef is often used to make header files idempotent by defining a token once the file has been included and checking that the token was not set at the top of that file.*/

#ifndef _MAIN_H
#define _MAIN_H

#include "struct.h"

/*get_option subroutine prototypes */
void get_options (int argc, char* argv[]);

/* read_array subroutine prototypes  */
extern char** alloc2c (int rows, int cols);
extern void write_imported (const char* stream_nm);
extern void get_matrix_size (FILE* fp);
extern void read_labels (FILE* fp);
extern void read_discrete (FILE* fp);
extern void read_continuous (FILE* fp);
extern void discretize (const char* stream_nm);
extern void read_list (FILE* fp);

/*make_graph subroutine prototypes */
extern void make_graph (const char* fn);

/* expand subroutine prototypes */
extern void read_and_solve_blocks(FILE *fb, const char *fn);

#endif
