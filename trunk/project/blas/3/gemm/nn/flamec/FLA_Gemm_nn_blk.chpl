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
use constants;
const blkSize:int = 10;
type elemType = real;
type indexType = int;
/*// def schurComplement(Ab: [1..n, 1..n+1] elemType, ptOp: dim_t) {
def schurComplement(Ab: [?AbD] elemType, ptOp: indexType) {
  //const AbD = Ab.domain;

  //
  // Calculate location of ptSol (see diagram above)
  //
  const ptSol = ptOp+blkSize;

  //
  // Copy data into replicated array so every processor has a local copy
  // of the data it will need to perform a local matrix-multiply.  These
  // replicated distributions aren't implemented yet, but imagine that
  // they look something like the following:
  //
  //var replAbD: domain(2) 
  //            dmapped new Dimensional(BlkCyc(blkSize), Replicated)) 
  //          = AbD[ptSol.., 1..#blkSize];
  //
  const replAD: domain(2) = AbD[ptSol.., ptOp..#blkSize],
        replBD: domain(2) = AbD[ptOp..#blkSize, ptSol..];
    
  const replA : [replAD] elemType = Ab[ptSol.., ptOp..#blkSize],
        replB : [replBD] elemType = Ab[ptOp..#blkSize, ptSol..];

  // do local matrix-multiply on a block-by-block basis
  forall (row,col) in AbD[ptSol.., ptSol..] by (blkSize, blkSize) {
    //
    // At this point, the dgemms should all be local, so assert that
    // fact
    //
    local {
      const aBlkD = replAD[row..#blkSize, ptOp..#blkSize],
            bBlkD = replBD[ptOp..#blkSize, col..#blkSize],
            cBlkD = AbD[row..#blkSize, col..#blkSize];

      dgemm(aBlkD.dim(1).length,
            aBlkD.dim(2).length,
            bBlkD.dim(2).length,
            replA(aBlkD),
            replB(bBlkD),
            Ab(cBlkD));
    }
  }
}*/

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
  //const AbD = Ab.domain;

  //
  // Calculate location of ptSol (see diagram above)
  //
//   const ptSol = ptOp+blkSize;

  //
  // Copy data into replicated array so every processor has a local copy
  // of the data it will need to perform a local matrix-multiply.  These
  // replicated distributions aren't implemented yet, but imagine that
  // they look something like the following:
  //
  //var replAbD: domain(2) 
  //            dmapped new Dimensional(BlkCyc(blkSize), Replicated)) 
  //          = AbD[ptSol.., 1..#blkSize];
  //


//   const replARowsD: domain(2) = A.base.bufDomain[1.., 1..#blkSize];
//         replAColsD: domain(2) = A.base.bufDomain[1..#blkSize, 1..];
//   const replBRowsD: domain(2) = B.base.bufDomain[1.., 1..#blkSize],
//    const     replBColsD: domain(2) = B.base.bufDomain[1..#blkSize, 1..];
//    const   replA : [replARowsD] elemType = A.base.buffer[1.., 1..#blkSize],
//          replB : [replBColsD] elemType = B.base.buffer[1..#blkSize, 1..];
// writeln("replARowsD: ",replARowsD);
// writeln("replBColsD: ", replBColsD);
// writeln("replA:\n", replA);
// writeln("replB:\n", replB);
  // do local matrix-multiply on a block-by-block basis
// startVerboseComm();
startCommDiagnostics();
   var loc:int = -1;
   forall (row,col) in C.base.bufDomain by (blkSize, blkSize) {
  loc = (loc + 1) % numLocales;
// writeln("loc: ", loc);
  on Locales(loc) {
//    const row = here.id * blkSize;
    const replARowsD: domain(2) = A.base.bufDomain[row..row+blkSize-1, 1..];
//        replAColsD: domain(2) = A.base.bufDomain[row..#blkSize, 1..];
//  const replBRowsD: domain(2) = B.base.bufDomain[1.., col..col+blkSize-1];
     const     replBColsD: domain(2) = B.base.bufDomain[1.., col..col+blkSize-1];
     const     replCD: domain(2) = C.base.bufDomain[row..row+blkSize-1, col..col+blkSize-1];
     var alphaVal: real;
     var betaVal: real;
     var replA : [replARowsD] elemType;
     var replB : [replBColsD] elemType;
     var replC : [replCD] elemType;
     cobegin {
       replA = A.base.buffer[row..row+blkSize-1, 1..];
       replB = B.base.buffer[1.., col..col+blkSize-1];
       replC = C.base.buffer(replCD);
//  writeln("id: ", here.id, " replARowsD: ", replARowsD);
//  writeln("replBColsD: ", replBColsD);
//  writeln("replA:\n", replA);
//  writeln("replB:\n", replB);
    //
    // At this point, the dgemms should all be local, so assert that
    // fact
    //
       alphaVal = alpha.base.buffer(1,1);
       betaVal = beta.base.buffer(1,1);
     }
     var cBlkD: domain(2);
     local {
      const aBlkD = replARowsD[row..row+blkSize-1, 1..];
// writeln("one");
      const bBlkD = replBColsD[1.., col..col+blkSize-1];
// writeln("two");
      cBlkD = replCD[row..row+blkSize-1, col..col+blkSize-1];
//  writeln("\naBlkD: ", aBlkD);
//  writeln(A.base.buffer(aBlkD));
//  writeln("bBlkD: ", bBlkD);
//  writeln(B.base.buffer(bBlkD));
// writeln("cBlkD: ", cBlkD);
// writeln("row: ", row, " col: ", col, " aBlkD.dim(1).length: ",aBlkD.dim(1).length, "  aBlkD.dim(2).length: ", aBlkD.dim(2).length, " bBlkD.dim(2).length: ",bBlkD.dim(1).length, " cBlkD: ", cBlkD);
      dgemm(aBlkD.dim(1).length,
            aBlkD.dim(2).length,
            bBlkD.dim(2).length,
            replA(aBlkD),
            replB(bBlkD),
            replC(cBlkD), //C.base.buffer(cBlkD),
            alphaVal,    //alpha.base.buffer(1,1),
            betaVal);    //beta.base.buffer(1,1));
    } // end local
// writeln("C: \n",C.base.buffer);
      C.base.buffer(cBlkD) = replC(cBlkD);
    }  // end on loc
  } // end forall
// stopVerboseComm();
stopCommDiagnostics();
writeln(getCommDiagnostics());
}

