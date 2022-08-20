#include "priceTimeLimitOrderBook.h"

int main ()
{
    PriceTimeLimitOrderBook priceTimeLimitOrderBook;
    priceTimeLimitOrderBook.processInputFile("../inputFiles/input_clean.txt");

    return 0;
}