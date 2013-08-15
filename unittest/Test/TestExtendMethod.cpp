#include "stdafx.h"

#include <sstream>
#include <vector>

#include "picotest.h"
#include "include/extend_method.h"
#include "include/container_make.h"


//////////////////////////////////////////////////
namespace {
//////////////////////////////////////////////////

// sample class
struct Widget
{
	Widget(int id, const std::string& name, double weight)
		: id_(id), name_(name), weight_(weight)
	{}
	int id_;
	std::string name_;
	double weight_;
};

// extend method with no parameter
EXTENDER_START(Widget, ToString, std::string)
	EXTEND_METHOD0(const) {
		std::stringstream ss;
		ss << this_.id_ << ":" << this_.name_;
		return ss.str();
	}
EXTENDER_END

// extend method with parameter
EXTENDER_START(Widget, Rename, void)
	EXTEND_METHOD1(std::string,name,) {
		this_.name_ = name;
	}
EXTENDER_END

// extend method for container
EXTENDER_START(std::vector<Widget>, WeightSum, double)
	EXTEND_METHOD0(const) {
		double ret = 0;
		for(auto itr = this_.begin(); itr != this_.end(); ++itr)
		{
			ret += itr->weight_;
		}
		return ret;
	}
EXTENDER_END

//-----------------------------------
// Test case
//-----------------------------------
TEST(ExntedMethodTest, NonParameterMethod)
{
	Widget wid(39, "sample", 20.0);
	EXPECT_EQ("39:sample", wid->*ToString());
}
TEST(ExntedMethodTest, OneParameterMethod)
{
	Widget wid(3, "sample", 20.0 );
	wid->*Rename("another_sample");
	EXPECT_EQ("another_sample", wid.name_);
}
TEST(ExntedMethodTest, VectorMethod)
{
	auto widArr = (GimLib::make_vector<Widget>()
		, Widget( 39, "sample", 20.0 )
		, Widget( 39, "sample", 20.0 )
		, Widget( 39, "sample", 20.0 ) )();
	EXPECT_DOUBLE_EQ(60.0, widArr->*WeightSum());
}
	

//////////////////////////////////////////////////
} // end no name namespace
//////////////////////////////////////////////////