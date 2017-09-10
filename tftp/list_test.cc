extern "C" {
#include "list.h"
}
#include <gtest/gtest.h>

TEST(list_test, create_session_list) 
{
	SESSION_LIST *plist = create_session_list();
    EXPECT_NE(plist, (SESSION_LIST*)NULL);
    EXPECT_EQ(plist->next, (SESSION_LIST*)NULL);
    EXPECT_EQ(plist->value, (SESSION*)NULL);
}

TEST(list_test, operate_list)
{
	SESSION_LIST *plist = create_session_list();
    ASSERT_EQ(0, get_list_length(plist));

    SESSION* psession = (SESSION*)malloc(sizeof(SESSION));
    insert_to_session_list(plist, psession);
    ASSERT_EQ(1, get_list_length(plist));

    remove_from_list(plist, psession);
    ASSERT_EQ(0, get_list_length(plist));
    
}


int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
