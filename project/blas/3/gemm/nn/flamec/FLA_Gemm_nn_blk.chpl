/*

Modified from $CHPL_HOME/examples/hpcc/hpl.chpl

*/



//
// Distributed matrix-multiply for HPL. The idea behind this algorithm is that
// some point the matrix will be partioned as shown in the following diagram:
//
//    [1]----+-----+-----+-----+
//     |     |bbbbb|bbbbb|bbbbb|  Solve for the dotted region by
//     |     |bbbbb|bbbbb|bbbbb|  multiplying the 'a' and 'b' region.
//     |     |bbbbb|bbbbb|bbbbb|  The 'a' region is a block column, the
//     +----[2]----+-----+-----+  'b' region is a block row.
//     |aaaaa|.....|.....|.....|
//     |aaaaa|.....|.....|.....|  The vertex labeled [1] is location
//     |aaaaa|.....|.....|.....|  (ptOp, ptOp) in the code below.
//     +-----+-----+-----+-----+
//     |aaaaa|.....|.....|.....|  The vertex labeled [2] is location
//     |aaaaa|.....|.....|.....|  (ptSol, ptSol)
//     |aaaaa|.....|.....|.....|
//     +-----+-----+-----+-----+
//
// Every locale with a block of data in the dotted region updates
// itself by multiplying the neighboring a-region block to its left
// with the neighboring b-region block above it and subtracting its
// current data from the result of this multiplication. To ensure that
// all locales have local copies of the data needed to perform this
// multiplication we copy the data A and B data into the replA and
// replB arrays, which will use a dimensional (block-cyclic,
// replicated-block) distribution (or vice-versa) to ensure that every
// locale only stores one copy of each block it requires for all of
// its rows/columns.
//
use constants, CyclicDist;
// const blkSize:int = 100;
type elemType = real;
type indexType = int;

//
// calculate C = C + A * B.
//
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

def FLA_Gemm_nn_blk( alpha: FLA_Obj, A: FLA_Obj, B: FLA_Obj, beta: FLA_Obj, C:FLA_Obj ): FLA_Error
{
// startVerboseComm();
// startCommDiagnostics();
   var blkSize: int;
   if A.base.bufDomain.high(2) % numLocales == 0 then
     blkSize = A.base.bufDomain.high(2)/numLocales;
   else
     blkSize = 10;
   var loc:int = -1;
   for (row,col) in C.base.bufDomain by (blkSize, blkSize) {
     loc = (loc + 1) % numLocales;
     on Locales(loc) {
       const localRow = row;
       const localCol = col;
       const localBlkSize = blkSize;
       const replARowsD: domain(2) = A.base.bufDomain[localRow..localRow+localBlkSize-1, 1..];
       const replBColsD: domain(2) = B.base.bufDomain[1.., localCol..localCol+localBlkSize-1];
       const replCD:     domain(2) = C.base.bufDomain[localRow..localRow+localBlkSize-1, localCol..localCol+localBlkSize-1];
       var alphaVal: real;
       var betaVal: real;
       var replA : [replARowsD] elemType;
       var replB : [replBColsD] elemType;
       var replC : [replCD] elemType;

       replA = A.base.buffer(replARowsD);
       replB = B.base.buffer(replBColsD);
       replC = C.base.buffer(replCD);
       alphaVal = alpha.base.buffer(1,1);
       betaVal = beta.base.buffer(1,1);

       var cBlkD: domain(2);
       local {
         dgemm(replA.domain.dim(1).length,
               replA.domain.dim(2).length,
               replB.domain.dim(2).length,
               replA,
               replB,
               replC, //C.base.buffer(cBlkD),
               alphaVal,    //alpha.base.buffer(1,1),
               betaVal);    //beta.base.buffer(1,1));
        } // end local
      C.base.buffer(replCD) = replC;
      }  // end on loc
    } // end forall
// stopVerboseComm();
// stopCommDiagnostics();
// writeln(getCommDiagnostics());
}

