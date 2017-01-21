//Clock.h
//Jason Hsiung
//hsunweih@gmail.com
#ifndef _CLOCK_H
#define _CLOCK_H

#include <string>

class Clock{
private :
         unsigned int         cycle;
         unsigned int         cycle_max;
         std::string          name; 
public :

        Clock(std::string, unsigned int);
       ~Clock();

        unsigned int  getCycle() const;	
        int tick();
};//class Clock

#endif /* not defined _CLOCK_H */
