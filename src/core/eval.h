/**
 * 
 * eval.h -- time and states statistics
 * 
 * Author: Pavel Bednar <bednarpavel@outlook.cz>
 * 
 * This program was created as a part of diploma thesis
 * on the topic of deciding WS1S at FIT BUT.
 * 
*/

#include <chrono>
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;
using std::min_element;
using std::max_element;


#define STATS_LIMIT 1000000
#define OP_TIMES_SIZE 5


enum op_times {
    project_time,
    inter_time,
    minimization_time,
    complement_time,
    determinization_time
};


/**
 * Class for time and states statistics within automata construction
*/
class timeStats
{
    // time
    double times_arr[OP_TIMES_SIZE][STATS_LIMIT];
    int count_arr[OP_TIMES_SIZE];
    std::chrono::high_resolution_clock::time_point local_time_arr[STATS_LIMIT];
    int local_time_arr_last;

    // states
    int states_arr[OP_TIMES_SIZE][STATS_LIMIT];
    int local_states_arr[STATS_LIMIT];
public:
    timeStats() : times_arr{}, count_arr{}, local_time_arr{}, local_time_arr_last{}, states_arr{}, local_states_arr{} {}
    int start(int states_number);
    void end(int id, int kind, int states_number);
    void times_stats(int kind);
    void states_stats(int kind);
    void printStats();
    double getMinTime();
};
