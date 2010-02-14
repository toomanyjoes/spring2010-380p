#define MAX_THREAD 1000

void parallel_prefix(void *data, int num_elem, int nodeval_elem);
void calculate(void *inputArray, int id, int numtasks, int inputArraySize, int nodeval_elem);

typedef struct {
	int myData;
	int id;
	int processed;
	int inputArraySize;
	int numtasks;
} dataStruct;


