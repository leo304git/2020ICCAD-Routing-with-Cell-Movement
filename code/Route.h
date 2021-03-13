#ifndef ROUTE_H
#define ROUTE_H
#include <cmath>
#include <iostream>
#include <algorithm>
#include<vector>
//#include "Net.h"
#include "Pin.h"
#include "Ggrid.h"

//class Net;
class Route
{
    public:
        Route();
        Route(int, int, int, int);
        Route(int, int, int, int, int, int);//newnew
        virtual ~Route();
        void set2Dpos(int, int, int, int);
        void set3Dpos(int, int, int, int, int, int);//newnew
        int dist(Pin*);
        bool Trelation(Pin*);
        char direction();
        bool contain(Pin*);
        Route Tcreate(Pin*);
        Route& operator=(const Route&);
        bool operator==(const Route&);//new
        bool ggridOK(vector<vector<vector<Ggrid*>>>&);
        bool demanding(vector<vector<vector<Ggrid*>>>&);
        bool returning(vector<vector<vector<Ggrid*>>>&);
        int getlength(); // newnew
        int get_sRowIdx(){return sRowIdx;}
        int get_eRowIdx(){return eRowIdx;}
        int get_sColIdx(){return sColIdx;}
        int get_eColIdx(){return eColIdx;}
        int get_sLayIdx(){return sLayIdx;}
        int get_eLayIdx(){return eLayIdx;}

    protected:

    private:
        friend class Net;
        int sRowIdx;
        int sColIdx;
        int sLayIdx;
        int eRowIdx;
        int eColIdx;
        int eLayIdx;
        int length;
        //Net* net;
        //Pin* s_pin; //new
        //Pin* e_pin; //new
        Pin* steiner;
};

#endif // ROUTE_H
