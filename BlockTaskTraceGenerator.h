//BlockTaskTraceGenerator.cpp
//Jason Hsiung
//hsunweih@gmail.com
#ifndef _BLOCKTASKTRACEGENERATOR_H
#define _BLOCKTASKTRACEGENERATOR_H

#include <utility>
#include <string>
#include <map>
#include <vector>
#include "BlockTask.h"

class BlockTaskTraceGenerator
{
private:
          static unsigned int bttracegen_seq_id;
         
          unsigned int seq_id;
          std::string name;
          unsigned int max_atoms; 
          //maximum number of atoms per BlockTask(BT)
          unsigned int task_cyc;
          //number of cycles to generate BTs
          unsigned int max_concr;
          //maximum level of concurrency per cycle
          unsigned int atom_service_min_delay;
          //the minimum number of cycles a atom will appear
          //on trace after its mother blocktask shows up in the trace
          unsigned int atom_service_max_delay;
          //the maximum number of cycles a atom will appear
          //on trace after its mother blocktask shows up in the trace
          unsigned int statistics_bts;
          unsigned int statistics_atoms;
          std::map<std::pair <unsigned int,unsigned int>, std::string>
          trace_map;
          //<<cycle index, concurrency index>, BT name>
          std::map<std::pair <unsigned int,unsigned int>, std::string>
          atom_trace_map;
          //<<cycle index, concurrency index>, ATOM name>
 
          std::map<std::string, BlockTask>  blocktask_map;
          //<BT name, BT object>
           
 public: 
          BlockTaskTraceGenerator
          (std::string, unsigned int,
           unsigned int, unsigned int,
           unsigned int, unsigned int);
          //name, max atoms, task cycles,
          //max concurrency, min delay, max delay
          BlockTaskTraceGenerator();
         ~BlockTaskTraceGenerator();
          
          unsigned int getBTAtomNum(std::string) const;
          void printTraceMap();
          void printAtomTraceMap();
          void tickBT(std::string); //BT name
          void tickBTAtoms(std::string); //BT name
          void tickAtom(std::string); //Atom full name
          std::vector<std::string> getAllAtomFullNames(std::string);
          std::pair<unsigned int, unsigned int> printStatistics();
          //BT name
          
          typedef std::map<std::pair <unsigned int,unsigned int>,
                           std::string>::iterator  trace_map_itr;
          trace_map_itr trace_map_begin();
          trace_map_itr trace_map_end();
          
          typedef std::map<std::pair <unsigned int,unsigned int>,
                           std::string>::iterator  atom_trace_map_itr;

          atom_trace_map_itr atom_trace_map_begin();
          atom_trace_map_itr atom_trace_map_end();


          BlockTaskState getBTState(std::string);
          AtomState getAtomState(std::string);
          std::string getAtomStateString(std::string);
          BlockTask::atom_map_itr atom_map_ofaBT_begin(std::string);
          BlockTask::atom_map_itr atom_map_ofaBT_end(std::string);

};//class BlockTaskTraceGenerator

#endif
