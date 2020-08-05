// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "btreeplus.h"

bool checkVectorOfEntriesInNode(NodeOfBTreePlus* node, std::vector<int> vec)
{
	if (node->getVectorOfEntries().size() != vec.size())
		return false;
	
	for (int index = 0; index < vec.size(); ++index)
	{
		if (node->getVectorOfEntries().at(index) != vec.at(index))
			return false;
	}
	
	return true;
}

bool checkCountOfChilds(NodeOfBTreePlus* node, int count)
{
	return node->getVectorOfPointers().size() == count;
}

TEST_CASE( "Testing the insert call in the bplustree", "[single-file]" )
{
	BTreePlus b (5);
	b.insert(40);
	
	NodeOfBTreePlus* root = b.getRoot();
	
	REQUIRE( root != nullptr);
	REQUIRE( checkVectorOfEntriesInNode(root, {40}));
	
	b.insert(50);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40,50}));
	
	b.insert(75);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40,50,75}));
	
	b.insert(35);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {35,40,50,75}));
	
	b.insert(20);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {20,35}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {40,50,75}));
	
	b.insert(60);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {20,35}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {40,50,60,75}));
	
	b.insert(4);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {4,20,35}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {40,50,60,75}));
	
	b.insert(2);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {2,4,20,35}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {40,50,60,75}));
	
	b.insert(80);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40,60}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {2,4,20,35}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {40,50}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(2), {60,75,80}));
	
	b.insert(90);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40,60}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {2,4,20,35}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {40,50}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(2), {60,75,80,90}));
	
	b.insert(100);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40,60,80}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {2,4,20,35}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {40,50}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(2), {60,75}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(3), {80,90,100}));
	
	b.insert(110);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40,60,80}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {2,4,20,35}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {40,50}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(2), {60,75}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(3), {80,90,100,110}));
	
	b.insert(120);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40,60,80,100}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {2,4,20,35}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {40,50}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(2), {60,75}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(3), {80,90}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(4), {100,110,120}));
	
	b.insert(130);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40,60,80,100}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {2,4,20,35}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {40,50}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(2), {60,75}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(3), {80,90}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(4), {100,110,120,130}));
	
	b.insert(140);
	
	root = b.getRoot();
	NodeOfBTreePlus* left = root->pointerByIndex(0);
	NodeOfBTreePlus* right = root->pointerByIndex(1);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {80}));
	
	REQUIRE( checkVectorOfEntriesInNode(left, {40,60}));
	REQUIRE( checkVectorOfEntriesInNode(right, {100,120}));
	
	REQUIRE( checkVectorOfEntriesInNode(left->pointerByIndex(0), {2,4,20,35}));
	REQUIRE( checkVectorOfEntriesInNode(left->pointerByIndex(1), {40,50}));
	REQUIRE( checkVectorOfEntriesInNode(left->pointerByIndex(2), {60,75}));
	
	REQUIRE( checkVectorOfEntriesInNode(right->pointerByIndex(0), {80,90}));
	REQUIRE( checkVectorOfEntriesInNode(right->pointerByIndex(1), {100,110}));
	REQUIRE( checkVectorOfEntriesInNode(right->pointerByIndex(2), {120,130,140}));
}

TEST_CASE( "Testing the remove call in the bplustree", "[single-file]" )
{
	BTreePlus b (5);
	b.insert(40);
	b.insert(50);
	b.insert(75);
	b.insert(35);
	b.insert(20);
	b.insert(60);	
	b.insert(4);
	b.insert(2);
	b.insert(80);
	b.insert(90);
	b.insert(100);
	b.insert(110);
	b.insert(120);
	b.insert(130);
	b.insert(140);
	
	NodeOfBTreePlus* root = b.getRoot();
	NodeOfBTreePlus* left = root->pointerByIndex(0);
	NodeOfBTreePlus* right = root->pointerByIndex(1);
	
	REQUIRE( checkVectorOfEntriesInNode(root, {80}));
	
	REQUIRE( checkVectorOfEntriesInNode(left, {40,60}));
	REQUIRE( checkVectorOfEntriesInNode(right, {100,120}));
	
	REQUIRE( checkVectorOfEntriesInNode(left->pointerByIndex(0), {2,4,20,35}));
	REQUIRE( checkVectorOfEntriesInNode(left->pointerByIndex(1), {40,50}));
	REQUIRE( checkVectorOfEntriesInNode(left->pointerByIndex(2), {60,75}));
	
	REQUIRE( checkVectorOfEntriesInNode(right->pointerByIndex(0), {80,90}));
	REQUIRE( checkVectorOfEntriesInNode(right->pointerByIndex(1), {100,110}));
	REQUIRE( checkVectorOfEntriesInNode(right->pointerByIndex(2), {120,130,140}));
	
	REQUIRE(b.remove(140));
	
	REQUIRE( checkVectorOfEntriesInNode(root, {80}));
	
	REQUIRE( checkVectorOfEntriesInNode(left, {40,60}));
	REQUIRE( checkVectorOfEntriesInNode(right, {100,120}));
	
	REQUIRE( checkVectorOfEntriesInNode(left->pointerByIndex(0), {2,4,20,35}));
	REQUIRE( checkVectorOfEntriesInNode(left->pointerByIndex(1), {40,50}));
	REQUIRE( checkVectorOfEntriesInNode(left->pointerByIndex(2), {60,75}));
	
	REQUIRE( checkVectorOfEntriesInNode(right->pointerByIndex(0), {80,90}));
	REQUIRE( checkVectorOfEntriesInNode(right->pointerByIndex(1), {100,110}));
	REQUIRE( checkVectorOfEntriesInNode(right->pointerByIndex(2), {120,130}));
	
	REQUIRE(b.remove(130));
	root = b.getRoot();
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40,60, 80,100}));
	
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {2,4,20,35}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {40,50}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(2), {60,75}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(3), {80,90}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(4), {100,110,120}));
	
	REQUIRE(b.remove(100));
	root = b.getRoot();
	
	REQUIRE( checkVectorOfEntriesInNode(root, {40,60, 80,110}));
	
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {2,4,20,35}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {40,50}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(2), {60,75}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(3), {80,90}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(4), {110,120}));
	
	REQUIRE(b.remove(40));
	root = b.getRoot();
	
	REQUIRE( checkVectorOfEntriesInNode(root, {35,60, 80,110}));
	
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {2,4,20}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {35,50}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(2), {60,75}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(3), {80,90}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(4), {110,120}));
	
	REQUIRE(b.remove(4));
	root = b.getRoot();
	
	REQUIRE( checkVectorOfEntriesInNode(root, {35,60, 80,110}));
	
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {2,20}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {35,50}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(2), {60,75}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(3), {80,90}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(4), {110,120}));
	
	REQUIRE(b.remove(110));
	root = b.getRoot();
	
	REQUIRE( checkVectorOfEntriesInNode(root, {35,60, 80}));
	
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(0), {2,20}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(1), {35,50}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(2), {60,75}));
	REQUIRE( checkVectorOfEntriesInNode(root->pointerByIndex(3), {80,90,120}));

}

TEST_CASE( "Testing the search call in the bplustree", "[single-file]" )
{
	BTreePlus b (5);
	std::vector<int> input = {40,50,75,35,20,60,4,2,80,90,100,110,120,130,140};
	for (auto number: input)
		b.insert(number);
	
	for (auto number: input)
		REQUIRE(b.search(number) != nullptr);
	
	REQUIRE(b.search(700) == nullptr);
	REQUIRE(b.search(800) == nullptr);
}

TEST_CASE( "Testing the search call in the bplustree, second case", "[single-file]" )
{
	BTreePlus b (5);
	std::vector<int> input = {40,50,75,35,20,60,4,2,80,90,100,110,120,130,140};	
	std::vector<int> inputToRemove = {140,130,100,40,4,110};
	
	for (auto number: input)
		b.insert(number);
	
	for (auto number: input)
		REQUIRE(b.search(number) != nullptr);
	
	for (auto number: inputToRemove)
		b.remove(number);
	
	for (auto number: inputToRemove)
		REQUIRE(b.search(number) == nullptr);
	
	REQUIRE(b.search(700) == nullptr);
	REQUIRE(b.search(800) == nullptr);
	
	REQUIRE(b.search(75) != nullptr);
	REQUIRE(b.search(50) != nullptr);
}

TEST_CASE( "Testing the insertion of duplicated entries", "[single-file]" )
{
	BTreePlus b (5);
	
	REQUIRE(b.insert(40));
	REQUIRE(b.insert(50));
	REQUIRE(b.insert(75));
	REQUIRE(b.insert(35));
	
	REQUIRE(!b.insert(40));
	REQUIRE(!b.insert(50));
	REQUIRE(!b.insert(75));
	REQUIRE(!b.insert(35));
}

TEST_CASE( "Testing the insertion of one hundred elements", "[single-file]" )
{
	BTreePlus b (5);
	
	std::vector<int> vec =
	{
		96, 54, 34, 75, 64, 23, 11, 85, 61, 37, 97, 15, 38, 40, 33, 57, 16, 1, 
		51, 65, 28, 76, 66, 25, 30, 42, 90, 24, 7, 95, 83, 73, 71, 45, 39, 10, 
		41, 79, 21, 2, 5, 35, 17, 13, 81, 58, 4, 82, 32, 88, 36, 78, 18, 99, 46, 
		43, 8, 22, 59, 87, 19, 77, 49, 53, 67, 6, 12, 93, 74, 98, 60, 3, 84, 26, 
		80, 20, 62, 72, 89, 56, 9, 29, 92, 14, 48, 27, 68, 31, 47, 52, 0, 63, 
		50, 69, 55, 70, 86, 91, 94, 44
	};
	
	bool ret = true;
	for (int index=0;index < vec.size(); ++index)
	{
		ret = b.insert(40);
		REQUIRE(ret);
	}
}
