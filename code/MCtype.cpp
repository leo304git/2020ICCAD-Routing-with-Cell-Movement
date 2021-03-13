#include "MCtype.h"

MCtype::MCtype(int num)
{
	num_name = num;
	name = new string[num];
	pins = new Pin*[num];
	blkgs = new Blockage*[num];
	num_pins = new int[num];
	num_blkgs = new int[num];
 }

 MCtype::~MCtype()
 {

  }

  void MCtype::writeMC(int MCindex, string type_name, int pinnum, int blkgnum)
  {
  	num_pins[MCindex] = pinnum;
  	num_blkgs[MCindex] = blkgnum;
  	name[MCindex] = type_name;
  	pins[MCindex] = new Pin[pinnum];
  	blkgs[MCindex] = new Blockage[blkgnum];
  	return;
  }

  void MCtype::writePin(int MCindex, int Pinindex, string pinname, Layer* layer)
  {
  	pins[MCindex][Pinindex].setdata(pinname, layer);
  	return;
  }

  void MCtype::writeBlkg(int MCindex, int Blkgindex, string blkgname, Layer* layer, int demand)
  {
  	blkgs[MCindex][Blkgindex].setdata(blkgname, layer, demand);
  	return;
  }
