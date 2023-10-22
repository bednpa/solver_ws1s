/**
 * 
 * auxiliary.h -- auxiliary functions
 * 
 * Author: Pavel Bednar <bednarpavel@outlook.cz>
 * 
 * This program was created as a part of diploma thesis
 * on the topic of deciding WS1S at FIT BUT.
 * 
*/

#include "../mata/nfa.hh"
#include "../ast/ident.h"
#include "../ast/ast.h"
#include <vector>
#include <cmath> 
#include <exception>
#include <string>


using Mata::Nfa::Nfa;
using Mata::Nfa::Symbol;
using std::vector;
using std::deque;
using std::pair;

using std::cout;
using std::endl;
using std::sort;
using std::max;
using std::string;
using std::exception;


///////// Helper functions /////////


/**
 * Computes number of states in input automaton (ignores states with no transitions)
 * @param[in] aut Input automaton
 * @return Number of states in input automaton
*/
unsigned long num_states(const Nfa &aut);


///////// Custom exceptions /////////


/**
 * Exception class for unimplemented terms (e.g. terms from WS2S)
*/
class unimplExc : public exception {
private:
    string message;

public:
    unimplExc(const string& msg) : message(msg) {}

    const char* what() const noexcept {
        return message.c_str();
    }
};


///////// Functions for creating automaton for given formula /////////


/**
 * Creates automaton for term true
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term true
*/
Nfa codeTrue(const unsigned long &act_ident_count);


/**
 * Creates automaton for term false
 * @return Automaton for term false
*/
Nfa codeFalse();


/**
 * Creates automaton for term zero order variable
 * @param[in] v1 Variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term zero order variable
*/
Nfa codeVar0(const Ident &v1, const unsigned long &act_ident_count);


/**
 * Creates automaton for term empty set
 * @param[in] v1 Variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term empty set
*/
Nfa codeEmptySet(const Ident &v1, const unsigned long &act_ident_count);


/**
 * Creates automaton for term assign number
 * @param[in] v1 Variable to be assigned
 * @param[in] n Assign value
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term assign number
*/
Nfa codeAsignInt(const Ident &v1, const int &n, const unsigned long &act_ident_count);


/**
 * Creates automaton for term set
 * @param[in] v1 Variable to be assigned
 * @param[in] elements Assign values
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term set
*/
Nfa codeSet(const Ident &v1, const ASTList *elements, const unsigned long &act_ident_count);


/**
 * Creates automaton for term presburget constant
 * @param[in] v1 Variable to be assigned
 * @param[in] n Assign value
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term presburger constant
*/
Nfa codePresbConst(const Ident &v1, const int &n, const unsigned long &act_ident_count);


/**
 * Creates automaton for term singleton set
 * @param[in] v1 Variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term singleton set
*/
Nfa codeSingleton(const Ident &v1, const unsigned long &act_ident_count);


/**
 * Creates automaton for term less
 * @param[in] v1 First variable to be assigned
 * @param[in] v2 Second variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term less
*/
Nfa codeLess(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count);


/**
 * Creates automaton for term less or equal
 * @param[in] v1 First variable to be assigned
 * @param[in] v2 Second variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term less or equal
*/
Nfa codeLessEq(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count);


/**
 * Creates automaton for term in
 * @param[in] v1 First variable to be assigned
 * @param[in] v2 Second variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term in
*/
Nfa codeIn(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count);


/**
 * Creates automaton for term not in
 * @param[in] v1 First variable to be assigned
 * @param[in] v2 Second variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term not in
*/
Nfa codeNotIn(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count);


/**
 * Creates automaton for term subset or equal
 * @param[in] v1 First variable to be assigned
 * @param[in] v2 Second variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term subset or equal
*/
Nfa codeSub(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count);


/**
 * Creates automaton for term union
 * @param[in] v1 First variable to be assigned
 * @param[in] v2 Second variable to be assigned
 * @param[in] v3 Third variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term union
*/
Nfa codeUnion(const Ident &v1, const Ident &v2, const Ident &v3, const unsigned long &act_ident_count);


/**
 * Creates automaton for term inter
 * @param[in] v1 First variable to be assigned
 * @param[in] v2 Second variable to be assigned
 * @param[in] v3 Third variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term inter
*/
Nfa codeInter(const Ident &v1, const Ident &v2, const Ident &v3, const unsigned long &act_ident_count);


/**
 * Creates automaton for term set minus
 * @param[in] v1 First variable to be assigned
 * @param[in] v2 Second variable to be assigned
 * @param[in] v3 Third variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term set minus
*/
Nfa codeSetMinus(const Ident &v1, const Ident &v2, const Ident &v3, const unsigned long &act_ident_count);


/**
 * Creates automaton for term equal
 * @param[in] v1 First variable to be assigned
 * @param[in] v2 Second variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term equal
*/
Nfa codeEqual1(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count);


/**
 * Creates automaton for term not equal
 * @param[in] v1 First variable to be assigned
 * @param[in] v2 Second variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term not equal
*/
Nfa codeNotEqual1(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count);


/**
 * Creates automaton for term equal
 * @param[in] v1 First variable to be assigned
 * @param[in] v2 Second variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term equal
*/
Nfa codeEqual2(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count);


/**
 * Creates automaton for term not equal
 * @param[in] v1 First variable to be assigned
 * @param[in] v2 Second variable to be assigned
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton for term not equal
*/
Nfa codeNotEqual2(const Ident &v1, const Ident &v2, const unsigned long &act_ident_count);


/**
 * Creates automaton for equation x = y + addition
 * @param[in] x First variable in above equation
 * @param[in] y Second variable in above equation
 * @param[in] addition Number of succesors (additiion)
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton corresponding to equation x = y + addition
 */
Nfa codeSucc(const Ident &v1, const Ident &v2, const int &addition, const unsigned long &act_ident_count);


///////// Functions for restricting vars to singleton sets /////////


/**
 * Applies necessary singleton restrictions to input automaton
 * @param[in] aut Input automaton
 * @param[in] vars Variables to be restricted
 * @param[in] act_ident_count Number of variables in current context
 * @return Automaton with singleton restrictions applied
*/
void applySingletonRestrictions(Nfa &aut, const IdentList &vars, const unsigned long &act_ident_count);


///////// Automata algorithms /////////


/**
 * Adds final states into skip edge in forward direction
 * @param[inout] aut Input and output automaton
 * @param[in] as Source state of edge
 * @param[in] symb Symbol on edge
 * @param[in] fs Target state of edge
 * @param[in] level Level of states in input automaton
 * @param[in] num_idents Number of variables in current context
 */
void addFinalStates(Nfa &aut, const State &as, const Symbol &symb, const State &fs, int *level, const unsigned long &num_idents);


/**
 * Adds final states into skip edge in backward direction
 * @param[inout] aut Input and output automaton
 * @param[in] as Source state of edge
 * @param[in] symb Symbol on edge
 * @param[in] fs Target state of edge
 * @param[in] level Level of states in input automaton
 * @param[in] num_idents Number of variables in current context
 */
void addFinalStatesRev(Nfa &aut, const State &as, const Symbol &symb, const State &fs, int *level, const unsigned long &num_idents);


/**
 * Removes useless zeros in input automaton
 * @param[in] aut Input automaton
 * @param[in] act_ident_count Number of vars in current context
 * @return Automaton without useless zeros
 */
Nfa removeUselessZeros(const Nfa &aut, const unsigned long &act_ident_count);


/**
 * Redirects every edge going to first in pair to second in pair
 * @param[inout] new_aut Input and output automaton
 * @param[in] egdes_to_be_removed Pairs determines redirection
 * @return Automaton without useless zeros
 */
void redirectEdges(Nfa &new_aut, const vector<pair<State,State>> &egdes_to_be_removed);


/**
 * Computes edge length after projection
 * @param[in] as_lvl Level of source state in edge
 * @param[in] edge_len Length of origin edge
 * @param[in] x Variable to be projected
 * @param[in] aic Number of vars in current context
 * @return Correct length of new edge
 */
unsigned long computeEdgeLen(unsigned long as_lvl, unsigned long edge_len, unsigned long x, unsigned long aic);


/**
 * Performs automata projection operation
 * @param[in] aut Input automaton
 * @param[in] x Varible which will be projected
 * @param[in] act_ident_count Number of vars in current context
 * @return Automaton after projection
 */
Nfa project(const Nfa &aut, const Ident &x, const unsigned long &act_ident_count);


/**
 * Performs automata complement operation
 * @param[in] aut Input automaton
 * @param[in] act_ident_count Number of vars in current context
 * @return Automaton after complementation
 */
Nfa complement(const Nfa &aut, const unsigned long &act_ident_count);


/**
 * Replace common edges with one skip edge
 * @param[in] aut Input automaton
 * @param[in] pred_init_state Starting state of new skip edge
 * @param[in] init_state State after starting state of new skip edge (start state of computation)
 * @return Target state of new skip edge
 */
State addSkipEdge(const Nfa &aut, const State &pred_init_state, const State &init_state);


/**
 * Replace common edges with skip edges, if possible
 * @param[in] aut Input automaton
 * @return Automaton with maximal number of skip edges
 */
Nfa reduceToSkipEdges(const Nfa &aut);


/**
 * Performs automata union operation
 * @param[in] aut1 First input automaton
 * @param[in] aut2 Second input automaton
 * @return Union automaton
 */
Nfa unionization(const Nfa &aut1, const Nfa &aut2);


/**
 * Performs automata determinizationn operation
 * @param[in] aut Input automaton
 * @return Determinized automaton
 */
Nfa determinization(const Nfa &aut);


/**
 * Performs automata minimization operation
 * @param[in] aut Input automaton
 * @return Minimized automaton
 */
Nfa minimization(const Nfa &aut);


/**
 * Performs automata intersection operation
 * @param[in] aut1 First input automaton
 * @param[in] aut2 Second input automaton
 * @return Intersection automaton
 */
Nfa inter(const Nfa &aut1, const Nfa &aut2);

