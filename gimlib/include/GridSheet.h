#ifndef INCLUDED_GIMLIB_GRIDSHEET
#define INCLUDED_GIMLIB_GRIDSHEET

//! get character from number
#define GIM_GET_X_CHAR_1 A
#define GIM_GET_X_CHAR_2 B
#define GIM_GET_X_CHAR_3 C
#define GIM_GET_X_CHAR_4 D
#define GIM_GET_X_CHAR_5 E
#define GIM_GET_X_CHAR_6 F
#define GIM_GET_X_CHAR_7 G
#define GIM_GET_X_CHAR_8 H
#define GIM_GET_X_CHAR(n) GIM_GET_X_CHAR_##n

#define GIM_GET_X_FORMER_CHAR_2 A
#define GIM_GET_X_FORMER_CHAR_3 B
#define GIM_GET_X_FORMER_CHAR_4 C
#define GIM_GET_X_FORMER_CHAR_5 D
#define GIM_GET_X_FORMER_CHAR_6 E
#define GIM_GET_X_FORMER_CHAR_7 F
#define GIM_GET_X_FORMER_CHAR_8 G
#define GIM_GET_X_FORMER_CHAR(n) GIM_GET_X_FORMER_CHAR_##n

#define GIM_GET_Y_CHAR_1 a
#define GIM_GET_Y_CHAR_2 b
#define GIM_GET_Y_CHAR_3 c
#define GIM_GET_Y_CHAR_4 d
#define GIM_GET_Y_CHAR_5 e
#define GIM_GET_Y_CHAR_6 f
#define GIM_GET_Y_CHAR_7 g
#define GIM_GET_Y_CHAR_8 h
#define GIM_GET_Y_CHAR(n) GIM_GET_Y_CHAR_##n

#define GIM_GET_Y_FORMER_CHAR_2 a
#define GIM_GET_Y_FORMER_CHAR_3 b
#define GIM_GET_Y_FORMER_CHAR_4 c
#define GIM_GET_Y_FORMER_CHAR_5 d
#define GIM_GET_Y_FORMER_CHAR_6 e
#define GIM_GET_Y_FORMER_CHAR_7 f
#define GIM_GET_Y_FORMER_CHAR_8 g
#define GIM_GET_Y_FORMER_CHAR(n) GIM_GET_Y_FORMER_CHAR_##n

//! create method to return grid point
#define GIM_DEF_GRID_POINT(NX,NY) Point GIM_GET_X_CHAR(NX)##GIM_GET_Y_CHAR(NY)() const	\
 { return Point(GIM_GET_X_CHAR(NX), GIM_GET_Y_CHAR(NY)); }

//! create method to set interval of grid point
#define GIM_DEF_INTERVAL_X(n)														\
	int GIM_GET_X_FORMER_CHAR(n)##GIM_GET_X_CHAR(n)() const							\
	{ return GIM_GET_X_FORMER_CHAR(n) + GIM_GET_X_CHAR(n); }						\
	void GIM_GET_X_FORMER_CHAR(n)##GIM_GET_X_CHAR(n)(int dist)						\
	{ GIM_GET_X_CHAR(n) = GIM_GET_X_FORMER_CHAR(n) + dist; }
#define GIM_DEF_INTERVAL_Y(n)														\
	int GIM_GET_Y_FORMER_CHAR(n)##GIM_GET_Y_CHAR(n)() const							\
	{ return GIM_GET_Y_FORMER_CHAR(n) + GIM_GET_Y_CHAR(n); }						\
	void GIM_GET_Y_FORMER_CHAR(n)##GIM_GET_Y_CHAR(n)(int dist)						\
	{ GIM_GET_Y_CHAR(n) = GIM_GET_Y_FORMER_CHAR(n) + dist; }

//! create Grid that on X-axis
#define GIM_DEF_NEXT_AXIS_X_GRID(X, Y)								\
	template <> struct Grid<X, Y> : public virtual Grid<X - 1, Y>	\
	{																\
		int GIM_GET_X_CHAR(X);										\
		GIM_DEF_GRID_POINT(X, Y)									\
		GIM_DEF_INTERVAL_X(X)										\
	};
		
//! create Grid that on Y-axis
#define GIM_DEF_NEXT_AXIS_Y_GRID(X, Y)								\
	template <> struct Grid<X, Y> : public virtual Grid<X, Y - 1>	\
	{																\
		int GIM_GET_Y_CHAR(Y);										\
		GIM_DEF_GRID_POINT(X, Y)									\
		GIM_DEF_INTERVAL_Y(Y)										\
	};

//! create Grid
#define GIM_DEF_NEXT_GRID(X, Y)																\
	template <> struct Grid<X, Y> : public virtual Grid<X - 1, Y>, virtual Grid<X, Y - 1>	\
	{																						\
		GIM_DEF_GRID_POINT(X, Y)															\
		GIM_DEF_INTERVAL_X(X)																\
		GIM_DEF_INTERVAL_Y(Y)																\
	};


namespace GimLib {

//-------------------------------
//! grid cross point
//-------------------------------
struct Point
{
	Point() : x(0), y(0) {}
	Point(int ax, int ay) : x(ax), y(ay) {}
	int x;
	int y;
	friend bool operator==(const Point& lhs, const Point& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}
};

//-------------------------------
//! Grid class template
//-------------------------------
template <int X, int Y> struct Grid;

//! Grid on origin
template <> struct Grid<1,1>
{
	Grid() : A(0), a(0) {}
	const int A;
	const int a;
	GIM_DEF_GRID_POINT(1,1)
};

//-------------------------------
//! instantiate concrete Grid
//-------------------------------
GIM_DEF_NEXT_AXIS_X_GRID( 2, 1)
GIM_DEF_NEXT_AXIS_X_GRID( 3, 1)
GIM_DEF_NEXT_AXIS_X_GRID( 4, 1)
GIM_DEF_NEXT_AXIS_X_GRID( 5, 1)
GIM_DEF_NEXT_AXIS_X_GRID( 6, 1)
GIM_DEF_NEXT_AXIS_X_GRID( 7, 1)
GIM_DEF_NEXT_AXIS_X_GRID( 8, 1)

GIM_DEF_NEXT_AXIS_Y_GRID( 1, 2)
GIM_DEF_NEXT_AXIS_Y_GRID( 1, 3)
GIM_DEF_NEXT_AXIS_Y_GRID( 1, 4)
GIM_DEF_NEXT_AXIS_Y_GRID( 1, 5)
GIM_DEF_NEXT_AXIS_Y_GRID( 1, 6)
GIM_DEF_NEXT_AXIS_Y_GRID( 1, 7)
GIM_DEF_NEXT_AXIS_Y_GRID( 1, 8)

GIM_DEF_NEXT_GRID( 2, 2)
GIM_DEF_NEXT_GRID( 2, 3)
GIM_DEF_NEXT_GRID( 2, 4)
GIM_DEF_NEXT_GRID( 2, 5)
GIM_DEF_NEXT_GRID( 2, 6)
GIM_DEF_NEXT_GRID( 2, 7)
GIM_DEF_NEXT_GRID( 2, 8)

GIM_DEF_NEXT_GRID( 3, 2)
GIM_DEF_NEXT_GRID( 3, 3)
GIM_DEF_NEXT_GRID( 3, 4)
GIM_DEF_NEXT_GRID( 3, 5)
GIM_DEF_NEXT_GRID( 3, 6)
GIM_DEF_NEXT_GRID( 3, 7)
GIM_DEF_NEXT_GRID( 3, 8)

GIM_DEF_NEXT_GRID( 4, 2)
GIM_DEF_NEXT_GRID( 4, 3)
GIM_DEF_NEXT_GRID( 4, 4)
GIM_DEF_NEXT_GRID( 4, 5)
GIM_DEF_NEXT_GRID( 4, 6)
GIM_DEF_NEXT_GRID( 4, 7)
GIM_DEF_NEXT_GRID( 4, 8)

GIM_DEF_NEXT_GRID( 5, 2)
GIM_DEF_NEXT_GRID( 5, 3)
GIM_DEF_NEXT_GRID( 5, 4)
GIM_DEF_NEXT_GRID( 5, 5)
GIM_DEF_NEXT_GRID( 5, 6)
GIM_DEF_NEXT_GRID( 5, 7)
GIM_DEF_NEXT_GRID( 5, 8)

GIM_DEF_NEXT_GRID( 6, 2)
GIM_DEF_NEXT_GRID( 6, 3)
GIM_DEF_NEXT_GRID( 6, 4)
GIM_DEF_NEXT_GRID( 6, 5)
GIM_DEF_NEXT_GRID( 6, 6)
GIM_DEF_NEXT_GRID( 6, 7)
GIM_DEF_NEXT_GRID( 6, 8)

GIM_DEF_NEXT_GRID( 7, 2)
GIM_DEF_NEXT_GRID( 7, 3)
GIM_DEF_NEXT_GRID( 7, 4)
GIM_DEF_NEXT_GRID( 7, 5)
GIM_DEF_NEXT_GRID( 7, 6)
GIM_DEF_NEXT_GRID( 7, 7)
GIM_DEF_NEXT_GRID( 7, 8)

GIM_DEF_NEXT_GRID( 8, 2)
GIM_DEF_NEXT_GRID( 8, 3)
GIM_DEF_NEXT_GRID( 8, 4)
GIM_DEF_NEXT_GRID( 8, 5)
GIM_DEF_NEXT_GRID( 8, 6)
GIM_DEF_NEXT_GRID( 8, 7)
GIM_DEF_NEXT_GRID( 8, 8)
} // end namespace GimLib

#endif

