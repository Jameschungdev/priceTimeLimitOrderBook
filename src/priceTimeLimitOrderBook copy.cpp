// #include "priceTimeLimitOrderBook.h"

// PriceTimeLimitOrderBook::PriceTimeLimitOrderBook()
// {
//     std::cout << "PriceTimeLimitOrderBook Created" << std::endl;
// }

// PriceTimeLimitOrderBook::~PriceTimeLimitOrderBook()
// {
//     // Output state of logbook.

//     while(!bidPool.empty())
//     {

//     }
// }

// // Processes an input file line by line.

// void PriceTimeLimitOrderBook::processInputFile(std::string filePath)
// {
//     std::ifstream inputFile(filePath);
//     std::string line;
//     while (std::getline(inputFile, line))
//     {
//         processInputLine(line);
//     }
// }

// // Processes an input line.

// void PriceTimeLimitOrderBook::processInputLine(std::string inputLine)
// {
//     // TODO: test for invalid input.
//     // TODO: Add gtest for this function.

//     // Split line with comma delimiter.

//     std::stringstream streamLine(inputLine);
//     std::vector<std::string> inputParameters;

//     std::string subStringBuffer;

//     while (streamLine.good())
//     {
//         getline(streamLine, subStringBuffer, ',');

//         inputParameters.push_back(subStringBuffer);

//         // std::cout<<subStringBuffer<<std::endl;
//     }

//     // Error Handling

//     if (inputParameters.size() != 5)
//     {
//         std::cerr << "Wrong number of input parameters for {" << subStringBuffer << "}, Input skipped.\n";
//         return;
//     }

//     // TODO: Add for error handlers
//     /**
//      *  Each input is A/B
//      *  Valid numbers
//      * **/

//     // Sort order to ASK/BID/DEL

//     if (inputParameters[0] == "A")
//     {
//         if (inputParameters[2] == "S")
//         {
//             inputNewAsk(inputParameters);
//         }
//         else if (inputParameters[2] == "B")
//         {
//             inputNewBid(inputParameters);
//         }
//         else
//         {
//             std::cerr << "Added input neither ASK or BID but {" << inputParameters[2] << "}\n";
//         }
//     }
//     else if (inputParameters[0] == "X")
//     {
//         // Pass the order ID for deletion.

//         inputNewDeleteOrder(inputParameters[1]);
//     }
//     else
//     {
//         std::cerr << "Order input neither Adding or Deleting but {" << inputParameters[0] << "}\n";
//     }
// }

// // Handles new bid orders.

// void PriceTimeLimitOrderBook::inputNewBid(std::vector<std::string> inputParameters)
// {
//     double bidPrice = stod(inputParameters[4]);
//     int quantity = stoi(inputParameters[3]);
//     std::string orderId = inputParameters[1];

//     OrderNode *newOrder = new OrderNode('B', orderId, quantity, bidPrice);
//     orderNodeDirectory[orderId] = newOrder;

//     if (lastOrderNodeAtPrice.find(bidPrice) != lastOrderNodeAtPrice.end())
//     {
//         // This means there are waiting orders waiting. Add this order at end of queue and exit.

//         OrderNode *lastOrder = lastOrderNodeAtPrice[bidPrice];
//         lastOrder->next = newOrder;
//         lastOrderNodeAtPrice[bidPrice] = newOrder;

//         return;
//     }
//     else
//     {
//         // No waiting orders at current price. Compare if there is a match. Orderwise add a new log/list.

//         // Check if match is possible.

//         // if (askPool.empty())
//         // {
//         //     bidPool.push(std::make_pair(bidPrice, newOrder));
//         // }
//         // else if (askPool.top().first < newOrder->price)
//         // {
//             // Loop through queue until no match or quantity runs out. 
//             // stdout each trade succeeded.

//             // while(newOrder->quantity > 0 || askPool.empty() || bidPrice > askPool.top().first)
//             // {   
//             //     if(askPool.top().second->next != NULL)
//             //     {
//             //         // Iterate through lists at current price.

//             //         OrderNode *priceHeadNode = askPool.top().second;
//             //         OrderNode *askOrderToProcess = priceHeadNode->next;

//             //         while (newOrder->quantity > 0 || askOrderToProcess != NULL)
//             //         {
//             //             askOrderToProcess = priceHeadNode->next;

//             //             if (askOrderToProcess->quantity == newOrder->quantity)
//             //             {
//             //                 // Complete both requests.

//             //                 priceHeadNode->next = askOrderToProcess->next;

//             //                 std::cout << askOrderToProcess->quantity << "share of XYZ were sold at " << askOrderToProcess->price << " USD\n";

//             //                 break;
//             //             }
//             //             else if (askOrderToProcess->quantity <= newOrder->quantity)
//             //             {
//             //                 // Complete ask request, but continue newOrder.

//             //                 priceHeadNode->next = askOrderToProcess->next;

//             //                 newOrder->quantity -= askOrderToProcess->quantity;

//             //                 std::cout << askOrderToProcess->quantity << "share of XYZ were sold at " << askOrderToProcess->price << " USD\n";
//             //             }
//             //             else if (askOrderToProcess->quantity >= newOrder->quantity)
//             //             {
//             //                 // Complete newOrder, but update ask request.

//             //                 askOrderToProcess->quantity -= newOrder->quantity;

//             //                 std::cout << newOrder->quantity << "share of XYZ were sold at " << newOrder->price << " USD\n";

//             //                 break;
//             //             }
//             //         }
//             //     }
//             //     else
//             //     {
//             //         // List at current price exhausted. Remove price bracket and go to next lowest price. 

//             //         askPool.pop();
//             //     }
//             // }

//             // Log remaining orders.

//             // if(newOrder->quantity > 0)
//             // {
//             //     std::cout<<"log"<<std::endl;
//             //     bidPool.push(std::make_pair(bidPrice, newOrder));
//             // }

//         // }
//     }

//     // Log remaining Bids
// }

// // Handles new ask orders.

// void PriceTimeLimitOrderBook::inputNewAsk(std::vector<std::string> inputParameters)
// {
//     // Check if match is possible.

//     // Log remaining bids.
// }

// // Handles new delete orders.

// void PriceTimeLimitOrderBook::inputNewDeleteOrder(std::string orderId)
// {
//     OrderNode *orderNode;

//     if (orderNodeDirectory.find(orderId) != orderNodeDirectory.end())
//     {
//         orderNode = orderNodeDirectory[orderId];
//     }
//     else
//     {
//         std::cerr << "Could not find order to delete: orderID {" << orderId << "}\n";
//         return;
//     }

//     // Relink next node.

//     if (orderNode->next != NULL)
//     {
//         orderNode->next->prev = orderNode->prev;
//     }

//     // Relink prev node.

//     if (orderNode->prev != NULL)
//     {
//         orderNode->prev->next = orderNode->next;
//     }

//     // Delete hash row, free linkNode

//     orderNodeDirectory.erase(orderId);
//     free(orderNode);
//     return;
// }