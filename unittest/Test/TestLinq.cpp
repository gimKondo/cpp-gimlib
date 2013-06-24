#include "stdafx.h"
#include "picotest.h"
#include "include/Linq.h"

//////////////////////////////////////////////////
namespace {
//////////////////////////////////////////////////
using namespace GimLib::Linq;
using namespace GimLib::Linq::Eval;

//-----------------------------------
// テストケース
//-----------------------------------

TEST(LinqTest, vectorのSelect)
{
	std::vector<int> nums;
	nums.push_back(1);
	nums.push_back(2);
	nums.push_back(3);
	nums.push_back(4);
	nums.push_back(5);
	nums.push_back(6);
	nums.push_back(7);
	nums.push_back(8);

	auto arr = From(nums)
				| Where<int>([](int n) { return n % 2 != 0; })
				| Select<double, int>([] (int n) { return n / 2.0; })
				| All;

	EXPECT_DOUBLE_EQ( 0.5, arr[0]);
	EXPECT_DOUBLE_EQ( 1.5, arr[1]);
	EXPECT_DOUBLE_EQ( 2.5, arr[2]);
	EXPECT_DOUBLE_EQ( 3.5, arr[3]);
}
	
	
//////////////////////////////////////////////////
} // end no name namespace
//////////////////////////////////////////////////