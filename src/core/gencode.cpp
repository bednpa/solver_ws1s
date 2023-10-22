/**
 * 
 * gencode.cpp -- implementation of translating formulas to automata
 * 
 * Author: Pavel Bednar <bednarpavel@outlook.cz>
 * 
 * This program was created as a part of diploma thesis
 * on the topic of deciding WS1S at FIT BUT.
 * 
*/

#include "../ast/symboltable.h"
#include "../core/atomaut.h"
#include "../core/auxiliary.h"
#include "../mata/nfa.hh"
#include "../ast/predlib.h"
#include <algorithm>

using std::cout;
using std::endl;
using Mata::Nfa::Nfa;

extern SymbolTable symbolTable;
extern PredicateLib predicateLib;


void ASTTerm1_Var1::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    in_aut = codeEqual1(carry, mapping.getMapping(n), act_ident_count); 
}


void ASTTerm1_Dot::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Dot");
}

void ASTTerm1_Up::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Up");
}

void ASTTerm1_Root::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Root");
}

void ASTTerm1_Int::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    in_aut = codeAsignInt(carry, n, act_ident_count);
}


void ASTTerm1_Plus::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    Ident l_var = carry;
    Ident r_var = carry + 1;

    Nfa a1;
    t->genCode(a1, act_ident_count, mapping, r_var);

    in_aut = codeSucc(l_var, r_var, n, act_ident_count);
    in_aut = inter(in_aut, a1);
}


void ASTTerm1_Minus::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    Ident l_var = carry;
    Ident r_var = carry + 1;

    Nfa a1;
    t->genCode(a1, act_ident_count, mapping, r_var);

    in_aut = codeSucc(r_var, l_var, n, act_ident_count);
    in_aut = inter(in_aut, a1);
}


void ASTTerm1_PlusModulo::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("PlusModulo");
}

void ASTTerm1_MinusModulo::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("MinusModulo");
}

void ASTTerm1_Min::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Min");
}

void ASTTerm1_Max::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Max");
}

void ASTTerm1_TreeRoot::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("TreeRoot");
}


void ASTTerm2_Var2::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    in_aut = codeEqual2(carry, mapping.getMapping(n), act_ident_count);
}


void ASTTerm2_VarTree::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("VarTree");
}

void ASTTerm2_Dot::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Dot");
}

void ASTTerm2_Up::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Up");
}


void ASTTerm2_Empty::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    in_aut = codeEmptySet(carry, act_ident_count);
}


void ASTTerm2_Union::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    Ident l_var = carry;
    Ident m_var = carry + 1;
    Ident r_var = carry + 2;

    Nfa a1;
    T1->genCode(a1, act_ident_count, mapping, r_var);
    Nfa a2;
    T2->genCode(a2, act_ident_count, mapping, m_var);

    in_aut = codeUnion(l_var, r_var, m_var, act_ident_count);
    in_aut = inter(in_aut, a1);
    in_aut = inter(in_aut, a2);
}


void ASTTerm2_Inter::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    Ident l_var = carry;
    Ident m_var = carry + 1;
    Ident r_var = carry + 2;

    Nfa a1;
    T1->genCode(a1, act_ident_count, mapping, r_var);
    Nfa a2;
    T2->genCode(a2, act_ident_count, mapping, m_var);

    in_aut = codeInter(l_var, r_var, m_var, act_ident_count);
    in_aut = inter(in_aut, a1);
    in_aut = inter(in_aut, a2);
}


void ASTTerm2_Setminus::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    Ident l_var = carry;
    Ident m_var = carry + 1;
    Ident r_var = carry + 2;

    Nfa a1;
    T1->genCode(a1, act_ident_count, mapping, r_var);
    Nfa a2;
    T2->genCode(a2, act_ident_count, mapping, m_var);

    in_aut = codeSetMinus(l_var, r_var, m_var, act_ident_count);
    in_aut = inter(in_aut, a1);
    in_aut = inter(in_aut, a2);
}


void ASTTerm2_Set::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    in_aut = codeSet(carry, elements, act_ident_count);
}


void ASTTerm2_Plus::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Plus");
}

void ASTTerm2_Minus::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Minus");
}

void ASTTerm2_Interval::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Interval");
}


void ASTTerm2_PresbConst::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    in_aut = codePresbConst(carry, value, act_ident_count);
}


void ASTTerm2_Formula::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Formula");
}


void ASTForm_Var0::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    in_aut = codeVar0(mapping.getMapping(n), act_ident_count);
}


void ASTForm_True::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    in_aut = codeTrue(act_ident_count);
}


void ASTForm_False::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    in_aut = codeFalse();
}


void ASTForm_In::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    int *l_depth = new int();
    int *r_depth = new int();
    *l_depth = 1;
    *r_depth = 1;
    t1->getDepth(l_depth);
    T2->getDepth(r_depth);

    Ident l_var = act_ident_count;
    Ident r_var = act_ident_count + *l_depth;
    unsigned long new_ident_count = act_ident_count + *l_depth + *r_depth;

    Nfa a1;
    t1->genCode(a1, new_ident_count, mapping, l_var);
    Nfa a2;
    T2->genCode(a2, new_ident_count, mapping, r_var);

    in_aut = codeIn(l_var, r_var, new_ident_count);
    in_aut = inter(a1, in_aut);
    in_aut = inter(a2, in_aut);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
}


void ASTForm_Notin::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) // TODO proverit ten automat (i neq1 a neq2)
{
    int *l_depth = new int();
    int *r_depth = new int();
    *l_depth = 1;
    *r_depth = 1;
    t1->getDepth(l_depth);
    T2->getDepth(r_depth);

    Ident l_var = act_ident_count;
    Ident r_var = act_ident_count + *l_depth;
    unsigned long new_ident_count = act_ident_count + *l_depth + *r_depth;

    Nfa a1;
    t1->genCode(a1, new_ident_count, mapping, l_var);
    Nfa a2;
    T2->genCode(a2, new_ident_count, mapping, r_var);

    in_aut = codeNotIn(l_var, r_var, new_ident_count);
    in_aut = inter(a1, in_aut);
    in_aut = inter(a2, in_aut);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
}

void ASTForm_RootPred::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("RootPred");
}

void ASTForm_EmptyPred::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("EmptyPred");
}

void ASTForm_FirstOrder::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("FirstOrder");
}


void ASTForm_Sub::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    int *l_depth = new int();
    int *r_depth = new int();
    *l_depth = 1;
    *r_depth = 1;
    T1->getDepth(l_depth);
    T2->getDepth(r_depth);

    Ident l_var = act_ident_count;
    Ident r_var = act_ident_count + *l_depth;
    unsigned long new_ident_count = act_ident_count + *l_depth + *r_depth;

    Nfa a1;
    T1->genCode(a1, new_ident_count, mapping, l_var);
    Nfa a2;
    T2->genCode(a2, new_ident_count, mapping, r_var);

    in_aut = codeSub(l_var, r_var, new_ident_count);
    in_aut = inter(a1, in_aut);
    in_aut = inter(a2, in_aut);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
}


void ASTForm_Equal1::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    int *l_depth = new int();
    int *r_depth = new int();
    *l_depth = 1;
    *r_depth = 1;
    t1->getDepth(l_depth);
    t2->getDepth(r_depth);

    Ident l_var = act_ident_count;
    Ident r_var = act_ident_count + *l_depth;
    unsigned long new_ident_count = act_ident_count + *l_depth + *r_depth;

    Nfa a1;
    t1->genCode(a1, new_ident_count, mapping, l_var);
    Nfa a2;
    t2->genCode(a2, new_ident_count, mapping, r_var);

    in_aut = codeEqual1(l_var, r_var, new_ident_count);
    in_aut = inter(a1, in_aut); 
    in_aut = inter(a2, in_aut);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
}


void ASTForm_Equal2::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    int *l_depth = new int();
    int *r_depth = new int();
    *l_depth = 1;
    *r_depth = 1;
    T1->getDepth(l_depth);
    T2->getDepth(r_depth);

    Ident l_var = act_ident_count;
    Ident r_var = act_ident_count + *l_depth;
    unsigned long new_ident_count = act_ident_count + *l_depth + *r_depth;

    Nfa a1;
    T1->genCode(a1, new_ident_count, mapping, l_var);
    Nfa a2;
    T2->genCode(a2, new_ident_count, mapping, r_var);

    in_aut = codeEqual2(l_var, r_var, new_ident_count);
    in_aut = inter(a1, in_aut);
    in_aut = inter(a2, in_aut);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
}


void ASTForm_NotEqual1::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    int *l_depth = new int();
    int *r_depth = new int();
    *l_depth = 1;
    *r_depth = 1;
    t1->getDepth(l_depth);
    t2->getDepth(r_depth);

    Ident l_var = act_ident_count;
    Ident r_var = act_ident_count + *l_depth;
    unsigned long new_ident_count = act_ident_count + *l_depth + *r_depth;

    Nfa a1;
    t1->genCode(a1, new_ident_count, mapping, l_var);
    Nfa a2;
    t2->genCode(a2, new_ident_count, mapping, r_var);

    in_aut = codeNotEqual1(l_var, r_var, new_ident_count);
    in_aut = inter(a1, in_aut);
    in_aut = inter(a2, in_aut);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
}


void ASTForm_NotEqual2::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    int *l_depth = new int();
    int *r_depth = new int();
    *l_depth = 1;
    *r_depth = 1;
    T1->getDepth(l_depth);
    T2->getDepth(r_depth);

    Ident l_var = act_ident_count;
    Ident r_var = act_ident_count + *l_depth;
    unsigned long new_ident_count = act_ident_count + *l_depth + *r_depth;

    Nfa a1;
    T1->genCode(a1, new_ident_count, mapping, l_var);
    Nfa a2;
    T2->genCode(a2, new_ident_count, mapping, r_var);

    in_aut = codeNotEqual2(l_var, r_var, new_ident_count);
    in_aut = inter(a1, in_aut);
    in_aut = inter(a2, in_aut);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
}


void ASTForm_Less::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    int *l_depth = new int();
    int *r_depth = new int();
    *l_depth = 1;
    *r_depth = 1;
    t1->getDepth(l_depth);
    t2->getDepth(r_depth);

    Ident l_var = act_ident_count;
    Ident r_var = act_ident_count + *l_depth;
    unsigned long new_ident_count = act_ident_count + *l_depth + *r_depth;

    Nfa a1;
    t1->genCode(a1, new_ident_count, mapping, l_var);
    Nfa a2;
    t2->genCode(a2, new_ident_count, mapping, r_var);

    in_aut = codeLess(l_var, r_var, new_ident_count);
    in_aut = inter(a1, in_aut);
    in_aut = inter(a2, in_aut);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
}


void ASTForm_LessEq::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    int *l_depth = new int();
    int *r_depth = new int();
    *l_depth = 1;
    *r_depth = 1;
    t1->getDepth(l_depth);
    t2->getDepth(r_depth);

    Ident l_var = act_ident_count;
    Ident r_var = act_ident_count + *l_depth;
    unsigned long new_ident_count = act_ident_count + *l_depth + *r_depth;

    Nfa a1;
    t1->genCode(a1, new_ident_count, mapping, l_var);
    Nfa a2;
    t2->genCode(a2, new_ident_count, mapping, r_var);

    in_aut = codeLessEq(l_var, r_var, new_ident_count);
    in_aut = inter(a1, in_aut);
    in_aut = inter(a2, in_aut);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
}


void ASTForm_WellFormedTree::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("WellFormedTree");
}


void ASTForm_Impl::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    Nfa a1;
    f1->genCode(a1, act_ident_count, mapping, carry);
    Nfa a2;
    f2->genCode(a2, act_ident_count, mapping, carry);
    Nfa n_a1 = complement(a1, act_ident_count);
    in_aut = unionization(n_a1, a2);
}


void ASTForm_Biimpl::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    Nfa a1;
    f1->genCode(a1, act_ident_count, mapping, carry);
    Nfa a2;
    f2->genCode(a2, act_ident_count, mapping, carry);
    Nfa n_a1 = complement(a1, act_ident_count);
    Nfa n_a2 = complement(a2, act_ident_count);
    Nfa aut1 = unionization(n_a1, a2);
    Nfa aut2 = unionization(n_a2, a1);
    in_aut = inter(aut1, aut2);
}


void ASTForm_And::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    Nfa a1;
    f1->genCode(a1, act_ident_count, mapping, carry);
    Nfa a2;
    f2->genCode(a2, act_ident_count, mapping, carry);
    in_aut = inter(a1, a2);
}


void ASTForm_IdLeft::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("IdLeft");
}


void ASTForm_Or::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    Nfa a1;
    f1->genCode(a1, act_ident_count, mapping, carry);
    Nfa a2;
    f2->genCode(a2, act_ident_count, mapping, carry);
    in_aut = unionization(a1, a2);
}


void ASTForm_Not::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    f->genCode(in_aut, act_ident_count, mapping, carry);
    in_aut = complement(in_aut, act_ident_count);
}


void ASTForm_Ex0::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    IdentList *vars = vl->copy();
    unsigned long new_ident_count = act_ident_count + vars->size();
    int i = 0;
    for (auto v: *vars)
    {
        mapping.addRecord(v, act_ident_count + i);
        i++;
    }

    f->genCode(in_aut, new_ident_count, mapping, carry);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
}


void ASTForm_Ex1::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    IdentList *vars = vl->copy();
    unsigned long new_ident_count = act_ident_count + vars->size();
    int i = 0;
    for (auto v: *vars)
    {
        mapping.addRecord(v, act_ident_count + i);
        i++;
    }

    f->genCode(in_aut, new_ident_count, mapping, carry);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        Nfa s_aut = codeSingleton(i, new_ident_count); 
        in_aut = inter(s_aut, in_aut);
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
}


void ASTForm_Ex2::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    IdentList *vars = vl->copy();
    unsigned long new_ident_count = act_ident_count + vars->size();
    int i = 0;
    for (auto v: *vars)
    {
        mapping.addRecord(v, act_ident_count + i);
        i++;
    }

    f->genCode(in_aut, new_ident_count, mapping, carry);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
}


void ASTForm_All0::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    IdentList *vars = vl->copy();
    unsigned long new_ident_count = act_ident_count + vars->size();
    int i = 0;
    for (auto v: *vars)
    {
        mapping.addRecord(v, act_ident_count + i);
        i++;
    }

    f->genCode(in_aut, new_ident_count, mapping, carry);
    in_aut = complement(in_aut, new_ident_count);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
    in_aut = complement(in_aut, new_ident_count);
}


void ASTForm_All1::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    IdentList *vars = vl->copy();
    unsigned long new_ident_count = act_ident_count + vars->size();
    int i = 0;
    for (auto v: *vars)
    {
        mapping.addRecord(v, act_ident_count + i);
        i++;
    }

    f->genCode(in_aut, new_ident_count, mapping, carry);
    in_aut = complement(in_aut, new_ident_count);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        Nfa s_aut = codeSingleton(i, new_ident_count); 
        in_aut = inter(s_aut, in_aut);
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
    in_aut = complement(in_aut, new_ident_count);
}


void ASTForm_All2::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    IdentList *vars = vl->copy();
    unsigned long new_ident_count = act_ident_count + vars->size();
    int i = 0;
    for (auto v: *vars) 
    {
        mapping.addRecord(v, act_ident_count + i);
        i++;
    }

    f->genCode(in_aut, new_ident_count, mapping, carry);
    in_aut = complement(in_aut, new_ident_count);

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
    in_aut = complement(in_aut, new_ident_count);
}


void ASTForm_Let0::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Let0");
}

void ASTForm_Let1::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Let1");
}

void ASTForm_Let2::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Let2");
}


void ASTForm_Call::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry)
{
    PredLibEntry *pred = predicateLib.lookup(n);
    IdentList *def = pred->formals;
    ASTList *call = args;
    unsigned long new_ident_count = act_ident_count;

    int remapped_def_array[def->size()];
    int origin_def_array[def->size()];
    bool needs_new_var[def->size()];

    int i = 0;
    ASTList::iterator call_it = call->begin();
    for (const auto &x: *def)
    {
        Ident v;
        if ((*call_it)->kind == aVar0)
        {
            v = mapping.getMapping(((ASTForm_Var0*)(*call_it))->getN());
            needs_new_var[i] = false;
        }
        else if ((*call_it)->kind == aVar1)
        {
            v = mapping.getMapping(((ASTTerm1_Var1*)(*call_it))->getN());
            needs_new_var[i] = false;
        }
        else if ((*call_it)->kind == aVar2)
        {
            v = mapping.getMapping(((ASTTerm2_Var2*)(*call_it))->getVar());
            needs_new_var[i] = false;
        }
        else
        {
            v = new_ident_count;
            needs_new_var[i] = true;
            new_ident_count++;
        }
        mapping.addRecord(x, v);
        remapped_def_array[i] = v;
        origin_def_array[i] = x;

        i++;
        call_it++;
    }

    pred->ast->genCode(in_aut, new_ident_count, mapping, carry);

    call_it = call->begin();
    for (int i = 0; i < def->size(); i++)
    {
        if (needs_new_var[i] && (*call_it)->order == oForm)
        {
            ASTForm_Var0 *ast_v = new ASTForm_Var0(origin_def_array[i],dummyPos);
            Nfa a1;
            (new ASTForm_Biimpl(ast_v,((ASTForm*)(*call_it)),dummyPos))->genCode(a1, new_ident_count, mapping, carry);
            in_aut = inter(in_aut, a1);
        }
        else if (needs_new_var[i] && (*call_it)->order == oTerm1)
        {
            ASTTerm1_Var1 *ast_v = new ASTTerm1_Var1(origin_def_array[i],dummyPos);
            Nfa a1;
            (new ASTForm_Equal1(ast_v,((ASTTerm1*)(*call_it)),dummyPos))->genCode(a1, new_ident_count, mapping, carry);
            in_aut = inter(in_aut, a1);
        }
        else if (needs_new_var[i] && (*call_it)->order == oTerm2)
        {
            ASTTerm2_Var2 *ast_v = new ASTTerm2_Var2(origin_def_array[i],dummyPos);
            Nfa a1;
            (new ASTForm_Equal2(ast_v,((ASTTerm2*)(*call_it)),dummyPos))->genCode(a1, new_ident_count, mapping, carry);
            in_aut = inter(in_aut, a1);
        }
        call_it++;
    }

    for (int i = 0; i < def->size(); i++)
    {
        if (needs_new_var[i] && symbolTable.lookupType(origin_def_array[i]) == Varname1)
        {
            Nfa s_aut = codeSingleton(remapped_def_array[i], new_ident_count);
            in_aut = inter(in_aut, s_aut);
        }
    }

    for (Ident i = new_ident_count - 1; i >= act_ident_count && i >= 0; i--)
    {
        in_aut = project(in_aut, i, new_ident_count);
        new_ident_count--;
    }
}


void ASTForm_Import::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Import");
}

void ASTForm_Export::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Export");
}

void ASTForm_Prefix::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Prefix");
}

void ASTForm_Restrict::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("Restrict");
}

void ASTForm_InStateSpace1::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("InStateSpace1");
}

void ASTForm_InStateSpace2::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("InStateSpace2");
}

void ASTForm_SomeType::genCode(Nfa &in_aut, unsigned long act_ident_count, remapVars mapping, int carry) {
    throw unimplExc("SomeType");
}
