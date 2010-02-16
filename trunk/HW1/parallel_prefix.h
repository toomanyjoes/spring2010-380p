#define MAX_THREAD 1000

int parallel_prefix(void *data, int num_elem);
void *thread_main(void *arg);

typedef struct {
	int myData;
	int id;
	int processed;
} dataStruct;


