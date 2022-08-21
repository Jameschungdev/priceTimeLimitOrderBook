#include "priceTimeLimitOrderBook.h"

int main ()
{
    PriceTimeLimitOrderBook priceTimeLimitOrderBook;
    priceTimeLimitOrderBook.processInputFile("../inputFiles/input_email.txt");

    return 0;
}