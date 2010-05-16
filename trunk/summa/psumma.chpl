// defines our main function

use Time;

config var Afile: string = "";
config var Bfile: string = "";
config var blkrows: int = -1;
config var blkcols: int = -1;
config var answer: string = "answer";
config var alphaVal: real = 1.0;
config var betaVal: real = 1.0;
type elemType = real;
type indexType = int;

//     Parallel Scalable Universal Matrix Multiplication Algortihm (SUMMA)
//    [1]----+-----+-----+-----+    [1]----+-----+-----+-----+
//     |aaaaa|.....|.....|.....|     |bbbbb|bbbbb|bbbbb|bbbbb| 
//     |aaaaa|.....|.....|.....|     |bbbbb|bbbbb|bbbbb|bbbbb| 
//     |aaaaa|.....|.....|.....|     |bbbbb|bbbbb|bbbbb|bbbbb|  
//     +----[2]----+-----+-----+     +----[2]----+-----+-----+ 
//     |aaaaa|.....|.....|.....|     |.....|.....|.....|.....|
//     |aaaaa|.....|.....|.....|     |.....|.....|.....|.....|  
//     |aaaaa|.....|.....|.....|     |.....|.....|.....|.....| 
//     +-----+-----+-----+-----+     +-----+-----+-----+-----+
//     |aaaaa|.....|.....|.....|     |.....|.....|.....|.....| 
//     |aaaaa|.....|.....|.....|     |.....|.....|.....|.....|  
//     |aaaaa|.....|.....|.....|     |.....|.....|.....|.....|
//     +-----+-----+-----+-----+     +-----+-----+-----+-----+
//
//

def main()
{
	if Afile == "" || Bfile == ""
	{
		writeln("Must provide 2 files containing matrices.");
		exit(1);
	}
	if blkrows == -1 || blkcols == -1
	{
		writeln("Must enter a blocksize 'i.e. --blkrows 3 --blkcols 3'");
		exit(1);
	}
	var A = readArray(Afile);
	var B = readArray(Bfile);
	var m = A.domain.high(1);
	var n = A.domain.high(2);
	var p = B.domain.high(2);
	var C: [1..m, 1..p] real;
	var loc1:int = -1;	
	var loc2:int = -1;
	var loc1cnt:int = -1;	
	var loc2cnt:int = -1;
	
	const AD = A.domain,
			BD = B.domain,
			CD = C.domain;

	var myTimer: Timer;
	myTimer.start();

	for pnl in 1..n by blkcols
	{
	
 		//startCommDiagnostics();
		loc1 = (loc1 + 1) % numLocales;
		loc1cnt = loc1cnt + 1;
		on Locales(loc1)
		{
			var start: int = pnl;
			//writeln(start, " -> ", "[",start%(m+1)+1,", ", (start+blksize-1)%(m+1)+1,"]");
			var replAD: domain(2) = AD[.., start..start+blkcols-1],
				 replBD: domain(2) = BD[start..start+blkcols-1, ..];			 
				 //replCD: domain(2) = CD[start..start+blksize-1, ..];
		
			var replA : [replAD] real = A[.., start..start+blkcols-1],
				 replB : [replBD] real = B[start..start+blkcols-1, ..];				 
				 //replC : [replCD] real = C[start..start+blksize-1, ..];
		
			for blk in 1..m by blkrows
			{		
				loc2 = (loc2 + 1) % numLocales;
				loc2cnt = loc2cnt + 1;
				on Locales(loc2)
			   {
					var start1: int = blk;
					
					var aBlkD: domain(2) = replAD[start1..start1+blkrows-1, ..],							   
						 repl2BD: domain(2) = replBD,
						 replCD: domain(2) = CD[start1..start1+blkrows-1, ..];
					
					var aBlk : [aBlkD] real = replA[start1..start1+blkrows-1, ..],
				 		 repl2B : [repl2BD] real = replB,
				 		 replC : [replCD] real;
				 		 
					local {
						dgemm(aBlk.domain.dim(1).length,
					   aBlk.domain.dim(2).length,
					   repl2B.domain.dim(2).length,
					   aBlk,
					   repl2B,
					   replC,
					   1.0,
					   1.0);  
					} 
					
					//writeln(loc2, "aBlk: ", aBlk, "\n\n");
					//writeln("replB: ", repl2B, "\n\n");
					//writeln("replC: ", replC, "\n\n");
					C[loc2cnt%(m/blkrows)*blkrows+1..(loc2cnt%(m/blkrows)*blkrows)+blkrows, ..] += replC;
					//writeln(loc2, "C: ", C, "\n\n");
				}
			}
		} 	
		//stopCommDiagnostics();
  		//writeln(getCommDiagnostics());
	}
	myTimer.stop();
	//writeln("A:\n",A);
	//writeln("B:\n",B);
	//writeln("C:\n",C);
	writeArray(C, answer);
	writeln("Time: ", myTimer.elapsed());
	
// end forall
// stopVerboseComm();
//   stopCommDiagnostics();
  //writeln(getCommDiagnostics());
  //var diag = getCommDiagnostics();
  //var total = 0;
  //for i in diag.domain
  //{
    //writeln(diag(i));
    //writeln(diag(i)(1) + diag(i)(2));
  //  total += diag(i)(1) + diag(i)(2);
  //}
  //writeln("total: ", total);
}

def dgemm(p: indexType,       // number of rows in A
          q: indexType,       // number of cols in A, number of rows in B
          r: indexType,       // number of cols in B
          A: [1..p, 1..q] ?t,
          B: [1..q, 1..r] t,
          C: [1..p, 1..r] t,
          alpha: t,
          beta: t) {
  // Calculate (i,j) using a dot product of a row of A and a column of B.
  for i in 1..p do
    for j in 1..r do
      for k in 1..q do
        C[i,j] += (alpha * A[i, k]) * (beta * B[k, j]);
}

def blas_dgemv( transa: string, m: int, n: int, alpha: real, a: [?aDom] real, lda: int, x: [?xDom] real, incx: int, beta: real, y: [?yDom] real, incy: int )
{
	var AprimeDom: domain(2);
	var Aprime: [AprimeDom] real;
	AprimeDom = [1..aDom.high(1), 1..aDom.high(2)];
	if(transa == "true")
	{
		AprimeDom = [1..aDom.high(2), 1..aDom.high(1)];
	}
	cobegin
	{
		forall i in yDom
		{
			y(i) = beta * y(i);
		}
		forall (i,j) in aDom
		{
			Aprime(j,i) = alpha * a(i,j);
		}
	}
 	forall (i,j) in aDom
 	{
 		y(i) += Aprime(i,j) * x(j);
 	}
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
  var XD: domain(2) = [1..m, 1..n];
  var X: [XD] real;

  //
  // Read in the array elements one by one (eventually, you should be
  // able to read in the array wholesale, but this isn't currently
  // supported.
  //
  for i in 1..m do
    for j in 1..n do
      infile.read(X[i,j]);

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

