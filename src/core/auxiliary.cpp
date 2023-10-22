/**
 * 
 * auxiliary.cpp -- implementation of auxiliary functions
 * 
 * Author: Pavel Bednar <bednarpavel@outlook.cz>
 * 
 * This program was created as a part of diploma thesis
 * on the topic of deciding WS1S at FIT BUT.
 * 
*/

#include "../core/auxiliary.h"
#include "../ast/symboltable.h"
#include "../core/atomaut.h"
#include "../core/eval.h"


extern timeStats ts;

extern bool print_stats;


unsigned long num_states(const Nfa &aut)
{
    unsigned long cnt = 0;
    for (unsigned long as = 0; as < aut.states_number(); as++)
    {
        if(!aut.transition_relation[as].empty())
        {
            cnt++;
        }
    }
    return cnt;
}


Nfa codeTrue(const unsigned long &act_ident_count)
{
    if (act_ident_count > 0)
    {
        Nfa aut(2);
        aut.add_trans(0, 0, State{1, act_ident_count});
        aut.add_trans(0, 1, State{1, act_ident_count});
        aut.add_trans(1, 0, State{1, act_ident_count});
        aut.add_trans(1, 1, State{1, act_ident_count});
        aut.make_initial(0);
        aut.make_final(1);
        aut = minimization(aut);
        return aut;
    }
    else
    {
        Nfa aut(1);
        aut.make_initial(0);
        aut.make_final(0);
        return aut;
    }
}


Nfa codeFalse()
{
    Nfa aut(1);
    aut.make_initial(0);
    return aut;
}


Nfa codeVar0(const Ident &v1, const unsigned long &act_ident_count)
{
    const unsigned long num_states = 2;
    const int trans[] = {
    //    0  1
    /*0*/ -1, 1,
    /*1*/ 1, 1
    };
    Nfa aut = createAut1(v1, trans, num_states, act_ident_count, false);
    aut.make_initial(0);
    aut.make_final(1);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeEmptySet(const Ident &v1, const unsigned long &act_ident_count)
{
    const unsigned long num_states = 2;
    const int trans[] = {
    //    0  1
    /*0*/ 1, 1,
    /*1*/ 1, -1
    };
    Nfa aut = createAut1(v1, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(1);
    aut = determinization(aut); // TODO asi to musi byt determinizovano, protoze predem nevim jestli vznikne NFA nebo DFA
    aut = minimization(aut);
    return aut;
}


Nfa codeAsignInt(const Ident &v1, const int &n, const unsigned long &act_ident_count)
{
    unsigned long num_states = 3 + n;
    /*    0  1
    *0* 1, 1,
    ---------
    *i* i+1, -1, -1, -1,
    --------------------
    *n+1* -1, n+2,
    *n+2* n+2, -1 */
    int trans[2 * num_states];
    trans[0] = 1; trans[1] = 1;
    for (int i = 1; i <= n; i++)
    {
        trans[2*i] = i + 1; trans[2*i+1] = -1;
    }
    trans[2*(n+1)] = -1; trans[2*(n+1)+1] = n + 2;
    trans[2*(n+2)] = n + 2; trans[2*(n+2)+1] = -1;
    Nfa aut = createAut1(v1, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(n + 2);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeSet(const Ident &v1, const ASTList *elements, const unsigned long &act_ident_count)
{
    int len = elements->size();
    int values[len];
    int i = 0;
    for (ASTList::iterator it = elements->begin(); it != elements->end(); it++)
    {
        values[i] = ((ASTTerm1_Int*)(*it))->value();
        i++;
    }
    sort(values, values + len);
    int max_n = values[len - 1];

    unsigned long num_states = 3 + max_n;
    /*    0  1
    *0* 1, 1,
    ---------
    *i* i+1, -1, -1, -1,
    --------------------
    *n+1* -1, n+2,
    *n+2* n+2, -1 */
    int trans[2 * num_states];
    trans[0] = 1; trans[1] = 1;
    int last_s = 1;
    for (auto n: values)
    {
        for (int i = last_s; i <= n; i++)
        {
            trans[2*i] = i + 1; trans[2*i+1] = -1;
        }
        trans[2*(n+1)] = -1; trans[2*(n+1)+1] = n + 2;
        last_s = n + 2;
    }
    trans[2*(max_n+2)] = max_n + 2; trans[2*(max_n+2)+1] = -1;
    Nfa aut = createAut1(v1, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(max_n + 2);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codePresbConst(const Ident &v1, const int &n, const unsigned long &act_ident_count)
{
    int res = n;
    unsigned long bit_size_n = 0;
    while (res != 0)
    {
        res /= 2;
        bit_size_n++;
    }

    unsigned long num_states = 2 + bit_size_n;
    /*    0  1
    *0* 1, 1,
    ---------
    *i* i+1, -1, -1, -1,
    --------------------
    *n+1* -1, n+2,
    *n+2* n+2, -1 */
    int trans[2 * num_states];
    trans[0] = 1; trans[1] = 1;

    int act_res = n;
    for(int i = 1; act_res != 0; i++)
    {
        if (act_res % 2 == 0)
        {
            trans[2*i] = i + 1; trans[2*i+1] = -1;
        }
        else
        {
            trans[2*i] = -1; trans[2*i+1] = i + 1;
        }
        act_res /= 2;
    }
    trans[2*(num_states-1)] = num_states - 1; trans[2*(num_states-1)+1] = -1;
    Nfa aut = createAut1(v1, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(num_states - 1);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeSingleton(const Ident &v1, const unsigned long &act_ident_count)
{
    const unsigned long num_states = 3;
    const int trans[] = {
    //    0  1
    /*0*/ 1, 1,
    /*1*/ 1, 2,
    /*2*/ 2, -1
    };
    Nfa aut = createAut1(v1, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(2);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeIn(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count)
{
    const unsigned long num_states = 3;
    const int trans[] = {
    //    00 01 10 11
    /*0*/ 1, 1, 1, 1,
    /*1*/ 1, 1, -1, 2,
    /*2*/ 2, 2, -1, -1
    };
    Nfa aut = createAut2(v1, v2, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(2);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeNotIn(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count)
{
    const unsigned long num_states = 3;
    const int trans[] = {
    //    00 01 10 11
    /*0*/ 1, 1, 1, 1,
    /*1*/ 1, 1, 2, -1,
    /*2*/ 2, 2, -1, -1
    };
    Nfa aut = createAut2(v1, v2, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(2);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeSub(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count)
{
    if (v1 == v2)
    {
        return codeTrue(act_ident_count);
    }
    const unsigned long num_states = 2;
    const int trans[] = {
    //    00 01 10 11
    /*0*/ 1, 1, 1, 1,
    /*1*/ 1, 1, -1, 1
    };
    Nfa aut = createAut2(v1, v2, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(1);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeLess(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count)
{
    if (v1 == v2)
    {
        return codeFalse();
    }
    const unsigned long num_states = 4;
    const int trans[] = {
    //    00 01 10 11
    /*0*/ 1, 1, 1, 1,
    /*1*/ 1, -1, 2, -1,
    /*2*/ 2, 3, -1, -1,
    /*3*/ 3, -1, -1, -1
    };
    Nfa aut = createAut2(v1, v2, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(3);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeLessEq(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count)
{
    if (v1 == v2)
    {
        return codeTrue(act_ident_count);
    }
    const unsigned long num_states = 4;
    const int trans[] = {
    //    00 01 10 11
    /*0*/ 1, 1, 1, 1,
    /*1*/ 1, -1, 2, 3,
    /*2*/ 2, 3, -1, -1,
    /*3*/ 3, -1, -1, -1
    };
    Nfa aut = createAut2(v1, v2, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(3);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeUnion(const Ident &v1, const Ident &v2, const Ident &v3, const unsigned long &act_ident_count)
{
    if (v1 == v2)
    {
        return codeEqual2(v1, v2, act_ident_count);
    }
    const unsigned long num_states = 2;
    const int trans[] = {
    //    000 001 010 011 100 101 110 111
    /*0*/ 1,  1,  1,  1,  1,  1,  1,  1,
    /*1*/ 1, -1, -1, -1, -1,  1,  1,  1
    };
    Nfa aut = createAut3(v1, v2, v3, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(1);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeInter(const Ident &v1, const Ident &v2, const Ident &v3, const unsigned long &act_ident_count)
{
    if (v1 == v2)
    {
        return codeEqual2(v1, v2, act_ident_count);
    }
    const unsigned long num_states = 2;
    const int trans[] = {
    //    000 001 010 011 100 101 110 111
    /*0*/ 1,  1,  1,  1,  1,  1,  1,  1,
    /*1*/ 1,  1,  1, -1, -1, -1, -1,  1
    };
    Nfa aut = createAut3(v1, v2, v3, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(1);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeSetMinus(const Ident &v1, const Ident &v2, const Ident &v3, const unsigned long &act_ident_count)
{
    if (v1 == v2)
    {
        return codeEmptySet(v1, act_ident_count);
    }
    const unsigned long num_states = 2;
    const int trans[] = {
    //    000 001 010 011 100 101 110 111
    /*0*/ 1,  1,  1,  1,  1,  1,  1,  1,
    /*1*/ 1,  1, -1,  1, -1, -1,  1, -1
    };
    Nfa aut = createAut3(v1, v2, v3, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(1);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeEqual1(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count)
{
    if (v1 == v2)
    {
        return codeTrue(act_ident_count);
    }
    const unsigned long num_states = 3;
    const int trans[] = {
    //    00 01 10 11
    /*0*/ 1, 1, 1, 1,
    /*1*/ 1, -1, -1, 2,
    /*2*/ 2, -1, -1, -1
    };
    Nfa aut = createAut2(v1, v2, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(2);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeNotEqual1(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count)
{
    if (v1 == v2)
    {
        return codeFalse();
    }
    const unsigned long num_states = 5;
    const int trans[] = {
    //    00 01 10 11
    /*0*/ 1, 1, 1, 1,
    /*1*/ 1, 2, 3, -1,
    /*2*/ 2, -1, 4, -1,
    /*3*/ 3, 4, -1, -1,
    /*4*/ 4, -1, -1, -1
    };
    Nfa aut = createAut2(v1, v2, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(4);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeEqual2(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count)
{
    if (v1 == v2)
    {
        return codeTrue(act_ident_count);
    }
    const unsigned long num_states = 2;
    const int trans[] = {
    //    00 01 10 11
    /*0*/ 1, 1, 1, 1,
    /*1*/ 1, -1, -1, 1
    };
    Nfa aut = createAut2(v1, v2, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(1);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeNotEqual2(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count)
{
    if (v1 == v2)
    {
        return codeFalse();
    }
    const unsigned long num_states = 3;
    const int trans[] = {
    //    00 01 10 11
    /*0*/ 1, 1, 1, 1,
    /*1*/ 1, 2, 2, 1,
    /*2*/ 2, 2, 2, 2
    };
    Nfa aut = createAut2(v1, v2, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(2);
    aut = determinization(aut);
    aut = minimization(aut);
    return aut;
}


Nfa codeSucc(const Ident &v1, const Ident &v2, const int &addition, const unsigned long &act_ident_count)
{
    if (v1 == v2 && addition == 0)
    {
        return codeTrue(act_ident_count);
    }
    if (v1 == v2 && addition != 0)
    {
        return codeFalse();
    }
    unsigned long num_states = 3 + addition;
    /*    00 01 10 11
    *0* 1, 1, 1, 1,
    *1* 1, 2, -1, -1,
    -----------------
    *i  * i, -1, -1, -1,
    *i+1* i + 1, -1, -1, -1
    ----------------------------
    *n  * -1, -1, 2 + addition, -1
    *n+1* 2 + addition, -1, -1, -1 */
    int trans[4 * num_states];
    trans[0] = 1; trans[1] = 1; trans[2] = 1; trans[3] = 1;
    trans[4] = 1; trans[5] = 2; trans[6] = -1; trans[7] = -1;
    for (int i = 2; i < 1 + addition; i++)
    {
        trans[4*i] = i + 1; trans[4*i+1] = -1; trans[4*i+2] = -1; trans[4*i+3] = -1;
    }
    trans[4*(addition+1)] = -1; trans[4*(addition+1)+1] = -1; trans[4*(addition+1)+2] = addition+2; trans[4*(addition+1)+3] = -1;
    trans[4*(addition+2)] = addition+2; trans[4*(addition+2)+1] = -1; trans[4*(addition+2)+2] = -1; trans[4*(addition+2)+3] = -1;
    Nfa aut = createAut2(v1, v2, trans, num_states, act_ident_count);
    aut.make_initial(0);
    aut.make_final(addition+2);
    aut = determinization(aut);
    return aut;
}


void applySingletonRestrictions(Nfa &aut, const IdentList &vars, const unsigned long &act_ident_count)
{
    for (const auto &s: vars)
    {
        Nfa s_aut = codeSingleton(s, act_ident_count);
        aut = inter(aut, s_aut);
    }
}


void addFinalStates(Nfa &aut, const State &as, const Symbol &symb, const State &fs, int *level, const unsigned long &num_idents)
{
    int num_states = aut.states_number();
    int num_inner_states = max(0, static_cast<int>(ceil((fs.jump + level[as] - num_idents) / (double)num_idents)));
    aut.increase_size(num_states + num_inner_states);

    unsigned long act_as;
    unsigned long act_fs;

    if (num_inner_states == 0)
    {
        aut.add_trans(as, symb, fs);
    }
    else
    {
        act_as = as;
        act_fs = num_states++;
        aut.add_trans(act_as, symb, State{act_fs, num_idents - level[as]});

        for (int i = 0; i < num_inner_states - 1; i++)
        {
            act_as = act_fs;
            act_fs = num_states++;
            aut.add_trans(act_as, 0, State{act_fs, num_idents});
            aut.add_trans(act_as, 1, State{act_fs, num_idents});
            aut.make_final(act_as);
        }

        act_as = act_fs;
        act_fs = fs;
        aut.add_trans(act_as, 0, State{act_fs, fs.jump - ((num_inner_states - 1) * num_idents + (num_idents - level[as]))});
        aut.add_trans(act_as, 1, State{act_fs, fs.jump - ((num_inner_states - 1) * num_idents + (num_idents - level[as]))});
        aut.make_final(act_as);
    }

    level[fs] = (level[as] + fs.jump) % num_idents;
    if (level[fs] == 0)
    {
        aut.make_final(fs);
    }
}



void addFinalStatesRev(Nfa &aut, const State &as, const Symbol &symb, const State &fs, int *level, const unsigned long &num_idents)
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

        aut.add_trans(act_as, symb, State{act_fs, fs.jump - ((num_inner_states - 1) * num_idents + (num_idents - level[fs]))});
        aut.remove_trans(as, symb, fs);
    }

    level[as] = (level[fs] + fs.jump) % num_idents;
    if (level[as] == 0 && symb == 0 && !aut.has_initial(as))
    {
        aut.make_final(as);
    }
}


void redirectEdges(Nfa &new_aut, const vector<pair<State,State>> &egdes_to_be_removed)
{
    // every edge going to e.first redirects to e.second
    for (int act_as = 0; act_as < new_aut.states_number(); act_as++)
    {
        for (const auto &move: new_aut.transition_relation[act_as])
        {
            for (const auto act_fs: move.states_to)
            {
                for (const auto &e: egdes_to_be_removed)
                {
                    if (e.first == act_fs)
                    {
                        new_aut.add_trans(act_as, move.symbol, State{e.second, act_fs.jump});
                    }
                }
            }
        }
    }
}


Nfa removeUselessZeros(const Nfa &aut, const unsigned long &act_ident_count)
{
    // structure for automata reverse iterations efectively
    vector<pair<Symbol,State>> preds[aut.states_number()]; 

    for (unsigned long as = 0; as < aut.states_number(); as++)
    {
        for (const auto &move: aut.transition_relation[as])
        {
            for (const auto fs: move.states_to)
            {
                preds[fs].push_back(pair(move.symbol, State{as, fs.jump}));
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

        for (const auto &p: preds[fs])
        {
            addFinalStatesRev(new_aut, p.second, p.first, State{fs, p.second.jump}, level, act_ident_count);
            if (p.first == 0)
            {
                q.push_back(p.second);
            }
        }
    }
    return new_aut;
}


unsigned long computeEdgeLen(unsigned long as_lvl, unsigned long edge_len, unsigned long x, unsigned long aic)
{
    unsigned long cnt = 0;
    for (unsigned long i = as_lvl; i < as_lvl + edge_len; i++)
    {
        if (i % aic == x)
        {
            cnt++;
        }
    }
    return edge_len - cnt;
}


Nfa project(const Nfa &aut, const Ident &x, const unsigned long &act_ident_count)
{
    int tid1;
    if (print_stats)
    {
        tid1 = ts.start(num_states(aut));
    }

    // trivial cases
    if (act_ident_count == 1 && !is_lang_empty(aut))
    {
        return codeTrue(0);
    }
    if (act_ident_count == 1 && is_lang_empty(aut))
    {
        return codeFalse();
    }

    Nfa t_aut = aut;

    // project
    Nfa new_aut(t_aut.states_number());
    new_aut.initial_states = t_aut.initial_states;
    new_aut.final_states = t_aut.final_states;

    int level[t_aut.states_number()];
    vector<pair<State,State>> egdes_to_be_removed;

    vector<unsigned long> q;
    bool seen[t_aut.states_number()];
    memset(seen, false, t_aut.states_number() * sizeof(bool));

    for (auto init_state: t_aut.initial_states)
    {
        level[init_state] = 0;
        q.push_back(init_state);
    }

    // main loop
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

        for (const auto &move: t_aut.transition_relation[as])
        {
            for (const auto &fs: move.states_to)
            {
                level[fs] = (fs.jump + level[as]) % act_ident_count;
                // project var is at the start of this edge which is skip edge
                if (level[as] == x && fs.jump > 1)
                {
                    new_aut.add_trans(as, 0, State{fs, computeEdgeLen(level[as], fs.jump, x, act_ident_count)}); // TODO jump_cut pres for je jistejsi
                    new_aut.add_trans(as, 1, State{fs, computeEdgeLen(level[as], fs.jump, x, act_ident_count)});
                }
                // project var is at the start of this edge which is regular edge (skips nothing)
                else if (level[as] == x && fs.jump == 1)
                {
                    egdes_to_be_removed.push_back(pair(as, fs));
                }
                // project var is not at the start of this edge
                else
                {
                    new_aut.add_trans(as, move.symbol, State{fs, computeEdgeLen(level[as], fs.jump, x, act_ident_count)});
                }

                q.push_back(fs);
            }
        }
    }

    // redirect edges (can not be removed earlier because of first case in main automata loop)
    redirectEdges(new_aut, egdes_to_be_removed);

    // determinization
    new_aut = determinization(new_aut); // one init state after

    // removing useless zeros
    new_aut = removeUselessZeros(new_aut, act_ident_count - 1);

    if (print_stats)
    {
        ts.end(tid1, project_time, num_states(new_aut));
    }

    new_aut = minimization(new_aut);

    return new_aut;
}


Nfa complement(const Nfa &aut, const unsigned long &act_ident_count)
{
    int tid1;
    if (print_stats)
    {
        tid1 = ts.start(num_states(aut));
    }

    // trivial cases
    if (act_ident_count == 0 && !is_lang_empty(aut))
    {
        return codeFalse();
    }
    if (is_lang_empty(aut))
    {
        return codeTrue(act_ident_count);
    }

    // 1 init state needed (for completement)
    Nfa d_aut = determinization(aut);

    // complete and flip states
    Nfa new_aut(d_aut.states_number());
    State sink = new_aut.add_state();
    new_aut.add_trans(sink, 0, State{sink, act_ident_count});
    new_aut.add_trans(sink, 1, State{sink, act_ident_count});
    new_aut.make_final(sink);

    int level[d_aut.states_number()];

    vector<unsigned long> q;
    bool seen[d_aut.states_number()];
    memset(seen, false, d_aut.states_number() * sizeof(bool));

    for (const auto &init_state: d_aut.initial_states)
    {
        level[init_state] = 0;
        new_aut.make_initial(init_state);
        q.push_back(init_state);
    }

    // main loop
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

        bool zero_trans = false;
        bool one_trans = false;
        for (const auto &move: d_aut.transition_relation[as])
        {
            for (const auto &fs: move.states_to)
            {
                addFinalStates(new_aut, as, move.symbol, fs, level, act_ident_count);
                if (move.symbol == 0)
                {
                    zero_trans = true;
                }
                if (move.symbol == 1)
                {
                    one_trans = true;
                }

                q.push_back(fs);
            }
        }
        if (zero_trans == false)
        {
            new_aut.add_trans(as, 0, State{sink, act_ident_count - level[as]});
        }
        if (one_trans == false)
        {
            new_aut.add_trans(as, 1, State{sink, act_ident_count - level[as]});
        }
    }

    for (const auto &fs: d_aut.final_states)
    {
        new_aut.remove_final(fs);
    }

    if (print_stats)
    {
        ts.end(tid1, complement_time, num_states(new_aut));
    }

    return new_aut;
}


State addSkipEdge(const Nfa &aut, const State &pred_init_state, const State &init_state)
{
    bool seen[aut.states_number()];
    memset(seen, false, aut.states_number() * sizeof(bool));
    seen[pred_init_state] = true;

    unsigned long skip_len = init_state.jump;
    unsigned long as = init_state;
    while (true)
    {
        if (aut.transition_relation[as].empty() || aut.has_final(as) || seen[as])
        {
            return State{as, skip_len};
        }

        seen[as] = true;
        State fs = *aut.transition_relation[as].begin()->states_to.begin();
        if (aut.has_trans(as, 0, fs) && aut.has_trans(as, 1, fs))
        {
            as = fs;
            skip_len += fs.jump;
        }
        else
        {
            return State{as, skip_len};
        }
    }
}


Nfa reduceToSkipEdges(const Nfa &aut)
{
    Nfa new_aut(aut.states_number());
    new_aut.initial_states = aut.initial_states;
    new_aut.final_states = aut.final_states;

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
                State fs = addSkipEdge(aut, as, tmp_fs);
                new_aut.add_trans(as, move.symbol, fs);

                q.push_back(fs);
            }
        }
    }

    return new_aut;
}



Nfa unionization(const Nfa &aut1, const Nfa &aut2)
{
    Nfa aut = uni(aut1, aut2);
    aut = minimization(aut);
    return aut;
}


Nfa minimization(const Nfa &aut)
{
    int tid1;
    if (print_stats)
    {
        tid1 = ts.start(num_states(aut));
    }

    Nfa aut1 = aut;
    aut1 = revert(aut1);
    aut1 = determinize(aut1);
    aut1 = revert(aut1);
    aut1 = determinize(aut1);
    aut1 = reduceToSkipEdges(aut1);

    if (print_stats)
    {
        ts.end(tid1, minimization_time, num_states(aut1));
    }

    return aut1;
}


Nfa inter(const Nfa &aut1, const Nfa &aut2)
{
    int tid1;
    if (print_stats)
    {
        tid1 = ts.start(num_states(aut1) + num_states(aut2));
    }

    Nfa aut = intersection(aut1, aut2);

    if (print_stats)
    {
        ts.end(tid1, inter_time, num_states(aut));
    }

    aut = minimization(aut);

    return aut;
}


Nfa determinization(const Nfa &aut)
{
    int tid1;
    if (print_stats)
    {
        tid1 = ts.start(num_states(aut));
    }

    Nfa aut1 = determinize(aut);

    if (print_stats)
    {
        ts.end(tid1, determinization_time, num_states(aut1));
    }

    return aut1;
}




