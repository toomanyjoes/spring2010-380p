
use constants;

def blas_dgemv( transa: string, m: int, n: int, alpha: real, a: [?aDom] real, lda: int, x: [?xDom] real, incx: int, beta: real, y: [?yDom] real, incy: int )
{
	if transa == BLIS_TRANSPOSE
	{
		forall (i,j) in aDom
		{
			y(incy,i) = beta * y(incy,i) + (alpha * a(j,i)) * x(incx,j);
		}
	}
	else
	{
		forall (i,j) in aDom
		{
			y(incy,i) = beta * y(incy,i) + (alpha * a(i,j)) * x(incx,j);
		}
	}
}

