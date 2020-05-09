#define CATCH_CONFIG_MAIN
#include <iostream>

#include "cs221util/catch.hpp"
#include "queue.h"
#include "stack.h"

using namespace std;

TEST_CASE("stack::basic functions", "[weight=1][part=stack]")
{
    // cout << "Testing Stack..." << endl;
    Stack<int> intStack;
    vector<int> result;
    vector<int> expected;
    for (int i = 10; i > 0; i--)
    {
        expected.push_back(i);
    }
    for (int i = 1; i <= 10; i++)
    {
        intStack.push(i);
    }
    // cout << intStack.peek() << endl;
    while (!intStack.isEmpty())
    {
      result.push_back(intStack.pop());
    }
    REQUIRE(result == expected);
}

TEST_CASE("queue::basic functions", "[weight=1][part=queue]")
{
    // cout << "Testing Queue..." << endl;
    Queue<int> intQueue;
    vector<int> result;
    vector<int> expected;
    for (int i = 1; i <= 10; i++)
    {
        expected.push_back(i);
    }
    for (int i = 1; i <= 10; i++)
    {
        intQueue.enqueue(i);
        // cout << intQueue.peek() << " ";
    }
    // cout << intStack.peek() << endl;
    while (!intQueue.isEmpty())
    {
        // cout << intQueue.peek() << " ";
        result.push_back(intQueue.dequeue());
    }
    REQUIRE(result == expected);
}

TEST_CASE("queue::peek function", "[weight=1][part=queue]")
{
    // cout << "Testing Queue..." << endl;
    Queue<int> intQueue;

    for (int i = 1; i <= 100000; i++) {
      intQueue.enqueue(i);
    }

    bool result = true;
    for (int i = 1; i <= 100000; i++) {
      result = intQueue.peek() == 1 && result;
    }

    REQUIRE(result == true);
}
