

def blas_dgemv( m: int, n: int, alpha: real, a: [?aDom] real, lda: int, x: [?xDom] real, incx: int, beta: real, y: [?yDom] real, incy: int )
{
	forall i in yDom
	{
		y(i) = beta * y(i);
	}
	var Aprime: [aDom] real = a;
	forall ij in aDom
	{
		Aprime(ij) = alpha * a(ij);
	}

 	forall (i,j) in aDom
 	{
 		y(i) += Aprime(i,j) * x(j);
 	}
}



