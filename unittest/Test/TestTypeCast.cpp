#include "stdafx.h"
#include "picotest.h"
#include "include/mixin.h"
#include "include/TypeCast.h"


//////////////////////////////////////////////////
namespace {
//////////////////////////////////////////////////

//-----------------------------------
// �e�X�g�p�N���X
//-----------------------------------
struct Hoge
{
	int foo() { return 0; }
	int foo() const { return 1; }
};

int bar(int& n) { return 0; }
int bar(const int& n) { return 1; }

//-----------------------------------
// �e�X�g�P�[�X
//-----------------------------------
//
TEST(TypCastTest, ���\�b�h)
{
	Hoge hoge;
	EXPECT_EQ(0, hoge.foo());
	EXPECT_EQ(1, GimLib::add_const(hoge).foo());
}

TEST(TypCastTest, �֐�)
{
	int n = 0;
	EXPECT_EQ(0, bar(n));
	EXPECT_EQ(1, bar(GimLib::add_const(n)));
}
//////////////////////////////////////////////////
} // end no name namespace
//////////////////////////////////////////////////