#include "stdafx.h"
#include "picotest.h"
#include "include/Matrix.h"
#include "GimLibTestHelper.h"

using GimLib::Matrix;

//////////////////////////////////////////////////
namespace {
//////////////////////////////////////////////////

//-----------------------------------
// テストケース
//-----------------------------------
//
TEST(MatrixTest, Matrix_4_3)
{
	Matrix<int, 4, 3> m;
	m.Aa = 0; m.Ba = 1; m.Ca = 2; m.Da = 3;
	m.Ab =10; m.Bb =11; m.Cb =12; m.Db =13;
	m.Ac =20; m.Bc =21; m.Cc =22; m.Dc =23;

	EXPECT_EQ( 0, m.Aa);
	EXPECT_EQ( 1, m.Ba);
	EXPECT_EQ( 2, m.Ca);
	EXPECT_EQ( 3, m.Da);
	EXPECT_EQ(10, m.Ab);
	EXPECT_EQ(11, m.Bb);
	EXPECT_EQ(12, m.Cb);
	EXPECT_EQ(13, m.Db);
	EXPECT_EQ(20, m.Ac);
	EXPECT_EQ(21, m.Bc);
	EXPECT_EQ(22, m.Cc);
	EXPECT_EQ(23, m.Dc);
}

//////////////////////////////////////////////////
} // end no name namespace
////////////////////