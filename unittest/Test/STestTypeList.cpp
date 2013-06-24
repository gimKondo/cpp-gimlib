#include "stdafx.h"
#include "include/TypeList.h"

using namespace GimLib;
using namespace GimLib::TL;

//`````````````````````````
// ƒRƒ“ƒpƒCƒ‹ƒeƒXƒg—p‚Ìnamespace ForStaticTest
//`````````````````````````
//™SizeSum
static_assert(SizeSum<TYPELIST_3(int, int, int)>::value == 12, "SizeSum");
//™Length
static_assert(Length<NullType>::value == 0, "Lenght");
static_assert(Length<TYPELIST_3(int,short,char)>::value == 3, "Lenght");
//™TypeAt
static_assert( IsSameType< TypeAt<TYPELIST_3(int,short,char),0>::Result, int  >::value, "TypeAt");
static_assert( IsSameType< TypeAt<TYPELIST_3(int,short,char),1>::Result, short>::value, "TypeAt");
//™TypeAtNonStrict
static_assert( IsSameType< TypeAtNonStrict<TYPELIST_3(int,short,char) ,0>::Result, int  >::value, "TypeAtNonStrict");
static_assert( IsSameType< TypeAtNonStrict<TYPELIST_3(int,short,char) ,1>::Result, short>::value, "TypeAtNonStrict");
static_assert( IsSameType< TypeAtNonStrict<TYPELIST_3(int,short,char) ,4>::Result, NullType>::value, "TypeAtNonStrict");
static_assert( IsSameType< TypeAtNonStrict<TYPELIST_3(int,short,char) ,4,int>::Result, int>::value, "TypeAtNonStrict");
//™IndexOf
static_assert(IndexOf<TYPELIST_3(int,short,char),int>::value == 0, "IndexOf");
static_assert(IndexOf<TYPELIST_3(int,short,char),float>::value == -1, "IndexOf");
static_assert(IndexOf<NullType,int>::value == -1, "IndexOf");
//™Append
static_assert( Length<Append<TYPELIST_3(int,short,char),long>::Result>::value == 4, "Append");
static_assert( IndexOf<Append<TYPELIST_3(int,short,char),long>::Result, long>::value == 3, "Append");
static_assert( IsSameType<TypeAt<Append<TYPELIST_3(int,short,char),long>::Result, 3>::Result, long>::value, "Append");
static_assert( Length<Append<TYPELIST_1(int),long>::Result>::value == 2, "Append");
static_assert( Length<	Append<	TYPELIST_3(int,short,char), TYPELIST_2(float,double)>::Result>::value == 5, "Append");
//™Erase
static_assert( IsSameType<Erase<NullType,int>::Result, NullType>::value, "Erase");
static_assert( Length<Erase<TYPELIST_3(int,short,char),int>::Result>::value == 2, "Erase");
static_assert( Length<Erase<TYPELIST_3(int,int,char),int>::Result>::value == 2, "Erase");
static_assert( Length<Erase<TYPELIST_3(int,short,char),float>::Result>::value == 3, "Erase");
//™EraseAll
static_assert( IsSameType<EraseAll<NullType,int>::Result, NullType>::value, "EraseAll");
static_assert( Length<EraseAll<TYPELIST_3(int,int,char),int>::Result>::value == 1, "EraseAll");
//™NoDuplicates
static_assert( Length<NoDuplicates<TYPELIST_5(int,int,char,char,int)>::Result>::value == 2, "NoDuplicates");
static_assert( IndexOf<NoDuplicates<TYPELIST_5(int,int,char,char,int)>::Result,int>::value == 0, "NoDuplicates");
static_assert( IndexOf<NoDuplicates<TYPELIST_5(int,int,char,char,int)>::Result,char>::value == 1, "NoDuplicates");
//™Replace
static_assert( IsSameType<Replace<NullType,int, char>::Result, NullType>::value, "Replace");
static_assert( IndexOf<	Replace<TYPELIST_3(int,short,char),int, unsigned int>::Result, unsigned int>::value == 0, "Replace");
//™ReplaceAll
static_assert( IsSameType<ReplaceAll<NullType,int, char>::Result, NullType>::value, "ReplaceAll");
static_assert( IndexOf<Replace<TYPELIST_5(int,int,short,char,int),int, float>::Result, float>::value == 0, "ReplaceAll");
static_assert( IsSameType<
				TypeAt<
						ReplaceAll<TYPELIST_5(int,int,short,char,int),int, float>::Result
					,	4>::Result
				,	float>::value, "ReplaceAll");

//™Reverse
static_assert(IsSameType<Reverse<NullType>::Result, NullType>::value, "Reverse");
static_assert( IndexOf< Reverse<TYPELIST_5(char,short,int,float,double)>::Result, double>::value == 0, "Reverse");
static_assert( IndexOf< Reverse<TYPELIST_5(char,short,int,float,double)>::Result, short>::value == 3, "Reverse");
//™MostDerived
struct TestB___ {};
struct TestDa1___ : public TestB___ {};
struct TestDa2_1_ : public TestDa1___ {};
struct TestDa2_2_ : public TestDa1___ {};
struct TestDb1___ : public TestB___ {};
struct TestDb2_1_ : public TestDb1___ {};
struct TestDb2_2_ : public TestDb1___ {};
static_assert(IsSameType<MostDerived<NullType,int>::Result, int>::value, "MostDerived");
static_assert(IsSameType<MostDerived<TYPELIST_2(short,int),int>::Result, int>::value, "MostDerived");
static_assert(
	IsSameType<	MostDerived<TYPELIST_5(char,short,int,float,double), char>::Result
			,	char>::value, "MostDerived");
static_assert(
	IsSameType<	MostDerived<TYPELIST_5(char,short,int,float,double), float>::Result
			,	float>::value, "MostDerived");
static_assert(
	IsSameType<	MostDerived<TYPELIST_3(TestB___,TestDa2_1_,TestDa1___), TestB___>::Result
			,	TestDa2_1_>::value, "MostDerived");
static_assert(	//“¯ŠK‘w‚Ì‚à‚Ì‚ÍŒã‚ë‚É‚ ‚é‚à‚Ì‚ª—Dæ‚Á‚Û‚¢‚ªAAA
	IsSameType<	MostDerived<	TYPELIST_4(TestB___,TestDa2_1_,TestDa1___,TestDa2_2_)
							,	TestB___>::Result
			,	TestDa2_2_>::value, "MostDerived");
static_assert(	//•K‚¸‚µ‚à‚»‚¤‚Å‚Í‚È‚­A•ªŠò‚µ‚½ŠK‘w‚ÌŒp³ŠÖŒW‚ğæ‚èo‚·ƒ‹[ƒ‹‚Í•¡G
	IsSameType<
		MostDerived<	TYPELIST_5(TestDb1___,TestDb2_2_,TestDa2_1_,TestDb2_1_,TestDa1___)
					,	TestB___>::Result
			,	TestDa2_1_>::value, "MostDerived");
//™DerivedToFront
struct TestD1__ : public TestB___ {};
struct TestD2__ : public TestD1__ {};
struct TestD3__ : public TestD2__ {};
struct TestD4__ : public TestD3__ {};
static_assert(
	IndexOf<
		DerivedToFront<
			TYPELIST_5(TestB___,TestD1__,TestD2__,TestD3__,TestD4__)>::Result
		,	TestB___>::value == 4, "DerivedToFront");
static_assert(
	IndexOf<
		DerivedToFront<
			TYPELIST_5(TestD3__,TestD2__,TestB___,TestD1__,TestD4__)>::Result
		,	TestB___>::value == 4, "DerivedToFront");
static_assert(
	IndexOf<
		DerivedToFront<
			TYPELIST_5(TestD3__,TestD2__,TestB___,TestD1__,TestD4__)>::Result
		,	TestD1__>::value == 3, "DerivedToFront");
static_assert(
	IndexOf<
		DerivedToFront<
			TYPELIST_5(TestD3__,TestD2__,TestB___,TestD1__,TestD4__)>::Result
		,	TestD2__>::value == 2, "DerivedToFront");
static_assert(
	IndexOf<
		DerivedToFront<
			TYPELIST_5(TestD3__,TestD2__,TestB___,TestD1__,TestD4__)>::Result
		,	TestD3__>::value == 1, "DerivedToFront");
static_assert(
	IndexOf<
		DerivedToFront<
			TYPELIST_5(TestD3__,TestD2__,TestB___,TestD1__,TestD4__)>::Result
		,	TestD4__>::value == 0, "DerivedToFront");
