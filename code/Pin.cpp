#include "Pin.h"

Pin::Pin()
{
	name = "noname";
	listed = false;
	connected = false;
	corner = 0;
	//layer = "nolayer";
}

Pin::Pin(string n, Layer* l)
{
	name = n;
	layer = *l;
	listed = false;
	connected = false;
	corner = 0;
}

Pin::~Pin()
{

}
void Pin::setdata(string n, Layer* l)
{
	name = n;
	layer = *l;
	return;
}

void Pin::setpos(int x, int y)
{
    rowIdx = x;
    colIdx = y;
    return;
}
Pin& Pin::operator =(const Pin& p)
{
	name = p.name;
	layer = p.layer;
	rowIdx = p.rowIdx;
	colIdx = p.colIdx;
	corner = p.corner; // new
	net_next_pin = p.net_next_pin;
	return *this;
}

string Pin::getname()
{
	return name;
}

int Pin::getRowIdx()
{
    return rowIdx;
}

int Pin::getColIdx()
{
    return colIdx;
}

int Pin::getLayIdx() //new
{
    return layer.getHeight();
}

Layer Pin::getlayer()
{
	return layer;
}

Pin* Pin::getNet_next_pin()
{
    return net_next_pin;
}

void Pin::setNet_next_pin(Pin* p)
{
    net_next_pin = p;
    listed = true;
    return;
}

bool Pin::isListed()
{
    return listed;
}

void Pin::deListed()
{
    listed = false;
    return;
}

void Pin::makeConnected()
{
    connected = true;
    return;
}

bool Pin::isConnected()
{
    return connected;
}

void Pin::set_corner()
{
	corner = true;
	return;
}

bool Pin::is_corner()
{
	return corner;
}