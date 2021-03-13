#ifndef EXTRADEMAND_H
#define EXTRADEMAND_H
#include <string>
#include "Layer.h"

using namespace std;

class sameDemand
{
    public:
        sameDemand();
        sameDemand(string, string, Layer*, int);
        virtual ~sameDemand();
        bool is_sameDemand(string&,string&);
        int get_Layer_height(){return layer->getHeight();}
        int get_demand(){return demand;}

    protected:

    private:
        string mc1;
        string mc2;
        Layer* layer;
        int demand;
};

class adjDemand
{
    public:
        adjDemand();
        adjDemand(string, string, Layer*, int);
        virtual ~adjDemand();
        bool is_adjDemand(string&,string&);
        int get_Layer_height(){return layer->getHeight();}
        int get_demand(){return demand;}

    protected:

    private:
        string mc1;
        string mc2;
        Layer* layer;
        int demand;
};

#endif // EXTRADEMAND_H
