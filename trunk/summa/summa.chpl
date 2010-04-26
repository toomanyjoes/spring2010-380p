// defines our main function

use Time;

config var Afile: string = "";
config var Bfile: string = "";
config var answer: string = "answer";
config var alphaVal: real = 1.0;
config var betaVal: real = 1.0;

def main()
{
	if Afile == "" || Bfile == ""
	{
		writeln("Must provide 2 files containing matrices.");
		exit(1);
	}
	var A = readArray(Afile);
	var B = readArray(Bfile);
	var m = A.domain.high(1);
	var n = A.domain.high(2);
	var p = B.domain.high(2);
	var C: [1..m, 1..p] real;
	var Col: [1..m] real;
	var Row: [1..p] real;
	var loc1: int = -1;
	var loc2: int = -1;
	var myTimer: Timer;
	myTimer.start();

	for k in (1..n)
	{
		loc1 = (k + 1) % numLocales;
		on Locales(loc1)
		{
			Col = A[1..m, k];
			Row = B[k, 1..p];
			forall (i,j) in C.domain
			{
				//loc2 = (i + 1) % numLocales;
				//on Locales(loc2)
				//{
				//local
				//{
					C(i,j) += Col(i) * Row(j);
				//}
				//}
			}
		}
	}
	
	myTimer.stop();
//	writeln("A:\n",A);
//	writeln("B:\n",B);
	writeln("C:\n", C);
	writeln("Time: ", myTimer.elapsed());
}

// From fileIO.chpl in Chapel tutorial
// This function reads a new array out of a file and returns it
//
def readArray(filename) {
   // Create an input file with the specified filename in read mode
  var infile = new file(filename, FileAccessMode.read);

  // Open the file
  infile.open();

  // Read the number of rows and columns in the array in from the file
  var m = infile.read(int), 
      n = infile.read(int);

  // Declare an array of the specified dimensions
  var X: [1..m, 1..n] real;

  //
  // Read in the array elements one by one (eventually, you should be
  // able to read in the array wholesale, but this isn't currently
  // supported.
  //
  for i in 1..m do
    for j in 1..n do
      infile.read(X(i,j));

  // Close the file
  infile.close();

  // Return the array
  return X;
}

//
// this function writes a square array out to a file
//
def writeArray(X, filename) {
  // Create an output file with the specified filename in write mode
  var outfile = new file(filename, FileAccessMode.write);

  // Open the file
  outfile.open();

  // Write the problem size in each dimension to the file
  outfile.writeln(X.domain.high(1), " ", X.domain.high(2));

  // write out the array itself
  outfile.write(X);

  // close the file
  outfile.close();
}

