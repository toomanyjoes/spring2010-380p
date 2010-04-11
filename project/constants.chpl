module constants
{
use FLA_Obj;

param TRUE: int = 1;	//#define TRUE  1
param FALSE: int = 0;	//#define FALSE 0

param FLA_SUCCESS: int = -1;	// #define FLA_SUCCESS           (-1)
param FLA_FAILURE: int = -2;	// #define FLA_FAILURE           (-2)


type FLA_Bool = int;	//typedef int FLA_Bool;
type FLA_Error = int;	// typedef int FLA_Error;
type FLA_Quadrant = int; // typedef int FLA_Quadrant;
type FLA_Datatype = int;	// typedef int FLA_Datatype;
type FLA_Elemtype = int;	// typedef int FLA_Elemtype;
type FLA_Side = int;		//typedef int FLA_Side;
/*typedef int FLA_Uplo;*/
type FLA_Trans = int;	//typedef int FLA_Trans;
type FLA_Conj = int;	//typedef int FLA_Conj;
/*typedef int FLA_Diag;
typedef int FLA_Dimension;
typedef int FLA_Pivot_type;
typedef int FLA_Direct;
typedef int FLA_Store;
typedef int FLA_Matrix_type;
typedef int FLA_Precision;
typedef int FLA_Domain;*/
type dim_t = uint;	// typedef unsigned int dim_t;

type size_t = int;


/* Encodes the default level of internal error checking chosen at
   configure-time. */
param FLA_INTERNAL_ERROR_CHECKING_LEVEL: int = 2;	//#define FLA_INTERNAL_ERROR_CHECKING_LEVEL 2



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

//FLA_Quadrant
param FLA_TL: int = 11; //#define FLA_TL                 11
param FLA_TR: int = 12; //#define FLA_TR                 12
param FLA_BL: int = 21; //#define FLA_BL                 21
param FLA_BR: int = 22; //#define FLA_BR                 22

// FLA_Datatype
param FLA_FLOAT: int = 100;	//#define FLA_FLOAT             100
param FLA_DOUBLE: int = 101;	//#define FLA_DOUBLE            101
param FLA_COMPLEX: int = 102;	//#define FLA_COMPLEX           102
param FLA_DOUBLE_COMPLEX: int = 103;	//#define FLA_DOUBLE_COMPLEX    103
param FLA_INT: int = 104;	//#define FLA_INT               104
param FLA_CONSTANT: int = 105;	//#define FLA_CONSTANT          105


// FLA_Trans
param FLA_NO_TRANSPOSE: int = 400;	//#define FLA_NO_TRANSPOSE      400
param FLA_TRANSPOSE: int = 401;		//#define FLA_TRANSPOSE         401
param FLA_CONJ_TRANSPOSE: int = 402;	//#define FLA_CONJ_TRANSPOSE    402
param FLA_CONJ_NO_TRANSPOSE: int = 403;	//#define FLA_CONJ_NO_TRANSPOSE 403
param FLA_TRANS_MASK: int = 0x3;	//#define FLA_TRANS_MASK        0x3


// FLA_Conj
param FLA_NO_CONJUGATE: int = 450;	//#define FLA_NO_CONJUGATE      450
param FLA_CONJUGATE: int = 451;	//#define FLA_CONJUGATE         451


// --- Error-related macro definitions -----------------------------------------

// Useful when determining the relative index base of the error codes.
//#define FLA_ERROR_CODE_MIN                    (-10)

// FLA_Error values.
/*#define FLA_INVALID_SIDE                      (-10)
#define FLA_INVALID_UPLO                      (-11)
#define FLA_INVALID_TRANS                     (-12)
#define FLA_INVALID_TRANS_GIVEN_DATATYPE      (-13)*/
param FLA_INVALID_CONJ: int = -14;	//#define FLA_INVALID_CONJ                      (-14)
/*#define FLA_INVALID_DIRECT                    (-15)
#define FLA_INVALID_STOREV                    (-16)
#define FLA_INVALID_DATATYPE                  (-17)
#define FLA_INVALID_INTEGER_DATATYPE          (-18)
#define FLA_INVALID_REAL_DATATYPE             (-19)
#define FLA_INVALID_COMPLEX_DATATYPE          (-20)
#define FLA_OBJECT_NOT_INTEGER                (-21)
#define FLA_OBJECT_NOT_REAL                   (-22)
#define FLA_OBJECT_NOT_COMPLEX                (-23)
#define FLA_OBJECT_NOT_SQUARE                 (-24)*/
param FLA_OBJECT_NOT_SCALAR: int = -25;	//#define FLA_OBJECT_NOT_SCALAR                 (-25)
param FLA_OBJECT_NOT_VECTOR: int = -26;	//#define FLA_OBJECT_NOT_VECTOR                 (-26)*/
param FLA_INCONSISTENT_DATATYPES: int = -27;	//#define FLA_INCONSISTENT_DATATYPES            (-27)
param FLA_NONCONFORMAL_DIMENSIONS: int = -28;	//#define FLA_NONCONFORMAL_DIMENSIONS           (-28)
/*#define FLA_UNEQUAL_VECTOR_LENGTHS            (-29)
#define FLA_INVALID_HESSENBERG_INDICES        (-30)
#define FLA_INVALID_VECTOR_LENGTH             (-31)
#define FLA_NULL_POINTER                      (-32)
#define FLA_SPECIFIED_OBJ_DIM_MISMATCH        (-33)
#define FLA_INVALID_PIVOT_TYPE                (-35)
#define FLA_MALLOC_RETURNED_NULL_POINTER      (-37)
#define FLA_OBJECT_BASE_BUFFER_MISMATCH       (-38)
#define FLA_OBJECTS_NOT_VERTICALLY_ADJ        (-39)
#define FLA_OBJECTS_NOT_HORIZONTALLY_ADJ      (-40)
#define FLA_ADJACENT_OBJECT_DIM_MISMATCH      (-41)
#define FLA_OBJECTS_NOT_VERTICALLY_ALIGNED    (-42)
#define FLA_OBJECTS_NOT_HORIZONTALLY_ALIGNED  (-43)*/
param FLA_INVALID_FLOATING_DATATYPE: int = -44;	//#define FLA_INVALID_FLOATING_DATATYPE         (-44)
param FLA_OBJECT_NOT_FLOATING_POINT: int = -45;	//#define FLA_OBJECT_NOT_FLOATING_POINT         (-45)
/*#define FLA_INVALID_BLOCKSIZE_VALUE           (-46)
#define FLA_OPEN_RETURNED_ERROR               (-47)
#define FLA_LSEEK_RETURNED_ERROR              (-48)
#define FLA_CLOSE_RETURNED_ERROR              (-49)
#define FLA_UNLINK_RETURNED_ERROR             (-50)
#define FLA_READ_RETURNED_ERROR               (-51)
#define FLA_WRITE_RETURNED_ERROR              (-52)
#define FLA_INVALID_QUADRANT                  (-53)
#define FLA_NOT_YET_IMPLEMENTED               (-54)
#define FLA_EXPECTED_NONNEGATIVE_VALUE        (-55)
#define FLA_SUPERMATRIX_NOT_ENABLED           (-56)
#define FLA_UNDEFINED_ERROR_CODE              (-57)
#define FLA_INVALID_DIAG                      (-58)*/
param FLA_INCONSISTENT_OBJECT_PRECISION: int = -59;	// #define FLA_INCONSISTENT_OBJECT_PRECISION     (-59)
/*#define FLA_INVALID_BLOCKSIZE_OBJ             (-60)
#define FLA_VECTOR_LENGTH_BELOW_MIN           (-61)
#define FLA_PTHREAD_CREATE_RETURNED_ERROR     (-63)
#define FLA_PTHREAD_JOIN_RETURNED_ERROR       (-64)
#define FLA_INVALID_ISGN_VALUE                (-65)
#define FLA_CHOL_FAILED_MATRIX_NOT_SPD        (-67)
#define FLA_INVALID_ELEMTYPE                  (-68)
#define FLA_POSIX_MEMALIGN_FAILED             (-69)
#define FLA_INVALID_SUBMATRIX_DIMS            (-70)
#define FLA_INVALID_SUBMATRIX_OFFSET          (-71)
#define FLA_OBJECT_NOT_SCALAR_ELEMTYPE        (-72)
#define FLA_OBJECT_NOT_MATRIX_ELEMTYPE        (-73)
#define FLA_ENCOUNTERED_NON_POSITIVE_NTHREADS (-74)
#define FLA_INVALID_CONJ_GIVEN_DATATYPE       (-75)
#define FLA_INVALID_COMPLEX_TRANS             (-76)
#define FLA_INVALID_REAL_TRANS                (-77)
#define FLA_INVALID_BLAS_TRANS                (-78)*/
param FLA_INVALID_NONCONSTANT_DATATYPE: int = -79;	//#define FLA_INVALID_NONCONSTANT_DATATYPE      (-79)
param FLA_OBJECT_NOT_NONCONSTANT: int = -80;	//#define FLA_OBJECT_NOT_NONCONSTANT            (-80)
/*#define FLA_OBJECT_DATATYPES_NOT_EQUAL        (-82)
#define FLA_DIVIDE_BY_ZERO                    (-83)
#define FLA_OBJECT_ELEMTYPES_NOT_EQUAL        (-84)
#define FLA_INVALID_PIVOT_INDEX_RANGE         (-85)
#define FLA_HOUSEH_PANEL_MATRIX_TOO_SMALL     (-86)
#define FLA_INVALID_OBJECT_LENGTH             (-87)
#define FLA_INVALID_OBJECT_WIDTH              (-88)
#define FLA_INVALID_ERROR_CHECKING_LEVEL      (-89)
#define FLA_ATTEMPTED_OVER_REPART_2X2         (-90)*/
param FLA_ATTEMPTED_OVER_REPART_2X1: int = -91;	//#define FLA_ATTEMPTED_OVER_REPART_2X1         (-91)
/*#define FLA_ATTEMPTED_OVER_REPART_1X2         (-92)
#define FLA_EXTERNAL_LAPACK_NOT_IMPLEMENTED   (-93)
#define FLA_INVALID_ROW_STRIDE                (-94)
#define FLA_INVALID_COL_STRIDE                (-95)
#define FLA_INVALID_STRIDE_COMBINATION        (-96)
#define FLA_INVALID_VECTOR_DIM                (-97)
*/

// --- conj ---

const BLIS_NO_CONJUGATE: string = "n";	//#define BLIS_NO_CONJUGATE      'n'
const BLIS_CONJUGATE: string = "c";	//#define BLIS_CONJUGATE         'c'


// constant FLA_Obj objects
const FLA_TWO: FLA_Obj = new FLA_Obj();
const FLA_ONE: FLA_Obj = new FLA_Obj();
const FLA_ONE_HALF: FLA_Obj = new FLA_Obj();
const FLA_ZERO: FLA_Obj = new FLA_Obj();
const FLA_MINUS_ONE_HALF: FLA_Obj = new FLA_Obj();
const FLA_MINUS_ONE: FLA_Obj = new FLA_Obj();
const FLA_MINUS_TWO: FLA_Obj = new FLA_Obj();

}
