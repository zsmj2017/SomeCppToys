#include <gtest\gtest.h>
#include "Unique_ptr.h"
#include <string>
#include <vector>

using namespace::std;

TEST(Uptr, Ctor) {
	{
		Unique_ptr<int> up(new int(5));
		EXPECT_TRUE(up);
	}
	{
		Unique_ptr<int> up1(new int(5));
		Unique_ptr<int> up2(move(up1));
		EXPECT_FALSE(up1);
		EXPECT_TRUE(up2);
	}
}

TEST(Uptr, Dtor) {
	{
		int* p = new int(5); 
		{
			Unique_ptr<int> up(p);
		}
		EXPECT_FALSE(*p==5);
	}
}

TEST(Uptr, Opearor) {
	{
		Unique_ptr<int> up1(new int(5));
		Unique_ptr<int> up2=std::move(up1);
		EXPECT_TRUE(up2);
	}
	{
		Unique_ptr<string> up(new string("Hello,World"));
		EXPECT_EQ(*up, "Hello,World");
	}
	{
		Unique_ptr<vector<int> > up(new vector<int>{ 1,2,3 });
		EXPECT_EQ(up->front(), 1);
	}
}

TEST(Uptr, GETTER) {
	{
		vector<int>* vp = new vector<int>{ 1,2,3 };
		Unique_ptr<vector<int> > up(vp);
		EXPECT_EQ(up.get(), vp);
	}
}

TEST(Uptr, SETTER) {
	{
		Unique_ptr<vector<int> > up(new vector<int>{ 1,2,3 });
		up.reset();
		EXPECT_FALSE(up);
		up.reset(new vector<int>{ 4,5,6 });
		EXPECT_TRUE(up);
	}
	{
		Unique_ptr<string > up(new string("TEST UPTR'S RELEASE"));
		string* sp = up.release();
		EXPECT_FALSE(up);
		EXPECT_TRUE(sp);
		EXPECT_EQ(*sp, "TEST UPTR'S RELEASE");
	}
}