#include "stdafx.h"
#include "GimLibTestHelper.h"
#include "../gimlib/include/GridSheet.h"

namespace GimLib
{
	void PrintTo(const Point& pt, ::std::ostream* os)
	{
		*os << "(" << pt.x << ", " << pt.y << ")";
	}
}