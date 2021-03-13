#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>
#include "parser.h"
#include "MCtype.h"
#include "Layer.h"
#include "Net.h"
//#include "Ggrid.h"

using namespace std;

int MCindex=0;

enum
reading_states{
    GRIDS=1,
    LAYERS,
    NONDEFAULT,
    MASTERCELLS,
    EXTRA_DEMAND,
    CELL_INST,
    NETS,
    ROUTES,
};

bool
GgridMgr::readFile(const string& filename)
{
    fstream file;
    file.open(filename,ios::in);
    if(!file){return 0;}
    else{
        string str;
        int flag=0;
        while (getline(file,str)){
            if(str.substr(0,11)=="MaxCellMove"){flag=0;}
            else if(str.substr(0,16)=="GGridBoundaryIdx"){flag=1;}
            else if(str.substr(0,8)=="NumLayer"){flag=2;}
            else if(str.substr(0,24)=="NumNonDefaultSupplyGGrid"){flag=3;}
            else if(str.substr(0,13)=="NumMasterCell"){flag=4;}
            else if(str.substr(0,26)=="NumNeighborCellExtraDemand"){flag=5;}
            else if(str.substr(0,11)=="NumCellInst"){flag=6;}
            else if(str.substr(0,7)=="NumNets"){flag=7;}
            else if(str.substr(0,9)=="NumRoutes"){flag=8;}
            switch(flag)
            {
                case 0:
                    read_max_move(str);
                    break;
                case GRIDS:
                    read_Ggrids(str);break;
                case LAYERS:
                    read_Layers(str);break;
                case NONDEFAULT:
                    read_nonDefault(str);break;
                case MASTERCELLS:
                    read_MasterCells(str);break;
                case EXTRA_DEMAND:
                    read_extra_demand(str);break;
                case CELL_INST:
                    read_cellInst(str);break;
                case NETS:
                    read_nets(str);break;
                case ROUTES:
                    read_routes(str);break;
            }
        }
    }
    file.close();
    std::cout<<"end of reading..."<<endl;
    return true;
}

void
GgridMgr::read_max_move(string& str)
{
    int num=0;
    stringstream ss(str.substr(11));
    ss>>num;
    max_move=num;
}


void
GgridMgr::read_Ggrids(string& str)
{
    int num=0;
    stringstream ss(str.substr(16));
    ss>>num;
    row[0]=num;
    ss>>num;
    column[0]=num;
    ss>>num;
    row[1]=num;
    ss>>num;
    column[1]=num;
}

void
GgridMgr::read_Layers(string& str)
{
    if(str.substr(0,8)=="NumLayer"){
        int num=0;
        stringstream ss(str.substr(8));
        ss>>num;
        height=num;
    }
    else if(str.substr(0,3)=="Lay"){
        stringstream ss(str.substr(3));
        string name;
        char d;
        int index;
        ss>>name;
        ss>>index;
        ss>>d;
        if(supply.empty()){supply.resize(height);}
        if(direction.empty()){direction.resize(height);}
        ss>>supply[index-1];
        if(d=='H'){direction[index-1]=1;}
        else if(d=='V'){direction[index-1]=0;}
        Layer* l=new Layer(name,index, d, supply[index-1]);
        layers.push_back(l);
    }

}

void
GgridMgr::read_nonDefault(string& str)
{
    if(Ggrids.empty()){this->build_ggrids();}
    if(str[0]-'0'>=0 &&str[0]-'9'<=0){
        int x,y,z,s;
        stringstream ss(str);
        ss>>x;
        ss>>y;
        ss>>z;
        ss>>s;
        Ggrids[x][y][z]->setSupply(s);
    }
    else{
        int num=0;
        stringstream ss(str.substr(8));
        ss>>num;
        non_default=num;
    }
}

void
GgridMgr::build_ggrids()
{
    for(int i=row[0];i<=row[1];i++){
        if(Ggrids.empty()){Ggrids.resize(row[1]+1);}
        for(int j=column[0];j<=column[1];j++){
            if(Ggrids[i].empty()){Ggrids[i].resize(column[1]+1);}
            for(int k=1;k<=height;k++){
                if(Ggrids[i][j].empty()){Ggrids[i][j].resize(height+1);}
                Ggrid* gg=new Ggrid(supply[k-1],direction[k-1],i,j,k);
                Ggrids[i][j][k]=gg;
            }
        }
    }
}

void
GgridMgr::read_MasterCells(string& str)
{
    if(str.substr(0,13)=="NumMasterCell"){
        stringstream s1(str.substr(13));
        int num;
        s1>>num;
        MC=new MCtype(num);
    }
    else if(str.substr(0,10)=="MasterCell"){
        stringstream s1(str.substr(10));
        int num,pinnum,blkgnum;
        string name;
        s1>>name;
        stringstream s2(name.substr(2));
        s2>>num;
        s1>>pinnum;
        s1>>blkgnum;
        MCindex=num-1;
        MC->writeMC(MCindex,name,pinnum,blkgnum);
    }
    else if(str.substr(0,3)=="Pin"){
        string name;
        string layer;
        int num;
        int l_num;
        stringstream ss(str.substr(3));
        ss>>name;
        ss>>layer;
        stringstream s1(name.substr(1));
        s1>>num;
        stringstream s2(layer.substr(1));
        s2>>l_num;
        MC->writePin(MCindex,num-1,name,layers[l_num-1]);
    }
    else if(str.substr(0,4)=="Blkg"){
        string name, layer;
        int num,demand,l_num;
        stringstream ss(str.substr(4));
        ss>>name;
        ss>>layer;
        ss>>demand;
        stringstream s1(name.substr(1));
        s1>>num;
        stringstream s2(layer.substr(1));
        s1>>l_num;
        MC->writeBlkg(MCindex,num-1,name,layers[l_num-1],demand);
    }
}

void//TODO
GgridMgr::read_extra_demand(string& str)
{
    if(str.substr(0,26)=="NumNeighborCellExtraDemand"){
        int num;
        stringstream ss(str.substr(26));
        ss>>num;
    }
    else if(str.substr(0,8)=="sameGGrid"){
        stringstream ss(str.substr(8));
        string MC1,MC2,M1;
        int demand;
        ss>>MC1;
        ss>>MC2;
        ss>>M1;
        ss>>demand;
        Layer* l = layers[0];
        for (int i=0;i<layers.size();i++){
            if(M1==layers[i]->get_name()){
                l=layers[i];
            }
        }
        sameDemand* s = new sameDemand(MC1, MC2, l, demand);
        sameDemands.push_back(s);
    }
    else if(str.substr(0,8)=="adjHGGrid"){
        stringstream ss(str.substr(8));
        string MC1,MC2,M1;
        int demand;
        ss>>MC1;
        ss>>MC2;
        ss>>M1;
        ss>>demand;
        Layer* l = layers[0];
        for (int i=0;i<layers.size();i++){
            if(M1==layers[i]->get_name()){
                l=layers[i];
            }
        }
        adjDemand* a = new adjDemand(MC1, MC2, l, demand);
        adjDemands.push_back(a);
    }
}

void
GgridMgr::read_cellInst(string& str)
{
    static int cnt = 0;
    if(str.substr(0,11)=="NumCellInst"){
        int num;
        stringstream ss(str.substr(11));
        ss>>num;//TODO
        return;
    }
    else if (str.substr(0,8)=="CellInst"){
        stringstream ss(str.substr(8));
        string name,type,mobility;
        int Row,Column;
        bool m;
        ss>>name;
        ss>>type;
        ss>>Row;
        ss>>Column;
        ss>>mobility;
        if(mobility=="Fixed"){m=0;}
        else if (mobility=="Movable"){m=1;}
        CellInst* CI=new CellInst(Row,Column,name,type,m,MC);

        CellInsts.push_back(CI);
    }
    int x = CellInsts[cnt]->getRow();
    int y = CellInsts[cnt]->getCol();
    for (int j=0; j<CellInsts[cnt]->get_num_blkg(); j++) {
        Ggrids[x][y][CellInsts[cnt]->get_blkg()[j].getlayer().getHeight()]->demanding(CellInsts[cnt]->get_blkg()[j].getdemand());
    }

    for(int i=0;i<CellInsts.size();i++){
        int x1=CellInsts[i]->getRow();
        int y1=CellInsts[i]->getCol();
        string type=CellInsts[i]->get_type();
        for (int j=i+1;j<CellInsts.size();j++){
            if(x1==CellInsts[j]->getRow() && y1==CellInsts[j]->getCol()){
                string type2=CellInsts[j]->get_type();
                for(int k=0;k<sameDemands.size();k++){
                    if(sameDemands[k]->is_sameDemand(type,type2)){
                            //cout<<"same"<<endl;
                        Ggrids[x1][y1][sameDemands[k]->get_Layer_height()]->demanding(sameDemands[k]->get_demand());
                    }
                }
            }
            else if(x==CellInsts[j]->getRow() && (y1-CellInsts[j]->getCol()) == 1) {
                string type2=CellInsts[j]->get_type();
                for(int k=0;k<adjDemands.size();k++){
                    if(adjDemands[k]->is_adjDemand(type,type2)){
                        Ggrids[x1][y1][adjDemands[k]->get_Layer_height()]->demanding(adjDemands[k]->get_demand());
                        //cout<<"adj"<<endl;
                    }
                }
            }
             else if(x==CellInsts[j]->getRow() && y1-CellInsts[j]->getCol() == -1) {
                string type2=CellInsts[j]->get_type();
                for(int k=0;k<adjDemands.size();k++){
                    if(adjDemands[k]->is_adjDemand(type,type2)){
                        if(!CellInsts[i]->get_right() && !CellInsts[j]->get_left()){
                            CellInsts[i]->set_right(1);CellInsts[j]->set_left(1);
                            Ggrids[x1][y1][adjDemands[k]->get_Layer_height()]->demanding(adjDemands[k]->get_demand());
                            Ggrids[CellInsts[j]->getRow()][CellInsts[j]->getCol()][adjDemands[k]->get_Layer_height()]->demanding(adjDemands[k]->get_demand());
                        }
                        //cout<<"adj"<<endl;
                    }
                }
            }
        }
    }
    cnt++;
}

void //TODO
GgridMgr::read_nets(string& str)
{
    static int net_num=0,pin_num=0;
    if(str.substr(0,7)=="NumNets"){
        stringstream ss(str.substr(7));
        int num;
        ss>>num;
    }
    else if (str.substr(0,3)=="Net"){
        stringstream ss(str.substr(3));
        string netname,constr;
        int num;
        ss>>netname;
        ss>>num;
        ss>>constr; //no constraint??
        Layer* l = layers[0];
        for (int i=0;i<layers.size();i++){
            if(constr==layers[i]->get_name()){
                l=layers[i];
            }
        }
        Net* n=new Net(netname,num,*l,layers);
        Nets.push_back(n);
        net_num++;
        pin_num=0;
    }
    else if (str.substr(0,3)=="Pin"){
        stringstream ss(str.substr(3));
        string s,cell_name,pin_name;
        Pin* p=new Pin();
        ss>>s;
        int i;
        for (i=0; i<s.length(); i++) {
            if (s[i] == '/') break;
            cell_name += s[i];
        }
        for (i+=1; i<s.length(); i++) {
            pin_name += s[i];
        }
        for (unsigned j=0;j<CellInsts.size();j++){
            if(cell_name==CellInsts[j]->get_name()){
                p->set_cell_name(cell_name);
                *p=CellInsts[j]->get_pin(pin_name);
                Nets[net_num-1]->writePin(pin_num,*p);
                CellInsts[j]->set_net(Nets[net_num-1]);
            }
        }
        pin_num++;
    }
}

void //TODO
GgridMgr::read_routes(string& str)
{
    static int net_num_2=0;
    static string net_name="";
    if(str.substr(0,9)=="NumRoutes"){
        stringstream ss(str.substr(9));
        int num;
        ss>>num;
    }
    else if(str[0]-'0'>=0 &&str[0]-'9'<=0){
        stringstream ss(str);
        int sr,sc,sl,er,ec,el,num;
        string name;
        ss>>sr;
        ss>>sc;
        ss>>sl;
        ss>>er;
        ss>>ec;
        ss>>el;
        ss>>name;
        if(name!=net_name){net_name=name;net_num_2=0;}
        Route* r=new Route(sr,sc,sl,er,ec,el);
        r->demanding(Ggrids);
        for(int i=0;i<Nets.size();i++){
            if(Nets[i]->get_name()==name){
                Nets[i]->add_route(net_num_2,r);

            }
        }
    }
}


void
GgridMgr::write_file()
{
    fstream file;
    file.open("output.txt",ios::out);
    if(file.is_open()){
        file<<"NumMovedCellInst "<<moved.size()<<endl;
        for(int i=0;i<moved.size();i++){
            file<<"CellInst "<<moved[i]->get_name()<<" "<<moved[i]->getRow()<<" "<<moved[i]->getCol()<<endl;
        }
        int num_of_total_toutes=0;
        for(int i=0;i<Nets.size();i++){
                num_of_total_toutes+=Nets[i]->get_num_route()+Nets[i]->get_num_route_I()+Nets[i]->get_num_route_S();
            }
        file<<"NumRoutes "<<num_of_total_toutes<<endl;
        for(int i=0;i<Nets.size();i++){
            for(int j=0;j<Nets[i]->get_num_route();j++){
                //file<<"routes:"<<endl;
                file<<Nets[i]->get_routes()[j].get_sRowIdx()<<" "
                <<Nets[i]->get_routes()[j].get_sColIdx()<<" "
                <<Nets[i]->get_routes()[j].get_sLayIdx()<<" "
                <<Nets[i]->get_routes()[j].get_eRowIdx()<<" "
                <<Nets[i]->get_routes()[j].get_eColIdx()<<" "
                <<Nets[i]->get_routes()[j].get_eLayIdx()<<" "
                <<Nets[i]->get_name()<<endl;
            }
            for(int j=0;j<Nets[i]->get_num_route_I();j++){
                //file<<"routes_I:"<<endl;
                file<<Nets[i]->get_routes_I()[j]->get_sRowIdx()<<" "
                <<Nets[i]->get_routes_I()[j]->get_sColIdx()<<" "
                <<Nets[i]->get_routes_I()[j]->get_sLayIdx()<<" "
                <<Nets[i]->get_routes_I()[j]->get_eRowIdx()<<" "
                <<Nets[i]->get_routes_I()[j]->get_eColIdx()<<" "
                <<Nets[i]->get_routes_I()[j]->get_eLayIdx()<<" "
                <<Nets[i]->get_name()<<endl;
            }
            for(int j=0;j<Nets[i]->get_num_route_S();j++){
                //file<<"routes_S:"<<endl;
                file<<Nets[i]->get_routes_S()[j]->get_sRowIdx()<<" "
                <<Nets[i]->get_routes_S()[j]->get_sColIdx()<<" "
                <<Nets[i]->get_routes_S()[j]->get_sLayIdx()<<" "
                <<Nets[i]->get_routes_S()[j]->get_eRowIdx()<<" "
                <<Nets[i]->get_routes_S()[j]->get_eColIdx()<<" "
                <<Nets[i]->get_routes_S()[j]->get_eLayIdx()<<" "
                <<Nets[i]->get_name()<<endl;
            }
        }
    }
    else{/*error*/}
    file.close();
    std::cout<<"end of writing..."<<endl;
    return;
}



//placement


void
GgridMgr::placement(vector<CellInst*>& Cells)
{
    static int cnt = 0;
    for (unsigned j=0;j<CellInsts.size();j++) {
        CellInsts[j]->set_fanout();
    }
    for(int i=0;i<Cells.size();i++){
        if(Cells[i]->is_moveable()){
            double newRow=0,newCol=0;
            int maxRow=0,minRow=0,maxCol=0,minCol=0;
            for (int j=0;j<Cells[i]->getfanout().size();j++){
                int x=0,y=0;
                x=Cells[i]->getfanout()[j]->getRowIdx();
                //cout<<j<<" x:"<<x<<endl;
                y=Cells[i]->getfanout()[j]->getColIdx();
                //cout<<j<<" y:"<<y<<endl;
                if(j==0){maxRow=x;minRow=x;maxCol=y;minCol=y;}
                newRow+=x;
                newCol+=y;
                if (x > maxRow) {maxRow = x;}
                if (x < minRow) {minRow = x;}
                if (y > maxCol) {maxCol = y;}
                if (y > minCol) {minCol = y;}
            }
            newRow=newRow/Cells[i]->getfanout().size();
            newRow=round(newRow);
            newCol=newCol/Cells[i]->getfanout().size();
            //cout<<Cells[i]->getfanout().size()<<endl;
            newCol=round(newCol);
            bool change = 0;

            //cout<<Cells[i]->get_name()<<" "<<Cells[i]->getRow()<<" "<<Cells[i]->getCol()<<"->"<<newRow<<" "<<newCol<<endl;
            if(!calculate(Cells[i],newRow,newCol,1)){
                //TODO!!!!!!!!!!!!!!!!!!!!!!!!!!
                int demand[4]={0}, current_x[4]={newRow}, current_y[4]={newCol};
                pair<int, int> p[4];
                if(newRow+1<=row[1]){
                    for(int h=0;h<height;h++){
                        demand[0]+=Ggrids[newRow+1][newCol][h+1]->get_availability();
                    }
                    current_x[0]=newRow+1;
                    current_y[0]=newCol;
                    p[0] = make_pair(demand[0], 0);
                }
                if(newRow-1>=row[0]){
                    for(int h=0;h<height;h++){
                        demand[1]+=Ggrids[newRow-1][newCol][h+1]->get_availability();
                    }
                    current_x[1]=newRow-1;
                    current_y[1]=newCol;
                    p[1] = make_pair(demand[1], 1);
                }
                if(newCol+1<=column[1]){
                    for(int h=0;h<height;h++){
                        demand[2]+=Ggrids[newRow][newCol+1][h+1]->get_availability();
                    }
                    current_x[2]=newRow;
                    current_y[2]=newCol+1;
                    p[2] = make_pair(demand[2], 2);
                }
                if(newCol-1>=column[0]){
                    for(int h=0;h<height;h++){
                        demand[3]+=Ggrids[newRow][newCol-1][h+1]->get_availability();
                    }
                    current_x[3]=newRow;
                    current_y[3]=newCol-1;
                    p[3] = make_pair(demand[3], 3);
                }
                std::sort(p, p+4, [](const std::pair<int,int> &left, const std::pair<int,int> &right)
                {return left.first > right.first;});
                for(int ii=0;ii<4;ii++){
                    if(calculate(Cells[i],current_x[p[ii].second],current_y[p[ii].second],1)){
                        newRow = current_x[p[ii].second];
                        newCol = current_y[p[ii].second];
                        change = 1;
                        //cout<<"OK1 ";
                        //cout<<Cells[i]->get_name()<<" "<<Cells[i]->getRow()<<" "<<Cells[i]->getCol()<<"->"<<newRow<<" "<<newCol<<endl;
                        break;
                    }
                }
            } else{change = 1;/*cout<<"OK2 "<<Cells[i]->get_name()<<" "<<Cells[i]->getRow()<<" "<<Cells[i]->getCol()<<"->"<<newRow<<" "<<newCol<<endl;*/}
            if (change){
                calculate(Cells[i],Cells[i]->getRow(), Cells[i]->getCol(),0);
                Cells[i]->set_newcol(newCol);
                Cells[i]->set_newrow(newRow);
                Cells[i]->set_reduce(maxRow-minRow+maxCol-minCol);
            }
            int w=Cells[i]->get_reduce();
            pair<int,CellInst*> p1;
            p1 = make_pair(w, Cells[i]);
            moveable_Cells.push_back(p1);
        }
        else{continue;}
    }
    std::sort(moveable_Cells.begin(), moveable_Cells.end(), [](const std::pair<int,CellInst*> &left, const std::pair<int,CellInst*> &right)
        {return left.first > right.first;});
    for (int i=0;i<max_move;i++){
        moveable_Cells[i].second->set_Idx();
        if(cnt == 0){
            moved.push_back(moveable_Cells[i].second);
            //cout<<moved[i]->get_name()<<" "<<moved[i]->getRow()<<" "<<moved[i]->getCol()<<endl;

            if (moved_nets.size() == 0) {
                moved_nets = moved[i]->get_net();
            }
            for (int j=0; j<moved[i]->get_net().size(); j++) {
                bool check = 1;
                for (int k=0; k<moved_nets.size(); k++) {
                    if (moved_nets[k]->get_name() == moved[i]->get_net()[j]->get_name()){
                        check = 0;
                        break;
                    }
                }
                if (check) {
                    moved_nets.push_back(moved[i]->get_net()[j]);
                }
            }
            std::sort(moved_nets.begin(), moved_nets.end(), [](Net* &left, Net* &right)
            {return left->get_num_pin() < right->get_num_pin();});
        }
        moveable_Cells[i].second->reset_net(Ggrids);

        //set routing
        //recursive??
    }


    cnt++;
    if (cnt < 3) {placement(moved);}
}




//routing

void
GgridMgr::routing()
{
    std::cout<<"end of placement..."<<endl;
    for (int i=0; i<moved_nets.size(); i++) {
        //cout << moved_nets[i]->get_length() <<endl;
        moved_nets[i]->update_length();
    }
    for (unsigned j=0;j<CellInsts.size();j++) {
        CellInsts[j]->set_fanout();
    }
    //cout<<"what???"<<endl;

    for (int i=0; i<moved_nets.size(); i++){
        cout<<moved_nets[i]->get_name()<<endl;
        //cout<<"decideLayer"<<endl;
        moved_nets[i]->decideLayer();
        //cout<<"pinList"<<endl;
        moved_nets[i]->pinList();
        //cout<<"routing_2D"<<endl;        
        moved_nets[i]->routing_2D(Ggrids);
        //cout<<"end_of_routing_2D"<<endl;                
    }
    std::cout<<"end_of_routing..."<<endl;
}



bool
GgridMgr::calculate(CellInst* Cells,int newRow, int newCol,bool d)
{
    int demand_same[height] = {0};
    int demand_adj[height] = {0};
    int demand_blkg[height] = {0};
    if(d){
        for(int a=0;a<CellInsts.size();a++){
            if(newRow==CellInsts[a]->getRow()&& newCol==CellInsts[a]->getCol()&& Cells->get_name()!=CellInsts[a]->get_name()&& CellInsts[a]->get_same()==0){
                for (int c=0; c<sameDemands.size(); c++) {
                    if (sameDemands[c]->is_sameDemand(Cells->get_type(), CellInsts[a]->get_type())) {
                        demand_same[sameDemands[c]->get_Layer_height()-1] += sameDemands[c]->get_demand();
                        //cout << "samegg..." << endl;
                    }
                }
            }
            else if (newRow==CellInsts[a]->getRow() && newCol-CellInsts[a]->getCol()==1 && Cells->get_name()!=CellInsts[a]->get_name()&&CellInsts[a]->get_right()==0){
                for (int c=0; c<adjDemands.size(); c++) {
                    if (adjDemands[c]->is_adjDemand(Cells->get_type(), CellInsts[a]->get_type())) {
                        if (!Ggrids[CellInsts[a]->getRow()][CellInsts[a]->getCol()][adjDemands[c]->get_Layer_height()]->ggridok(adjDemands[c]->get_demand())){
                            return false;
                        }
                        demand_adj[adjDemands[c]->get_Layer_height()-1] += adjDemands[c]->get_demand();
                    }
                }
            }
            else if (newRow==CellInsts[a]->getRow() && newCol-CellInsts[a]->getCol()==-1 && Cells->get_name()!=CellInsts[a]->get_name()&& CellInsts[a]->get_left()==0){
                for (int c=0; c<adjDemands.size(); c++) {
                    if (adjDemands[c]->is_adjDemand(Cells->get_type(), CellInsts[a]->get_type())) {
                        if (!Ggrids[CellInsts[a]->getRow()][CellInsts[a]->getCol()][adjDemands[c]->get_Layer_height()]->ggridok(adjDemands[c]->get_demand())){
                            return false;
                        }
                        demand_adj[adjDemands[c]->get_Layer_height()-1] += adjDemands[c]->get_demand();
                    }
                }
            }
        }
    }
    else{
        for(int a=0;a<CellInsts.size();a++){
            if(newRow==CellInsts[a]->getRow()&& newCol==CellInsts[a]->getCol()&& Cells->get_name()!=CellInsts[a]->get_name()&& CellInsts[a]->get_same()==1){
                for (int c=0; c<sameDemands.size(); c++) {
                    if (sameDemands[c]->is_sameDemand(Cells->get_type(), CellInsts[a]->get_type())) {
                        demand_same[sameDemands[c]->get_Layer_height()-1] += sameDemands[c]->get_demand();
                        //cout << "samegg..." << endl;
                    }
                }
            }
            else if (newRow==CellInsts[a]->getRow() && newCol-CellInsts[a]->getCol()==1 && Cells->get_name()!=CellInsts[a]->get_name()&&CellInsts[a]->get_right()==1){
                for (int c=0; c<adjDemands.size(); c++) {
                    if (adjDemands[c]->is_adjDemand(Cells->get_type(), CellInsts[a]->get_type())) {
                        demand_adj[adjDemands[c]->get_Layer_height()-1] += adjDemands[c]->get_demand();
                    }
                }
            }
            else if (newRow==CellInsts[a]->getRow() && newCol-CellInsts[a]->getCol()==-1 && Cells->get_name()!=CellInsts[a]->get_name()&& CellInsts[a]->get_left()==1){
                for (int c=0; c<adjDemands.size(); c++) {
                    if (adjDemands[c]->is_adjDemand(Cells->get_type(), CellInsts[a]->get_type())) {
                        demand_adj[adjDemands[c]->get_Layer_height()-1] += adjDemands[c]->get_demand();
                    }
                }
            }
        }
    }
    for (int b=0; b<Cells->get_num_blkg(); b++) {
        demand_blkg[Cells->get_blkg()[b].getlayer().getHeight()-1] += Cells->get_blkg()[b].getdemand();
    }


    if (d) {
        for(int b=0;b<height;b++){
            if(!Ggrids[newRow][newCol][b+1]->ggridok(demand_same[b]+demand_adj[b]+demand_blkg[b])){
                //TODO!!!!!!!!!!!!!!!!!!!!!
                //cout<<"Not OK QAQ ";
                //cout<<Cells->get_name()<<" "<<Cells->getRow()<<" "<<Cells->getCol()<<"->"<<newRow<<" "<<newCol<<endl;
                return false;
            }
        }
    }


    for(int b=0;b<height;b++){
        if(d){
            Ggrids[newRow][newCol][b+1]->demanding(demand_same[b]+demand_adj[b]+demand_blkg[b]);
            for (int c=0; c<sameDemands.size(); c++) {
                for(int a=0;a<CellInsts.size();a++){
                    if (newRow==CellInsts[a]->getRow()&&newCol==CellInsts[a]->getCol() && Cells->get_name()!=CellInsts[a]->get_name()){
                        if (sameDemands[c]->is_sameDemand(Cells->get_type(), CellInsts[a]->get_type())) {
                            CellInsts[a]->set_same(1);
                            Cells->set_same(1);
                        }
                    }
                }
            }
            for (int c=0; c<adjDemands.size(); c++) {
                for(int a=0;a<CellInsts.size();a++){
                    if (newRow==CellInsts[a]->getRow()&&newCol-CellInsts[a]->getCol()==1 && Cells->get_name()!=CellInsts[a]->get_name()){
                        if (adjDemands[c]->is_adjDemand(Cells->get_type(), CellInsts[a]->get_type())) {
                            Ggrids[CellInsts[a]->getRow()][CellInsts[a]->getCol()][adjDemands[c]->get_Layer_height()]->demanding(adjDemands[c]->get_demand());
                            CellInsts[a]->set_right(1);
                            Cells->set_left(1);
                        }
                    }
                    else if (newRow==CellInsts[a]->getRow()&&newCol-CellInsts[a]->getCol()==-1 && Cells->get_name()!=CellInsts[a]->get_name()){
                        if (adjDemands[c]->is_adjDemand(Cells->get_type(), CellInsts[a]->get_type())) {
                            Ggrids[CellInsts[a]->getRow()][CellInsts[a]->getCol()][adjDemands[c]->get_Layer_height()]->demanding(adjDemands[c]->get_demand());
                            CellInsts[a]->set_left(1);
                            Cells->set_right(1);
                        }
                    }
                }
            }
        }
        else{
            Ggrids[newRow][newCol][b+1]->returning(demand_same[b]+demand_adj[b]+demand_blkg[b]);
            for (int c=0; c<sameDemands.size(); c++) {
                for(int a=0;a<CellInsts.size();a++){
                    if (newRow==CellInsts[a]->getRow()&&newCol==CellInsts[a]->getCol() && Cells->get_name()!=CellInsts[a]->get_name()){
                        if (sameDemands[c]->is_sameDemand(Cells->get_type(), CellInsts[a]->get_type())) {
                            CellInsts[a]->set_same(1);
                            Cells->set_same(1);
                        }
                    }
                }
            }
            for (int c=0; c<adjDemands.size(); c++) {
                for(int a=0;a<CellInsts.size();a++){
                    if (newRow==CellInsts[a]->getRow() && newCol-CellInsts[a]->getCol()==1 && Cells->get_name()!=CellInsts[a]->get_name()){
                        if (adjDemands[c]->is_adjDemand(Cells->get_type(), CellInsts[a]->get_type())) {
                            Ggrids[CellInsts[a]->getRow()][CellInsts[a]->getCol()][adjDemands[c]->get_Layer_height()]->returning(adjDemands[c]->get_demand());
                            CellInsts[a]->set_right(0);
                        }
                    }
                    else if (newRow==CellInsts[a]->getRow() && newCol-CellInsts[a]->getCol()==-1 && Cells->get_name()!=CellInsts[a]->get_name()){
                        if (adjDemands[c]->is_adjDemand(Cells->get_type(), CellInsts[a]->get_type())) {
                            Ggrids[CellInsts[a]->getRow()][CellInsts[a]->getCol()][adjDemands[c]->get_Layer_height()]->returning(adjDemands[c]->get_demand());
                            CellInsts[a]->set_left(0);
                        }
                    }
                }
            }
        }
    }
    return true;
}
