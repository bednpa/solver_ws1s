/**
 * 
 * atomaut.h -- creation of atomic automata
 * 
 * Author: Pavel Bednar <bednarpavel@outlook.cz>
 * 
 * This program was created as a part of diploma thesis
 * on the topic of deciding WS1S at FIT BUT.
 * 
*/

#include "../mata/nfa.hh"
#include "../ast/ident.h"
#include <queue>


using Mata::Nfa::Nfa;
using Mata::Nfa::State;


/**
 * Function for creating one var automaton 
 * @param[in] v1 The var
 * @param[in] trans Input var transitions in output automaton
 * @param[in] num_states Number of states in output automaton
 * @param[in] num_idents Number of variables in formula
 * @param[in] boolean_prefix Decides whether threats first state as bool vars assignment
*/
Nfa createAut1(const int &v1, const int *trans, const int &num_states, const unsigned long &num_idents, const bool &boolean_prefix = true);


/**
 * Function for creating two var automaton 
 * @param[in] v1 First var
 * @param[in] v2 Second var
 * @param[in] trans Input var transitions in output automaton
 * @param[in] num_states Number of states in output automaton
 * @param[in] num_idents Number of variables in formula
 * @param[in] boolean_prefix Decides whether threats first state as bool vars assignment
*/
Nfa createAut2(const int &v1, const int &v2, const int *trans, const int &num_states, const unsigned long &num_idents, const bool &boolean_prefix = true);


/**
 * Function for creating three var automaton 
 * @param[in] v1 First var
 * @param[in] v2 Second var
 * @param[in] v3 Third var
 * @param[in] trans Input var transitions in output automaton
 * @param[in] num_states Number of states in output automaton
 * @param[in] num_idents Number of variables in formula
 * @param[in] boolean_prefix Decides whether threats first state as bool vars assignment
*/
Nfa createAut3(const int &v1, const int &v2, const int &v3, const int *trans, const int &num_states, const unsigned long &num_idents, const bool &boolean_prefix = true);