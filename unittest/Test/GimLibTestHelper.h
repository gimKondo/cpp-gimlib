#ifndef INCLUDED_GIMLIB_TESTHELPER
#define INCLUDED_GIMLIB_TESTHELPER

#include <iostream>

namespace GimLib
{
	struct Point;
	void PrintTo(const Point& pt, ::std::ostream* os);
}


#endif