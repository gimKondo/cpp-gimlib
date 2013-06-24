#include "stdafx.h"
#include "picotest.h"
#include "include/mixin.h"
#include "include/TypeCast.h"


//////////////////////////////////////////////////
namespace {
//////////////////////////////////////////////////

//-----------------------------------
// テスト用クラス
//-----------------------------------
struct Hoge
{
	int foo() { return 0; }
	int foo() const { return 1; }
};

int bar(int& n) { return 0; }
int bar(const int& n) { return 1; }

//-----------------------------------
// テストケース
//-----------------------------------
//
TEST(TypCastTest, メソッド)
{
	Hoge hoge;
	EXPECT_EQ(0, hoge.foo());
	EXPECT_EQ(1, GimLib::add_const(hoge).foo());
}

TEST(TypCastTest, 関数)
{
	int n = 0;
	EXPECT_EQ(0, bar(n));
	EXPECT_EQ(1, bar(GimLib::add_const(n)));
}
//////////////////////////////////////////////////
} // end no name namespace
//////////////////////////////////////////////////