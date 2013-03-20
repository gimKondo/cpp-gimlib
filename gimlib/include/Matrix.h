#ifndef INCLUDED_GIMLIB_MATRIX
#define INCLUDED_GIMLIB_MATRIX

#define GIM_GET_ROW_CHAR_1 A
#define GIM_GET_ROW_CHAR_2 B
#define GIM_GET_ROW_CHAR_3 C
#define GIM_GET_ROW_CHAR_4 D
#define GIM_GET_ROW_CHAR_5 E
#define GIM_GET_ROW_CHAR_6 F
#define GIM_GET_ROW_CHAR_7 G
#define GIM_GET_ROW_CHAR_8 H

#define GIM_GET_COL_CHAR_1 a
#define GIM_GET_COL_CHAR_2 b
#define GIM_GET_COL_CHAR_3 c
#define GIM_GET_COL_CHAR_4 d
#define GIM_GET_COL_CHAR_5 e
#define GIM_GET_COL_CHAR_6 f
#define GIM_GET_COL_CHAR_7 g
#define GIM_GET_COL_CHAR_8 h

#define GIM_GET_ROW_CHAR(n) GIM_GET_ROW_CHAR_##n
#define GIM_GET_COL_CHAR(n) GIM_GET_COL_CHAR_##n


//! Matrix generator
#define GIM_DEF_MATRIX_EDGE_ROW(C)														\
	template <typename T> struct Matrix<T, 1, C> : public virtual Matrix<T, 1, C - 1>	\
	{																					\
		T GIM_GET_ROW_CHAR(1)##GIM_GET_COL_CHAR(C);										\
	};
#define GIM_DEF_MATRIX_EDGE_COL(R)														\
	template <typename T> struct Matrix<T, R, 1> : public virtual Matrix<T, R - 1, 1>	\
	{																					\
		T GIM_GET_ROW_CHAR(R)##GIM_GET_COL_CHAR(1);										\
	};
#define GIM_DEF_MATRIX(R, C)																						\
	template <typename T> struct Matrix<T, R, C> : public virtual Matrix<T, R - 1, C>, virtual Matrix<T, R, C - 1>	\
	{																												\
		T GIM_GET_ROW_CHAR(R)##GIM_GET_COL_CHAR(C);																	\
	};
namespace GimLib {

//-------------------------------
//! Matrix class template
//-------------------------------
template <typename T, int R, int C> struct Matrix;

template <typename T> struct Matrix<T, 1, 1>
{
	T Aa;
};

//-------------------------------
//! instantiate Matrix
//-------------------------------
GIM_DEF_MATRIX_EDGE_ROW( 2)
GIM_DEF_MATRIX_EDGE_ROW( 3)
GIM_DEF_MATRIX_EDGE_ROW( 4)
GIM_DEF_MATRIX_EDGE_ROW( 5)
GIM_DEF_MATRIX_EDGE_ROW( 6)
GIM_DEF_MATRIX_EDGE_ROW( 7)
GIM_DEF_MATRIX_EDGE_ROW( 8)

GIM_DEF_MATRIX_EDGE_COL( 2)
GIM_DEF_MATRIX_EDGE_COL( 3)
GIM_DEF_MATRIX_EDGE_COL( 4)
GIM_DEF_MATRIX_EDGE_COL( 5)
GIM_DEF_MATRIX_EDGE_COL( 6)
GIM_DEF_MATRIX_EDGE_COL( 7)
GIM_DEF_MATRIX_EDGE_COL( 8)

GIM_DEF_MATRIX( 2, 2)
GIM_DEF_MATRIX( 3, 2)
GIM_DEF_MATRIX( 4, 2)
GIM_DEF_MATRIX( 5, 2)
GIM_DEF_MATRIX( 6, 2)
GIM_DEF_MATRIX( 7, 2)
GIM_DEF_MATRIX( 8, 2)

GIM_DEF_MATRIX( 2, 3)
GIM_DEF_MATRIX( 3, 3)
GIM_DEF_MATRIX( 4, 3)
GIM_DEF_MATRIX( 5, 3)
GIM_DEF_MATRIX( 6, 3)
GIM_DEF_MATRIX( 7, 3)
GIM_DEF_MATRIX( 8, 3)

GIM_DEF_MATRIX( 2, 4)
GIM_DEF_MATRIX( 3, 4)
GIM_DEF_MATRIX( 4, 4)
GIM_DEF_MATRIX( 5, 4)
GIM_DEF_MATRIX( 6, 4)
GIM_DEF_MATRIX( 7, 4)
GIM_DEF_MATRIX( 8, 4)

GIM_DEF_MATRIX( 2, 5)
GIM_DEF_MATRIX( 3, 5)
GIM_DEF_MATRIX( 4, 5)
GIM_DEF_MATRIX( 5, 5)
GIM_DEF_MATRIX( 6, 5)
GIM_DEF_MATRIX( 7, 5)
GIM_DEF_MATRIX( 8, 5)

GIM_DEF_MATRIX( 2, 6)
GIM_DEF_MATRIX( 3, 6)
GIM_DEF_MATRIX( 4, 6)
GIM_DEF_MATRIX( 5, 6)
GIM_DEF_MATRIX( 6, 6)
GIM_DEF_MATRIX( 7, 6)
GIM_DEF_MATRIX( 8, 6)

GIM_DEF_MATRIX( 2, 7)
GIM_DEF_MATRIX( 3, 7)
GIM_DEF_MATRIX( 4, 7)
GIM_DEF_MATRIX( 5, 7)
GIM_DEF_MATRIX( 6, 7)
GIM_DEF_MATRIX( 7, 7)
GIM_DEF_MATRIX( 8, 7)

GIM_DEF_MATRIX( 2, 8)
GIM_DEF_MATRIX( 3, 8)
GIM_DEF_MATRIX( 4, 8)
GIM_DEF_MATRIX( 5, 8)
GIM_DEF_MATRIX( 6, 8)
GIM_DEF_MATRIX( 7, 8)
GIM_DEF_MATRIX( 8, 8)
}

#endif
