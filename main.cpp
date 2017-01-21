//main.cpp
//Jason Hsiung
//hsunweih@gmail.com
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

#include "Clock.h"
#include "Atom.h"
#include "Cache.h"
#include "BlockTask.h"
#include "BlockTaskTraceGenerator.h"

using namespace std;

int main(){
   unsigned int cycle_to_sim = 20;
   unsigned int max_atom_per_bt = 10;
   unsigned int tasking_cycles  = 2;
   unsigned int max_concurrency = 3;
   unsigned int atom_service_min_delay = 1;
   unsigned int atom_service_max_delay = 10;

   BlockTaskTraceGenerator BTTraceGen
   ("BTTraceGen_0", max_atom_per_bt, tasking_cycles,
    max_concurrency, atom_service_min_delay,
    atom_service_max_delay);

   BTTraceGen.printTraceMap();
   BTTraceGen.printAtomTraceMap();

   unsigned int btselwin_cap = 10;//in number of BT 
   Cache BTSelectionWin
   ("BTSELW_0", "type-0", btselwin_cap);
   
   unsigned int atomselwin_cap = 100;//in number of atom
   //Too few entreis can cause deadlock
   Cache AtomSelectionWin
   ("ATOMSELW_0", "type-0", atomselwin_cap);

   unsigned int atomcache_cap = 10;
   //in number of atoms, has at least to be as many as max_atom_per_bt;

   Cache AtomCache
   ("AtomCache_0", "type-0", atomcache_cap); 
   //Add BT in trace_map to BT selection window
   //Add BT in selection window to cache (Allocation)
   Clock clk0("clk0", cycle_to_sim);
   do{
      //std::cout<<"Cycle: "<<clk0.getCycle()<<std::endl;

      AtomCache.snapShot(clk0);
      AtomSelectionWin.snapShot(clk0);
      BTSelectionWin.snapShot(clk0);

      AtomCache.removeActiveResd
      (clk0, BTTraceGen);
      //Atom ticked from ACTIVE to RETIRED
      //BT ticked from ACTIVE to RETIRED if its 
      //last atom serviced

      AtomSelectionWin.removeRetiredResd
      (clk0, BTTraceGen, "atom");
      //Remove RETIRED Atoms

      AtomCache.tryServiceResd
      (clk0, AtomSelectionWin, BTTraceGen);  
      //Atom ticked from ISSUED to ACTIVE 

      AtomSelectionWin.tryAddResd
      (clk0, BTTraceGen, "atom");
      //Atom ticked from NOT_ISSUED to ISSUED

      AtomCache.tryAddBlockResd
      (clk0, BTSelectionWin, BTTraceGen);
      //BT ticked from ISSUED to ACTIVE 

      BTSelectionWin.tryAddResd
      (clk0, BTTraceGen);
      //BT ticked from NOT_ISSUED to ISSUED
      
      BTSelectionWin.removeRetiredResd
       (clk0, BTTraceGen, "blocktask");
      //Remove RETIRED BT 
      
     /*
      std::ostringstream oss_clk0_cycle;
      oss_clk0_cycle << clk0.getCycle();
      std::string bt_name = "BlockTask_"+oss_clk0_cycle.str();
      BlockTask bt0(bt_name, 5, "type_0");
      for(int atomitr = 0; atomitr < 5; atomitr++){
         std::string atom_name = "Atom_"+oss_clk0_cycle.str();
         Atom atom0(atom_name, "type_0", bt_name); 
         bt0.addAtom(clk0, atom0);
      }
      std::cout<<clk0.getCycle()<<std::endl;
      */
   }while(clk0.tick() == 0);
  BTSelectionWin.printHistory();
  AtomCache.printHistory();    
  AtomSelectionWin.printHistory();
  
  BTSelectionWin.printSnapShots();
  AtomCache.printSnapShots();    
  AtomSelectionWin.printSnapShots();
  //////////////////verification
  std::pair<unsigned int, unsigned int> bt_atom_pair_generated;
  std::pair<unsigned int, unsigned int> bt_in_and_out;
  std::pair<unsigned int, unsigned int> atom_in_and_out_cache;
  std::pair<unsigned int, unsigned int> atom_in_and_out_selwin;
  bt_atom_pair_generated = BTTraceGen.printStatistics();
  bt_in_and_out = BTSelectionWin.printStatistics();
  atom_in_and_out_cache = AtomCache.printStatistics();
  atom_in_and_out_selwin = AtomSelectionWin.printStatistics();

  if(bt_atom_pair_generated.first  != bt_in_and_out.first ||
     bt_atom_pair_generated.first  != bt_in_and_out.second ||
     bt_atom_pair_generated.second != atom_in_and_out_cache.first ||
     bt_atom_pair_generated.second != atom_in_and_out_cache.second ||
     bt_atom_pair_generated.second != atom_in_and_out_selwin.first ||
     bt_atom_pair_generated.second != atom_in_and_out_selwin.second ||
     atom_in_and_out_cache.second  != atom_in_and_out_cache.first ||
     atom_in_and_out_selwin.second  != atom_in_and_out_selwin.first
     ){

     std::cout<<"!!SOMETHING WRONG WITH THE SIMULATOR!!"<<std::endl;
     std::cout<<"!!SOMETHING WRONG WITH THE SIMULATOR!!"<<std::endl;
     std::cout<<"!!SOMETHING WRONG WITH THE SIMULATOR!!"<<std::endl;
     std::cout<<"!!SOMETHING WRONG WITH THE SIMULATOR!!"<<std::endl;
     std::cout<<"!!SOMETHING WRONG WITH THE SIMULATOR!!"<<std::endl;
     std::cout<<"!!SOMETHING WRONG WITH THE SIMULATOR!!"<<std::endl;
  }
  //verification/////////////////////


  return 0; 
}//main
