// Price Time Limit Order Book
// Can input files or individual lines.

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <list>
#include <queue>

// Doubly linked List

class OrderNode
{
    public:
        OrderNode()
        {
            bidOrAsk = 'E';
            orderId = -1;
            quantity = -1;
            price = -1;
        }

        OrderNode(char bidOrAskInput, std::string orderIdInput, int quantityInput, int priceInput)
        {
            bidOrAsk = bidOrAskInput;
            orderId = orderIdInput;
            quantity = quantityInput;
            price = priceInput;
        }

        char bidOrAsk; 
        std::string orderId;
        int quantity;
        double price;

        OrderNode* next = nullptr;
        OrderNode* prev = nullptr;
};


class PriceTimeLimitOrderBook
{
public:
    PriceTimeLimitOrderBook();
    ~PriceTimeLimitOrderBook(); // Save last data to file before destroying. 

    void processInputFile(std::string filePath);
    void processInputLine(std::string inputLine); // test for false input

private:
    void inputNewBid(std::vector<std::string> inputParameters);
    void inputNewAsk(std::vector<std::string> inputParameters);
    void inputNewDeleteOrder(std::string orderId);

    std::unordered_map<std::string, OrderNode*> orderNodeDirectory; // Used for deletion.
    

    // MaxHeap for the highest price for Bids.

    std::priority_queue<std::pair<int, OrderNode*>> bidPool; 
    std::unordered_map<int, OrderNode*> lastBidAtPrice;

    // MinHeap for the lowest price for Asks.

    std::priority_queue<std::pair<int, OrderNode*>, std::vector<std::pair<int, OrderNode*>>,
                        std::greater<std::pair<int, OrderNode*>>> askPool; 
    std::unordered_map<int, OrderNode*> lastAskAtPrice;

    
};