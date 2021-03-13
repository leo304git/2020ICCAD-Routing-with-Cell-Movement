#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include "MCtype.h"
#include "Ggrid.h"
#include "CellInst.h"
#include "ExtraDemand.h"
#include "Layer.h"
#include "Net.h"

using namespace std;

class GgridMgr
{
public:
    GgridMgr(){}
    ~GgridMgr(){}
    // Member functions about circuit construction
    bool readFile(const string&);
    void reset();
    void placement(vector<CellInst*>&);
    void routing();  //nnew
    void write_file();    
    vector<CellInst*>& getCellInsts(){return CellInsts;};


private:
    //read&build
    void read_max_move(string&);
    void read_Ggrids(string&);
    void read_Layers(string&);
    void read_nonDefault(string&);
    void read_MasterCells(string&);
    void read_extra_demand(string&);
    void read_cellInst(string&);
    void read_routes(string&);
    void read_nets(string&);
    void build_ggrids();
    bool calculate(CellInst* ,int ,int ,bool );

    //placement

    bool cmp(pair <int, CellInst*>&, pair<int, CellInst*>& );

    int column[2],row[2];
    size_t height;
    size_t max_move;
    size_t non_default;
    MCtype* MC;
    vector<int> supply;
    vector<char> direction;
    vector<vector<vector<Ggrid*>>> Ggrids;
    vector<CellInst*> CellInsts;
    vector<pair<int,CellInst*>> moveable_Cells;
    vector<CellInst*> moved;
    vector<Layer*> layers;
    vector<Net*> Nets;
    vector<Net*> moved_nets;
    vector<sameDemand*> sameDemands;
    vector<adjDemand*> adjDemands;

};
#endif // PARSER_H
