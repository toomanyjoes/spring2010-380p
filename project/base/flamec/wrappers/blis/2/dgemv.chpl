
use constants;

def blas_dgemv( transa: string, m: int, n: int, alpha: real, a: [?aDom] real, lda: int, x: [?xDom] real, incx: int, beta: real, y: [?yDom] real, incy: int )
{
	forall i in yDom
	{
		y(i) = beta * y(i);
	}
	var ProbSpace: domain(2) = [1..2, 1..2];
	var AprimeDom: domain(2) = [1..aDom.high(1), 1..aDom.high(2)];
	if(transa == BLIS_TRANSPOSE)
	{
		AprimeDom = [1..aDom.high(2), 1..aDom.high(1)];
	}
	var Aprime: [AprimeDom] real;
	forall (i,j) in aDom
	{
		Aprime(j,i) = alpha * a(i,j);
	}
 	forall (i,j) in aDom
 	{
 		y(incy,i) += Aprime(i,j) * x(incx,j);
 	}
}


