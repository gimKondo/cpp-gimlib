#include "stdafx.h"
#include "picotest.h"
#include "include/stl_extension.h"
#include "include/container_make.h"

using GimLib::StlExt::has	;
using GimLib::StlExt::hasKey;
using GimLib::StlExt::any	;
using GimLib::StlExt::all	;
using GimLib::StlExt::concat;

//////////////////////////////////////////////////
namespace {
//////////////////////////////////////////////////

//-----------------------------------
// ユーティリティ
//-----------------------------------
std::list<int> makeTestList() {
	std::list<int> nums;
	for (int i = 0; i < 16; ++i) { nums.push_back(i); }
	return nums;
}
std::vector<int> makeTestVector() {
	std::vector<int> nums;
	for (int i = 0; i < 16; ++i) { nums.push_back(i); }
	return nums;
}
std::set<int> makeTestSet() {
	std::set<int> nums;
	for (int i = 0; i < 16; ++i) { nums.insert(i); }
	return nums;
}
std::map<int,int> makeTestMap() {
	std::map<int,int> numPairs;
	for (int i = 0; i < 16; ++i) { numPairs[i] = i * 2; }
	return numPairs;
}

//-----------------------------------
// テストケース
//-----------------------------------

//has
TEST(TestStlExt, has＠list)
{
	std::list<int> nums = makeTestList();
	EXPECT_TRUE (nums|has( 3));
	EXPECT_FALSE(nums|has(16));
}
TEST(TestStlExt, has＠vector)
{
	std::vector<int> nums = makeTestVector();
	EXPECT_TRUE (nums|has( 3));
	EXPECT_FALSE(nums|has(16));
}
TEST(TestStlExt, has＠set)
{
	std::set<int> nums = makeTestSet();
	EXPECT_TRUE (nums|has( 3));
	EXPECT_FALSE(nums|has(16));
}
//hasKey
TEST(TestStlExt, hasKey)
{
	std::map<int,int> numPairs = makeTestMap();
	EXPECT_TRUE (numPairs|hasKey( 3));
	EXPECT_FALSE(numPairs|hasKey(16));
}
//any
TEST(TestStlExt, any＠list)
{
	std::list<int> nums = makeTestList();
	EXPECT_TRUE (nums|any([](int n) { return n % 2 == 0;}));
	EXPECT_FALSE(nums|any([](int n) { return 15 < n; }));
}
TEST(TestStlExt, any＠vector)
{
	std::vector<int> nums = makeTestVector();
	EXPECT_TRUE (nums|any([](int n) { return n % 2 == 0;}));
	EXPECT_FALSE(nums|any([](int n) { return 15 < n; }));
}
TEST(TestStlExt, any＠set)
{
	std::set<int> nums = makeTestSet();
	EXPECT_TRUE (nums|any([](int n) { return n % 2 == 0;}));
	EXPECT_FALSE(nums|any([](int n) { return 15 < n; }));
}
TEST(TestStlExt, any＠map)
{
	std::map<int,int> numPairs = makeTestMap();
	EXPECT_TRUE (numPairs|any([](std::pair<int,int> numPair) { return numPair.first % 2 == 0;}));
	EXPECT_FALSE(numPairs|any([](std::pair<int,int> numPair) { return 15 < numPair.first; }));
}
//all
TEST(TestStlExt, all＠list)
{
	std::list<int> nums = makeTestList();
	EXPECT_TRUE (nums|all([](int n) { return n < 16; }));
	EXPECT_FALSE(nums|all([](int n) { return n < 15; }));
}
TEST(TestStlExt, all＠vector)
{
	std::vector<int> nums = makeTestVector();
	EXPECT_TRUE (nums|all([](int n) { return n < 16; }));
	EXPECT_FALSE(nums|all([](int n) { return n < 15; }));
}
TEST(TestStlExt, all＠set)
{
	std::set<int> nums = makeTestSet();
	EXPECT_TRUE (nums|all([](int n) { return n < 16; }));
	EXPECT_FALSE(nums|all([](int n) { return n < 15; }));
}
TEST(TestStlExt, all＠map)
{
	std::map<int,int> numPairs = makeTestMap();
	EXPECT_TRUE (numPairs|all([](std::pair<int,int> numPair) { return numPair.first < 16; }));
	EXPECT_FALSE(numPairs|all([](std::pair<int,int> numPair) { return numPair.first < 15; }));
}
//concat
TEST(TestStlExt, concat)
{
	std::vector<int> nums1;
	std::vector<int> nums2;
	for (int i = 0; i < 3; ++i) {
		nums1.push_back( i		);
		nums2.push_back( i + 5	);
	}
	nums1|concat(nums2);
	EXPECT_EQ (6, nums1.size());
}
//make_map
TEST(TestStlExt, makeMap)
{
	auto trg = GimLib::make_map<int,char>(1,'a')(2,'b')(3,'c')(4,'d')();
	EXPECT_EQ ('a', trg[1]);
	EXPECT_EQ ('b', trg[2]);
	EXPECT_EQ ('c', trg[3]);
	EXPECT_EQ ('d', trg[4]);
}


//////////////////////////////////////////////////
} // end no name namespace
//////////////////////////////////////////////////