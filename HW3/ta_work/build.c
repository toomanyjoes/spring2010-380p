//sri
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(int argc,char ** argv){
  int rows = atoi(argv[1]);
  int cols = atoi(argv[2]);
  int  prob = (int)(100 * atof(argv[3]));
  FILE * temp = fopen("matrix.mm.temp","w+"); 
  FILE * out  = fopen("matrix.mm", "w");
  if(temp == 0 || out == 0){printf("I/O Failure");exit(1);}

  //build the output
  int r,c,entries = 0;
  double value = 0;
  srand ( time(NULL) );
  for(r=1;r<=rows;r++){
    for(c=1;c<=cols;c++){
      if(rand() % 100 < prob){

	/**

     ******EDIT HERE FOR DIFFERENT VALUES******

	**/

	//value is [0,1)
	value = ((double)rand()/((double)(RAND_MAX)+(double)(1)));

	fprintf(temp, "\t%d\t%d\t%e\n", r,c,value);
	entries++;
      }
    }
  }

  //write the header
  fprintf(out, "%%%MatrixMarket matrix coordinate real general\n");
  fprintf(out, "%d %d %d\n", rows, cols, entries);

  //copy temporary data to output file
  long size = ftell(temp);
  int count = 0;
  rewind(temp);
  char * buffer;
  buffer = (char*) malloc (sizeof(char) * 1024 * 1024 * 5);

  while( (count = fread (buffer,1, 1024 * 1024 * 5 ,temp)) != EOF){
      fwrite(buffer,1,count, out);
      if(ftell(temp) >= size){break;}
  }

  //remove temp file
  remove("matrix.mm.temp");

}



