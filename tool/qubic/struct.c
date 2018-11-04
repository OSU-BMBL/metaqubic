#include "struct.h"

/**************************************************************************/
/* helper functions for error msgs for allocating memory */

void progress(char *format, ...)
/* Print progress message */
{
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	fprintf(stdout, "\n");
	va_end(args);
}

void verboseDot()
/* Print "i-am-alive" dot */
{
	putchar('.');
	fflush(stdout);
}

void err(char *format, ...)
/* Print error message but do not exit */
{
	va_list args;
	va_start(args, format);
	fprintf(stderr, "[Error] ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	va_end(args);
}

void errAbort(char *format, ...)
/* Print error message and exit */
{
	va_list args;
	va_start(args, format);
	fprintf(stderr, "[Error] ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(1);
}

long clock1000()
/* A millisecond clock. */
{
    struct timeval tv;
    static long origSec;
    gettimeofday(&tv, NULL);
    if (origSec == 0) origSec = tv.tv_sec;
    return (tv.tv_sec-origSec)*1000 + tv.tv_usec / 1000;
}

void uglyTime(char *label, ...)
/* Print label and how long it's been since last call.  Call with 
 * a NULL label to initialize. */
{
    static long lastTime = 0;
    long time = clock1000();
    va_list args;
    va_start(args, label);
    if (label != NULL)
    {
        vfprintf(stdout, label, args);
        fprintf(stdout, " [%.3f seconds elapsed]\n", (time - lastTime)/1000.);
    }
    lastTime = time;
    va_end(args);
}

void* xmalloc ( int size )
/* Wrapper for standard mallc */
{
	/*malloc: The function malloc() returns a pointer to a chunk of memory of size size, or NULL if there is an error. 
	 *The memory pointed to will be on the heap, not the stack, so make sure to free it when you are done with it.*/
	register void* value = malloc(size);
	if (value == NULL)
		errAbort("Memory exhausted (xmalloc)");
	return value;
}

void* xrealloc ( void* ptr, int size )
/* Wrapper for standard reallc */
/* realloc may move the memory block to a new location, in which case the new location is returned. 
 * The content of the memory block is preserved up to the lesser of the new and old sizes, even if the block is moved. 
 * If the new size is larger, the value of the newly allocated portion is indeterminate.*/
{
	register void* value = realloc(ptr, size);
	if (value == NULL)
		errAbort("Memory exhausted (xrealloc)");
	return value;
}

/**************************************************************************/
struct dyStack *dsNew(int size)
/* Initialize the stack */
{
    int stackSize = (size+1) * sizeof(int);
    struct dyStack *ds = malloc(stackSize);
    dsClear(ds);
    return ds;
}

void dsPush(struct dyStack *ds, int element)
/* Add element to the stack */
{
    ds->items[++ds->top] = element;
}

void dsPrint(struct dyStack *ds)
/* Print out the stack elements */
{
    int i;
    printf("Stack contains %d elements\n", dsSize(ds));
    for (i=0; i<dsSize(ds); i++)
	    printf("%d ", ds->items[i]);
    putchar('\n');
}

bool isInStack(struct dyStack *ds, int element)
/* Test whether an elemente is in stack */
{
	bool flag = FALSE;
	int i;
	for (i=0; i<dsSize(ds); i++)
	{
		if (ds->items[i]==element)
		{
			flag = TRUE; break;
		}
  	}
	return flag;
}

int dsIntersect(struct dyStack *ds1, struct dyStack *ds2)
/* Return the number of common components between two arrays */
{
	int cnt = 0;
	int i;

	for (i=0; i<dsSize(ds1); i++)
		if (isInStack(ds2, ds1->items[i])) cnt++;

	return cnt;
}

/**************************************************************************/
/* file-related operations */

void *addSuffix(char *head, char *suffix)
/* Return a string containing headsuffix */
{
    char *ret = NULL;
    int size = strlen(head) + strlen(suffix) + 1;
    AllocArray(ret, size);
    snprintf(ret, size, "%s%s", head, suffix);
    return ret;
}

FILE *mustOpen(const char *fileName, char *mode)
/* Open a file or die */
{
    FILE *f;

    if (sameString(fileName, "stdin")) return stdin;
    if (sameString(fileName, "stdout")) return stdout;
    if ((f = fopen(fileName, mode)) == NULL)
    {
        char *modeName = "";
        if (mode)
        {
            if (mode[0] == 'r') modeName = " to read";
            else if (mode[0] == 'w') modeName = " to write";
            else if (mode[0] == 'a') modeName = " to append";
        }
        errAbort("Can't open %s%s: %s", fileName, modeName, strerror(errno));
    }
    return f;
}

/**************************************************************************/
