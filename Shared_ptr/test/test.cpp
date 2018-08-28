#include <gtest\gtest.h>
#include "Shared_ptr.h"
#include <string>
#include <vector>

using namespace::std;

TEST(Sptr, Ctor) {
	{
		Shared_ptr<int> sp(new int(5));
		EXPECT_TRUE(sp);
	}
	{
		Shared_ptr<vector<int> > sp1(new vector<int>{ 1,2,3 });
		EXPECT_TRUE(sp1);
		Shared_ptr<vector<int> > sp2(sp1);
		EXPECT_TRUE(sp2);
		Shared_ptr<vector<int> > sp3 = sp2;
		EXPECT_TRUE(sp3);
	}
	{
		Shared_ptr<vector<int> > sp1(new vector<int>{ 1,2,3 });
		Shared_ptr<vector<int> > sp2(move(sp1));
		EXPECT_FALSE(sp1);
		EXPECT_TRUE(sp2);
	}
}

TEST(Sptr, Dtor) {
	{
		int* p = new int(5);
		{
			Shared_ptr<int> sp(p);
		}
		EXPECT_FALSE(*p == 5);
	}
	{
		int* p = new int(5);
		Shared_ptr<int> sp1(p);
		Shared_ptr<int> sp2(sp1);
		sp1.~Shared_ptr();
		EXPECT_TRUE(*p == 5);
		sp2.~Shared_ptr();
		EXPECT_FALSE(*p == 5);
	}
}

TEST(Sptr, Opearor) {
	{
		Shared_ptr<int> sp1(new int(5));
		Shared_ptr<int> sp2((new int(6)));
		sp2 = sp1;
		EXPECT_TRUE(sp1);
		EXPECT_TRUE(sp2);
	}
	{
		Shared_ptr<int> sp1(new int(5));
		Shared_ptr<int> sp2((new int(6)));
		sp2 = move(sp1);
		EXPECT_FALSE(sp1);
		EXPECT_TRUE(sp2);
	}
	{
		Shared_ptr<string> sp(new string("Hello,World"));
		EXPECT_TRUE(sp);
		EXPECT_EQ(*sp, "Hello,World");
	}
	{
		Shared_ptr<vector<int> > sp(new vector<int>{ 1,2,3 });
		EXPECT_TRUE(sp);
		EXPECT_EQ(sp->front(), 1);
	}
}

TEST(Uptr, GETTER) {
	{
		vector<int>* vp = new vector<int>{ 1,2,3 };
		Shared_ptr<vector<int> > sp(vp);
		EXPECT_EQ(sp.get(), vp);
	}
	{
		Shared_ptr<vector<int> > sp(nullptr);
		EXPECT_EQ(sp.use_count(), 0);
	}
	{
		vector<int>* vp = new vector<int>{ 1,2,3 };
		Shared_ptr<vector<int> > sp1(vp);
		EXPECT_TRUE(sp1.unique());
		Shared_ptr<vector<int> > sp2(sp1);
		EXPECT_EQ(sp1.use_count(), 2);
		sp2.~Shared_ptr();
		EXPECT_TRUE(sp1.unique());
	}
}

TEST(Uptr, SETTER) {
	{
		Shared_ptr<vector<int> > sp(new vector<int>{ 1,2,3 });
		sp.reset();
		EXPECT_FALSE(sp);
		sp.reset(new vector<int>{ 4,5,6 });
		EXPECT_TRUE(sp);
	}
}