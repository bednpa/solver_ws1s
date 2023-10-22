/**
 * 
 * eval.cpp -- implementation of time and states statistics
 * 
 * Author: Pavel Bednar <bednarpavel@outlook.cz>
 * 
 * This program was created as a part of diploma thesis
 * on the topic of deciding WS1S at FIT BUT.
 * 
*/

#include "eval.h"


int timeStats::start(int states_number)
{
    if (local_time_arr_last < STATS_LIMIT)
    {
        local_time_arr[local_time_arr_last] = std::chrono::high_resolution_clock::now();
        local_states_arr[local_time_arr_last] = states_number;
        return local_time_arr_last++;
    }
    else
    {
        return 0;
    }
}


void timeStats::end(int id, int kind, int states_number)
{
    if (local_time_arr_last < STATS_LIMIT)
    {
        times_arr[kind][count_arr[kind]++] = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - local_time_arr[id]).count() / (double)1000000);
        states_arr[kind][count_arr[kind]++] = states_number - local_states_arr[id];
    }
}


double timeStats::getMinTime()
{
    if (count_arr[minimization_time] != 0)
    {
        double sum_v = 0;
        for (int i = 0; i < count_arr[minimization_time]; i++)
            sum_v += times_arr[minimization_time][i];
        return sum_v;
    }
    else
    {
        return 0;
    }
}


void timeStats::times_stats(int kind)
{
    if (count_arr[kind] != 0)
    {
        double max_v = *max_element(times_arr[kind] + 0, times_arr[kind] + count_arr[kind]);
        double min_v = *min_element(times_arr[kind] + 0, times_arr[kind] + count_arr[kind]);
        double sum_v = 0;
        for (int i = 0; i < count_arr[kind]; i++)
            sum_v += times_arr[kind][i];
        double avg_v = sum_v / count_arr[kind];

        if (kind == project_time)
        {
            cout << "     projection |";
        }
        else if (kind == inter_time)
        {
            cout << "   intersection |";
        }
        else if (kind == minimization_time)
        {
            cout << "   minimization |";
        }
        else if (kind == complement_time)
        {
            cout << "     complement |";
        }
        else if (kind == determinization_time)
        {
            cout << "determinization |";
        }
        cout << " min: " << min_v << " max: " << max_v << " avg: " << avg_v << " sum: " << sum_v << " cnt: " << count_arr[kind] << endl;
    }
}


void timeStats::states_stats(int kind)
{
    if (count_arr[kind] != 0)
    {
        int max_v = *max_element(states_arr[kind] + 0, states_arr[kind] + count_arr[kind]);
        int min_v = *min_element(states_arr[kind] + 0, states_arr[kind] + count_arr[kind]);
        int sum_v = 0;
        for (int i = 0; i < count_arr[kind]; i++)
            sum_v += states_arr[kind][i];
        double avg_v = (double)sum_v / (double)count_arr[kind];

        if (kind == project_time)
        {
            cout << "     projection |";
        }
        else if (kind == inter_time)
        {
            cout << "   intersection |";
        }
        else if (kind == minimization_time)
        {
            cout << "   minimization |";
        }
        else if (kind == complement_time)
        {
            cout << "     complement |";
        }
        else if (kind == determinization_time)
        {
            cout << "determinization |";
        }
        cout << " min: " << min_v << " max: " << max_v << " avg: " << avg_v << " sum: " << sum_v << " cnt: " << count_arr[kind] << endl;
    }
}


void timeStats::printStats()
{
    if (local_time_arr_last < STATS_LIMIT)
    {
        cout << "TIME STATS\n";
        times_stats(complement_time);
        times_stats(project_time);
        times_stats(inter_time);
        times_stats(minimization_time);
        times_stats(determinization_time);
        cout << "STATE STATS\n";
        states_stats(complement_time);
        states_stats(project_time);
        states_stats(inter_time);
        states_stats(minimization_time);
        states_stats(determinization_time);
    }
    else
    {
        cout << "Stats limit was reached, please increase value of STATS_LIMIT\n";
    }
}
