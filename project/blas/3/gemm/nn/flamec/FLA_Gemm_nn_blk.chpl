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
def schurComplement(Ab: [1..n, 1..n+1] elemType, ptOp: indexType) {
  const AbD = Ab.domain;

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
}

//
// calculate C = C - A * B.
//
def dgemm(p: indexType,       // number of rows in A
          q: indexType,       // number of cols in A, number of rows in B
          r: indexType,       // number of cols in B
          A: [1..p, 1..q] ?t,
          B: [1..q, 1..r] t,
          C: [1..p, 1..r] t) {
  // Calculate (i,j) using a dot product of a row of A and a column of B.
  for i in 1..p do
    for j in 1..r do
      for k in 1..q do
        C[i,j] -= A[i, k] * B[k, j];
}