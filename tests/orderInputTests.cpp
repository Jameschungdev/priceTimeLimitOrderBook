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

    orderBook.processInputLine("A,7,B,10,90");
    ASSERT_EQ(orderBook.getNumberOfWaitingAsks(), 1);
    ASSERT_EQ(orderBook.getNumberOfWaitingBids(), 1);
    ASSERT_EQ(orderBook.getSpread(), 20);
    ASSERT_EQ(orderBook.getFirstAskOrderQuantityAtPrice(110), 5);
    ASSERT_EQ(orderBook.getFirstBidOrderQuantityAtPrice(90), 10);

    // several more orders from buyers and sellers are added to our order book, but no trades happened yet.

    orderBook.processInputLine("A,2,S,10,110");
    orderBook.processInputLine("A,3,S,3,105");
    orderBook.processInputLine("A,4,S,7,105");
    orderBook.processInputLine("A,5,B,4,100");
    orderBook.processInputLine("A,6,B,6,100");
    orderBook.processInputLine("A,8,B,2,90");
    orderBook.processInputLine("A,9,B,3,90");

    ASSERT_EQ(orderBook.getNumberOfWaitingAsks(), 4);
    ASSERT_EQ(orderBook.getNumberOfWaitingBids(), 5);
    ASSERT_EQ(orderBook.getSpread(), 5);
    ASSERT_EQ(orderBook.getFirstAskOrderQuantityAtPrice(110), 5);
    ASSERT_EQ(orderBook.getFirstAskOrderQuantityAtPrice(105), 3);
    ASSERT_EQ(orderBook.getFirstBidOrderQuantityAtPrice(100), 4);
    ASSERT_EQ(orderBook.getFirstBidOrderQuantityAtPrice(90), 10);

    // some buyer places an “aggressive” order to buy 4 shares at the price of 105 USD

    orderBook.processInputLine("A,10,B,4,105");

    ASSERT_EQ(orderBook.getNumberOfWaitingAsks(), 3);
    ASSERT_EQ(orderBook.getNumberOfWaitingBids(), 5);
    ASSERT_EQ(orderBook.getSpread(), 5);
    EXPECT_EQ(orderBook.getFirstAskOrderQuantityAtPrice(110), 5);
    EXPECT_EQ(orderBook.getFirstAskOrderQuantityAtPrice(105), 6);
    EXPECT_EQ(orderBook.getFirstBidOrderQuantityAtPrice(100), 4);
    EXPECT_EQ(orderBook.getFirstBidOrderQuantityAtPrice(90), 10);

    // “aggressive” seller wants to sell 23 shares at 80 USD

    orderBook.processInputLine("A,11,S,23,80");

    ASSERT_EQ(orderBook.getNumberOfWaitingAsks(), 3);
    ASSERT_EQ(orderBook.getNumberOfWaitingBids(), 1);
    ASSERT_EQ(orderBook.getSpread(), 15);
    EXPECT_EQ(orderBook.getFirstAskOrderQuantityAtPrice(110), 5);
    EXPECT_EQ(orderBook.getFirstAskOrderQuantityAtPrice(105), 6);
    EXPECT_EQ(orderBook.getFirstBidOrderQuantityAtPrice(90), 2);

    // new buy order of 8 shares at the price 107 USD

    orderBook.processInputLine("A,12,B,8,107");

    ASSERT_EQ(orderBook.getNumberOfWaitingAsks(), 2);
    ASSERT_EQ(orderBook.getNumberOfWaitingBids(), 2);
    ASSERT_EQ(orderBook.getSpread(), 3);
    EXPECT_EQ(orderBook.getFirstAskOrderQuantityAtPrice(110), 5);

    EXPECT_NE(orderBook.getFirstAskOrderQuantityAtPrice(105), 6);

    EXPECT_EQ(orderBook.getFirstBidOrderQuantityAtPrice(107), 2);
    EXPECT_EQ(orderBook.getFirstBidOrderQuantityAtPrice(90), 2);
}