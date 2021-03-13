#ifndef NET_H
#define NET_H
#include <string>
#include <cmath>
#include <vector>
#include "Layer.h"
#include "Pin.h"
#include "Route.h"
#include "Ggrid.h"
//class Route;
class Net
{
    public:
        Net();
        Net(string,int, Layer&, vector<Layer*>&);
        virtual ~Net();
        void writePin(int, Pin&);
        void decideLayer();  //new
        void routing_2D(vector<vector<vector<Ggrid*>>>&);
        void pinList(); //construct pin list
        int manhattanDist(Pin*, Pin*);
        bool add_One_Pin(Pin*,vector<vector<vector<Ggrid*>>>&);
        void append(Pin**&, int&, Pin*);
        void append(Route*&, int&, Route);
        void append(Route**&, int&, Route*);//new
        bool Lcreate(Pin*, Pin*, vector<vector<vector<Ggrid*>>>&);
        bool routing_3D(Route&, Route&, Pin*,vector<vector<vector<Ggrid*>>>&); //new
        bool routing_3D(Pin*, Route&, Route&, Pin*, vector<vector<vector<Ggrid*>>>&); //new
        bool isRedundant(Route*); //new
        void shield_H(int, int, int, int, int, int, Route*); //newnew
        void shield_V(int, int, int, int, int, int, Route*); //newnew
        void shield_I(int, int, int, int, int, int, Route*); //newnew
        void update_length();
        int get_length() {return Length;}
        string& get_name(){return name;}
        Route*& get_routes(){return routes;}
        Route**& get_routes_I(){return routes_I;}
        Route**& get_routes_S(){return route_S;}
        int& get_num_route(){return num_route;}
        int& get_num_route_I(){return num_route_I;}
        int& get_num_route_S(){return num_route_S;}        
        void add_route(int&,Route* r);
        void reset_route(Pin*, string, vector<vector<vector<Ggrid*>>>&);
        int get_num_pin() {return num_pin;}
        Pin* get_pin(int j){return pins[j];}

    protected:

    private:
        Pin** pins;
        int num_pin;
        Pin** theConnected; //include (steiner) and corner points
        int num_theConnected;
        int num_unConnected; //new
        Layer* min_layer;
        Route* routes;
        Route** routes_I;//new
        int num_route_I;//new
        int num_route;
        Route** route_S; //newnew
        int num_route_S; //newnew
        string name;
        Layer* routingLayer_H; //new
        Layer* routingLayer_V; //new
        int routingLayer_H_index;
        int routingLayer_V_index;
        vector<Layer*> layers; //new
        int num_layer; //new
        int num_row; //newnew
        int num_column; //newnew
        int Length;
};
#endif // NET_H


