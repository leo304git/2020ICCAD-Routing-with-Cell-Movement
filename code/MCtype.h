#ifndef MCTYPE_H
#define MCTYPE_H
#include <string>
#include "Pin.h"
#include "Layer.h"
#include "Blockage.h"
using namespace std;

class MCtype
{
	public:
		MCtype(int);
		~MCtype();
		void writeMC(int, string, int, int);
		void writePin(int, int, string, Layer*);
		void writeBlkg(int, int, string, Layer*, int);
		int get_pin(){return pins[0]->getColIdx();}

	private:
		friend class CellInst;
		string* name;
		int num_name;
		int* num_pins;
		int* num_blkgs;
		Pin** pins;
		Blockage** blkgs;

};

#endif // MCTYPE_H
