#include "stdafx.h"
#include "picotest.h"
#include "include/extendable.h"


//////////////////////////////////////////////////
namespace {
//////////////////////////////////////////////////

//-----------------------------------
// テスト用クラス
//-----------------------------------
//拡張可能なアンドロイド
struct Sample
{
	explicit Sample(int n) :num_(n)
	{}
	//拡張メソッドを提供する仕組み
	EXTENABLE_CLASS_FORMULA(Android)
private:
	int num_;
};

//拡張メソッド定義
EXTENSION_METHOD_BEGIN(Sample,getNum)
	int operator()() { return this_->num_; }
EXTENSION_METHOD_END

//-----------------------------------
// テストケース
//-----------------------------------
//
TEST(ExtendableClassTest, call)
{
	auto sample = Sample(3);
	int n = sample.Ext<getNum>()();
	EXPECT_EQ (3, n);
}


//////////////////////////////////////////////////
} // end no name namespace
//////////////////////////////////////////////////
