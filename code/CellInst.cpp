#include "CellInst.h"

CellInst::CellInst(int x, int y, string n, string t, bool m, MCtype* consultor)
{
    rowIdx = x;
    colIdx = y;
    name = n;
    type = t;
    moveable = m;
    reduce = 0;
    consult(consultor);
	same=0;
	left=0;
	right=0;	
    for(int j=0; j<num_pin; j++)
	{
		pin[j].setpos(x,y);
	}
	for(int j=0; j<num_blkg; j++)
	{
		blkg[j].setpos(x,y);
	}
}

CellInst::~CellInst()
{
    //dtor
}

void CellInst::consult(MCtype* consultor)
{
	int type_num; //type���s��
	for(int i=0; i<consultor->num_name; i++) //�bMC��Ʈw���v���M��
	{
		if(type == consultor->name[i])
		{
			type_num = i;
			num_pin = consultor->num_pins[i];
			num_blkg = consultor->num_blkgs[i];
			pin = new Pin[num_pin];
			blkg = new Blockage[num_blkg]; //�ݬݭn���n�[��break
		}
	}

	for(int j=0; j<num_pin; j++)
	{
		pin[j] = consultor->pins[type_num][j];
	}
	for(int j=0; j<num_blkg; j++)
	{
		blkg[j] = consultor->blkgs[type_num][j];
	}

	return;
}

bool
CellInst::is_moveable()
{
    return moveable;
}

int
CellInst::getRow()
{
    return rowIdx;
}

int
CellInst::getCol()
{
    return colIdx;
}

void
CellInst::set_reduce(int x)
{
    int length = 0;
    for (int i=0; i<net.size(); i++) {
        length += net[i]->get_length();
    }
    reduce = length - x;
	//cout<<"reduce:"<<reduce<<endl;
}

Pin&
CellInst::get_pin(string& pin_name)
{
	for (int i=0;i<num_pin;i++){
		if(pin[i].get_name()==pin_name){return pin[i];}
	}
	Pin pin;
    return pin;
}

void
CellInst::set_net(Net* n)
{
    net.push_back(n);
}

void
CellInst::reset_net(vector<vector<vector<Ggrid*>>>& gg)
{
    for (int i=0; i<net.size(); i++) {
		for(int j=0;j<num_pin;j++){
        	net[i]->reset_route(&pin[j], name, gg);
		}
    }
}

void
CellInst::set_fanout()
{
    fanout.clear();
    for(unsigned i=0;i<net.size();i++){
        for(int j=0;j<net[i]->get_num_pin();j++){
            if (net[i]->get_pin(j)->get_cell_name() != name){
                fanout.push_back(net[i]->get_pin(j));
            }
        }
    }
}

void
CellInst::set_Idx()
{
    rowIdx=new_row;colIdx=new_col;
    for(int j=0; j<num_pin; j++){
		pin[j].setpos(new_row,new_col);
	}
	for(int j=0; j<num_blkg; j++){
		blkg[j].setpos(new_row,new_col);
	}
}
