//Atom.h
//Jason Hsiung
//hsunweih@gmail.com

#ifndef _ATOM_H
#define _ATOM_H

#include <string>
#include "CycleHistory.h"
#include "Clock.h"


enum class AtomState{
    NOT_ISSUED,
    ISSUED,
    ACTIVE,
    RETIRED
};

class Atom{
private:
   static unsigned int  atom_seq_id;
   unsigned int         seq_id;
   std::string          name;
   std::string          type_name; 
   std::string          mother_task_name;
   std::string          fullname;
   AtomState            state;
   CycleHistory         history;         

public :
        Atom(std::string, std::string, std::string);
        Atom(const Atom &);
        //name. type_name, mother task name
       ~Atom();

        std::string getName() const;	
        std::string getFullName() const;	
        void putEvent(const Clock &, std::string);
        void printHistory();
        void tick();
        std::string getState(); 
        AtomState getAtomState();       
};//class Atom

#endif /* not defined _ATOM_H */
