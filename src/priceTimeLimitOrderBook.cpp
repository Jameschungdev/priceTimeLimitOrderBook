#include "priceTimeLimitOrderBook.h"

PriceTimeLimitOrderBook::PriceTimeLimitOrderBook()
{
    std::cout << "PriceTimeLimitOrderBook Created" << std::endl;
}

PriceTimeLimitOrderBook::~PriceTimeLimitOrderBook()
{
    displayFinalState();
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
    // Split line with comma delimiter.

    std::stringstream streamLine(inputLine);
    std::vector<std::string> inputParameters;

    std::string subStringBuffer;

    while (streamLine.good())
    {
        getline(streamLine, subStringBuffer, ',');

        inputParameters.push_back(subStringBuffer);
    }

    // Error Handling

    if (inputParameters.size() != 5)
    {
        std::cerr << "Wrong number of input parameters for {" << subStringBuffer << "}, Input skipped.\n";
        return;
    }

    // TODO: Add for error handlers
    // TODO: GTest for main functions.
    /**
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
            std::cerr << "Added input neither ASK or BID but {" << inputParameters[2] << "}. Input skipped.\n";
            return;
        }
    }
    else if (inputParameters[0] == "X")
    {
        // Pass the order ID for deletion.

        inputNewDeleteOrder(inputParameters[1]);
    }
    else
    {
        std::cerr << "Order input neither Adding or Deleting but {" << inputParameters[0] << "}. Input skipped.\n";
        return;
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

        return;
    }
    else
    {
        // Check if there is a match. 
        
        while(!askPool.empty() && newOrder->price >= askPool.top().first && newOrder->quantity > 0)
        {
            OrderNode* currentOrder = askPool.top().second->next;

            if(currentOrder == nullptr)
                break;


            if(currentOrder->quantity == newOrder->quantity)
            {
                std::cout << currentOrder->quantity << "share of XYZ were sold at " << currentOrder->price << " USD\n";
                inputNewDeleteOrder(currentOrder->orderId);
                newOrder->quantity = 0;
                
                break;
            }
            else if (currentOrder->quantity > newOrder->quantity)
            {
                std::cout << newOrder->quantity << " share of XYZ were sold at " << currentOrder->price << " USD\n";
                currentOrder->quantity -= newOrder->quantity;
                newOrder->quantity = 0;
                break;
            }
            else if (currentOrder->quantity < newOrder->quantity)
            {
                std::cout << currentOrder->quantity << " share of XYZ were sold at " << currentOrder->price << " USD\n";
                inputNewDeleteOrder(currentOrder->orderId);
                newOrder->quantity -= currentOrder->quantity;
                
                // Loop through the rest of the OrderNodes at the same price.

                while (currentOrder != nullptr)
                {

                    currentOrder = askPool.top().second->next;

                    if (currentOrder == nullptr)
                        break;

                    if (currentOrder->quantity == newOrder->quantity)
                    {
                        std::cout << currentOrder->quantity << " share of XYZ were sold at " << currentOrder->price << " USD\n";
                        inputNewDeleteOrder(currentOrder->orderId);
                        newOrder->quantity = 0;
                        break;
                    }
                    else if (currentOrder->quantity > newOrder->quantity)
                    {
                        std::cout << newOrder->quantity << " share of XYZ were sold at " << currentOrder->price << " USD\n";
                        currentOrder->quantity -= newOrder->quantity;
                        newOrder->quantity = 0;
                        break;
                    }
                    else if (currentOrder->quantity < newOrder->quantity)
                    {
                        std::cout << currentOrder->quantity << " share of XYZ were sold at " << currentOrder->price << " USD\n";
                        inputNewDeleteOrder(currentOrder->orderId);
                        newOrder->quantity -= currentOrder->quantity;
                    }
                }
            }

            // All orders fulfilled at this price bucket. Remove.

            if (askPool.top().second->next == nullptr)
            {
                askPool.pop();

                if(askPool.empty())
                    break;
            }
        }



        // Log remaining orders.

        if(newOrder->quantity > 0)
        {
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

        return;
    }
    else
    {
        // Check if there is a match.
        
        while(!bidPool.empty() && newOrder->price <= bidPool.top().first && newOrder->quantity > 0)
        {
            OrderNode* currentOrder = bidPool.top().second->next;

            if(currentOrder == nullptr)
                break;


            if(currentOrder->quantity == newOrder->quantity)
            {
                std::cout << currentOrder->quantity << "share of XYZ were brought at " << currentOrder->price << " USD\n";
                inputNewDeleteOrder(currentOrder->orderId);
                newOrder->quantity = 0;
                
                break;
            }
            else if (currentOrder->quantity > newOrder->quantity)
            {
                std::cout << newOrder->quantity << " share of XYZ were brought at " << currentOrder->price << " USD\n";
                currentOrder->quantity -= newOrder->quantity;
                newOrder->quantity = 0;
                break;
            }
            else if (currentOrder->quantity < newOrder->quantity)
            {
                std::cout << currentOrder->quantity << " share of XYZ were brought at " << currentOrder->price << " USD\n";
                inputNewDeleteOrder(currentOrder->orderId);
                newOrder->quantity -= currentOrder->quantity;
                
                // Loop through the rest of the OrderNodes at the same price.

                while (currentOrder != nullptr)
                {
                    currentOrder = bidPool.top().second->next;
                    
                    if(currentOrder == nullptr)
                        break;

                    if (currentOrder->quantity == newOrder->quantity)
                    {
                        std::cout << currentOrder->quantity << " share of XYZ were brought at " << currentOrder->price << " USD\n";
                        inputNewDeleteOrder(currentOrder->orderId);
                        newOrder->quantity = 0;
                        break;
                    }
                    else if (currentOrder->quantity > newOrder->quantity)
                    {
                        std::cout << newOrder->quantity << " share of XYZ were brought at " << currentOrder->price << " USD\n";
                        currentOrder->quantity -= newOrder->quantity;
                        newOrder->quantity = 0;
                        break;
                    }
                    else if (currentOrder->quantity < newOrder->quantity)
                    {
                        std::cout << currentOrder->quantity << " share of XYZ were brought at " << currentOrder->price << " USD\n";
                        inputNewDeleteOrder(currentOrder->orderId);
                        newOrder->quantity -= currentOrder->quantity;
                    }
                }
            }


            // All orders fulfilled at this price bucket. Remove this price bucket.

            if (bidPool.top().second->next == nullptr)
            {
                bidPool.pop();

                if(bidPool.empty())
                    break;
            }

        }

        // Log remaining orders.

        if(newOrder->quantity > 0)
        {
            // std::cout<<"Added " <<newOrder->orderId<< " S price: "<<newOrder->price<<std::endl;

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
        std::cerr << "Could not find order to delete: orderID {" << orderId << "}, Order may have already been processed.\n";
        return;
    }

    // Relink next node.

    if (orderNode->next != NULL)
    {
        orderNode->next->prev = orderNode->prev;
    }

    // Relink prev node.

    if (orderNode->prev != NULL)
    {
        orderNode->prev->next = orderNode->next;
    }

    orderNodeDirectory.erase(orderId);

    free(orderNode);

    return;
}

void PriceTimeLimitOrderBook::displayFinalState()
{
    // Output state of logbook.

    std::cout<< "=================\n\nASK\n\n";

    if (!askPool.empty())
    {   
        std::vector<std::string> askLogs;
        while(!askPool.empty())
        {

            OrderNode* log = askPool.top().second->next;

            if(log != nullptr)
            {
                std::string logsForAPrice = std::to_string(askPool.top().first) + ": ";
                while(log != nullptr)
                {
                    logsForAPrice += std::to_string(log->quantity) + ' ';
                    log = log->next;
                }

                askLogs.push_back(logsForAPrice);
            }

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

            OrderNode* log = bidPool.top().second->next;

            if(log != nullptr)
            {
                std::string logsForAPrice = std::to_string(bidPool.top().first) + ": ";

                while(log != nullptr)
                {
                    logsForAPrice += std::to_string(log->quantity) + ' ';
                    log = log->next;
                }

                bidLogs.push_back(logsForAPrice);

            }


            bidPool.pop();
        }

        for(int i = 0; i<bidLogs.size(); i++)
        {
            std::cout<< bidLogs[i] << "\n\n";
        }
    }

    std::cout<< "BID\n\n=================" <<std::endl;
}