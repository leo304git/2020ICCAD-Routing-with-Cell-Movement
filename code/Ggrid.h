#ifndef GGRID_H
#define GGRID_H

#include<iostream>

using namespace std;

class Ggrid
{
    public:
        Ggrid(int, char, int, int, int);
        virtual ~Ggrid();
        void setSupply(int);
        int getSupply();
        bool ggridok();
        bool ggridok(int);
        void demanding(int);
        void returning(int);
        int get_availability(){return availability;}
    protected:

    private:
        int supply;
        int availability; // supply - demand
        char direction;
        int rowIdx, colIdx, highIdx;

};

#endif // GGRID_H
