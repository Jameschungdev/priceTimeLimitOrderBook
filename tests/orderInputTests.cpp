#include <gtest/gtest.h>
#include "priceTimeLimitOrderBook.h"

// Test based on email example. 

TEST(OrderInputTest, emailExample)
{
    PriceTimeLimitOrderBook orderBook;
    ASSERT_EQ(orderBook.getNumberOfWaitingAsks(), 0);
    ASSERT_EQ(orderBook.getNumberOfWaitingBids(), 0);
    ASSERT_EQ(orderBook.getSpread(), -1);

    orderBook.processInputLine("A,1,S,5,110");
    ASSERT_EQ(orderBook.getNumberOfWaitingAsks(), 1);
    ASSERT_EQ(orderBook.getNumberOfWaitingBids(), 0);
    ASSERT_EQ(orderBook.getSpread(), -1);
    ASSERT_EQ(orderBook.getFirstAskOrderQuantityAtPrice(110), 5);
    ASSERT_NE(orderBook.getFirstAskOrderQuantityAtPrice(95), 5);
}