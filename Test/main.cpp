#include "FibonacciHeap.hpp"
#include "Heap.hpp"
#include "PairingHeap.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include<time.h>
#include <chrono>
#include <queue>

using namespace Datastructures::Heaps;

const std::vector<std::string> operations = {
    "Insert",
    "ExtractTop",
    "Remove",
    "AugmentKey"
};

void PrintTimingInfo(
    const std::string& name,
    std::unordered_map<std::string, std::chrono::microseconds>& timing_info,
    std::unordered_map<std::string, int>& operation_count)
{
    std::stringstream ss;
    ss << "Timing Info for " << name << '\n';
    for (const std::string operation : operations)
    {
        ss  << '\t' << operation << '\n'
            << "\t\tNumber of Times Called: " << operation_count[operation] << '\n'
            << "\t\tAverage Time (microseconds): " << ((double)timing_info[operation].count() / (double)operation_count[operation]) << '\n';
    }

    std::cout << ss.str() << "\n\n";
}

template<typename Heap>
void TestHeap(const std::string& name, Heap& heap, bool min)
{
    std::unordered_map<int, Heap::NODE_TYPE> key_to_node;
    std::unordered_map<Heap::NODE_TYPE, int> node_to_key;

    std::unordered_map<std::string, std::chrono::microseconds> timing_info;
    std::unordered_map<std::string, int> operation_count;
    for (const std::string& operation : operations)
    {
        timing_info[operation] = std::chrono::microseconds();
        operation_count[operation] = 0;
    }

    std::ifstream in("HeapDriver.in");

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;

    std::string line = "";
    while (std::getline(in, line))
    {
        size_t colon = line.find_first_of(':');
        std::string action(line.substr(0, colon));
        std::string args(line.substr(colon + 1, line.length() - colon));

        if (action == "Insert")
        {
            size_t comma = args.find(',');
            int key = stoi(args.substr(0, comma));
            int value = stoi(args.substr(comma + 1, args.length() - comma));

            start = std::chrono::high_resolution_clock::now();
            key_to_node[key] = heap.Insert(value);
            end = std::chrono::high_resolution_clock::now();

            node_to_key[key_to_node[key]] = key;
        }
        else if (action == "ExtractTop")
        {
            start = std::chrono::high_resolution_clock::now();
            Heap::NODE_TYPE top = heap.ExtractTop();
            end = std::chrono::high_resolution_clock::now();

            int key = node_to_key[top];
            node_to_key.erase(top);
            key_to_node.erase(key);
        }
        else if (action == "Remove")
        {
            int key = stoi(args);

            if (!key_to_node.count(key)) { continue; }

            start = std::chrono::high_resolution_clock::now();
            heap.Remove(key_to_node[key]);
            end = std::chrono::high_resolution_clock::now();

            Heap::NODE_TYPE node = key_to_node[key];
            node_to_key.erase(node);
            key_to_node.erase(key);
        }
        else if (action == "AugmentKey")
        {
            size_t comma = args.find(',');
            int key = stoi(args.substr(0, comma));
            int value = stoi(args.substr(comma + 1, args.length() - comma));

            if (!key_to_node.count(key)) { continue; }
            Heap::NODE_TYPE node = key_to_node[key];

            start = std::chrono::high_resolution_clock::now();
            heap.AugmentKey(node, min ?  node->Key() - value : node->Key() + value);
            end = std::chrono::high_resolution_clock::now();
        }

        timing_info[action] += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        ++operation_count[action];
    }

    PrintTimingInfo(name, timing_info, operation_count);
}

template<typename Heap>
void TestHeap_LinkedList(const std::string& name, Heap& heap, bool min)
{
    std::unordered_map<std::string, std::chrono::microseconds> timing_info;
    std::unordered_map<std::string, int> operation_count;

    std::unordered_map<int, Heap::NODE_TYPE> key_to_node;
    std::unordered_map<Heap::NODE_TYPE, int> node_to_key;

    for (const std::string& operation : operations)
    {
        timing_info[operation] = std::chrono::microseconds();
        operation_count[operation] = 0;
    }

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;

    std::string action = "Insert";
    for (unsigned int i = 0; i < 250000; i++)
    {
        int val = rand() % 100;
        start = std::chrono::high_resolution_clock::now();
        Heap::NODE_TYPE node = heap.Insert(val);
        end = std::chrono::high_resolution_clock::now();

        key_to_node[i] = node;
        node_to_key[node] = i;

        timing_info[action] += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        ++operation_count[action];
    }

    action = "AugmentKey";
    for (unsigned int i = 0; i < 250000; i++)
    {
        size_t idx = rand() % 249999;
        int delta = rand() % 1000 + 1;

        Heap::NODE_TYPE node = key_to_node[i];

        start = std::chrono::high_resolution_clock::now();
        heap.AugmentKey(node, min ? node->Key() - delta : node->Key() + delta);
        end = std::chrono::high_resolution_clock::now();

        timing_info[action] += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        ++operation_count[action];
    }

    action = "ExtractTop";
    for (unsigned int i = 0; i < 125000; i++)
    {
        start = std::chrono::high_resolution_clock::now();
        Heap::NODE_TYPE node = heap.ExtractTop();
        end = std::chrono::high_resolution_clock::now();

        int key = node_to_key[node];

        node_to_key.erase(node);
        key_to_node.erase(key);

        timing_info[action] += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        ++operation_count[action];
    }

    action = "Remove";
    for (std::pair<Heap::NODE_TYPE, int> next : node_to_key)
    {
        start = std::chrono::high_resolution_clock::now();
        heap.Remove(next.first);
        end = std::chrono::high_resolution_clock::now();

        timing_info[action] += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        ++operation_count[action];
    }

    PrintTimingInfo(name, timing_info, operation_count);
}

template<typename Heap>
void TestHeap_Array(const std::string& name, Heap& heap, bool min)
{
    std::unordered_map<std::string, std::chrono::microseconds> timing_info;
    std::unordered_map<std::string, int> operation_count;

    for (const std::string& operation : operations)
    {
        timing_info[operation] = std::chrono::microseconds();
        operation_count[operation] = 0;
    }

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;

    std::string action = "Insert";
    for (unsigned int i = 0; i < 250000; i++)
    {
        int val = rand() % 100;
        start = std::chrono::high_resolution_clock::now();
        heap.Insert(val);
        end = std::chrono::high_resolution_clock::now();

        timing_info[action] += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        ++operation_count[action];
    }

    action = "AugmentKey";
    for (unsigned int i = 0; i < 250000; i++)
    {
        size_t idx = rand() % 249999;
        int delta = rand() % 1000 + 1;

        start = std::chrono::high_resolution_clock::now();
        heap.DeltaKey(idx, min ? -1 * delta : delta);
        end = std::chrono::high_resolution_clock::now();

        timing_info[action] += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        ++operation_count[action];
    }

    action = "ExtractTop";
    for (unsigned int i = 0; i < 125000; i++)
    {
        start = std::chrono::high_resolution_clock::now();
        heap.ExtractTop();
        end = std::chrono::high_resolution_clock::now();

        timing_info[action] += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        ++operation_count[action];
    }

    action = "Remove";
    size_t num_left = 125000;
    for (unsigned int i = 0; i < 125000; i++)
    {
        size_t idx = rand() % num_left;
        --num_left;

        start = std::chrono::high_resolution_clock::now();
        heap.Remove(idx);
        end = std::chrono::high_resolution_clock::now();

        timing_info[action] += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        ++operation_count[action];
    }

    PrintTimingInfo(name, timing_info, operation_count);
}

template<typename Heap>
void TestHeap_StlPriorityQueue(const std::string& name, Heap& heap, bool min)
{
    std::unordered_map<std::string, std::chrono::microseconds> timing_info;
    std::unordered_map<std::string, int> operation_count;

    for (const std::string& operation : operations)
    {
        timing_info[operation] = std::chrono::microseconds();
        operation_count[operation] = 0;
    }

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;

    std::string action = "Insert";
    for (unsigned int i = 0; i < 250000; i++)
    {
        int val = rand() % 100;
        start = std::chrono::high_resolution_clock::now();
        heap.push(val);
        end = std::chrono::high_resolution_clock::now();

        timing_info[action] += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        ++operation_count[action];
    }

    /*
    action = "AugmentKey";
    for (unsigned int i = 0; i < 250000; i++)
    {
        size_t idx = rand() % 249999;
        int delta = rand() % 1000 + 1;

        start = std::chrono::high_resolution_clock::now();
        heap.DeltaKey(idx, min ? -1 * delta : delta);
        end = std::chrono::high_resolution_clock::now();

        timing_info[action] += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        ++operation_count[action];
    }*/

    action = "ExtractTop";
    for (unsigned int i = 0; i < 125000; i++)
    {
        start = std::chrono::high_resolution_clock::now();
        heap.pop();
        end = std::chrono::high_resolution_clock::now();

        timing_info[action] += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        ++operation_count[action];
    }

    /*
    action = "Remove";
    size_t num_left = 125000;
    for (unsigned int i = 0; i < 125000; i++)
    {
        size_t idx = rand() % num_left;
        --num_left;

        start = std::chrono::high_resolution_clock::now();
        heap.Remove(idx);
        end = std::chrono::high_resolution_clock::now();

        timing_info[action] += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        ++operation_count[action];
    }
    */

    PrintTimingInfo(name, timing_info, operation_count);
}

void TestMaxHeap()
{
    MaxHeap<int> heap;
    TestHeap_Array("MaxHeap", heap, false);
}

void TestMinHeap()
{
    MinHeap<int> heap;
    TestHeap_Array("MinHeap", heap, true);
}

void TestMaxStlPriorityQueue()
{
    std::priority_queue<int, std::vector<int>, std::less<int>> heap;
    TestHeap_StlPriorityQueue("MaxStlPriorityQueue", heap, false);
}

void TestMinStlPriorityQueue()
{
    std::priority_queue<int, std::vector<int>, std::greater<int>> heap;
    TestHeap_StlPriorityQueue("MinStlPriorityQueue", heap, true);
}

void TestMaxFibonacciHeap()
{
    MaxFibonacciHeap<int> fib_heap;
    TestHeap_LinkedList("Max Fibonacci Heap with Consecutive Operations", fib_heap, false);

    MaxFibonacciHeap<int> fib_heap2;
    TestHeap("Max Fibonacci Heap with Random Operations", fib_heap2, false);
}

void TestMinFibonacciHeap()
{
    MinFibonacciHeap<int> fib_heap;
    TestHeap_LinkedList("Min Fibonacci Heap with Consecutive Operations", fib_heap, true);

    MinFibonacciHeap<int> fib_heap2;
    TestHeap("Min Fibonacci Heap with Random Operations", fib_heap2, true);
}

void TestMaxPairingHeap()
{
    MaxPairingHeap<int> heap;
    TestHeap_LinkedList("Max Pairing Heap with Consecutive Operations", heap, false);

    MaxPairingHeap<int> heap2;
    TestHeap("Max Pairing Heap with Random Operations", heap2, false);
}

void TestMinPairingHeap()
{
    MinPairingHeap<int> heap;
    TestHeap_LinkedList("Min Pairing Heap with Consecutive Operations", heap, true);

    MinPairingHeap<int> heap2;
    TestHeap("Min Pairing Heap with Random Operations", heap2, true);
}

void GenRandomHeapData(unsigned int num_operations, std::vector<std::string>& output)
{
    std::vector<int> node_keys;
    unsigned int count = 0;
    unsigned int key_count = 0;

    for (unsigned int i = 0; i < num_operations; ++i)
    {
        unsigned int next_op = 0;

        if (count)
        {
            next_op = std::rand() % operations.size();
        }

        std::stringstream ss;
        ss << operations[next_op] << ':';
        switch (next_op)
        {
        // inserting
        case 0:
        {
            int key = key_count;
            ++key_count;
            unsigned int value = std::rand() % 100;
            ss << key << ',' << value;
            node_keys.push_back(key);
            ++count;
            break;
        }
        // Extract Top
        case 1:
            --count;
            break;
        // Removing
        case 2:
        {
            unsigned int index = std::rand() % node_keys.size();
            unsigned int key = node_keys[index];
            std::vector<int>::iterator it = std::find(node_keys.begin(), node_keys.end(), key);
            node_keys.erase(it);
            ss << key;
            --count;
            break;
        }
        // Augmenting
        case 3:
        {
            unsigned int index = std::rand() % node_keys.size();
            unsigned int key = node_keys[index];
            unsigned int value = std::rand() % 100 + 1;
            ss << key << ',' << value;
            break;
        }
        }

        output.push_back(ss.str());
    }
}

int main()
{
    // Sed the rand
    srand(static_cast<int>(time(0)));

    bool gen_file = false;
    if (gen_file)
    {
        std::vector<std::string> save_to_file;
        GenRandomHeapData(1000000, save_to_file);

        // Dump to a file
        std::ofstream out("HeapDriver.in");
        if (!out.is_open())
        {
            std::cout << "Could not create output file\n";
            return -1;
        }

        for (const std::string& next : save_to_file)
        {
            out << next << '\n';
        }

        out.close();

        return 0;
    }
    
    TestMaxHeap();
    TestMinHeap();
    
    TestMinStlPriorityQueue();
    TestMaxStlPriorityQueue();

    TestMaxFibonacciHeap();
    TestMinFibonacciHeap();

    TestMaxPairingHeap();
    TestMinPairingHeap();

    int temp;
    std::cin >> temp;

    return 0;
}