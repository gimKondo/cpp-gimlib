#include "stdafx.h"
#include "picotest.h"
#include "include/mixin.h"
#include "include/TemplateList.h"


using GimLib::MixinBase;
using namespace GimLib::TL;

#define TYPELIST_P1_1(P, T1) TYPELIST_1(T1<P>)
#define TYPELIST_P1_2(P, T1, T2) TYPELIST_2(T1<P>, T2<P>)

//////////////////////////////////////////////////
namespace {
//////////////////////////////////////////////////

//-----------------------------------
// テスト用クラス
//-----------------------------------
template <typename THIS_TYPE> struct Sum
{
	Sum() : sum_num(8) {}
	int sum()
	{
		THIS_TYPE* this_ = static_cast<THIS_TYPE*>(this);
		return this_->num1 + this_->num2;
	}
	int sum_num;
};
template <typename THIS_TYPE> struct Sub
{
	Sub() : sub_num(4) {}
	int sub()
	{
		THIS_TYPE* this_ = static_cast<THIS_TYPE*>(this);
		return this_->num1 - this_->num2;
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
// テストケース
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
