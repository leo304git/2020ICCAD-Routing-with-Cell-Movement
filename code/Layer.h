#ifndef LAYER_H
#define LAYER_H
#include <string>

using namespace std;

class Layer
{
    public:
        Layer();
        Layer(string, int, char, int);
        virtual ~Layer();
        Layer& operator = (const Layer&);
        string& get_name();
        int getHeight(); //new
        char getDirection();//new

    protected:

    private:
        string name;
        int height;
        char direction;
        int demand;

};

#endif // LAYER_H

