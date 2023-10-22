/**
 * 
 * unused.cpp -- implementation of auxiliary algorithms used in
 *               experiments phase which reject them or algorithms
 *               which can be used in the future
 * 
 * Author: Pavel Bednar <bednarpavel@outlook.cz>
 * 
 * This program was created as a part of diploma thesis
 * on the topic of deciding WS1S at FIT BUT.
 * 
*/

#include "unused.h"


void addFinalStatesRev2(Nfa &aut, const State &as, const bool &on_zero_path, const State &fs, int *level, const unsigned long &num_idents)
{
    int num_states = aut.states_number();
    int num_inner_states = max(0, static_cast<int>(ceil((fs.jump + level[fs] - num_idents) / (double)num_idents)));
    aut.increase_size(num_states + num_inner_states);

    unsigned long act_as;
    unsigned long act_fs;

    if (num_inner_states > 0)
    {
        act_as = num_states++;
        act_fs = fs;
        aut.add_trans(act_as, 0, State{act_fs, num_idents - level[fs]});
        aut.add_trans(act_as, 1, State{act_fs, num_idents - level[fs]});
        aut.make_final(act_as);

        for (int i = 0; i < num_inner_states - 1; i++)
        {
            act_fs = act_as;
            act_as = num_states++;
            aut.add_trans(act_as, 0, State{act_fs, num_idents});
            aut.add_trans(act_as, 1, State{act_fs, num_idents});
            aut.make_final(act_as);
        }

        act_fs = act_as;
        act_as = as;

        if (aut.has_trans(as, 0, fs))
        {
            aut.add_trans(act_as, 0, State{act_fs, fs.jump - ((num_inner_states - 1) * num_idents + (num_idents - level[fs]))});
            aut.remove_trans(as, 0, fs);
        }
        if (aut.has_trans(as, 1, fs))
        {
            aut.add_trans(act_as, 1, State{act_fs, fs.jump - ((num_inner_states - 1) * num_idents + (num_idents - level[fs]))});
            aut.remove_trans(as, 1, fs);
        }
    }

    level[as] = (level[fs] + fs.jump) % num_idents;
    if (level[as] == 0 && on_zero_path && !aut.has_initial(as))
    {
        aut.make_final(as);
    }
}


Nfa removeUselessZeros2(const Nfa &aut, unsigned long act_ident_count)
{
    // structure for automata reverse iterations efectively
    vector<pair<State, State>> zero_paths;
    vector<pair<State, State>> one_paths;


    for (unsigned long as = 0; as < aut.states_number(); as++)
    {
        for (const auto &move: aut.transition_relation[as])
        {
            for (const auto fs: move.states_to)
            {
                if (move.symbol == 0)
                {
                    zero_paths.push_back(pair<State, State>(as, fs));
                }
                if (move.symbol == 1)
                {
                    one_paths.push_back(pair<State, State>(as, fs));
                }
            }
        }
    }

    // removing zeros
    Nfa new_aut = aut;

    int level[aut.states_number()];

    vector<unsigned long> q;
    bool seen[aut.states_number()];
    memset(seen, false, aut.states_number() * sizeof(bool));

    for (auto final_state: aut.final_states)
    {
        q.push_back(final_state);
        level[final_state] = 0;
    }

    while (!q.empty())
    {
        unsigned long fs = q.back();
        q.pop_back();
        if (seen[fs])
        {
            continue;
        }
        else
        {
            seen[fs] = true;
        }

        // delete uselless zero on edge starting with 0
        for (const auto &p: zero_paths)
        {
            if (p.second == fs)
            {
                addFinalStatesRev2(new_aut, p.first, true , p.second, level, act_ident_count);
                q.push_back(p.first);
            }
        }

        // delete uselless zero on edge starting with 1 (because can skip lot of variables)
        for (const auto &p: one_paths)
        {
            if (p.second == fs)
            {
                addFinalStatesRev2(new_aut, p.first, false, p.second, level, act_ident_count);
            }
        }
    }
    return new_aut;
}


Nfa removeUselessZeros3(const Nfa &aut, const unsigned long &act_ident_count)
{
    Nfa new_aut = aut;

    int level[aut.states_number()];
    vector<unsigned long> closed;
    deque<unsigned long> q;

    for (auto final_state: aut.final_states)
    {
        q.push_back(final_state);
        level[final_state] = 0;
    }

    while (!q.empty())
    {
        unsigned long act_state = q.front();
        q.pop_front();
        closed.push_back(act_state);

        for (unsigned long as = 0; as < aut.states_number(); as++)
        {
            for (const auto &move: aut.transition_relation[as])
            {
                for (const auto fs: move.states_to)
                {
                    if (act_state == fs)
                    {
                        // as fs remove zeros
                        addFinalStatesRev(new_aut, as, move.symbol, fs, level, act_ident_count);
                        if (move.symbol == 0 && find(closed.begin(), closed.end(), as) == closed.end() && find(q.begin(), q.end(), as) == q.end())
                        {
                            q.push_back(as);
                        }
                    }
                }
            }
        }
    }
    return new_aut;
}


Nfa addInnersAfterReverse(const Nfa &new_aut)
{
    Nfa res_aut{new_aut.states_number()};
    res_aut.initial_states = new_aut.initial_states;
    res_aut.final_states = new_aut.final_states;

    // add inner states when jump is present
    vector<unsigned long> q;
    bool seen[new_aut.states_number()];
    memset(seen, false, new_aut.states_number() * sizeof(bool));

    for (const auto &init_state: new_aut.initial_states)
    {
        q.push_back(init_state);
    }

    while (!q.empty())
    {
        unsigned long as = q.back();
        q.pop_back();
        if (seen[as])
        {
            continue;
        }
        else
        {
            seen[as] = true;
        }

        vector<pair<Symbol,State>> dest_states;

        for (const auto &move: new_aut.transition_relation[as])
        {
            for (const auto &fs: move.states_to)
            {
                dest_states.push_back(pair(move.symbol, fs));

                q.push_back(fs);
            }
        }

        sort(dest_states.begin(), dest_states.end(), [](const auto &e1, const auto &e2) { return e1.second.jump < e2.second.jump; });
        State act_state = as;
        unsigned long act_jump = 1;
        for (const auto &ds: dest_states)
        {
            if (ds.second.jump > act_jump)
            {
                State inner_state = res_aut.add_state();
                res_aut.add_trans(act_state, 0, State{inner_state, ds.second.jump - act_jump});
                res_aut.add_trans(act_state, 1, State{inner_state, ds.second.jump - act_jump});
                act_state = inner_state;
                act_jump = ds.second.jump;
            }
            res_aut.add_trans(act_state, ds.first, State{ds.second.state, 1});
        }
    }
    return res_aut;
}



Nfa reverse(const Nfa &aut)
{
    Nfa new_aut(aut.states_number());
    new_aut.initial_states = aut.final_states;
    new_aut.final_states = aut.initial_states;

    vector<unsigned long> q;
    bool seen[aut.states_number()];
    memset(seen, false, aut.states_number() * sizeof(bool));

    // reverse automaton
    for (const auto &init_state: aut.initial_states)
    {
        q.push_back(init_state);
    }

    while (!q.empty())
    {
        unsigned long as = q.back();
        q.pop_back();
        if (seen[as])
        {
            continue;
        }
        else
        {
            seen[as] = true;
        }

        for (const auto &move: aut.transition_relation[as])
        {
            for (const auto &fs: move.states_to)
            {
                new_aut.add_trans(fs, move.symbol, State{as, fs.jump});

                q.push_back(fs);
            }
        }
    }

    new_aut = addInnersAfterReverse(new_aut);
    return new_aut;
}


Nfa unpackFromSkipEdges(const Nfa &aut)
{
    Nfa new_aut(aut.states_number());
    new_aut.initial_states = aut.initial_states;
    new_aut.final_states = aut.final_states;

    vector<unsigned long> closed;
    deque<unsigned long> q;

    for (const auto &init_state: aut.initial_states)
    {
        q.push_back(init_state);
    }

    while (!q.empty())
    {
        unsigned long as = q.front();
        q.pop_front();
        closed.push_back(as);

        for (const auto &move: aut.transition_relation[as])
        {
            for (const auto &fs: move.states_to)
            {
                if (fs.jump == 1)
                {
                    new_aut.add_trans(as, move.symbol, State{fs, 1});
                }
                else
                {
                    State fin_state = new_aut.add_state();
                    State act_state = fin_state;
                    new_aut.add_trans(as, move.symbol, fin_state);
                    for (unsigned long j = 0; j < fs.jump - 2; j++)
                    {
                        fin_state = new_aut.add_state();
                        new_aut.add_trans(act_state, 0, fin_state);
                        new_aut.add_trans(act_state, 1, fin_state);
                        act_state = fin_state;
                    }
                    new_aut.add_trans(act_state, 0, State{fs, 1});
                    new_aut.add_trans(act_state, 1, State{fs, 1});
                }

                if (find(closed.begin(), closed.end(), fs) == closed.end() && find(q.begin(), q.end(), fs) == q.end())
                {
                    q.push_back(fs);
                }
            }
        }
    }
    return new_aut;
}


void getSuccCount(const Nfa &aut, unsigned long *succ)
{
    memset(succ, 0, aut.states_number() * sizeof(unsigned long));

    vector<unsigned long> q;
    bool seen[aut.states_number()];
    memset(seen, false, aut.states_number() * sizeof(bool));

    for (const auto &init_state: aut.initial_states)
    {
        q.push_back(init_state);
    }

    while (!q.empty())
    {
        unsigned long as = q.back();
        q.pop_back();
        if (seen[as])
        {
            continue;
        }
        else
        {
            seen[as] = true;
        }

        int old_fs = -1;

        for (const auto &move: aut.transition_relation[as])
        {
            for (const auto &fs: move.states_to)
            {
                if (fs != old_fs)
                {
                    succ[as]++;
                }
                old_fs = fs;

                q.push_back(fs);
            }
        }
    }
}


void getPredCount(const Nfa &aut, unsigned long *pred)
{
    memset(pred, 0, aut.states_number() * sizeof(unsigned long));

    vector<unsigned long> q;
    bool seen[aut.states_number()];
    memset(seen, false, aut.states_number() * sizeof(bool));

    for (const auto &init_state: aut.initial_states)
    {
        q.push_back(init_state);
    }

    while (!q.empty())
    {
        unsigned long as = q.back();
        q.pop_back();
        if (seen[as])
        {
            continue;
        }
        else
        {
            seen[as] = true;
        }

        int old_fs = -1;

        for (const auto &move: aut.transition_relation[as])
        {
            for (const auto &fs: move.states_to)
            {
                if (fs != old_fs)
                {
                    pred[fs]++;
                }
                old_fs = fs;

                q.push_back(fs);
            }
        }
    }
}


vector<State> addSkipEdgeNfa(const Nfa &aut, const State &init_state, const unsigned long *pred, const unsigned long *succ)
{
    vector<State> fs_vec;
    unsigned long skip_len_arr[aut.states_number()];
    bool is_last_arr[aut.states_number()];
    memset(skip_len_arr, 0, aut.states_number() * sizeof(unsigned long));
    memset(is_last_arr, false, aut.states_number() * sizeof(unsigned long));
    skip_len_arr[init_state] = init_state.jump;

    vector<unsigned long> q;
    bool seen[aut.states_number()];
    memset(seen, false, aut.states_number() * sizeof(bool));

    q.push_back(init_state);

    while (!q.empty())
    {
        unsigned long as = q.back();
        q.pop_back();
        if (seen[as])
        {
            continue;
        }
        else
        {
            seen[as] = true;
        }

        is_last_arr[as] = true;

        for (const auto &move: aut.transition_relation[as])
        {
            for (const auto &fs: move.states_to)
            {
                if (aut.has_trans(as, 0, fs) && aut.has_trans(as, 1, fs) && !aut.has_final(as) && pred[as] < 2 && succ[as] < 2)
                {
                    skip_len_arr[fs] = skip_len_arr[as] + fs.jump;
                    is_last_arr[as] = false;

                    q.push_back(fs);
                }
            }
        }
    }

    for (unsigned long i = 0; i < aut.states_number(); i++)
    {
        if (is_last_arr[i])
        {
            fs_vec.push_back(State{i, skip_len_arr[i]});
        }
    }

    return fs_vec;
}


Nfa reduceToSkipEdgesNfa(const Nfa &aut)
{
    Nfa new_aut(aut.states_number());
    new_aut.initial_states = aut.initial_states;
    new_aut.final_states = aut.final_states;

    unsigned long pred[aut.states_number()];
    getPredCount(aut, pred);
    unsigned long succ[aut.states_number()];
    getSuccCount(aut, succ);

    vector<unsigned long> q;
    bool seen[aut.states_number()];
    memset(seen, false, aut.states_number() * sizeof(bool));

    for (const auto &init_state: aut.initial_states)
    {
        q.push_back(init_state);
    }

    while (!q.empty())
    {
        unsigned long as = q.back();
        q.pop_back();
        if (seen[as])
        {
            continue;
        }
        else
        {
            seen[as] = true;
        }

        for (const auto &move: aut.transition_relation[as])
        {
            for (const auto &tmp_fs: move.states_to)
            {
                for (const auto &fs: addSkipEdgeNfa(aut, tmp_fs, pred, succ))
                {
                    new_aut.add_trans(as, move.symbol, fs);
                    q.push_back(fs);
                }
            }
        }
    }

    return new_aut;
}


Nfa minimization4(const Nfa &aut)
{
    Nfa aut1 = unpackFromSkipEdges(aut);
    aut1 = reduce(aut1);
    aut1 = reduceToSkipEdges(aut1);
    return aut1;
}


Nfa minimization3(const Nfa &aut)
{
    Nfa aut1 = aut;

    int cnt = 100;
    while (cnt > 0)
    {
        unsigned long in_states = num_states(aut1);
        aut1 = reduceToSkipEdges(aut1);
        unsigned long out_states = num_states(aut1);
        if (out_states == in_states)
        {
            break;
        }
        cnt--;
    }

    return aut1;
}


Nfa minimization2(const Nfa &aut)
{
    Nfa aut1 = unpackFromSkipEdges(aut);
    aut1 = revert(aut1);
    aut1 = determinize(aut1);
    aut1 = revert(aut1);
    aut1 = determinize(aut1);
    aut1 = reduceToSkipEdges(aut1);

    return aut1;
}


Nfa getUsefulStates(const Nfa &aut)
{
    Nfa new_aut(aut.states_number());
    new_aut.initial_states = aut.final_states;
    new_aut.final_states = aut.initial_states;

    std::vector<unsigned long> closed;
    std::deque<unsigned long> q;

    // reverse automaton
    for (const auto &init_state: aut.initial_states)
    {
        q.push_back(init_state);
    }

    while (!q.empty())
    {
        unsigned long as = q.front();
        q.pop_front();
        closed.push_back(as);

        for (const auto &move: aut.transition_relation[as])
        {
            for (const auto &fs: move.states_to)
            {
                new_aut.add_trans(fs, 0, State{as, 1});

                if (find(closed.begin(), closed.end(), fs) == closed.end() && find(q.begin(), q.end(), fs) == q.end())
                {
                    q.push_back(fs);
                }
            }
        }
    }

    std::vector<bool> isTerminating(new_aut.states_number(), false);
    std::vector<unsigned long> new_closed;
    std::deque<unsigned long> new_q;

    for (const auto &init_state: new_aut.initial_states)
    {
        new_q.push_back(init_state);
    }

    while (!new_q.empty())
    {
        unsigned long as = new_q.front();
        new_q.pop_front();
        new_closed.push_back(as);
        isTerminating[as] = true;

        for (const auto &move: new_aut.transition_relation[as])
        {
            for (const auto &fs: move.states_to)
            {

                if (find(new_closed.begin(), new_closed.end(), fs) == new_closed.end() && find(new_q.begin(), new_q.end(), fs) == new_q.end())
                {
                    new_q.push_back(fs);
                }
            }
        }
    }

    Nfa new_new_aut(aut.states_number());
    new_new_aut.initial_states = aut.initial_states;
    new_new_aut.final_states = aut.final_states;
    std::vector<unsigned long> new_new_closed;
    std::deque<unsigned long> new_new_q;

    for (const auto &init_state: new_new_aut.initial_states)
    {
        new_new_q.push_back(init_state);
    }

    while (!new_new_q.empty())
    {
        unsigned long as = new_new_q.front();
        new_new_q.pop_front();
        new_new_closed.push_back(as);

        for (const auto &move: aut.transition_relation[as])
        {
            for (const auto &fs: move.states_to)
            {
                if (isTerminating[fs])
                {
                    new_new_aut.add_trans(as, move.symbol, fs);

                    if (find(new_new_closed.begin(), new_new_closed.end(), fs) == new_new_closed.end() && find(new_new_q.begin(), new_new_q.end(), fs) == new_new_q.end())
                    {
                        new_new_q.push_back(fs);
                    }
                }
            }
        }
    }

    return new_new_aut;
}


Nfa addZeroLevelStates(const Nfa &aut, unsigned long aic)
{
    Nfa new_aut(aut.states_number());
    new_aut.initial_states = aut.initial_states;
    new_aut.final_states = aut.final_states;

    vector<unsigned long> q;
    bool seen[aut.states_number()];
    memset(seen, false, aut.states_number() * sizeof(bool));
    unsigned long level[aut.states_number()];
    vector<pair<State,State>> end_inner_states;

    for (const auto &init_state: aut.initial_states)
    {
        q.push_back(init_state);
        level[init_state] = 0;
    }

    while (!q.empty())
    {
        unsigned long as = q.back();
        q.pop_back();
        if (seen[as])
        {
            continue;
        }
        else
        {
            seen[as] = true;
        }

        for (const auto &move: aut.transition_relation[as])
        {
            for (const auto &fs: move.states_to)
            {
                if (level[as] != 0 && level[as] + fs.jump > aic)
                {
                    unsigned long edge_len = fs.jump - (aic - level[as]);
                    auto it = std::find_if(end_inner_states.begin(), end_inner_states.end(), [&fs,&edge_len](const auto &pair) { return pair.first.state == fs.state && pair.first.jump - pair.second.jump == edge_len; });

                    State inner_state;
                    if (it != end_inner_states.end()) {
                        inner_state.state = it->second.state;
                        inner_state.jump = aic - level[as];
                    } else {
                        inner_state.state = new_aut.add_state();
                        inner_state.jump = aic - level[as];
                        end_inner_states.push_back(pair(fs, inner_state));
                        new_aut.add_trans(inner_state, 0, State{fs, fs.jump - (aic - level[as])});
                        new_aut.add_trans(inner_state, 1, State{fs, fs.jump - (aic - level[as])});
                    }
                    new_aut.add_trans(as, move.symbol, State{inner_state, aic - level[as]});

                    /*State inner_state = new_aut.add_state();
                    new_aut.add_trans(as, move.symbol, State{inner_state, aic - level[as]});
                    new_aut.add_trans(inner_state, 0, State{fs, fs.jump - (aic - level[as])});
                    new_aut.add_trans(inner_state, 1, State{fs, fs.jump - (aic - level[as])});*/
                }
                else
                {
                    new_aut.add_trans(as, move.symbol, fs);
                }
                level[fs] = (level[as] + fs.jump) % aic;

                q.push_back(fs);
            }
        }
    }

    return new_aut;
}