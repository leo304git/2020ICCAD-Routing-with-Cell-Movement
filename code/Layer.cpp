#include <string>
#include <iostream>
#include "Layer.h"


Layer::Layer()
{
    height = 0;
    //direction = "n";
    demand = 0;
}
Layer::Layer (string n,int h, char dir, int dem)
{
    name=n;
    height = h;
    direction = dir;
    demand = dem;
}

Layer::~Layer()
{
    //dtor
}

Layer& Layer::operator = (const Layer& l)
{
    name = l.name; //newnew
    height = l.height;
    direction = l.direction;
    demand = l.demand;
    return *this;
}

string&
Layer::get_name()
{
    return name;
}

int Layer::getHeight() //new
{
    return height;
}

char Layer::getDirection() //new
{
    return direction;
}

