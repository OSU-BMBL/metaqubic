/* Author:Qin Ma <maqin@csbl.bmb.uga.edu>, Jan. 22, 2010
 * Usage: This is part of bicluster package. Use, redistribution, modify without limitations
 * Process the options for the commandline tool
 */

/***************************************************************************/ 

#include "get_options.h"

/***************************************************************************/ 
static const char USAGE[] = 
"===================================================================\n\
[Usage]\n\
$ ./qubic -i filename [argument list]\n\
===================================================================\n\
[Input]\n\
-i : input file must be one of two tab-delimited formats\n\
  A) continuous data (default, use pre-set discretization (see -q and -r))\n\
     -------------------------------------\n\
     o        cond1    cond2    cond3\n\
     gene1      2.4      3.5     -2.4\n\
     gene2     -2.1      0.0      1.2\n\
     -------------------------------------\n\
  B) discrete data with arbitray classes (turn on -d)\n\
     use '0' for missing or insignificant data\n\
     -------------------------------------\n\
     o        cond1    cond2    cond3\n\
     gene1        1        2        2\n\
     gene2       -1        2        0\n\
     -------------------------------------\n\
-q : use quantile discretization for continuous data\n\
     default: 0.06 (see details in Method section in paper)\n\
-r : the number of ranks as which we treat the up(down)-regulated value\n\
     when discretization\n\
     default: 1\n\
-d : discrete data, where user should send their processed data\n\
     to different value classes, see above\n\
-b : the file to expand in specific environment\n\
-T : to-be-searched TF name, just consider the seeds containing current TF\n\
     default format: B1234\n\
-P : the flag to enlarge current biclsuter by the pvalue constrain\n\
-S : the flag using area as the value of bicluster to determine when stop\n\
-C : the flag using the lower bound of condition number (5 persents of the gene number)\n\
-l : the list of genes out of the input file on which we do bicluster\n\
===================================================================\n\
[Output]\n\
-o : number of blocks to report, default: 100\n\
-f : filtering overlapping blocks,\n\
     default: 1 (do not remove any blocks)\n\
-k : minimum column width of the block,\n\
     default: 5% of columns, minimum 2 columns\n\
-c : consistency level of the block (0.5-1.0], the minimum ratio between the\n\
     number of identical valid symbols in a column and the total number \n\
     of rows in the output\n\
     default: 0.95\n\
-s : expansion flag\n\
===================================================================\n";

static void init_options ()
{
	/* default parameters */
	/* strcpy: Copies the C string pointed by source into the array pointed by destination, including the terminating null character.
	 * To avoid overflows, the size of the array pointed by destination shall be long enough to contain the same C string as source (including the terminating null character), and should not overlap in memory with source
	 */
	strcpy(po->FN, " ");
	strcpy(po->BN, " ");
	strcpy(po->LN, " ");
	strcpy(po->TFname, " ");
	po->IS_DISCRETE = FALSE;
	po->IS_TFname = FALSE;
	po->IS_pvalue = FALSE;
	po->COL_WIDTH = 2;
	po->DIVIDED = 1;
	/*.06 is set as default for its best performance for ecoli and yeast functional analysis*/
	po->QUANTILE = .06;
	po->TOLERANCE = .95;
	po->FP = NULL;
	po->FB = NULL;
	po->RPT_BLOCK = 100;
	po->SCH_BLOCK = 200;
	po->FILTER = 1;
	po->IS_SWITCH = FALSE;
	po->IS_area = FALSE;
	po->IS_cond = FALSE;
	po->IS_list = FALSE;
}

/*argc is a count of the arguments supplied to the program and argc[] is an array of pointers to the strings which are those arguments-its type is array of pointer to char
 */
void get_options (int argc, char* argv[])
{
	int op;
	bool is_valid = TRUE;

	/*set memory for the point which is decleared in struct.h*/
	AllocVar(po);
	/*Initialize the point*/
	init_options();
	
	/*The getopt function gets the next option argument from the argument list specified by the argv and argc arguments. 
	 *Normally these values come directly from the arguments received by main
	 */
	/*An option character in this string can be followed by a colon (:) to indicate that it takes a required argument.
	 *If an option character is followed by two colons (::), its argument is optional
	 *if an option character is followed by no colons, it does not need argument
	 */
	while ((op = getopt(argc, argv, "i:b:q:r:dsf:k:o:c:T:PSCl:h")) >0)
	{
		switch (op)
		{
			/*optarg is set by getopt to point at the value of the option argument, for those options that accept arguments*/
			case 'i': strcpy(po->FN, optarg); break;
			case 'b': strcpy(po->BN, optarg); break;
			/*atof can convert string to double*/
			case 'q': po->QUANTILE = atof(optarg); break;
			/*atoi can convert string to integer*/
			case 'r': po->DIVIDED = atoi(optarg); break;
			case 'd': po->IS_DISCRETE = TRUE; break;
			case 's': po->IS_SWITCH = TRUE; break;
			case 'f': po->FILTER = atof(optarg); break; 
			case 'k': po->COL_WIDTH = atoi(optarg); break;
			case 'c': po->TOLERANCE = atof(optarg); break;
			case 'o':
				po->RPT_BLOCK = atoi(optarg); 
				po->SCH_BLOCK = 2*po->RPT_BLOCK;
				/* ensure enough searching space */
				/*if (po->SCH_BLOCK < 1000) po->SCH_BLOCK = 1000;*/ 
				break;
			/*puts writes the C string pointed by str to stdout and appends a newline character ('\n')*/
			/*exit(0) causes the program to exit with a successful termination
			 *break is normally used to jump to the end of the current block of code
			 *exit is normally used to shut down the current process
			 */
			case 'T': strcpy(po->TFname, optarg); po->IS_TFname = TRUE; break;
			case 'P': po->IS_pvalue = TRUE; break; 
			case 'S': po->IS_area = TRUE; break; 
			case 'C': po->IS_cond = TRUE; break; 
			case 'l': strcpy(po->LN, optarg); po->IS_list =TRUE; break;
			case 'h': puts(USAGE); exit(0); 
			/*if expression does not match any constant-expression, control is transferred to the statement(s) that follow the optional default label*/
			default : is_valid = FALSE;
		}
	}
	/* basic sanity check */
        if (is_valid && po->FN[0] == ' ') 
	{
		/*errAbort("You must specify input file (-i)");*/
		puts(USAGE); 
		exit(0);
	}
	if (is_valid)
	{
		po->FP = mustOpen(po->FN, "r");
	}
	if (po->IS_SWITCH) 
	{	
		po->IS_DISCRETE = TRUE; 
		po->FB = mustOpen(po->BN, "r");
	}
	if (po->IS_list)
	{
		po->FL = mustOpen(po->LN, "r");
	}
	
	/* option value range check */
	if ((po->QUANTILE >.5) || (po->QUANTILE <= 0))
	{
		err("-q quantile discretization should be (0,.5]");
		is_valid = FALSE;
	}
	if ((po->FILTER > 1) || (po->FILTER < 0))
	{
		err("-f overlapping filtering should be [0,1.]");
		is_valid = FALSE;
	}
	if ((po->TOLERANCE > 1) || (po->TOLERANCE <= .5))
	{
		err("-c noise ratio should be (.5,1]");
		is_valid = FALSE;
	}
	if (po->COL_WIDTH < 2 && po->COL_WIDTH != -1)
	{
		err("-k minimum column width should be >=2");
		is_valid = FALSE;
	}
	if (po->RPT_BLOCK <= 0)
	{
		err("-n number of blocks to report should be >0");
		is_valid = FALSE;
	}
	if (!is_valid)
		errAbort("Type -h to view possible options");

}
/***************************************************************************/ 

