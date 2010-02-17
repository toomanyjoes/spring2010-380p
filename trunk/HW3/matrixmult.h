typedef struct {
	int rows;		// number of matrix rows
	int columns;		// number of matrix columns
	int non_zero_entries;	// number of non-zero entries i.e. size of arrays
	int *row_coord;		// array storing row coordinates
	int *col_coord;		// array storing column coordinates
	double *entry;		// array storing the non-zero entries
} matrix;

void freeMatrix(matrix *);
