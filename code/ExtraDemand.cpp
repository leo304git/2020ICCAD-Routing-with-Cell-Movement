#include "ExtraDemand.h"

sameDemand::sameDemand()
{
    //ctor
}

sameDemand::sameDemand(string a,string b,Layer* c,int d)
{
    mc1 = a;
    mc2 = b;
    layer = c;
    demand = d;
}

sameDemand::~sameDemand()
{
    //dtor
}

bool
sameDemand::is_sameDemand(string& a,string& b)
{
    return (a==mc1&&b==mc2)||(a==mc2&&b==mc1);
}



//=========================================================================



adjDemand::adjDemand()
{
    //ctor
}

adjDemand::adjDemand(string a,string b,Layer* c,int d)
{
    mc1 = a;
    mc2 = b;
    layer = c;
    demand = d;
}


adjDemand::~adjDemand()
{
    //dtor
}

bool
adjDemand::is_adjDemand(string& a,string& b)
{
    return (a==mc1&&b==mc2)||(a==mc2&&b==mc1);
}

