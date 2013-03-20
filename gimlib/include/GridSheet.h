#ifndef INCLUDED_GIMLIB_GRID
#define INCLUDED_GIMLIB_GRID

//! get former charactor
#define GIM_GET_FORMER_CHAR_B A
#define GIM_GET_FORMER_CHAR_C B
#define GIM_GET_FORMER_CHAR_D C
#define GIM_GET_FORMER_CHAR_E D
#define GIM_GET_FORMER_CHAR_F E
#define GIM_GET_FORMER_CHAR_G F
#define GIM_GET_FORMER_CHAR_H G

#define GIM_GET_FORMER_CHAR_b a
#define GIM_GET_FORMER_CHAR_c b
#define GIM_GET_FORMER_CHAR_d c
#define GIM_GET_FORMER_CHAR_e d
#define GIM_GET_FORMER_CHAR_f e
#define GIM_GET_FORMER_CHAR_g f
#define GIM_GET_FORMER_CHAR_h g

#define GIM_GET_FORMER_CHAR(C) GIM_GET_FORMER_CHAR_##C

//! create method to return grid point
#define GIM_DEF_GRID_POINT(X,Y) Point X##Y() const { return Point(X, Y); }

//! create method to set interval of grid point
#define GIM_DEF_INTERVAL(C)															\
	int GIM_GET_FORMER_CHAR(C)##C() const { return GIM_GET_FORMER_CHAR(C) + C; }	\
	void GIM_GET_FORMER_CHAR(C)##C(int dist) { C = GIM_GET_FORMER_CHAR(C) + dist; }

//! create Grid that on X-axis
#define GIM_DEF_NEXT_AXIS_X_GRID(NX, NY, CX, CY)						\
	template <> struct Grid<NX, NY> : public virtual Grid<NX - 1, NY>	\
	{																	\
		int CX;															\
		GIM_DEF_GRID_POINT(CX, CY)										\
		GIM_DEF_INTERVAL(CX)											\
	};
		
//! create Grid that on Y-axis
#define GIM_DEF_NEXT_AXIS_Y_GRID(NX, NY, CX, CY)						\
	template <> struct Grid<NX, NY> : public virtual Grid<NX, NY - 1>	\
	{																	\
		int CY;															\
		GIM_DEF_GRID_POINT(CX, CY)										\
		GIM_DEF_INTERVAL(CY)											\
	};

//! create Grid
#define GIM_DEF_NEXT_GRID(NX, NY, CX, CY)														\
	template <> struct Grid<NX, NY> : public virtual Grid<NX - 1, NY>, virtual Grid<NX, NY - 1>	\
	{																							\
		GIM_DEF_GRID_POINT(CX, CY)																\
		GIM_DEF_INTERVAL(CX)																	\
		GIM_DEF_INTERVAL(CY)																	\
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
	GIM_DEF_GRID_POINT(A,a)
};

//-------------------------------
//! instantiate concrete Grid
//-------------------------------
GIM_DEF_NEXT_AXIS_X_GRID( 2, 1, B, a)
GIM_DEF_NEXT_AXIS_X_GRID( 3, 1, C, a)
GIM_DEF_NEXT_AXIS_X_GRID( 4, 1, D, a)
GIM_DEF_NEXT_AXIS_X_GRID( 5, 1, E, a)
GIM_DEF_NEXT_AXIS_X_GRID( 6, 1, F, a)
GIM_DEF_NEXT_AXIS_X_GRID( 7, 1, G, a)
GIM_DEF_NEXT_AXIS_X_GRID( 8, 1, H, a)

GIM_DEF_NEXT_AXIS_Y_GRID( 1, 2, A, b)
GIM_DEF_NEXT_AXIS_Y_GRID( 1, 3, A, c)
GIM_DEF_NEXT_AXIS_Y_GRID( 1, 4, A, d)
GIM_DEF_NEXT_AXIS_Y_GRID( 1, 5, A, e)
GIM_DEF_NEXT_AXIS_Y_GRID( 1, 6, A, f)
GIM_DEF_NEXT_AXIS_Y_GRID( 1, 7, A, g)
GIM_DEF_NEXT_AXIS_Y_GRID( 1, 8, A, h)

GIM_DEF_NEXT_GRID( 2, 2, B, b)
GIM_DEF_NEXT_GRID( 2, 3, B, c)
GIM_DEF_NEXT_GRID( 2, 4, B, d)
GIM_DEF_NEXT_GRID( 2, 5, B, e)
GIM_DEF_NEXT_GRID( 2, 6, B, f)
GIM_DEF_NEXT_GRID( 2, 7, B, g)
GIM_DEF_NEXT_GRID( 2, 8, B, h)

GIM_DEF_NEXT_GRID( 3, 2, C, b)
GIM_DEF_NEXT_GRID( 3, 3, C, c)
GIM_DEF_NEXT_GRID( 3, 4, C, d)
GIM_DEF_NEXT_GRID( 3, 5, C, e)
GIM_DEF_NEXT_GRID( 3, 6, C, f)
GIM_DEF_NEXT_GRID( 3, 7, C, g)
GIM_DEF_NEXT_GRID( 3, 8, C, h)

GIM_DEF_NEXT_GRID( 4, 2, D, b)
GIM_DEF_NEXT_GRID( 4, 3, D, c)
GIM_DEF_NEXT_GRID( 4, 4, D, d)
GIM_DEF_NEXT_GRID( 4, 5, D, e)
GIM_DEF_NEXT_GRID( 4, 6, D, f)
GIM_DEF_NEXT_GRID( 4, 7, D, g)
GIM_DEF_NEXT_GRID( 4, 8, D, h)

GIM_DEF_NEXT_GRID( 5, 2, E, b)
GIM_DEF_NEXT_GRID( 5, 3, E, c)
GIM_DEF_NEXT_GRID( 5, 4, E, d)
GIM_DEF_NEXT_GRID( 5, 5, E, e)
GIM_DEF_NEXT_GRID( 5, 6, E, f)
GIM_DEF_NEXT_GRID( 5, 7, E, g)
GIM_DEF_NEXT_GRID( 5, 8, E, h)

GIM_DEF_NEXT_GRID( 6, 2, F, b)
GIM_DEF_NEXT_GRID( 6, 3, F, c)
GIM_DEF_NEXT_GRID( 6, 4, F, d)
GIM_DEF_NEXT_GRID( 6, 5, F, e)
GIM_DEF_NEXT_GRID( 6, 6, F, f)
GIM_DEF_NEXT_GRID( 6, 7, F, g)
GIM_DEF_NEXT_GRID( 6, 8, F, h)

GIM_DEF_NEXT_GRID( 7, 2, G, b)
GIM_DEF_NEXT_GRID( 7, 3, G, c)
GIM_DEF_NEXT_GRID( 7, 4, G, d)
GIM_DEF_NEXT_GRID( 7, 5, G, e)
GIM_DEF_NEXT_GRID( 7, 6, G, f)
GIM_DEF_NEXT_GRID( 7, 7, G, g)
GIM_DEF_NEXT_GRID( 7, 8, G, h)

GIM_DEF_NEXT_GRID( 8, 2, H, b)
GIM_DEF_NEXT_GRID( 8, 3, H, c)
GIM_DEF_NEXT_GRID( 8, 4, H, d)
GIM_DEF_NEXT_GRID( 8, 5, H, e)
GIM_DEF_NEXT_GRID( 8, 6, H, f)
GIM_DEF_NEXT_GRID( 8, 7, H, g)
GIM_DEF_NEXT_GRID( 8, 8, H, h)
} // end namespace GimLib

#endif

