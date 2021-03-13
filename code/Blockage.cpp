#include "Blockage.h"

Blockage::Blockage()
{
	name = "noname";
	//layer = "nolayer";
	demand = 0;
 }

 Blockage::Blockage(string n, Layer* l, int d)
 {
 	name = n;
 	layer = *l;
 	demand = d;
 }

 Blockage::~Blockage()
 {

 }

 void Blockage::setdata(string n, Layer* l, int d)
 {
 	name = n;
 	layer = *l;
 	demand = d;
 	return;
 }

 Blockage& Blockage::operator =(const Blockage& b)
 {
 	name = b.name;
 	layer = b.layer;
 	demand = b.demand;
 	return *this;
 }

 string Blockage::getname()
 {
 	return name;
 }

 Layer Blockage::getlayer()
 {
 	return layer;
 }

 int Blockage::getdemand()
 {
 	return demand;
 }

 void
 Blockage::setpos(int x,int y)
 {
     RowIdx=x;
     ColIdx=y;
     return;
 }
