#pragma once

namespace GimLib {

//int値を型として扱うためのクラス
template <int v> struct Int2Type {
	static const int value = v;
};
//型をユニークな別の型に変換
//template <typename T> struct Type2Type {
//	typedef T OriginalType;
//};

//flagによってT,Uいずれかの型を選択する
//falseのときはT、trueのときはUが選択される
template <bool flag, typename T, typename U> struct Select {
	typedef T Result;
};
template <typename T, typename U> struct Select<false,T,U> {
	typedef U Result;
};

//同じ型かどうかを判定
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

//TとUの変換に関する関係を調べるテンプレート
// 1) existsはTからUへの暗黙の型変換がある場合にtrueになる
//	  (例：TがUのpublicな派生クラスの場合)
// 2) exists2WayはTからU、UからT両方の暗黙変換が可能な場合にtrueになる
//	  (例：intとchar)
// 3) sameTypeはTとUが同じ型の場合にtrueとなる

template <typename T, typename U>
struct Conversion
{
private:
	typedef char					Yes	;
	typedef struct { char v[2]; }	No	;	//YesとNoでsizeofの結果が違うのがポイント
 	static Yes	test(U);	//暗黙の型変換があるならこちら
	static No	test(...);	//それ以外ならこちらのオーバロード
 	static T makeT();
public:
	//チェック結果（DerivedがBaseの派生だったらtrue）
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

//TがUをpublic継承しているか、または同じ型である場合にvalueがtrueになる
template <typename T, typename U> struct SuperSubclass {
	static const int value =(	Conversion<const volatile U*, const volatile T*>::exists
							&&	!Conversion<const volatile T*, const volatile void*>::sameType);
	//コンパイラにT、Uの完全な定義を作成させるためのダミー
	enum{ dontUseWithIncompleteTypes = ( sizeof (T) == sizeof (U) ) };
};
template <> struct SuperSubclass<void, void> {
	static const int value = false;
};
template <class U> struct SuperSubclass<void, U> 
{
	static const int value = (	Conversion<const volatile U*, const volatile void*>::exists &&
							&&	!Conversion<const volatile void*, const volatile void*>::sameType);
    //コンパイラにUの完全な定義を作成させるためのダミー
	enum{ dontUseWithIncompleteTypes = ( 0 == sizeof (U) ) };
};
template <class T> struct SuperSubclass<T,void> 
{
	static const int value = (	Conversion<const volatile void*, const volatile T*>::exists &&
							&&	!Conversion<const volatile T*, const volatile void*>::sameType);
    //コンパイラにTの完全な定義を作成させるためのダミー
	enum{ dontUseWithIncompleteTypes = ( 0 == sizeof (T) ) };
};


//TがUをpublic継承している場合にvalueがtrueになる
template<class T,class U>
struct SuperSubclassStrict
{
	static const int value =	(Conversion<const volatile U*, const volatile T*>::exists
							&&	!Conversion<const volatile T*, const volatile void*>::sameType
							&&	!Conversion<const volatile T*, const volatile U*>::sameType);
    //コンパイラにT、Uの完全な定義を作成させるためのダミー
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
    //コンパイラにUの完全な定義を作成させるためのダミー
	enum{ dontUseWithIncompleteTypes = ( 0 == sizeof (U) ) };
};

template<class T>
struct SuperSubclassStrict<T, void> 
{
    static const int value =	(Conversion<const volatile void*, const volatile T*>::exists
							&&	!Conversion<const volatile T*, const volatile void*>::sameType
							&&	!Conversion<const volatile T*, const volatile void*>::sameType);
    
    //コンパイラにTの完全な定義を作成させるためのダミー
    enum{ dontUseWithIncompleteTypes = ( sizeof (T) == 0 ) };
};
}