#include "stdafx.h"
#include "picotest.h"
#include "include/extendable.h"


//////////////////////////////////////////////////
namespace {
//////////////////////////////////////////////////

//-----------------------------------
// �e�X�g�p�N���X
//-----------------------------------
//�g���\�ȃA���h���C�h
struct Sample
{
	explicit Sample(int n) :num_(n)
	{}
	//�g�����\�b�h��񋟂���d�g��
	EXTENABLE_CLASS_FORMULA(Android)
private:
	int num_;
};

//�g�����\�b�h��`
EXTENSION_METHOD_BEGIN(Sample,getNum)
	int operator()() { return this_->num_; }
EXTENSION_METHOD_END

//-----------------------------------
// �e�X�g�P�[�X
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
