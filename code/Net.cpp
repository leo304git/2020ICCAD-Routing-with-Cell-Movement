#include "Net.h"
#include <iostream>
#include <stdlib.h>

Net::Net()
{
    num_pin = 0;
}

Net::Net(string Net_name,int n, Layer& l, vector<Layer*>& lyr)
{
    num_pin = n;
    min_layer = &l;
    pins = new Pin*[num_pin];
    num_route = 0;
    num_route_I=0;
    num_route_S=0;
    num_row=0;
    num_column=0;
    name=Net_name;
    num_theConnected = 1; //new
    num_unConnected = 0; //new
    layers = lyr;
    num_layer=lyr.size();
}

Net::~Net()
{
    //dtor
}

void Net::writePin(int pin_index, Pin& p)
{
    pins[pin_index] = &p;
    return;
}

void Net::decideLayer() //new
{
    int layer_dist = 0;
    int min_layer_dist = 0;
    int min_layer_index = min_layer->getHeight()-1;
    for(int j=0; j<num_pin; j++)
    {
        min_layer_dist += abs(pins[j]->getLayIdx() - min_layer->getHeight());
    }
    for(int i=min_layer->getHeight()-1; i<num_layer; i++) // i start form zero
    {
        for(int j=0; j<num_pin; j++)
        {
            layer_dist += abs(pins[j]->getLayIdx() - layers[i]->getHeight());
        }
        if(layer_dist < min_layer_dist)
        {
            min_layer_dist = layer_dist;
            min_layer_index = i;
        }
        layer_dist = 0;
    }
    /*
    if(layers[min_layer_index]->getDirection() == 'H')
    {
        routingLayer_H = layers[min_layer_index];  // so that routingLayer_H->getHeight() == i
        routingLayer_V = layers[min_layer_index+1];
    }
    else
    {
        routingLayer_V = layers[min_layer_index];  // so that routingLayer_V->getHeight() == i
        routingLayer_H = layers[min_layer_index+1];
    }*/
    if(layers[min_layer_index]->getDirection() == 'H'){
        //cout<<layers[min_layer_index+1]<<endl;
        routingLayer_H = layers[min_layer_index]; // so that routingLayer_H->getHeight() == i
        routingLayer_H_index = min_layer_index;
        //cout<<num_layer<<endl;
        if(min_layer_index-1 >=  min_layer->getHeight()-1)
        {
            routingLayer_V = layers[min_layer_index-1];
            routingLayer_V_index = min_layer_index-1;
        }
        else if(min_layer_index+1 < num_layer)
        {
            routingLayer_V = layers[min_layer_index+1];
            routingLayer_V_index = min_layer_index+1;
        }
        else
        {
            routingLayer_V = layers[min_layer_index];
            routingLayer_V_index = min_layer_index;
        }
    }
    else
    {
        routingLayer_V = layers[min_layer_index]; // so that routingLayer_V->getHeight() == i
        routingLayer_V_index = min_layer_index;
       if(min_layer_index-1 >=  min_layer->getHeight()-1)
        {
            routingLayer_H = layers[min_layer_index-1];
            routingLayer_H_index = min_layer_index-1;
        }
        else if(min_layer_index+1 < num_layer)
        {
            routingLayer_H = layers[min_layer_index+1];
            routingLayer_H_index = min_layer_index+1;
        }
        else
        {
            routingLayer_H = layers[min_layer_index];
            routingLayer_H_index = min_layer_index;
        }
    }
    cout<<" routingLayer_H "<<  routingLayer_H->getDirection()<<" "<<routingLayer_H->getHeight()<<endl;
    cout<<" routingLayer_V "<<  routingLayer_V->getDirection()<<" "<<routingLayer_V->getHeight()<<endl;    

    return;

}


int Net::manhattanDist(Pin* p1, Pin* p2)
{
    int rowDist, colDist;
    rowDist = abs(p1->getRowIdx() - p2->getRowIdx());
    colDist = abs(p1->getColIdx() - p2->getColIdx());
    return rowDist + colDist;
}

void Net::pinList()
{
    int current_min_dist;
    int i = 0; //current pin index
    int num_listed = 1;
    cout<<"num_pin"<<num_pin<<endl;
    while(num_listed < num_pin)
    {
        int min_index = 0;
        current_min_dist = -1; //any impossible number
        for(int j=0; j<num_pin; j++)
        {
            if(j != i && pins[j]->isListed()==false)
            {
                if(current_min_dist == -1)
                {
                    current_min_dist = manhattanDist(pins[i], pins[j]);
                    pins[i]->setNet_next_pin(pins[j]);
                    min_index = j;
                }
                else
                {
                    if(manhattanDist(pins[i], pins[j]) < current_min_dist)
                    {
                        pins[i]->getNet_next_pin()->deListed();
                        current_min_dist = manhattanDist(pins[i], pins[j]);
                        pins[i]->setNet_next_pin(pins[j]);
                        min_index = j;
                    }
                }
            }
        }
        i = min_index;
        num_listed++;
    }
    pins[i]->setNet_next_pin(pins[0]); //circulate

    //test

    int counter=0;
    Pin* current_pin;
    current_pin = pins[0];
    while(counter<num_pin)
    {
        int x = current_pin->getRowIdx();
        int y = current_pin->getColIdx();
        int z= current_pin->getLayIdx();
        cout << "<" << x << "," << y <<" "<<z<<">" << " -> ";
        current_pin = current_pin->getNet_next_pin();
        counter++;
    }
    cout << endl;
    return;
}

void Net::routing_2D(vector<vector<vector<Ggrid*>>>& gg)
{
    theConnected = new Pin*[1];
    Pin** unConnected;
    int num_unConnected = 0;
    theConnected[0] = pins[0];
    num_theConnected = 1;
    Pin* to_be_added;
    to_be_added = pins[0]->getNet_next_pin();
    while(to_be_added != pins[0])
    {
        if(add_One_Pin(to_be_added, gg))
        {
            append(theConnected, num_theConnected, to_be_added);
        }
        else
        {
            //template �ݧ�
            append(unConnected, num_unConnected, to_be_added);

        }
        to_be_added = to_be_added->getNet_next_pin();

    }

    //new
    

    while(routingLayer_H_index < num_layer && routingLayer_V_index < num_layer
            && num_unConnected != 0)
    {
        bool makeprogress = true;
        routingLayer_H = layers[routingLayer_H_index];
        routingLayer_V = layers[routingLayer_V_index];
        cout << "routingLayer_H_index = " << routingLayer_H_index << endl;
        cout << "routingLayer_V_index = " << routingLayer_V_index << endl;
        for(int i=0; i<num_unConnected; i++)
        cout << "unConnected " << i << " = " << unConnected[i]->getRowIdx() << " " << unConnected[i]->getColIdx()<<" "<<unConnected[i]->getLayIdx() << endl;
        while(makeprogress == true && num_unConnected != 0)
        {
            makeprogress = false;
            Pin** temp;
            int num_temp = 0;
            cout<<"num_unConnected "<<num_unConnected<<endl;
            for(int i=0; i<num_unConnected; i++)
            {
                if(add_One_Pin(unConnected[i], gg))
                {
                    makeprogress = true;
                    append(theConnected, num_theConnected, unConnected[i]);
                }
                else
                {
                    append(temp, num_temp, unConnected[i]);
                }
            }
             for(int i=0; i<num_temp; i++)
            cout << "temp " << i << " = " << temp[i]->getRowIdx() << " " << temp[i]->getColIdx()<<" "<<temp[i]->getLayIdx() << endl;
            unConnected = temp;
            //delete temp;
            num_unConnected = num_temp;
        }
        if(routingLayer_H_index < routingLayer_V_index)
        {
            routingLayer_H_index += 2;
        }
        else
        {
            routingLayer_V_index += 2;
        }
    }


    


    //rearrange
    for(int i=0;i<num_route;i++){
        int x1=routes[i].get_sRowIdx();
        int x2=routes[i].get_eRowIdx();
        int y1=routes[i].get_sColIdx();
        int y2=routes[i].get_eColIdx();
        int z1=routes[i].get_sLayIdx();
        int z2=routes[i].get_eLayIdx();
        for(int x=min(x1,x2);i<=max(x1,x2);i++){
            for(int y=min(y1,y2);i<=max(y1,y2);i++){
                for(int z=min(z1,z2);i<=max(z1,z2);i++){
                    gg[x][y][z]->demanding(1);
                }
            }
        }
    }
    for(int i=0;i<num_route_I;i++){
        int x1=routes_I[i]->get_sRowIdx();
        int x2=routes_I[i]->get_eRowIdx();
        int y1=routes_I[i]->get_sColIdx();
        int y2=routes_I[i]->get_eColIdx();
        int z1=routes_I[i]->get_sLayIdx();
        int z2=routes_I[i]->get_eLayIdx();
        for(int x=min(x1,x2);i<=max(x1,x2);i++){
            for(int y=min(y1,y2);i<=max(y1,y2);i++){
                for(int z=min(z1,z2);i<=max(z1,z2);i++){
                    gg[x][y][z]->demanding(1);
                }
            }
        }
    }
    for(int i=0;i<num_route_S;i++){
        int x1=route_S[i]->get_sRowIdx();
        int x2=route_S[i]->get_eRowIdx();
        int y1=route_S[i]->get_sColIdx();
        int y2=route_S[i]->get_eColIdx();
        int z1=route_S[i]->get_sLayIdx();
        int z2=route_S[i]->get_eLayIdx();
        for(int x=min(x1,x2);i<=max(x1,x2);i++){
            for(int y=min(y1,y2);i<=max(y1,y2);i++){
                for(int z=min(z1,z2);i<=max(z1,z2);i++){
                    gg[x][y][z]->demanding(1);
                }
            }
        }
    }


   cout << "routing 2D:" << endl;
    for(int i=0; i<num_route; i++)
    {

        cout << "( " << routes[i].sRowIdx << ", " << routes[i].sColIdx << ", " << routes[i].sLayIdx
            << ",, " << routes[i].eRowIdx << ", " << routes[i].eColIdx << ", " << routes[i].eLayIdx << " )" << endl;
    }
    cout << "routing I:" << endl;
    for(int i=0; i<num_route_I; i++)
    {

        cout << "( " << routes_I[i]->sRowIdx << ", " << routes_I[i]->sColIdx << ", " << routes_I[i]->sLayIdx
            << ",, " << routes_I[i]->eRowIdx << ", " << routes_I[i]->eColIdx << ", " << routes_I[i]->eLayIdx << " )" << endl;
    }
    update_length();
    system("pause");
    return;
}

bool Net::add_One_Pin(Pin* p, vector<vector<vector<Ggrid*>>>& gg)
{
    int current_dist_min; // change to theConnected.end()
    int min_pin_index;
    int min_route_index = -1;
    int min_rowIdx;
    int min_colIdx;
    int pin_to_route_dist_min = -1; //any impossible number
    int pin_to_pin_dist_min = -1;

    cout << "to be added: ";
    cout << "( " << p->getRowIdx() << ", " << p->getColIdx() << " )" << endl;
    cout << "num_route: " << num_route << endl;
    /*for(int i=0; i<num_route; i++)
    {
        cout << routes[num_route].sRowIdx << " " << routes[num_route].sColIdx << " " << routes[num_route].sLayIdx << " "
        << routes[num_route].eRowIdx << " " << routes[num_route].eColIdx << " " << routes[num_route].eLayIdx << endl;
    }*/
    if(num_route != 0) //to avoid the case where no route exists
    {
        for(int i=0; i<num_route; i++)
        {
            //cout<< "routes" << i << ".Trelation = " << routes[i].Trelation(p)<<endl;
            if(routes[i].Trelation(p))
            {
                if(pin_to_route_dist_min == -1)
                {
                   pin_to_route_dist_min = routes[i].dist(p);
                   min_route_index = i;
                }
                else
                {
                    if(routes[i].dist(p) < pin_to_route_dist_min)
                    {
                        pin_to_route_dist_min = routes[i].dist(p);
                        min_route_index = i;
                    }
                }
            }
        }
    }
    cout << "num_theConnected = " << num_theConnected << endl;

    for(int i=0; i<num_theConnected; i++)
    {
        //cout<<"min_route_index:" << min_route_index<<endl;
        if((min_route_index!=-1 && !routes[min_route_index].contain(theConnected[i]))
           || min_route_index == -1)
        {
            if(pin_to_pin_dist_min == -1)
            {
                pin_to_pin_dist_min = manhattanDist(theConnected[i], p);
                min_pin_index = i;
                //cout<<"?"<<min_pin_index<<"?"<<endl;
            }
            else
            {

                if(manhattanDist(theConnected[i], p) < pin_to_pin_dist_min)
                {
                    pin_to_pin_dist_min = manhattanDist(theConnected[i], p);
                    min_pin_index = i;
                    //cout<<"!"<<min_pin_index<<"!"<<endl;
                }
            }
        }
    }
    cout << "min_pin_index = " << min_pin_index << endl;

    if(pin_to_route_dist_min == -1)
    {
        //cout << "in" << endl;
        cout<<"1:"<<theConnected[min_pin_index]<<" "<<min_pin_index<<endl;
        if(Lcreate(theConnected[min_pin_index], p, gg))
        {
            for(int i=0; i<num_theConnected; i++)
            {
                cout << "the_Connected" << i << " = " << "(" << theConnected[i]->getRowIdx()
                << "," << theConnected[i]->getColIdx() << ")" << endl;
            }
            for(int i=0; i<num_route; i++)
            {
                cout << "routes" << i << " = " << "(" << routes[i].sRowIdx
                << "," << routes[i].sColIdx << ",," << routes[i].eRowIdx << ","
                << routes[i].eColIdx << ")" << endl;
            }
            cout<<"\n";
            return true;
        }
        else return false;
    }
    else
    {
        if(pin_to_route_dist_min <= pin_to_pin_dist_min || pin_to_pin_dist_min == -1)
        {
            
            //cout << pin_to_route_dist_min <<endl;
            Route r = routes[min_route_index].Tcreate(p);
            //cout << "r_added = " << r.sRowIdx << " " << r.sColIdx << " " << r.eRowIdx << " " << r.eColIdx << endl;
            if(routing_3D(routes[min_route_index], r, p, gg))
            {
                //cout << "in" << endl;
                append(routes, num_route, r);
                //append(theConnected, num_theConnected, r.steiner); ///////////////////////////////////////////////
                for(int i=0; i<num_theConnected; i++)
                {
                    cout << "the_Connected" << i << " = " << "(" << theConnected[i]->getRowIdx()
                    << "," << theConnected[i]->getColIdx() << ")" << endl;
                }
                for(int i=0; i<num_route; i++)
                {
                    cout << "routes" << i << " = " << "(" << routes[i].sRowIdx
                    << "," << routes[i].sColIdx << ",," << routes[i].eRowIdx << ","
                   << routes[i].eColIdx << ")" << endl;
                }
                cout<<"\n";
                return true;
            }
            else //r1, r2 explosion not done
            {
                if(pin_to_pin_dist_min != -1)
                {
                    cout<<"2:"<<theConnected[min_pin_index]<<" "<<min_pin_index<<endl;
                    cout<<theConnected[min_pin_index]->getRowIdx()<<" "<<theConnected[min_pin_index]->getColIdx()<<" "<<theConnected[min_pin_index]->getLayIdx()<<endl;
                    if(Lcreate(theConnected[min_pin_index], p, gg)) return true;
                }
                else return false;
            }
        }
        else
        {
            cout<<"3:"<<theConnected[min_pin_index]<<" "<<min_pin_index<<endl;
            if(Lcreate(theConnected[min_pin_index], p, gg))
            {
                for(int i=0; i<num_theConnected; i++)
                {
                    cout << "the_Connected" << i << " = " << "(" << theConnected[i]->getRowIdx()
                    << "," << theConnected[i]->getColIdx() << ")" << endl;
                }
                for(int i=0; i<num_route; i++)
                {
                    cout << "routes" << i << " = " << "(" << routes[i].sRowIdx
                    << "," << routes[i].sColIdx << ",," << routes[i].eRowIdx << ","
                    << routes[i].eColIdx << ")" << endl;
                }
                cout<<"\n";
                return true;
            }
            else
            {
                Route r = routes[min_route_index].Tcreate(p);
                if(routing_3D(routes[min_route_index], r, p, gg))
                {
                    append(routes, num_route, r);
                    //append(theConnected, num_theConnected, r.steiner); ////////////////////////////////////////////////////////////
                    return true;

                }
                else return false;
            }

        }

    }
}

//append change to template function
void Net::append(Pin**& group, int& length, Pin* p)
{
    cout<<"append 1"<<endl;
    if(length == 0) //newnew
    {
        group = new Pin*[1];
        group[0] = p;
        length = 1;
    }
    else
    {
        Pin** temp = new Pin*[length+1];
        for(int i=0; i<length; i++)
        {
            temp[i] = group[i];
        }
        temp[length] = p;
        group = temp;
        length++;
    }
    return;
}

void Net::append(Route**& group, int& length, Route* r) //new
{
    cout<<"append 2"<<endl;
    if(length == 0)
    {
        group = new Route*[1];
        group[0] = r;
        length = 1;
    //    cout << "routes3D[0]: " << group[0]->sRowIdx << "," << group[0]->sColIdx << "," << group[0]->sLayIdx
    //    << ",," << group[0]->eRowIdx << "," << group[0]->eColIdx << "," << group[0]->eLayIdx << endl;
    }
    else
    {
        Route** temp = new Route*[length+1];
        for(int i=0; i<length; i++)
        {
            temp[i] = group[i];
    //        cout << "routes3D[" << i << "]: " << group[i]->sRowIdx << "," << group[i]->sColIdx << "," << group[i]->sLayIdx
    //        << ",," << group[i]->eRowIdx << "," << group[i]->eColIdx << "," << group[i]->eLayIdx << endl;
        }
        temp[length] = r;
        group = temp;
        length++;
    //    cout << "routes3D[" << length-1 << "]: " << group[length-1]->sRowIdx << "," << group[length-1]->sColIdx << "," << group[length-1]->sLayIdx
    //    << ",," << group[length-1]->eRowIdx << "," << group[length-1]->eColIdx << "," << group[length-1]->eLayIdx << endl;
    }
    return;
}


void Net::append(Route*& group, int& length, Route r)//�]�ӺC�A��
{
    cout<<"append 3"<<endl;    
    //cout<<"!!!"<<group[0].sRowIdx<<","<<group[0].sColIdx<<","<<group[0].eRowIdx<<","<<group[0].eColIdx<<endl;
    if(length == 0)
    {
        group = new Route[1];
        group[0] = r;
        length = 1;
    }
    else
    {
        Route* temp = new Route[length+1];
        for(int i=0; i<length; i++)
        {
            temp[i] = group[i];
        }
        //cout<<"temp[0] : "<<temp[0].sRowIdx<<","<<temp[0].sColIdx<<","<<temp[0].eRowIdx<<","<<temp[0].eColIdx<<endl;
        temp[length] = r;
        group = temp;
        //delete [] temp;
        length++;
    }
    return;
}

bool Net::Lcreate(Pin* p1, Pin* p2, vector<vector<vector<Ggrid*>>>& gg)
{
    cout<<"Lcreate..."<<endl;
    Pin* corner1=new Pin();
    Pin* corner2=new Pin();
    Pin* min_corner;
    corner1->setpos(p1->getRowIdx(), p2->getColIdx());
    corner2->setpos(p2->getRowIdx(), p1->getColIdx());
    corner1->setNet_next_pin(p2->getNet_next_pin());
    corner2->setNet_next_pin(p2->getNet_next_pin());
    corner1->set_corner(); // new
    corner2->set_corner(); // new
    cout<<"lcreate's p1,p2"<<p1->getNet_next_pin()<<" "<<p2->getNet_next_pin()<<endl;
    min_corner = (manhattanDist(corner1, p2->getNet_next_pin())
                    < manhattanDist(corner2, p2->getNet_next_pin()))
                    ? corner1 : corner2;
    Route r1, r2;
    r1.set2Dpos(p1->getRowIdx(), p1->getColIdx(),
                min_corner->getRowIdx(), min_corner->getColIdx());
    r2.set2Dpos(min_corner->getRowIdx(), min_corner->getColIdx(),
                p2->getRowIdx(), p2->getColIdx());
    //cout << "in" << endl;
    //cout << "routing_3D(p1, r1, r2, p2, gg) = " << routing_3D(p1, r1, r2, p2, gg) <<endl;
    if(!routing_3D(p1, r1, r2, p2, gg)) //new
    {
        if(min_corner == corner1) min_corner = corner2;
        else min_corner = corner1;
        r1.set2Dpos(p1->getRowIdx(), p1->getColIdx(),
                min_corner->getRowIdx(), min_corner->getColIdx());
        r2.set2Dpos(min_corner->getRowIdx(), min_corner->getColIdx(),
                p2->getRowIdx(), p2->getColIdx());
        if(!routing_3D(p1, r1, r2, p2, gg)) return false; // new
        else
        {
            cout << "min_corner = " << min_corner->getRowIdx() << " " << min_corner->getColIdx() <<endl;
            cout << "min_corner is corner: " << min_corner->is_corner() << endl;
            append(theConnected, num_theConnected, min_corner);
            append(routes, num_route, r1);
            append(routes, num_route, r2);
            return true;
        }
    }
    else
    {
        cout << "min_corner = " << min_corner->getRowIdx() << " " << min_corner->getColIdx() <<endl;
            cout << "min_corner is corner: " << min_corner->is_corner() << endl;
        append(theConnected, num_theConnected, min_corner);
        append(routes, num_route, r1);
    //    cout<< "number of route = " << num_route<<endl;
    //    cout << ",," << routes[num_route-1].sRowIdx << "," << routes[num_route-1].sColIdx << "," << routes[num_route-1].sLayIdx <<
    //    ",," << routes[num_route-1].eRowIdx << "," << routes[num_route-1].eColIdx << "," << routes[num_route-1].eLayIdx << ",," << endl;
        append(routes, num_route, r2);
    //    cout<< "number of route = " <<num_route<<endl;
    //    cout << ",," << routes[num_route-1].sRowIdx << "," << routes[num_route-1].sColIdx << "," << routes[num_route-1].sLayIdx <<
    //      ",," << routes[num_route-1].eRowIdx << "," << routes[num_route-1].eColIdx << "," << routes[num_route-1].eLayIdx << ",," << endl;
    //    cout << ",," << routes[0].sRowIdx << "," << routes[0].sColIdx << ",," << routes[0].eRowIdx << "," << routes[0].eColIdx << ",," << endl;
        return true;
    }

    /*
    if(!r1.ggridOK() || !r2.ggridOK())
    {
        if(min_corner == corner1) min_corner = corner2;
        else min_corner = corner1;
        r1.set2Dpos(p1->getRowIdx(), p1->getColIdx(),
                min_corner->getRowIdx(), min_corner->getColIdx());
        r2.set2Dpos(min_corner->getRowIdx(), min_corner->getColIdx(),
                p2->getRowIdx(), p2->getColIdx());
        if(!r1.ggridOK() || !r2.ggridOK()) return false;
        else
        {
            append(theConnected, num_theConnected, min_corner);
            append(routes, num_route, r1);
            append(routes, num_route, r2);
            return true;
        }
    }
    else
    {
        append(theConnected, num_theConnected, min_corner);
        append(routes, num_route, r1);
        cout<< "number of route = " << num_route<<endl;
        cout<<"routes[num_route-1] sRow,sCol,eRow,eCol";
        cout << ":" << routes[num_route-1].sRowIdx << "," << routes[num_route-1].sColIdx << ";" << routes[num_route-1].eRowIdx << "," << routes[num_route-1].eColIdx << ",," << endl;
        append(routes, num_route, r2);
        cout<< "number of route = " <<num_route<<endl;
        cout<<"routes[num_route-1] sRow,sCol,eRow,eCol";
        cout << ":" << routes[num_route-1].sRowIdx << "," << routes[num_route-1].sColIdx << ";" << routes[num_route-1].eRowIdx << "," << routes[num_route-1].eColIdx << ",," << endl;
        //cout << ",," << routes[0].sRowIdx << "," << routes[0].sColIdx << ",," << routes[0].eRowIdx << "," << routes[0].eColIdx << ",," << endl;
        return true;
    }
    */
}

bool Net::routing_3D(Route& r_exist, Route& r, Pin* p, vector<vector<vector<Ggrid*>>>& gg) //new
{
    cout<<"routing_3D..."<<endl;
    Route* routeTE = new Route;
    Route* routeTS = new Route;
    if(r.direction() == 'C')
    {
        r.sLayIdx = r_exist.sLayIdx;
        r.eLayIdx = r_exist.eLayIdx;
    }
    if(r.direction() == 'H')
    {
        r.sLayIdx = routingLayer_H->getHeight();
        r.eLayIdx = routingLayer_H->getHeight();
        //routeTE->eLayIdx = routingLayer_V->getHeight();
    }
    else
    {
        r.sLayIdx = routingLayer_V->getHeight();
        r.eLayIdx = routingLayer_V->getHeight();
        //routeTE->eLayIdx = routingLayer_H->getHeight();
    }
    routeTE->sRowIdx = r.eRowIdx;
    routeTE->sColIdx = r.eColIdx;
    routeTE->sLayIdx = r.eLayIdx;
    routeTE->eRowIdx = r.eRowIdx;
    routeTE->eColIdx = r.eColIdx;
    routeTE->eLayIdx = r_exist.eLayIdx;
    //cout << "routeTE: " << routeTE->sRowIdx << "," << routeTE->sColIdx << "," << routeTE->sLayIdx
    //<< ",," << routeTE->eRowIdx << "," << routeTE->eColIdx << "," << routeTE->eLayIdx << endl;

    routeTS->eRowIdx = r.sRowIdx;
    routeTS->eColIdx = r.sColIdx;
    routeTS->eLayIdx = r.sLayIdx;
    routeTS->sRowIdx = r.sRowIdx;
    routeTS->sColIdx = r.sColIdx;
    routeTS->sLayIdx = p->getLayIdx();
    //cout << "routeTS: " << routeTS->sRowIdx << "," << routeTS->sColIdx << "," << routeTS->sLayIdx
    //<< ",," << routeTS->eRowIdx << "," << routeTS->eColIdx << "," << routeTS->eLayIdx << endl;

    if(r.ggridOK(gg))
    {
        cout << "isRedundant(TE) = " << isRedundant(routeTE) << endl;
        if((!isRedundant(routeTE) && !routeTE->ggridOK(gg)) ||
           !routeTS->ggridOK(gg))
            return false;
        else
        {
            cout<<"in"<<endl;
            if(!isRedundant(routeTE)) append(routes_I, num_route_I, routeTE);
            append(routes_I, num_route_I, routeTS);
            //cout << "r_added: " << r.sRowIdx << "," << r.sColIdx << "," << r.sLayIdx
            //<< ",," << r.eRowIdx << "," << r.eColIdx << "," << r.eLayIdx << endl;
            return true;
        }
    }
    else return false;
}

bool Net::routing_3D(Pin* p1, Route& r1, Route& r2, Pin* p2, vector<vector<vector<Ggrid*>>>& gg) //new
{
    cout<<"routing_3D2..."<<endl;
    Route* routeTS = new Route;
    Route* routeCO = new Route;
    Route* routeTE = new Route;
    if(r1.direction() == 'C')
    {
        if(r2.direction() == 'H')
        {
            r1.sLayIdx = routingLayer_H->getHeight();
            r1.eLayIdx = routingLayer_H->getHeight();
            r2.sLayIdx = routingLayer_H->getHeight();
            r2.eLayIdx = routingLayer_H->getHeight();
        }
        else if(r2.direction() == 'V')
        {
            r1.sLayIdx = routingLayer_V->getHeight();
            r1.eLayIdx = routingLayer_V->getHeight();
            r2.sLayIdx = routingLayer_V->getHeight();
            r2.eLayIdx = routingLayer_V->getHeight();
        }
        else
        {
            r1.sLayIdx = (routingLayer_H->getHeight() <= routingLayer_V->getHeight()) ? routingLayer_H->getHeight() : routingLayer_V->getHeight();
            r1.eLayIdx = r1.sLayIdx;
            r2.sLayIdx = r1.sLayIdx;
            r2.eLayIdx = r1.sLayIdx;
        }
    }
    else if(r1.direction() == 'H')
    {
        r1.sLayIdx = routingLayer_H->getHeight();
        r1.eLayIdx = routingLayer_H->getHeight();
        if(r2.direction() == 'C')
        {
            r2.sLayIdx = routingLayer_H->getHeight();
            r2.eLayIdx = routingLayer_H->getHeight();
        }
        else
        {
            r2.sLayIdx = routingLayer_V->getHeight();
            r2.eLayIdx = routingLayer_V->getHeight();
        }
    }
    else
    {
        r1.sLayIdx = routingLayer_V->getHeight();
        r1.eLayIdx = routingLayer_V->getHeight();
         if(r2.direction() == 'C')
        {
            r2.sLayIdx = routingLayer_V->getHeight();
            r2.eLayIdx = routingLayer_V->getHeight();
        }
        else
        {
            r2.sLayIdx = routingLayer_H->getHeight();
            r2.eLayIdx = routingLayer_H->getHeight();
        }
    }
    routeTS->sRowIdx = r1.sRowIdx;
    routeTS->sColIdx = r1.sColIdx;
    routeTS->eRowIdx = r1.sRowIdx;
    routeTS->eColIdx = r1.sColIdx;
    routeTS->eLayIdx = r1.sLayIdx;
    cout << "p1 is corner == " << p1->is_corner() << endl;
    if(p1->is_corner()) routeTS->sLayIdx = routeTS->eLayIdx; //new
    else routeTS->sLayIdx = p1->getLayIdx(); //new
    //cout << "routeTS: " << routeTS->sRowIdx << "," << routeTS->sColIdx << "," << routeTS->sLayIdx
    //<< ",," << routeTS->eRowIdx << "," << routeTS->eColIdx << "," << routeTS->eLayIdx << endl;

    routeCO->sRowIdx = r1.eRowIdx;
    routeCO->sColIdx = r1.eColIdx;
    routeCO->sLayIdx = r1.eLayIdx;
    routeCO->eRowIdx = r2.sRowIdx;
    routeCO->eColIdx = r2.sColIdx;
    routeCO->eLayIdx = r2.sLayIdx;
    //cout << "routeCO: " << routeCO->sRowIdx << "," << routeCO->sColIdx << "," << routeCO->sLayIdx
    //<< ",," << routeCO->eRowIdx << "," << routeCO->eColIdx << "," << routeCO->eLayIdx << endl;

    routeTE->sRowIdx = r2.eRowIdx;
    routeTE->sColIdx = r2.eColIdx;
    routeTE->sLayIdx = r2.eLayIdx;
    routeTE->eRowIdx = r2.eRowIdx;
    routeTE->eColIdx = r2.eColIdx;
    routeTE->eLayIdx = p2->getLayIdx();
    //cout << "routeTE: " << routeTE->sRowIdx << "," << routeTE->sColIdx << "," << routeTE->sLayIdx
    //<< ",," << routeTE->eRowIdx << "," << routeTE->eColIdx << "," << routeTE->eLayIdx << endl;
    //cout << "r1.ggridOK(gg)" << r1.ggridOK(gg) << endl;
    //cout << "r2.ggridOK(gg)" << r2.ggridOK(gg) << endl;
    if(r1.ggridOK(gg) && r2.ggridOK(gg))
    {
        if((!isRedundant(routeTS) && !routeTS->ggridOK(gg)) ||
           !routeCO->ggridOK(gg) || !routeTE->ggridOK(gg))
            return false;
        else
        {
            append(routes_I, num_route_I, routeTE);
            append(routes_I, num_route_I, routeCO);
            if(!isRedundant(routeTS) && !p1->is_corner()) append(routes_I, num_route_I, routeTS);//new
           /*cout << "r1 " << r1.sRowIdx << "," << r1.sColIdx << "," << r1.sLayIdx
            << ",," << r1.eRowIdx << "," << r1.eColIdx << "," << r1.eLayIdx << endl;
             cout << "r2 " << r2.sRowIdx << "," << r2.sColIdx << "," << r2.sLayIdx
            << ",," << r2.eRowIdx << "," << r2.eColIdx << "," << r2.eLayIdx << endl;*/
            return true;
        }
    }
    else return false;
}

bool Net::isRedundant(Route* r) //new
{
    for(int i=0; i<num_route_I; i++)
    {
        if(routes_I[i]->sRowIdx == r->sRowIdx && routes_I[i]->sColIdx == r->sColIdx)
        {
        if((((routes_I[i]->sLayIdx <= r->sLayIdx && r->sLayIdx <= routes_I[i]->eLayIdx) &&
            (routes_I[i]->sLayIdx <= r->eLayIdx && r->eLayIdx <= routes_I[i]->eLayIdx))) ||
           (((routes_I[i]->eLayIdx <= r->sLayIdx && r->sLayIdx <= routes_I[i]->sLayIdx) &&
            (routes_I[i]->eLayIdx <= r->eLayIdx && r->eLayIdx <= routes_I[i]->sLayIdx))))
            {
                //cout << "containing route index = " << i << endl;
                //cout << "sl: " << routes_I[i]->sLayIdx << "el" << routes_I[i]->eLayIdx << endl;
                return true;
            }
        }
    }
    return false;
}

void Net::shield_H(int box_sRowIdx, int box_eRowIdx, int box_sColIdx, //newnew
                 int box_eColIdx, int box_sLayIdx, int box_eLayIdx, //start low end high
                 Route* r)
{
    Route* route_start = new Route;
    Route* route_end = new Route;
    Route* route_US = new Route;
    Route* route_UM = new Route;
    Route* route_UE = new Route;
    Route* route_DS = new Route;
    Route* route_DM = new Route;
    Route* route_DE = new Route;
    Route* route_RS = new Route;
    Route* route_RM = new Route;
    Route* route_RE = new Route;
    Route* route_LS = new Route;
    Route* route_LM = new Route;
    Route* route_LE = new Route;
    int length_U = -1;
    int length_D = -1;
    int length_R = -1;
    int length_L = -1;
    if(r->direction() == 'H')
    {
        if(r->sColIdx <= r->eColIdx)
        {
            route_start->set3Dpos(r->sRowIdx, r->sColIdx, r->sLayIdx,
                                  r->sRowIdx, box_sColIdx, r->sLayIdx);
            route_end->set3Dpos(r->eRowIdx, box_eColIdx, r->eLayIdx,
                                r->eRowIdx, r->eColIdx, r->eLayIdx);
        }
        else
        {
            route_start->set3Dpos(r->eRowIdx, r->eColIdx, r->eLayIdx,
                                  r->eRowIdx, box_sColIdx, r->eLayIdx);
            route_end->set3Dpos(r->sRowIdx, box_eColIdx, r->sLayIdx,
                                r->sRowIdx, r->sColIdx, r->sLayIdx);
        }

        if(box_eLayIdx <= num_layer) //U
        {
            if(layers[box_eLayIdx-1]->getDirection() == 'H')
            {
                route_US->set3Dpos(r->sRowIdx, box_sColIdx, r->sLayIdx,
                                   r->sRowIdx, box_sColIdx, box_eLayIdx);
                route_UM->set3Dpos(r->sRowIdx, box_sColIdx, box_eLayIdx,
                                   r->sRowIdx, box_eColIdx, box_eLayIdx);
                route_UE->set3Dpos(r->sRowIdx, box_eColIdx, box_eLayIdx,
                                   r->sRowIdx, box_eColIdx, r->eLayIdx);
                length_U = route_US->getlength() + route_UM->getlength() + route_UE->getlength();
            }
            else if(layers[box_eLayIdx-1]->getDirection() == 'V' && box_eLayIdx+1<=num_layer)
            {
                route_US->set3Dpos(r->sRowIdx, box_sColIdx, r->sLayIdx,
                                   r->sRowIdx, box_sColIdx, box_eLayIdx+1);
                route_UM->set3Dpos(r->sRowIdx, box_sColIdx, box_eLayIdx+1,
                                   r->sRowIdx, box_eColIdx, box_eLayIdx+1);
                route_UE->set3Dpos(r->sRowIdx, box_eColIdx, box_eLayIdx+1,
                                   r->sRowIdx, box_eColIdx, r->eLayIdx);
                length_U = route_US->getlength() + route_UM->getlength() + route_UE->getlength();
            }
        }

        if(box_sLayIdx >= min_layer->getHeight()) //D
        {
            if(layers[box_sLayIdx-1]->getDirection() == 'H')
            {
                route_DS->set3Dpos(r->sRowIdx, box_sColIdx, r->sLayIdx,
                                   r->sRowIdx, box_sColIdx, box_sLayIdx);
                route_DM->set3Dpos(r->sRowIdx, box_sColIdx, box_sLayIdx,
                                   r->sRowIdx, box_eColIdx, box_sLayIdx);
                route_DE->set3Dpos(r->sRowIdx, box_eColIdx, box_sLayIdx,
                                   r->sRowIdx, box_eColIdx, r->eLayIdx);
                length_D = route_DS->getlength() + route_DM->getlength() + route_DE->getlength();
            }
            else if(layers[box_sLayIdx-1]->getDirection() == 'V' && box_eLayIdx-1>=min_layer->getHeight())
            {
                route_DS->set3Dpos(r->sRowIdx, box_sColIdx, r->sLayIdx,
                                   r->sRowIdx, box_sColIdx, box_sLayIdx-1);
                route_DM->set3Dpos(r->sRowIdx, box_sColIdx, box_sLayIdx-1,
                                   r->sRowIdx, box_eColIdx, box_sLayIdx-1);
                route_DE->set3Dpos(r->sRowIdx, box_eColIdx, box_sLayIdx-1,
                                   r->sRowIdx, box_eColIdx, r->eLayIdx);
                length_D = route_DS->getlength() + route_DM->getlength() + route_DE->getlength();
            }
        }

        //R L not considered yet

        append(route_S, num_route_S, route_start);
        append(route_S, num_route_S, route_end);
        if(length_U <= length_D)
        {
            append(route_S, num_route_S, route_US);
            append(route_S, num_route_S, route_UM);
            append(route_S, num_route_S, route_UE);
        }
        else
        {
            append(route_S, num_route_S, route_DS);
            append(route_S, num_route_S, route_DM);
            append(route_S, num_route_S, route_DE);
        }
    }
    return;
}

void Net::shield_I(int box_sRowIdx, int box_eRowIdx, int box_sColIdx, //newnew
                 int box_eColIdx, int box_sLayIdx, int box_eLayIdx, //start low end high
                 Route* r)
{
    Route* route_start = new Route;
    Route* route_end = new Route;
    Route* route_FS = new Route;
    Route* route_FM = new Route;
    Route* route_FE = new Route;
    Route* route_BS = new Route;
    Route* route_BM = new Route;
    Route* route_BE = new Route;
    Route* route_RS = new Route;
    Route* route_RM = new Route;
    Route* route_RE = new Route;
    Route* route_LS = new Route;
    Route* route_LM = new Route;
    Route* route_LE = new Route;
    int length_F = -1;
    int length_B = -1;
    int length_R = -1;
    int length_L = -1;

    if(r->sLayIdx <= r->eLayIdx)
    {
        route_start->set3Dpos(r->sRowIdx, r->sColIdx, r->sLayIdx,
                              r->sRowIdx, r->sColIdx, box_sLayIdx);
        route_end->set3Dpos(r->eRowIdx, r->eColIdx, box_eLayIdx,
                              r->eRowIdx, r->eColIdx, r->eLayIdx);
    }
    else
    {
        route_start->set3Dpos(r->eRowIdx, r->eColIdx, r->eLayIdx,
                              r->eRowIdx, r->eColIdx, box_sLayIdx);
        route_end->set3Dpos(r->sRowIdx, r->sColIdx, box_eLayIdx,
                              r->sRowIdx, r->sColIdx, r->sLayIdx);
    }

    if(box_eRowIdx <= num_row) //F
    {
        if(layers[box_sLayIdx-1]->getDirection() == 'V')
        {
            route_FS->set3Dpos(r->sRowIdx, r->sColIdx, box_sLayIdx,
                               box_eRowIdx, r->sColIdx, box_sLayIdx);
        }
        else
        {
            route_FS->set3Dpos(r->sRowIdx, r->sColIdx, box_sLayIdx-1,
                               box_eRowIdx, r->sColIdx, box_sLayIdx-1);
        }
        if(layers[box_eLayIdx-1]->getDirection() == 'V')
        {
            route_FE->set3Dpos(box_eRowIdx, r->sColIdx, box_eLayIdx,
                               r->eRowIdx, r->sColIdx, box_eLayIdx);
        }
        else
        {
            route_FE->set3Dpos(box_eRowIdx, r->sColIdx, box_eLayIdx+1,
                               r->eRowIdx, r->sColIdx, box_eLayIdx+1);
        }
        route_FM->set3Dpos(route_FS->eRowIdx, route_FS->eColIdx, route_FS->eLayIdx,
                           route_FE->sRowIdx, route_FE->sColIdx, route_FE->sLayIdx);
        length_F = route_FS->getlength() + route_FM->getlength() + route_FE->getlength();
    }

    if(box_sRowIdx >= 1) //B
    {
        if(layers[box_sLayIdx-1]->getDirection() == 'V')
        {
            route_BS->set3Dpos(r->sRowIdx, r->sColIdx, box_sLayIdx,
                               box_sRowIdx, r->sColIdx, box_sLayIdx);
        }
        else
        {
            route_BS->set3Dpos(r->sRowIdx, r->sColIdx, box_sLayIdx-1,
                               box_sRowIdx, r->sColIdx, box_sLayIdx-1);
        }
        if(layers[box_eLayIdx-1]->getDirection() == 'V')
        {
            route_BE->set3Dpos(box_sRowIdx, r->sColIdx, box_eLayIdx,
                               r->eRowIdx, r->sColIdx, box_eLayIdx);
        }
        else
        {
            route_BE->set3Dpos(box_eRowIdx, r->sColIdx, box_eLayIdx+1,
                               r->eRowIdx, r->sColIdx, box_eLayIdx+1);
        }
        route_BM->set3Dpos(route_BS->eRowIdx, route_BS->eColIdx, route_BS->eLayIdx,
                           route_BE->sRowIdx, route_BE->sColIdx, route_BE->sLayIdx);
        length_B = route_BS->getlength() + route_BM->getlength() + route_BE->getlength();
    }

    if(box_eColIdx <= num_column) //R
    {
        if(layers[box_sLayIdx-1]->getDirection() == 'H')
        {
            route_RS->set3Dpos(r->sRowIdx, r->sColIdx, box_sLayIdx,
                               r->sRowIdx, box_eColIdx, box_sLayIdx);
        }
        else
        {
            route_RS->set3Dpos(r->sRowIdx, r->sColIdx, box_sLayIdx-1,
                               r->sRowIdx, box_eColIdx, box_sLayIdx-1);
        }
        if(layers[box_eLayIdx-1]->getDirection() == 'H')
        {
            route_RE->set3Dpos(r->eRowIdx, box_eColIdx, box_eLayIdx,
                               r->eRowIdx, r->eColIdx, box_eLayIdx);
        }
        else
        {
            route_RE->set3Dpos(r->eRowIdx, box_eColIdx, box_eLayIdx+1,
                               r->eRowIdx, r->eColIdx, box_eLayIdx+1);
        }
        route_RM->set3Dpos(route_RS->eRowIdx, route_RS->eColIdx, route_RS->eLayIdx,
                           route_RE->sRowIdx, route_RE->sColIdx, route_RE->sLayIdx);
        length_R = route_RS->getlength() + route_RM->getlength() + route_RE->getlength();
    }

    if(box_sColIdx >= 1) //L
    {
        if(layers[box_sLayIdx-1]->getDirection() == 'H')
        {
            route_LS->set3Dpos(r->sRowIdx, r->sColIdx, box_sLayIdx,
                               r->sRowIdx, box_sColIdx, box_sLayIdx);
        }
        else
        {
            route_LS->set3Dpos(r->sRowIdx, r->sColIdx, box_sLayIdx-1,
                               r->sRowIdx, box_sColIdx, box_sLayIdx-1);
        }
        if(layers[box_eLayIdx-1]->getDirection() == 'H')
        {
            route_LE->set3Dpos(r->eRowIdx, box_sColIdx, box_eLayIdx,
                               r->eRowIdx, r->eColIdx, box_eLayIdx);
        }
        else
        {
            route_LE->set3Dpos(r->eRowIdx, box_sColIdx, box_eLayIdx+1,
                               r->eRowIdx, r->eColIdx, box_eLayIdx+1);
        }
        route_LM->set3Dpos(route_LS->eRowIdx, route_LS->eColIdx, route_LS->eLayIdx,
                           route_LE->sRowIdx, route_LE->sColIdx, route_LE->sLayIdx);
        length_L = route_LS->getlength() + route_LM->getlength() + route_LE->getlength();
    }

    if(length_L != -1 && length_L <= length_F && length_L <= length_B && length_L <= length_R)
    {
        append(route_S, num_route_S, route_LS);
        append(route_S, num_route_S, route_LM);
        append(route_S, num_route_S, route_LE);
    }
    else if(length_R != -1 && length_R <= length_F && length_R <= length_B && length_R <= length_L)
    {
        append(route_S, num_route_S, route_RS);
        append(route_S, num_route_S, route_RM);
        append(route_S, num_route_S, route_RE);
    }
    else if(length_B != -1 && length_B <= length_F && length_B <= length_R && length_B <= length_L)
    {
        append(route_S, num_route_S, route_BS);
        append(route_S, num_route_S, route_BM);
        append(route_S, num_route_S, route_BE);
    }
    else if(length_F != -1 && length_F <= length_B && length_F <= length_R && length_F <= length_L)
    {
        append(route_S, num_route_S, route_FS);
        append(route_S, num_route_S, route_FM);
        append(route_S, num_route_S, route_FE);
    }
    return;
}


void
Net::update_length()
{
    int l=0;
    for (int i=0;i<num_route;i++){
        l+=routes[i].getlength();
    }
    for (int i=0;i<num_route_I;i++){
        l+=routes_I[i]->getlength();
    }
    for (int i=0;i<num_route_S;i++){
        l+=route_S[i]->getlength();
    }
    Length = l;
    //cout<<"update "<<Length<<endl;    
}


void
Net::add_route(int& length, Route* r)//
{
    if(length == 0)
    {
        routes = new Route[1];
        routes[0] = *r;
        length = 1;
    }
    else
    {
        Route* temp = new Route[length+1];
        for(int i=0; i<length; i++)
        {
            temp[i] = routes[i];
        }
        temp[length] = *r;
        routes = temp;
        length++;
    }
    num_route++;
    update_length();

    return;
}

void
Net::reset_route(Pin* p, string n,vector<vector<vector<Ggrid*>>>& gg)
{
    for(int r=0;r<num_route;r++){
        routes[r].returning(gg);
    }
    if(num_route!=0){
        delete []routes;
        num_route=0;
    }
    for (int i=0;i<num_pin;i++){
        if (n==pins[i]->get_cell_name()&& pins[i]->getname()==p->getname()){
            Pin* temp=pins[i];
            //cout<<"old pin:"<<temp->getRowIdx()<<","<<temp->getColIdx()<<","<<temp->getLayIdx()<<"->";
            pins[i]=p;
            //cout<<"new pin:"<<pins[i]->getRowIdx()<<","<<pins[i]->getColIdx()<<","<<pins[i]->getLayIdx()<<endl;
            delete temp;
        }
    }
}
