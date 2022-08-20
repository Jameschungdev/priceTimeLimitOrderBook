#include "priceTimeLimitOrderBook.h"

PriceTimeLimitOrderBook::PriceTimeLimitOrderBook()
{
    std::cout << "PriceTimeLimitOrderBook Created" << std::endl;
}

PriceTimeLimitOrderBook::~PriceTimeLimitOrderBook()
{
    // Output state of logbook.

    std::cout<< "=================\n\nASK\n\n";

    if (!askPool.empty())
    {   
        std::vector<std::string> askLogs;
        while(!askPool.empty())
        {
            std::string logsForAPrice = std::to_string(askPool.top().first) + ": ";

            OrderNode* log = askPool.top().second->next;

            while(log != nullptr)
            {
                logsForAPrice += std::to_string(log->quantity) + ' ';
                log = log->next;
            }

            askLogs.push_back(logsForAPrice);

            askPool.pop();
        }

        for(int i = askLogs.size()-1; i>=0; i--)
        {
            std::cout<< askLogs[i] << "\n\n";
        }
    }



    std::cout<< "------------\n\n";


    if (!bidPool.empty())
    {   
        std::vector<std::string> bidLogs;
        while(!bidPool.empty())
        {
            std::string logsForAPrice = std::to_string(bidPool.top().first) + ": ";

            OrderNode* log = bidPool.top().second->next;

            while(log != nullptr)
            {
                logsForAPrice += std::to_string(log->quantity) + ' ';
                log = log->next;
            }

            bidLogs.push_back(logsForAPrice);

            bidPool.pop();
        }

        for(int i = 0; i<bidLogs.size(); i++)
        {
            std::cout<< bidLogs[i] << "\n\n";
        }
    }

    std::cout<< "BID\n\n=================" <<std::endl;

}

// Processes an input file line by line.

void PriceTimeLimitOrderBook::processInputFile(std::string filePath)
{
    std::ifstream inputFile(filePath);
    std::string line;
    while (std::getline(inputFile, line))
    {
        processInputLine(line);
    }
}

// Processes an input line.

void PriceTimeLimitOrderBook::processInputLine(std::string inputLine)
{
    // TODO: test for invalid input.
    // TODO: Add gtest for this function.

    // Split line with comma delimiter.

    std::stringstream streamLine(inputLine);
    std::vector<std::string> inputParameters;

    std::string subStringBuffer;

    while (streamLine.good())
    {
        getline(streamLine, subStringBuffer, ',');

        inputParameters.push_back(subStringBuffer);

        // std::cout<<subStringBuffer<<std::endl;
    }

    // Error Handling

    if (inputParameters.size() != 5)
    {
        std::cerr << "Wrong number of input parameters for {" << subStringBuffer << "}, Input skipped.\n";
        return;
    }

    // TODO: Add for error handlers
    /**
     *  Each input is A/B
     *  Valid numbers
     * **/

    // Sort order to ASK/BID/DEL

    if (inputParameters[0] == "A")
    {
        if (inputParameters[2] == "S")
        {
            inputNewAsk(inputParameters);
        }
        else if (inputParameters[2] == "B")
        {
            inputNewBid(inputParameters);
        }
        else
        {
            std::cerr << "Added input neither ASK or BID but {" << inputParameters[2] << "}\n";
        }
    }
    else if (inputParameters[0] == "X")
    {
        // Pass the order ID for deletion.

        inputNewDeleteOrder(inputParameters[1]);
    }
    else
    {
        std::cerr << "Order input neither Adding or Deleting but {" << inputParameters[0] << "}\n";
    }
}

// Handles new bid orders.

void PriceTimeLimitOrderBook::inputNewBid(std::vector<std::string> inputParameters)
{
    int bidPrice = stoi(inputParameters[4]);
    int quantity = stoi(inputParameters[3]);
    std::string orderId = inputParameters[1];

    OrderNode *newOrder = new OrderNode('B', orderId, quantity, bidPrice);
    orderNodeDirectory[orderId] = newOrder;

    if (lastBidAtPrice.find(bidPrice) != lastBidAtPrice.end())
    {
        // This means there are waiting orders waiting. Add this order at end of queue and exit.

        OrderNode *lastOrder = lastBidAtPrice[bidPrice];
        lastOrder->next = newOrder;
        newOrder->prev = lastOrder;
        lastBidAtPrice[bidPrice] = newOrder;

        std::cout<<"Attached " <<newOrder->orderId<< " B price: "<<newOrder->price<<std::endl;

        return;
    }
    else
    {
        // Check if there is a match. 




        // Log remaining orders.

        if(newOrder->quantity > 0)
        {
            std::cout<<"Added " <<newOrder->orderId<< " B price: "<<newOrder->price<<std::endl;

            OrderNode* emptyHead = new OrderNode();

            bidPool.push(std::make_pair(bidPrice, emptyHead));
            emptyHead->next = newOrder;
            newOrder->prev = emptyHead;

            lastBidAtPrice[bidPrice] = newOrder;
        }
    }

}

// Handles new ask orders.

void PriceTimeLimitOrderBook::inputNewAsk(std::vector<std::string> inputParameters)
{
    int askPrice = stoi(inputParameters[4]);
    int quantity = stoi(inputParameters[3]);
    std::string orderId = inputParameters[1];

    OrderNode *newOrder = new OrderNode('S', orderId, quantity, askPrice);
    orderNodeDirectory[orderId] = newOrder;

    if (lastAskAtPrice.find(askPrice) != lastAskAtPrice.end())
    {
        // This means there are waiting orders waiting. Add this order at end of queue and exit.

        OrderNode *lastOrder = lastAskAtPrice[askPrice];
        lastOrder->next = newOrder;
        newOrder->prev = lastOrder;

        lastAskAtPrice[askPrice] = newOrder;

        std::cout<<"Attached " <<newOrder->orderId<< " S price: "<<newOrder->price<<std::endl;

        return;
    }
    else
    {
        // Check if there is a match. 




        // Log remaining orders.

        if(newOrder->quantity > 0)
        {
            std::cout<<"Added " <<newOrder->orderId<< " S price: "<<newOrder->price<<std::endl;

            OrderNode* emptyHead = new OrderNode();

            askPool.push(std::make_pair(askPrice, emptyHead));
            emptyHead->next = newOrder;
            newOrder->prev = emptyHead;

            lastAskAtPrice[askPrice] = newOrder;
        }
    }
}

// Handles new delete orders.

void PriceTimeLimitOrderBook::inputNewDeleteOrder(std::string orderId)
{
    OrderNode *orderNode;

    if (orderNodeDirectory.find(orderId) != orderNodeDirectory.end())
    {
        orderNode = orderNodeDirectory[orderId];
    }
    else
    {
        std::cerr << "Could not find order to delete: orderID {" << orderId << "}\n";
        return;
    }

    // // Relink next node.

    if (orderNode->next != NULL)
    {
        orderNode->next->prev = orderNode->prev;
    }

    // Relink prev node.

    if (orderNode->prev != NULL)
    {
        orderNode->prev->next = orderNode->next;
    }

    // // Delete hash row, free linkNode

    std::cout<< "Deleted " << orderNode->orderId  << std::endl;

    orderNodeDirectory.erase(orderId);

    free(orderNode);

    return;
}