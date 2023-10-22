/**
 *
 * main.cpp -- main part of the program
 *
 * Author: Pavel Bednar <bednarpavel@outlook.cz>
 *
 * This program was created as a part of diploma thesis
 * on the topic of deciding WS1S at FIT BUT.
 *
*/

#include "ast/scanner.cpp"
#include "ast/untyped.h"
#include "ast/predlib.h"
#include "ast/offsets.h"
#include "ast/ast.h"
#include "ast/ident.h"
#include "env.h"
#include "mata/nfa.hh"
#include "core/auxiliary.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include "core/eval.h"
#include <cxxopts.hpp>


using namespace std;


IdentList globals;
MonaUntypedAST *untypedAST;
SymbolTable symbolTable(1019);
PredicateLib predicateLib;
int numTypes = 0;
Guide_Declaration *guide_declaration = 0;
Offsets offsets;
Ident lastPosVar, allPosVar;
int options_mode = LINEAR;
bool options_m2l = false;
bool options_alternativeM2LStr = false;
bool print_stats = false;
bool print_time = false;


timeStats ts;


int main(int argc, char **argv)
{
   MonaAST* _monaAST = nullptr;
   string input_file;

   cxxopts::Options options("ws1s_solver", "Program for deciding WS1S formulas");
   options.add_options()
      ("f,file", "Set input file", cxxopts::value<string>())
      ("t,time", "Print elapsed time")
      ("s,stats", "Print statistics")
      ("h,help", "Usage");

   try
   {
      auto result = options.parse(argc, argv);

      if (result.count("help"))
      {
         cout << options.help() << endl;
         return 0;
      }

      if (result.count("file"))
      {
         input_file = result["file"].as<string>();
      }
      else
      {
         cerr << "Argument -f, --file is mandatory." << endl;
         return 1;
      }

      if (result.count("time"))
      {
         print_time = true;
      }

      if (result.count("stats"))
      {
         print_stats = true;
      }
   }
   catch (const cxxopts::exceptions::exception& e)
   {
      cerr << e.what() << endl;
      return 1;
   }
   catch (...)
   {
      cerr << "error when parsing program arguments" << endl;
      return 1;
   }

   char* input_file_char = new char[input_file.length() + 1];
   strcpy(input_file_char, input_file.c_str());

   auto start = chrono::high_resolution_clock::now();

   loadFile(input_file_char);
   yyparse();
   _monaAST = untypedAST->typeCheck();
   lastPosVar = _monaAST->lastPosVar;
   allPosVar = _monaAST->allPosVar;
   globals = _monaAST->globals;
   unsigned long act_ident_count = globals.size();

   (_monaAST->formula)->dump();
   cout << endl;

   int new_var = 0;
   remapVars mapping;
   IdentList first_order_vars;
   for (const auto &origin_var: globals)
   {
      mapping.addRecord(origin_var, new_var);
      if (symbolTable.lookupType(origin_var) == Varname1)
      {
         first_order_vars.push_back(new_var);
      }
      new_var++;
   }

   Nfa aut;
   try
   {
      _monaAST->formula->genCode(aut, act_ident_count, mapping);
   }
   catch (const unimplExc& e)
   {
      cerr << "input formula contains unsupported term: " << e.what() << endl;
      return 1;
   }
   catch (...)
   {
      cerr << "unexpected error" << endl;
      return 1;
   }
   applySingletonRestrictions(aut, first_order_vars, act_ident_count);

   ofstream fdot("./dot/aut.dot");
   aut.print_to_DOT(fdot);

   Run *aut_run = new Run();
   bool language_emptiness;
   language_emptiness = is_lang_empty(aut, aut_run);

   if (language_emptiness == true)
   {
      cout << "formula is unsatisfiable" << endl;
   }
   else if (is_lang_empty(complement(aut, act_ident_count)))
   {
      cout << "formula is valid" << endl;
   }
   else
   {
      vector<Symbol> words;
      for (auto a: aut_run->word)
      {
         words.push_back(a);
      }
      vector<Symbol> jumps;
      for (auto i = aut_run->path.begin() + 1; i < aut_run->path.end(); i++)
      {
         jumps.push_back(i->jump);
      }
      vector<Symbol> unfold_word;
      for (int i = 0; i < words.size(); i++)
      {
         for (int j = 0; j < jumps[i]; j++)
         {
            unfold_word.push_back(words[i]);
         }
      }

      vector<int> vars_set[act_ident_count];
      for (int i = 0; i < unfold_word.size(); i++)
      {
         int remapped_i = mapping.getRevMapping(i % act_ident_count);
         if (i < act_ident_count && symbolTable.lookupType(remapped_i) == Varname0)
         {
            vars_set[i].push_back((unfold_word)[i]);
         }
         else if (i >= act_ident_count && symbolTable.lookupType(remapped_i) != Varname0 && (unfold_word)[i] == 1)
         {
            vars_set[i % act_ident_count].push_back(i / act_ident_count - 1);
         }
      }
      for (int i = 0; i < act_ident_count; i++)
      {
         int remapped_i = mapping.getRevMapping(i);
         if (symbolTable.lookupType(remapped_i) == Varname0)
         {
            cout << symbolTable.lookupSymbol(remapped_i) << " = bool(";
         }
         else
         {
            cout << symbolTable.lookupSymbol(remapped_i) << " = {";
         }
         if (vars_set[i].size() > 0)
         {
            for (int j = 0; j < vars_set[i].size() - 1; j++)
            {
               cout << vars_set[i][j] << ",";
            }
            cout << vars_set[i][vars_set[i].size() - 1];
         }
         if (symbolTable.lookupType(remapped_i) == Varname0)
         {
            cout << ")";
         }
         else
         {
            cout << "}";
         }
         cout << endl;
      }
   }

   auto end = chrono::high_resolution_clock::now();
   auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
   auto total_time = (float)duration.count() / 1000000;

   if (print_time)
   {
      cout << "time: " << total_time << "s\n";
   }

   if (print_stats)
   {
      ts.printStats();
   }

   return 0;
}
