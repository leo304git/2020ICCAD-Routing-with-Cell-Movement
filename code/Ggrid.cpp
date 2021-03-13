#include "Ggrid.h"

Ggrid::Ggrid(int sup, char d, int x, int y, int z)
{
    //ctor
    supply = sup;
    availability = supply;
    direction = d;
    rowIdx = x;
    colIdx = y;
    highIdx = z;

}

Ggrid::~Ggrid()
{
    //dtor
}

void
Ggrid::setSupply(int x)
{
    supply=x;
    return;
}

int
Ggrid::getSupply()
{
    return supply;
}

bool//new!
Ggrid::ggridok()
{
    //cout<<rowIdx<<" "<<colIdx<<" "<<highIdx;
    //cout<<" availability "<<availability<<endl;
    return (availability>0);
}

bool//new!
Ggrid::ggridok(int x)
{
    //cout<<rowIdx<<" "<<colIdx<<" "<<highIdx;
    //cout<<" availability "<<availability<<endl;
    return (availability-x>0);
}

void//new!
Ggrid::demanding(int d)
{
    //if(availability<0){cout<<"---"<<rowIdx<<" "<<colIdx<<" "<<highIdx<<" "<<availability<<endl;}
    availability-=d;
}

void//new!
Ggrid::returning(int r)
{
    availability+=r;
}
