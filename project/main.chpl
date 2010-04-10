// defines our main function

use constants, FLA_Gemm_nn_unb_var1;
def main()
{
	var t: FLA_Obj = new FLA_Obj();
	FLA_Gemm_nn_unb_var1( FLA_TWO, FLA_TWO, FLA_TWO, FLA_TWO, FLA_TWO );
}
