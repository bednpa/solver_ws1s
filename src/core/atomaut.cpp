/**
 * 
 * atomaut.cpp -- implementation of creating atomic automata
 * 
 * Author: Pavel Bednar <bednarpavel@outlook.cz>
 * 
 * This program was created as a part of diploma thesis
 * on the topic of deciding WS1S at FIT BUT.
 * 
*/

#include "../core/atomaut.h"


Nfa createAut1(const int &v1, const int *trans, const int &num_states, const unsigned long &num_idents, const bool &boolean_prefix) 
{
   unsigned long x1 = v1;
   unsigned long new_inner_state = num_states; 
   const unsigned long one_trans_len = 2;
   Nfa aut(one_trans_len * num_states);
   int start_state = 0;
   if (boolean_prefix) // first state trans are ignored and set to whatever
   {
      start_state = 1;
      aut.add_trans(0, 0, State{1, num_idents});
      aut.add_trans(0, 1, State{1, num_idents});
   }

   for (int i = start_state; i < num_states; i++)
   {
      unsigned long pre_state; // if prefix of 0s and 1s needed 
      bool pre_state_created = false;
      for (int j = 0; j < one_trans_len; j++)
      {
         if (trans[one_trans_len * i + j] != -1)
         {
            if (x1 > 0) // with pre-state
            {
               // trans 0
               if (j == 0)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  aut.add_trans(pre_state, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x1});
               }
               // trans 1
               if (j == 1)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  aut.add_trans(pre_state, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x1});
               }
            }
            else // without pre_state
            {
               // trans 0
               if (j == 0)
               {
                  aut.add_trans(i, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents});
               }
               // trans 1
               if (j == 1)
               {
                  aut.add_trans(i, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents});
               }
            }
         }
      }
   }
   for (int i = one_trans_len * num_states; i > new_inner_state; i--)
   {
      aut.transition_relation.pop_back();
   }
   return aut;
}




Nfa createAut2(const int &v1, const int &v2, const int *trans, const int &num_states, const unsigned long &num_idents, const bool &boolean_prefix) 
{
   unsigned long x1;
   unsigned long x2;
   unsigned long new_inner_state = num_states; 
   const unsigned long one_trans_len = 4;
   int indexes[one_trans_len];
   Nfa aut(one_trans_len * num_states);
   int start_state = 0;
   if (boolean_prefix) // first state trans are ignored and set to whatever
   {
      start_state = 1;
      aut.add_trans(0, 0, State{1, num_idents});
      aut.add_trans(0, 1, State{1, num_idents});
   } 

   if (v1 < v2)
   {
      // 0 1
      // 00 (0) <= 00 (0)
      // 01 (1) <= 01 (1)
      // 10 (2) <= 10 (2)
      // 11 (3) <= 11 (3)
      x1 = v1;
      x2 = v2;
      indexes[0] = 0;
      indexes[1] = 1;
      indexes[2] = 2;
      indexes[3] = 3;
   }
   else
   {
      // 1 0
      // 00 (0) <= 00 (0)
      // 01 (1) <= 10 (2)
      // 10 (2) <= 01 (1)
      // 11 (3) <= 11 (3)      
      x1 = v2;
      x2 = v1;
      indexes[0] = 0;
      indexes[1] = 2;
      indexes[2] = 1;
      indexes[3] = 3;
   }

   for (int i = start_state; i < num_states; i++)
   {
      unsigned long inner_state_0; // 00, 01 (OX)
      bool inner_state_0_created = false;
      unsigned long inner_state_1;// 10, 11 (1X)
      bool inner_state_1_created = false;
      unsigned long pre_state; // if prefix of 0s and 1s needed 
      bool pre_state_created = false;
      for (int j = 0; j < one_trans_len; j++)
      {
         if (trans[one_trans_len * i + j] != -1)
         {
            if (x1 > 0) // with pre-state
            {
               // trans 00
               if (indexes[j] == 0)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  if (inner_state_0_created == false)
                  {
                     inner_state_0 = new_inner_state++;
                     aut.add_trans(pre_state, 0, State{inner_state_0, x2 - x1});
                     inner_state_0_created = true;
                  }
                  aut.add_trans(inner_state_0, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x2});
               }
               // trans 01
               if (indexes[j] == 1)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  if (inner_state_0_created == false)
                  {
                     inner_state_0 = new_inner_state++;
                     aut.add_trans(pre_state, 0, State{inner_state_0, x2 - x1});
                     inner_state_0_created = true;
                  }
                  aut.add_trans(inner_state_0, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x2});
               }
               // trans 10
               if (indexes[j] == 2)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  if (inner_state_1_created == false)
                  {
                     inner_state_1 = new_inner_state++;
                     aut.add_trans(pre_state, 1, State{inner_state_1, x2 - x1});
                     inner_state_1_created = true;
                  }
                  aut.add_trans(inner_state_1, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x2});
               }
               // trans 11
               if (indexes[j] == 3)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  if (inner_state_1_created == false)
                  {
                     inner_state_1 = new_inner_state++;
                     aut.add_trans(pre_state, 1, State{inner_state_1, x2 - x1});
                     inner_state_1_created = true;
                  }
                  aut.add_trans(inner_state_1, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x2});
               }
            }
            else // without pre_state
            {
               // trans 00
               if (indexes[j] == 0)
               {
                  if (inner_state_0_created == false)
                  {
                     inner_state_0 = new_inner_state++;
                     aut.add_trans(i, 0, State{inner_state_0, x2 - x1});
                     inner_state_0_created = true;
                  }
                  aut.add_trans(inner_state_0, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x2});
               }
               // trans 01
               if (indexes[j] == 1)
               {
                  if (inner_state_0_created == false)
                  {
                     inner_state_0 = new_inner_state++;
                     aut.add_trans(i, 0, State{inner_state_0, x2 - x1});
                     inner_state_0_created = true;
                  }
                  aut.add_trans(inner_state_0, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x2});
               }
               // trans 10
               if (indexes[j] == 2)
               {
                  if (inner_state_1_created == false)
                  {
                     inner_state_1 = new_inner_state++;
                     aut.add_trans(i, 1, State{inner_state_1, x2 - x1});
                     inner_state_1_created = true;
                  }
                  aut.add_trans(inner_state_1, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x2});
               }
               // trans 11
               if (indexes[j] == 3)
               {
                  if (inner_state_1_created == false)
                  {
                     inner_state_1 = new_inner_state++;
                     aut.add_trans(i, 1, State{inner_state_1, x2 - x1});
                     inner_state_1_created = true;
                  }
                  aut.add_trans(inner_state_1, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x2});
               }
            }
         }
      }
   }
   for (int i = one_trans_len * num_states; i > new_inner_state; i--)
   {
      aut.transition_relation.pop_back();
   }
   return aut;
}



Nfa createAut3(const int &v1, const int &v2, const int &v3, const int *trans, const int &num_states, const unsigned long &num_idents, const bool &boolean_prefix) 
{
   unsigned long x1;
   unsigned long x2;
   unsigned long x3;
   unsigned long new_inner_state = num_states; 
   const unsigned long one_trans_len = 8;
   int indexes[one_trans_len];
   Nfa aut(one_trans_len * num_states);
   int start_state = 0;
   if (boolean_prefix) // first state trans are ignored and set to whatever
   {
      start_state = 1;
      aut.add_trans(0, 0, State{1, num_idents});
      aut.add_trans(0, 1, State{1, num_idents});
   }

   if (v1 < v2 && v2 < v3)
   {
      // 1 2 3
      // 000 (0) <= 000 (0)
      // 001 (1) <= 001 (1)
      // 010 (2) <= 010 (2)
      // 011 (3) <= 011 (3)
      // 100 (4) <= 100 (4)
      // 101 (5) <= 101 (5)
      // 110 (6) <= 110 (6)
      // 111 (7) <= 111 (7)
      x1 = v1;
      x2 = v2;
      x3 = v3;
      indexes[0] = 0;
      indexes[1] = 1;
      indexes[2] = 2;
      indexes[3] = 3;
      indexes[4] = 4;
      indexes[5] = 5;
      indexes[6] = 6;
      indexes[7] = 7;
   }
   else if (v1 < v3 && v3 < v2)
   {
      // 1 3 2
      // 000 (0) <= 000 (0)
      // 001 (1) <= 010 (2)
      // 010 (2) <= 001 (1)
      // 011 (3) <= 011 (3)
      // 100 (4) <= 100 (4)
      // 101 (5) <= 110 (6)
      // 110 (6) <= 101 (5)
      // 111 (7) <= 111 (7)
      x1 = v1;
      x2 = v3;
      x3 = v2;
      indexes[0] = 0;
      indexes[1] = 2;
      indexes[2] = 1;
      indexes[3] = 3;
      indexes[4] = 4;
      indexes[5] = 6;
      indexes[6] = 5;
      indexes[7] = 7;
   } 
   else if (v2 < v1 && v1 < v3)
   {
      // 2 1 3
      // 000 (0) <= 000 (0)
      // 001 (1) <= 001 (1)
      // 010 (2) <= 100 (4)
      // 011 (3) <= 101 (5)
      // 100 (4) <= 010 (2)
      // 101 (5) <= 011 (3)
      // 110 (6) <= 110 (6)
      // 111 (7) <= 111 (7)
      x1 = v2;
      x2 = v1;
      x3 = v3;
      indexes[0] = 0;
      indexes[1] = 1;
      indexes[2] = 4;
      indexes[3] = 5;
      indexes[4] = 2;
      indexes[5] = 3;
      indexes[6] = 6;
      indexes[7] = 7;
   } 
   else if (v2 < v3 && v3 < v1)
   {
      // 2 3 1
      // 000 (0) <= 000 (0)
      // 001 (1) <= 010 (2)
      // 010 (2) <= 100 (4)
      // 011 (3) <= 110 (6)
      // 100 (4) <= 001 (1)
      // 101 (5) <= 011 (3)
      // 110 (6) <= 101 (5)
      // 111 (7) <= 111 (7)
      x1 = v2;
      x2 = v3;
      x3 = v1; 
      indexes[0] = 0;
      indexes[1] = 4;
      indexes[2] = 1;
      indexes[3] = 5;
      indexes[4] = 2;
      indexes[5] = 6;
      indexes[6] = 3;
      indexes[7] = 7;
   }
   else if (v3 < v1 && v1 < v2)
   {
      // 3 1 2
      // 000 (0) <= 000 (0)
      // 001 (1) <= 100 (4)
      // 010 (2) <= 001 (1)
      // 011 (3) <= 101 (5)
      // 100 (4) <= 010 (2)
      // 101 (5) <= 110 (6)
      // 110 (6) <= 011 (3)
      // 111 (7) <= 111 (7)
      x1 = v3;
      x2 = v1;
      x3 = v2;
      indexes[0] = 0;
      indexes[1] = 2;
      indexes[2] = 4;
      indexes[3] = 6;
      indexes[4] = 1;
      indexes[5] = 3;
      indexes[6] = 5;
      indexes[7] = 7;
   }
   else if (v3 < v2 && v2 < v1)
   {
      // 3 2 1
      // 000 (0) <= 000 (0)
      // 001 (1) <= 100 (4)
      // 010 (2) <= 010 (2)
      // 011 (3) <= 110 (6)
      // 100 (4) <= 001 (1)
      // 101 (5) <= 101 (5)
      // 110 (6) <= 011 (3)
      // 111 (7) <= 111 (7)
      x1 = v3;
      x2 = v2;
      x3 = v1;
      indexes[0] = 0;
      indexes[1] = 4;
      indexes[2] = 2;
      indexes[3] = 6;
      indexes[4] = 1;
      indexes[5] = 5;
      indexes[6] = 3;
      indexes[7] = 7;
   }

   for (int i = start_state; i < num_states; i++)
   {
      unsigned long inner_state_0; // OXX
      bool inner_state_0_created = false;
      unsigned long inner_state_1;// 1XX
      bool inner_state_1_created = false;
      unsigned long inner_state_2;// 00X
      bool inner_state_2_created = false;
      unsigned long inner_state_3;// 01X
      bool inner_state_3_created = false;
      unsigned long inner_state_4;// 10X
      bool inner_state_4_created = false;
      unsigned long inner_state_5;// 11X
      bool inner_state_5_created = false;
      unsigned long pre_state; // if prefix of 0s and 1s needed 
      bool pre_state_created = false;
      for (int j = 0; j < one_trans_len; j++)
      {
         if (trans[one_trans_len * i + j] != -1)
         {
            if (x1 > 0) // with pre-state
            {
               // trans 000
               if (indexes[j] == 0)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  if (inner_state_0_created == false)
                  {
                     inner_state_0 = new_inner_state++;
                     aut.add_trans(pre_state, 0, State{inner_state_0, x2 - x1});
                     inner_state_0_created = true;
                  }
                  if (inner_state_2_created == false)
                  {
                     inner_state_2 = new_inner_state++;
                     aut.add_trans(inner_state_0, 0, State{inner_state_2, x3 - x2});
                     inner_state_2_created = true;
                  }
                  aut.add_trans(inner_state_2, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 001
               if (indexes[j] == 1)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  if (inner_state_0_created == false)
                  {
                     inner_state_0 = new_inner_state++;
                     aut.add_trans(pre_state, 0, State{inner_state_0, x2 - x1});
                     inner_state_0_created = true;
                  }
                  if (inner_state_2_created == false)
                  {
                     inner_state_2 = new_inner_state++;
                     aut.add_trans(inner_state_0, 0, State{inner_state_2, x3 - x2});
                     inner_state_2_created = true;
                  }
                  aut.add_trans(inner_state_2, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 010
               if (indexes[j] == 2)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  if (inner_state_0_created == false)
                  {
                     inner_state_0 = new_inner_state++;
                     aut.add_trans(pre_state, 0, State{inner_state_0, x2 - x1});
                     inner_state_0_created = true;
                  }
                  if (inner_state_3_created == false)
                  {
                     inner_state_3 = new_inner_state++;
                     aut.add_trans(inner_state_0, 1, State{inner_state_3, x3 - x2});
                     inner_state_3_created = true;
                  }
                  aut.add_trans(inner_state_3, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 011
               if (indexes[j] == 3)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  if (inner_state_0_created == false)
                  {
                     inner_state_0 = new_inner_state++;
                     aut.add_trans(pre_state, 0, State{inner_state_0, x2 - x1});
                     inner_state_0_created = true;
                  }
                  if (inner_state_3_created == false)
                  {
                     inner_state_3 = new_inner_state++;
                     aut.add_trans(inner_state_0, 1, State{inner_state_3, x3 - x2});
                     inner_state_3_created = true;
                  }
                  aut.add_trans(inner_state_3, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 100
               if (indexes[j] == 4)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  if (inner_state_1_created == false)
                  {
                     inner_state_1 = new_inner_state++;
                     aut.add_trans(pre_state, 1, State{inner_state_1, x2 - x1});
                     inner_state_1_created = true;
                  }
                  if (inner_state_4_created == false)
                  {
                     inner_state_4 = new_inner_state++;
                     aut.add_trans(inner_state_1, 0, State{inner_state_4, x3 - x2});
                     inner_state_4_created = true;
                  }
                  aut.add_trans(inner_state_4, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 101
               if (indexes[j] == 5)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  if (inner_state_1_created == false)
                  {
                     inner_state_1 = new_inner_state++;
                     aut.add_trans(pre_state, 1, State{inner_state_1, x2 - x1});
                     inner_state_1_created = true;
                  }
                  if (inner_state_4_created == false)
                  {
                     inner_state_4 = new_inner_state++;
                     aut.add_trans(inner_state_1, 0, State{inner_state_4, x3 - x2});
                     inner_state_4_created = true;
                  }
                  aut.add_trans(inner_state_4, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 110
               if (indexes[j] == 6)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  if (inner_state_1_created == false)
                  {
                     inner_state_1 = new_inner_state++;
                     aut.add_trans(pre_state, 1, State{inner_state_1, x2 - x1});
                     inner_state_1_created = true;
                  }
                  if (inner_state_5_created == false)
                  {
                     inner_state_5 = new_inner_state++;
                     aut.add_trans(inner_state_1, 1, State{inner_state_5, x3 - x2});
                     inner_state_5_created = true;
                  }
                  aut.add_trans(inner_state_5, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 111
               if (indexes[j] == 7)
               {
                  if (pre_state_created == false)
                  {
                     pre_state = new_inner_state++;
                     aut.add_trans(i, 0, State{pre_state, x1});
                     aut.add_trans(i, 1, State{pre_state, x1});
                     pre_state_created = true;
                  }
                  if (inner_state_1_created == false)
                  {
                     inner_state_1 = new_inner_state++;
                     aut.add_trans(pre_state, 1, State{inner_state_1, x2 - x1});
                     inner_state_1_created = true;
                  }
                  if (inner_state_5_created == false)
                  {
                     inner_state_5 = new_inner_state++;
                     aut.add_trans(inner_state_1, 1, State{inner_state_5, x3 - x2});
                     inner_state_5_created = true;
                  }
                  aut.add_trans(inner_state_5, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
            }
            else // without pre_state
            {
               // trans 000
               if (indexes[j] == 0)
               {
                  if (inner_state_0_created == false)
                  {
                     inner_state_0 = new_inner_state++;
                     aut.add_trans(i, 0, State{inner_state_0, x2 - x1});
                     inner_state_0_created = true;
                  }
                  if (inner_state_2_created == false)
                  {
                     inner_state_2 = new_inner_state++;
                     aut.add_trans(inner_state_0, 0, State{inner_state_2, x3 - x2});
                     inner_state_2_created = true;
                  }
                  aut.add_trans(inner_state_2, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 001
               if (indexes[j] == 1)
               {
                  if (inner_state_0_created == false)
                  {
                     inner_state_0 = new_inner_state++;
                     aut.add_trans(i, 0, State{inner_state_0, x2 - x1});
                     inner_state_0_created = true;
                  }
                  if (inner_state_2_created == false)
                  {
                     inner_state_2 = new_inner_state++;
                     aut.add_trans(inner_state_0, 0, State{inner_state_2, x3 - x2});
                     inner_state_2_created = true;
                  }
                  aut.add_trans(inner_state_2, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 010
               if (indexes[j] == 2)
               {
                  if (inner_state_0_created == false)
                  {
                     inner_state_0 = new_inner_state++;
                     aut.add_trans(i, 0, State{inner_state_0, x2 - x1});
                     inner_state_0_created = true;
                  }
                  if (inner_state_3_created == false)
                  {
                     inner_state_3 = new_inner_state++;
                     aut.add_trans(inner_state_0, 1, State{inner_state_3, x3 - x2});
                     inner_state_3_created = true;
                  }
                  aut.add_trans(inner_state_3, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 011
               if (indexes[j] == 3)
               {
                  if (inner_state_0_created == false)
                  {
                     inner_state_0 = new_inner_state++;
                     aut.add_trans(i, 0, State{inner_state_0, x2 - x1});
                     inner_state_0_created = true;
                  }
                  if (inner_state_3_created == false)
                  {
                     inner_state_3 = new_inner_state++;
                     aut.add_trans(inner_state_0, 1, State{inner_state_3, x3 - x2});
                     inner_state_3_created = true;
                  }
                  aut.add_trans(inner_state_3, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 100
               if (indexes[j] == 4)
               {
                  if (inner_state_1_created == false)
                  {
                     inner_state_1 = new_inner_state++;
                     aut.add_trans(i, 1, State{inner_state_1, x2 - x1});
                     inner_state_1_created = true;
                  }
                  if (inner_state_4_created == false)
                  {
                     inner_state_4 = new_inner_state++;
                     aut.add_trans(inner_state_1, 0, State{inner_state_4, x3 - x2});
                     inner_state_4_created = true;
                  }
                  aut.add_trans(inner_state_4, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 101
               if (indexes[j] == 5)
               {
                  if (inner_state_1_created == false)
                  {
                     inner_state_1 = new_inner_state++;
                     aut.add_trans(i, 1, State{inner_state_1, x2 - x1});
                     inner_state_1_created = true;
                  }
                  if (inner_state_4_created == false)
                  {
                     inner_state_4 = new_inner_state++;
                     aut.add_trans(inner_state_1, 0, State{inner_state_4, x3 - x2});
                     inner_state_4_created = true;
                  }
                  aut.add_trans(inner_state_4, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 110
               if (indexes[j] == 6)
               {
                  if (inner_state_1_created == false)
                  {
                     inner_state_1 = new_inner_state++;
                     aut.add_trans(i, 1, State{inner_state_1, x2 - x1});
                     inner_state_1_created = true;
                  }
                  if (inner_state_5_created == false)
                  {
                     inner_state_5 = new_inner_state++;
                     aut.add_trans(inner_state_1, 1, State{inner_state_5, x3 - x2});
                     inner_state_5_created = true;
                  }
                  aut.add_trans(inner_state_5, 0, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
               // trans 111
               if (indexes[j] == 7)
               {
                  if (inner_state_1_created == false)
                  {
                     inner_state_1 = new_inner_state++;
                     aut.add_trans(i, 1, State{inner_state_1, x2 - x1});
                     inner_state_1_created = true;
                  }
                  if (inner_state_5_created == false)
                  {
                     inner_state_5 = new_inner_state++;
                     aut.add_trans(inner_state_1, 1, State{inner_state_5, x3 - x2});
                     inner_state_5_created = true;
                  }
                  aut.add_trans(inner_state_5, 1, State{static_cast<unsigned long>(trans[one_trans_len * i + j]), num_idents - x3});
               }
            }
         }
      }
   }
   for (int i = one_trans_len * num_states; i > new_inner_state; i--)
   {
      aut.transition_relation.pop_back();
   }
   return aut;
}