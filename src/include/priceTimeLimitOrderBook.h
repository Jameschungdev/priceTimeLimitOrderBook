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
#include <memory>

// Doubly linked List

class OrderNode
{
    public:
        OrderNode()
        {
        }

        OrderNode(char bidOrAskInput, std::string orderIdInput, int quantityInput, int priceInput)
        {
            bidOrAsk = bidOrAskInput;
            orderId = orderIdInput;
            quantity = quantityInput;
            price = priceInput;
        }

        char bidOrAsk = 'U'; // Unassigned.
        std::string orderId = "unassignedOrderId";
        int quantity = -1;
        int price = -1;

        std::shared_ptr<OrderNode> next = nullptr;
        std::shared_ptr<OrderNode> prev = nullptr;
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

    std::unordered_map<std::string, std::shared_ptr<OrderNode>> orderNodeDirectory; // Used for deletion.

    // MaxHeap for the highest price for Bids.

    std::priority_queue<std::pair<int, std::shared_ptr<OrderNode>>> bidPool; 
    std::unordered_map<int, std::shared_ptr<OrderNode>> lastBidAtPrice;

    // MinHeap for the lowest price for Asks.

    std::priority_queue<std::pair<int, std::shared_ptr<OrderNode>>, std::vector<std::pair<int, std::shared_ptr<OrderNode>>>,
                        std::greater<std::pair<int, std::shared_ptr<OrderNode>>>> askPool; 
    std::unordered_map<int, std::shared_ptr<OrderNode>> lastAskAtPrice;

    void displayFinalState();
    
};