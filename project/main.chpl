// defines our main function

use Time, constants, FLA_Gemm_nn_unb_var1;

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
	var Aarr = readArray(Afile);
	var Barr = readArray(Bfile);

	var A: FLA_Obj = new FLA_Obj();
	var B: FLA_Obj = new FLA_Obj();
	var C: FLA_Obj = new FLA_Obj();
	var alpha: FLA_Obj = new FLA_Obj();
	var beta: FLA_Obj = new FLA_Obj();
	FLA_Obj_create(FLA_DOUBLE, Aarr.domain.high(1):uint, Aarr.domain.high(2):uint, 0, 0, A);
	FLA_Obj_create(FLA_DOUBLE, Barr.domain.high(1):uint, Barr.domain.high(2):uint, 0, 0, B);
	FLA_Obj_create(FLA_DOUBLE, Aarr.domain.high(1):uint, Barr.domain.high(2):uint, 0, 0, C);
	FLA_Obj_create(FLA_DOUBLE, 1, 1, 0, 0, alpha);
	FLA_Obj_create(FLA_DOUBLE, 1, 1, 0, 0, beta);

	A.base.buffer = Aarr;
	B.base.buffer = Barr;
	alpha.base.buffer(1,1) = alphaVal;
	beta.base.buffer(1,1) = betaVal;

	var myTimer: Timer;
	myTimer.start();
	FLA_Gemm_nn_unb_var1(alpha, A, B, beta, C);
	myTimer.stop();
//	writeln("A:\n",A.base.buffer);
//	writeln("B:\n",B.base.buffer);
//	writeln("C:\n", C.base.buffer);
	writeln("Time: ", myTimer.elapsed());
	writeArray(C.base.buffer, answer);
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

