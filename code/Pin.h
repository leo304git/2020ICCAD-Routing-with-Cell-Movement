#ifndef PIN_H
#define PIN_H
#include <string>
#include <iostream>
#include "Layer.h"

using namespace std;

class Pin
{
	public:
		Pin();
		Pin(string, Layer*);
		~Pin();
		void setdata(string, Layer*);
		void setpos(int, int);
		void set_cell_name(string s){cell_name = s;}
		string getname();
		int getRowIdx();
		int getColIdx();
		int getLayIdx(); //new
		Layer getlayer();
		Pin& operator = (const Pin&);
		Pin* getNet_next_pin();
		void setNet_next_pin(Pin*);
		bool isListed();
		void deListed();
		bool isConnected();
		void makeConnected();
		string& get_name(){return name;}
		string& get_cell_name(){return cell_name;}
		void set_corner(); // new
		bool is_corner(); //new

	private:
		string name;
		string cell_name;
		Layer layer;
		int rowIdx;
		int colIdx;
		Pin* net_next_pin;
		bool connected;
		bool listed;
		bool corner; // new
};

#endif // PIN_H
