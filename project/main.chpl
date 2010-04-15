// defines our main function

use constants, FLA_Gemm_nn_unb_var1;
def main()
{
	var A: FLA_Obj = new FLA_Obj();
	var B: FLA_Obj = new FLA_Obj();
	var C: FLA_Obj = new FLA_Obj();
	FLA_Obj_create(FLA_DOUBLE, 5, 5, 0, 0, A);
	FLA_Obj_create(FLA_DOUBLE, 5, 5, 0, 0, B);
	FLA_Obj_create(FLA_DOUBLE, 5, 5, 0, 0, C);

	// init A
	forall ij in A.base.buffer.domain
	{
		A.base.buffer(ij) = 2.0;
	}
	forall ij in B.base.buffer.domain
	{
		B.base.buffer(ij) = 3.0;
	}
	writeln("A:\n",A.base.buffer);
	writeln("B:\n",B.base.buffer);

	//                    alpha,  A, B,  beta,   C
	FLA_Gemm_nn_unb_var1(FLA_ONE, A, B, FLA_ONE, C);
	writeln("A:\n",A.base.buffer);
	writeln("B:\n",B.base.buffer);
	writeln("C:\n", C.base.buffer);
}
