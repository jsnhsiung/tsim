//BlockTask.h
//Jason Hsiung
//hsunweih@gmail.com
#ifndef _BLOCKTASK_H
#define _BLOCKTASK_H

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include "CycleHistory.h"
#include "Atom.h"

enum class BlockTaskState {
   NOT_ISSUED,
   ISSUED,
   ACTIVE,
   RETIRED
};

class BlockTask{
private: 
   static unsigned int blocktask_seq_id;     

   unsigned int  seq_id;
   std::string   name;
   unsigned int  atom_capacity;    
   std::string   type_name; 
   std::map      <std::string, Atom> atom_map;
   //<atom full name, atom obj>
   CycleHistory    history;
   BlockTaskState  state;

public:
          BlockTask(std::string, unsigned int, std::string);
          //name, atom_num, type_name
          BlockTask(const BlockTask&);	  
          BlockTask();
         ~BlockTask();

          void  tick();
          int   addAtom(const Clock &, Atom &);
          int   addAtom(Atom &);
          void  tickAtom(std::string);//atom_full_name
          void  tickAllAtoms();

          std::vector<std::string> 
             getAllAtomFullNames();//BT name;

          unsigned int getAtomCap() const;
          void putEvent(const Clock &, std::string);
          void printHistory();
          BlockTaskState getBTState();

          typedef std::map<std::string, Atom>::iterator atom_map_itr; 
          atom_map_itr atom_map_begin();
          atom_map_itr atom_map_end();

};//class BlockTask

#endif // not deifned _BLOCKTASK_H_
