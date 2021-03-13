#ifndef BLOCKAGE_H
#define BLOCKAGE_H
#include <string>
#include "Layer.h"
using namespace std;

class Blockage
{
	public:
		Blockage();
		Blockage(string, Layer*, int);
		~Blockage();
		void setdata(string, Layer*, int);
		Blockage& operator = (const Blockage&);
		string getname();
		Layer getlayer();
		int getdemand();
		void setpos(int,int);

	private:
		string name;
		Layer layer;
		int demand;
		int RowIdx;
		int ColIdx;

};

#endif // BLOCKAGE_H
