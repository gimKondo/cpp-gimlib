#pragma once

namespace GimLib {

//int�l���^�Ƃ��Ĉ������߂̃N���X
template <int v> struct Int2Type {
	static const int value = v;
};
//�^�����j�[�N�ȕʂ̌^�ɕϊ�
//template <typename T> struct Type2Type {
//	typedef T OriginalType;
//};

//flag�ɂ����T,U�����ꂩ�̌^��I������
//false�̂Ƃ���T�Atrue�̂Ƃ���U���I�������
template <bool flag, typename T, typename U> struct Select {
	typedef T Result;
};
template <typename T, typename U> struct Select<false,T,U> {
	typedef U Result;
};

//�����^���ǂ����𔻒�
template <typename T, typename U> struct IsSameType {
	static const bool value = false;
};
template <typename T> struct IsSameType<T,T> {
	static const bool value = true;
};


////////////////////////////////////////////////////////////////////////////////
// Helper types Small and Big - guarantee that sizeof(Small) < sizeof(Big)
////////////////////////////////////////////////////////////////////////////////
namespace Private
{
    template <class T, class U>
    struct ConversionHelper
    {
        typedef char Small;
        struct Big { char dummy[2]; };
        static Big   Test(...);
        static Small Test(U);
        static T MakeT();
    };
}

//T��U�̕ϊ��Ɋւ���֌W�𒲂ׂ�e���v���[�g
// 1) exists��T����U�ւ̈Öق̌^�ϊ�������ꍇ��true�ɂȂ�
//	  (��FT��U��public�Ȕh���N���X�̏ꍇ)
// 2) exists2Way��T����U�AU����T�����̈Öٕϊ����\�ȏꍇ��true�ɂȂ�
//	  (��Fint��char)
// 3) sameType��T��U�������^�̏ꍇ��true�ƂȂ�

template <typename T, typename U>
struct Conversion
{
private:
	typedef char					Yes	;
	typedef struct { char v[2]; }	No	;	//Yes��No��sizeof�̌��ʂ��Ⴄ�̂��|�C���g
 	static Yes	test(U);	//�Öق̌^�ϊ�������Ȃ炱����
	static No	test(...);	//����ȊO�Ȃ炱����̃I�[�o���[�h
 	static T makeT();
public:
	//�`�F�b�N���ʁiDerived��Base�̔h����������true�j
	static const bool exists	= sizeof(Yes) == sizeof(test(makeT()));
	static const bool exists2Way = exists && Conversion<U,T>::exists;
	static const bool sameType	= false;
};
template <typename T> struct Conversion<T, T>
{
	static const bool exists	= true;
	static const bool exists2Way= true;
	static const bool sameType	= true;
};
template <typename T> struct Conversion<void, T>
{
	static const bool exists	= false;
	static const bool exists2Way= false;
	static const bool sameType	= false;
};
template <typename T> struct Conversion<T, void>
{
	static const bool exists	= false;
	static const bool exists2Way= false;
	static const bool sameType	= false;
};
template <> struct Conversion<void, void>
{
	static const bool exists	= true;
	static const bool exists2Way= true;
	static const bool sameType	= true;
};

//T��U��public�p�����Ă��邩�A�܂��͓����^�ł���ꍇ��value��true�ɂȂ�
template <typename T, typename U> struct SuperSubclass {
	static const int value =(	Conversion<const volatile U*, const volatile T*>::exists
							&&	!Conversion<const volatile T*, const volatile void*>::sameType);
	//�R���p�C����T�AU�̊��S�Ȓ�`���쐬�����邽�߂̃_�~�[
	enum{ dontUseWithIncompleteTypes = ( sizeof (T) == sizeof (U) ) };
};
template <> struct SuperSubclass<void, void> {
	static const int value = false;
};
template <class U> struct SuperSubclass<void, U> 
{
	static const int value = (	Conversion<const volatile U*, const volatile void*>::exists &&
							&&	!Conversion<const volatile void*, const volatile void*>::sameType);
    //�R���p�C����U�̊��S�Ȓ�`���쐬�����邽�߂̃_�~�[
	enum{ dontUseWithIncompleteTypes = ( 0 == sizeof (U) ) };
};
template <class T> struct SuperSubclass<T,void> 
{
	static const int value = (	Conversion<const volatile void*, const volatile T*>::exists &&
							&&	!Conversion<const volatile T*, const volatile void*>::sameType);
    //�R���p�C����T�̊��S�Ȓ�`���쐬�����邽�߂̃_�~�[
	enum{ dontUseWithIncompleteTypes = ( 0 == sizeof (T) ) };
};


//T��U��public�p�����Ă���ꍇ��value��true�ɂȂ�
template<class T,class U>
struct SuperSubclassStrict
{
	static const int value =	(Conversion<const volatile U*, const volatile T*>::exists
							&&	!Conversion<const volatile T*, const volatile void*>::sameType
							&&	!Conversion<const volatile T*, const volatile U*>::sameType);
    //�R���p�C����T�AU�̊��S�Ȓ�`���쐬�����邽�߂̃_�~�[
	enum{ dontUseWithIncompleteTypes = ( sizeof (T) == sizeof (U) ) };
};

template<> struct SuperSubclassStrict<void, void> {
	static const int value = false;
};

template<class U> struct SuperSubclassStrict<void, U> 
{
    static const int value =	(Conversion<const volatile U*, const volatile void*>::exists
							&&	!Conversion<const volatile void*, const volatile void*>::sameType
							&&	!Conversion<const volatile void*, const volatile U*>::sameType);
    //�R���p�C����U�̊��S�Ȓ�`���쐬�����邽�߂̃_�~�[
	enum{ dontUseWithIncompleteTypes = ( 0 == sizeof (U) ) };
};

template<class T>
struct SuperSubclassStrict<T, void> 
{
    static const int value =	(Conversion<const volatile void*, const volatile T*>::exists
							&&	!Conversion<const volatile T*, const volatile void*>::sameType
							&&	!Conversion<const volatile T*, const volatile void*>::sameType);
    
    //�R���p�C����T�̊��S�Ȓ�`���쐬�����邽�߂̃_�~�[
    enum{ dontUseWithIncompleteTypes = ( sizeof (T) == 0 ) };
};
}