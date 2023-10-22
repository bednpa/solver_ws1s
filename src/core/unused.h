/**
 * 
 * unused.h -- implementation of auxiliary algorithms used in
 *             experiments phase which reject them or algorithms
 *             which can be used in the future
 * 
 * Author: Pavel Bednar <bednarpavel@outlook.cz>
 * 
 * This program was created as a part of diploma thesis
 * on the topic of deciding WS1S at FIT BUT.
 * 
*/

#include "../mata/nfa.hh"
#include "atomaut.h"
#include "auxiliary.h"


using Mata::Nfa::Nfa;
using Mata::Nfa::Symbol;
using Mata::Nfa::State;
using std::vector;
using std::deque;

using std::cout;
using std::endl;


/**
 * Adds final states into skip edge in backward direction (version 2)
 * @param[inout] aut Input and output automaton
 * @param[in] on_zero_path Flag if edge has symbol zero
 * @param[in] symb Symbol on edge
 * @param[in] fs Target state of edge
 * @param[in] level Level of states in input automaton
 * @param[in] num_idents Number of variables in current context
 */
void addFinalStatesRev2(Nfa &aut, const State &as, const bool &on_zero_path, const State &fs, int *level, const unsigned long &num_idents);


/**
 * Removes useless zeros in input automaton (version 2)
 * @param[in] aut Input automaton
 * @param[in] act_ident_count Number of vars in current context
 * @return Automaton without useless zeros
 */
Nfa removeUselessZeros2(const Nfa &aut, unsigned long act_ident_count);


/**
 * Removes useless zeros in input automaton (version 3)
 * @param[in] aut Input automaton
 * @param[in] act_ident_count Number of vars in current context
 * @return Automaton without useless zeros
 */
Nfa removeUselessZeros3(const Nfa &aut, const unsigned long &act_ident_count);


/**
 * Adds necessary inner states after reverse function
 * @param[in] new_aut Input automaton
 * @return Automaton with necessary inner states
*/
Nfa addInnersAfterReverse(const Nfa &new_aut);


/**
 * Revert of input automata
 * @param[in] aut Input automaton
 * @return Reverted automaton
*/
Nfa reverse(const Nfa &aut);


/**
 * Replace every skip edge to edge with length one (common edge)
 * @param[in] aut Input automaton
 * @return Automaton without skip edges
*/
Nfa unpackFromSkipEdges(const Nfa &aut);


/**
 * Computes successors for every state
 * @param[in] aut Input automaton
 * @param[out] succ Successors array
*/
void getSuccCount(const Nfa &aut, unsigned long *succ);


/**
 * Computes predecessors for every state
 * @param[in] aut Input automaton
 * @param[out] pred Predecessors array
*/
void getPredCount(const Nfa &aut, unsigned long *pred);


/**
 * Replace common edges with one skip edge (works also in NFA)
 * @param[in] aut Input automaton
 * @param[in] init_state Starting state of new skip edge
 * @param[in] pred Predecessors array
 * @param[in] succ Successors array
 * @return Target states of new skip edge
*/
vector<State> addSkipEdgeNfa(const Nfa &aut, const State &init_state, const unsigned long *pred, const unsigned long *succ);


/**
 * Replace common edges with skip edges, if possible (works also in NFA)
 * @param[in] aut Input automaton
 * @return Automaton with maximal number of skip edges
*/
Nfa reduceToSkipEdgesNfa(const Nfa &aut);


/**
 * Minimization based on unpacking from skip edges, Brzozowski algorithm and reduction to skip edges
 * @param[in] aut Input automaton
 * @return Minimized automaton
*/
Nfa minimization2(const Nfa &aut);


/**
 * Decreasing number of automata staates based on repeated reduction to skip edges
 * @param[in] aut Input automaton
 * @return Automaton with decreased number of states
*/
Nfa minimization3(const Nfa &aut);


/**
 * Minimization based on unpacking from skip edges, simulation algorithm and reduction to skip edges
 * @param[in] aut Input automaton
 * @return Automaton with decreased number of states
*/
Nfa minimization4(const Nfa &aut);


/**
 * Computes automaton without unreachable and not terminating states
 * @param[in] aut Input automaton
 * @return Automaton without unreachable and not terminating states
*/
Nfa getUsefulStates(const Nfa &aut);


/**
 * Adds states with level zero to input automaton
 * @param[in] aut Input automaton
 * @param[in] aic Number of vars in current context
 * @return Automaton with zero level states
*/
Nfa addZeroLevelStates(const Nfa &aut, unsigned long aic);
