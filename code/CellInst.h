#ifndef CELLINST_H
#define CELLINST_H
#include <string>
#include <vector>
#include "MCtype.h"
#include "Pin.h"
#include "Blockage.h"
#include "Net.h"
using namespace std;

class CellInst
{
    public:
        CellInst(int, int, string, string, bool, MCtype*);
        virtual ~CellInst();
        void consult(MCtype*);
        bool is_moveable();
        int getRow();
        int getCol();
        void set_newrow(int x){new_row=x;};
        void set_newcol(int y){new_col=y;};
        void set_reduce(int);
        int get_reduce(){return reduce;};
        void set_Idx();
        vector<Pin*>& getfanout(){return fanout;};
        string& get_name(){return name;};
        Pin& get_pin(string&);
        void set_net(Net*);
        void reset_net(vector<vector<vector<Ggrid*>>>&);
        void set_fanout();
        vector<Net*>& get_net(){return net;}
        int get_num_blkg(){return num_blkg;}
        Blockage*& get_blkg() {return blkg;}
        string& get_type(){return type;}
        void set_same(bool a){same=a;}
        void set_left(bool a){left=a;}
        void set_right(bool a){right=a;}
        bool get_same(){return same;}
        bool get_left(){return left;}
        bool get_right(){return right;}


    protected:

    private:
    	int rowIdx, colIdx;
        string name;
    	string type;
    	bool moveable;
    	Pin* pin;
    	Blockage* blkg;
    	int num_pin;
    	int num_blkg;
        vector<Pin*>   fanout;
        int new_col,new_row;
        vector<Net*> net;
        int reduce;           //wire length estimation
        bool same, left, right;         
};

#endif // CELLINST_H
