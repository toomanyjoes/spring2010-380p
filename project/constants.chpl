module constants
{
use FLA_Obj;

param FLA_SUCCESS: int = -1;	// #define FLA_SUCCESS           (-1)
param FLA_FAILURE: int = -2;	// #define FLA_FAILURE           (-2)


//typedef int FLA_Bool;
type FLA_Error = int;	// typedef int FLA_Error;
//typedef int FLA_Quadrant;
type FLA_Datatype = int;	// typedef int FLA_Datatype;
type FLA_Elemtype = int;	// typedef int FLA_Elemtype;
type FLA_Side = int;		//typedef int FLA_Side;
/*typedef int FLA_Uplo;
typedef int FLA_Trans;
typedef int FLA_Conj;
typedef int FLA_Diag;
typedef int FLA_Dimension;
typedef int FLA_Pivot_type;
typedef int FLA_Direct;
typedef int FLA_Store;
typedef int FLA_Matrix_type;
typedef int FLA_Precision;
typedef int FLA_Domain;*/
type dim_t = uint;	// typedef unsigned int dim_t;

type size_t = int;




// FLAME internal error checking level
param FLA_FULL_ERROR_CHECKING: int = 2;	//#define FLA_FULL_ERROR_CHECKING 2
param FLA_MIN_ERROR_CHECKING: int = 1;	//#define FLA_MIN_ERROR_CHECKING  1
param FLA_NO_ERROR_CHECKING: int = 0;	//#define FLA_NO_ERROR_CHECKING   0


// FLA_Elemtype
param FLA_MATRIX: int = 150;	//#define FLA_MATRIX            150
param FLA_SCALAR: int = 151;	//#define FLA_SCALAR            151

// FLA_Side
param FLA_TOP: int = 200;	//#define FLA_TOP               200
param FLA_BOTTOM: int = 201;	//#define FLA_BOTTOM            201
param FLA_LEFT: int = 210;	//#define FLA_LEFT              210
param FLA_RIGHT: int = 211;	//#define FLA_RIGHT             211

// Useful when determining the relative index base of the error codes.
param FLA_ERROR_CODE_MIN: int = -10;	//#define FLA_ERROR_CODE_MIN                    (-10)

param FLA_INVALID_SIDE: int = -10;	//#define FLA_INVALID_SIDE                      (-10)
param FLA_INVALID_ELEMTYPE: int = -68;	//#define FLA_INVALID_ELEMTYPE                  (-68)
param FLA_INVALID_DATATYPE: int = -17;	//#define FLA_INVALID_DATATYPE                  (-17)
param FLA_NULL_POINTER: int = -32;	//#define FLA_NULL_POINTER                      (-32)
param FLA_INVALID_ROW_STRIDE: int = -94;	//#define FLA_INVALID_ROW_STRIDE                (-94)
param FLA_INVALID_COL_STRIDE: int = -95;	//#define FLA_INVALID_COL_STRIDE                (-95)
param FLA_INVALID_STRIDE_COMBINATION: int = -96;	//#define FLA_INVALID_STRIDE_COMBINATION        (-96)

// Necessary when computing whether an error code is defined.
param FLA_ERROR_CODE_MAX: int = -97;	//#define FLA_ERROR_CODE_MAX                    (-97)


// FLA_Datatype
param FLA_FLOAT: int = 100;	//#define FLA_FLOAT             100
param FLA_DOUBLE: int = 101;	//#define FLA_DOUBLE            101
param FLA_COMPLEX: int = 102;	//#define FLA_COMPLEX           102
param FLA_DOUBLE_COMPLEX: int = 103;	//#define FLA_DOUBLE_COMPLEX    103
param FLA_INT: int = 104;	//#define FLA_INT               104
param FLA_CONSTANT: int = 105;	//#define FLA_CONSTANT          105


// FLA_Conj
param FLA_NO_CONJUGATE: int = 450;	//#define FLA_NO_CONJUGATE      450
param FLA_CONJUGATE: int = 451;	//#define FLA_CONJUGATE         451



// constant FLA_Obj objects
const FLA_TWO: FLA_Obj = new FLA_Obj();
const FLA_ONE: FLA_Obj = new FLA_Obj();
const FLA_ONE_HALF: FLA_Obj = new FLA_Obj();
const FLA_ZERO: FLA_Obj = new FLA_Obj();
const FLA_MINUS_ONE_HALF: FLA_Obj = new FLA_Obj();
const FLA_MINUS_ONE: FLA_Obj = new FLA_Obj();
const FLA_MINUS_TWO: FLA_Obj = new FLA_Obj();

}
