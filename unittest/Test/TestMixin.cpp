#include "stdafx.h"
#include "picotest.h"
#include "include/mixin.h"
#include "include/TemplateList.h"


using GimLib::MixinBase;
using GimLib::MixinFeatureBase;
using namespace GimLib::TL;

//////////////////////////////////////////////////
namespace {
//////////////////////////////////////////////////

//-----------------------------------
// test target
//-----------------------------------
MIXIN_CLASS_DEFINE(Sum)
{
	Sum() : sum_num(8) {}
	int sum()
	{
		return This()->num1 + This()->num2;
	}
	int sum_num;
};
MIXIN_CLASS_DEFINE(Sub)
{
	Sub() : sub_num(4) {}
	int sub()
	{
		return This()->num1 - This()->num2;
	}
	int sub_num;
};

struct MixedinClass : public MixinBase<TEMPLATELIST_2(MixedinClass, Sum, Sub)>
{
	MixedinClass(int n1, int n2)
		: num1(n1), num2(n2)
	{}
	int num1;
	int num2;
};


//-----------------------------------
// test case
//-----------------------------------
//
TEST(MixinClassTest, hoge)
{
	MixedinClass obj(5, 1);
	EXPECT_EQ(6, obj.sum());
	EXPECT_EQ(4, obj.sub());
}


//////////////////////////////////////////////////
} // end no name namespace
//////////////////////////////////////////////////
