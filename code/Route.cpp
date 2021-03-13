#include "Route.h"

Route::Route()
{
    sRowIdx = -1;
    sColIdx = -1;
    sLayIdx = -1;
    eRowIdx = -1;
    eColIdx = -1;
    eLayIdx = -1;
    length = 0;
}

Route::Route(int sr, int sc, int er, int ec)
{
    sRowIdx = sr;
    sColIdx = sc;
    sLayIdx = -1;
    eRowIdx = er;
    eColIdx = ec;
    eLayIdx = -1;
    length=abs(sRowIdx-eRowIdx)+abs(sColIdx-eColIdx)+abs(sLayIdx-sLayIdx);
}

Route::Route(int sr, int sc, int sl, int er, int ec, int el)
{
    sRowIdx = sr;
    sColIdx = sc;
    sLayIdx = sl;
    eRowIdx = er;
    eColIdx = ec;
    eLayIdx = el;
    length=abs(sRowIdx-eRowIdx)+abs(sColIdx-eColIdx)+abs(sLayIdx-sLayIdx);
}

Route::~Route()
{
    //dtor
}

void Route::set2Dpos(int _sRow, int _sCol, int _eRow, int _eCol)
{
    sRowIdx = _sRow;
    sColIdx = _sCol;
    eRowIdx = _eRow;
    eColIdx = _eCol;
    length=abs(sRowIdx-eRowIdx)+abs(sColIdx-eColIdx)+abs(sLayIdx-sLayIdx);
    return;
}

void Route::set3Dpos(int sr, int sc, int sl, int er, int ec, int el) //newnew
{
    sRowIdx = sr;
    sColIdx = sc;
    sLayIdx = sl;
    eRowIdx = er;
    eColIdx = ec;
    eLayIdx = el;
    length=abs(sRowIdx-eRowIdx)+abs(sColIdx-eColIdx)+abs(sLayIdx-sLayIdx);
    return;
}

int Route::dist(Pin* p)
{
    if(direction() == 'C')
    {
        if(p->getRowIdx() == this->sRowIdx)
             return abs(p->getColIdx() - this->sColIdx);
        else if(p->getColIdx() == this->sColIdx)
            return abs(p->getRowIdx() - this->sRowIdx);
        else
         {
             cout << "Route is not H or V." << endl;
            return -1;
        }
    }
   else if(direction() == 'H')
        return abs(p->getRowIdx() - this->sRowIdx);
    else if(direction() == 'V')
        return abs(p->getColIdx() - this->sColIdx);
    else
    {
        cout << "Route is not H or V." << endl;
        return -1;
    }

}

bool Route::Trelation(Pin* p)
{
    if(direction() == 'C')
    {
        if(p->getRowIdx() == sRowIdx || p->getColIdx() == sColIdx)
            return true;
        else
        {
            return false;
        }
        
    }
    else if(direction() == 'H')
    {
        //cout << "H" << endl;
        if(p->getColIdx()<=sColIdx && p->getColIdx()>=eColIdx)
            return true;
        else if(p->getColIdx()>=sColIdx && p->getColIdx()<=eColIdx)
            return true;
        else
            return false;
    }
    else if(direction() == 'V')
    {
        //cout << "V" << endl;
        if(p->getRowIdx()<=sRowIdx && p->getRowIdx()>=eRowIdx)
        {
            //cout << "hello" << endl;
            return true;
        }

        else if(p->getRowIdx()>=sRowIdx && p->getRowIdx()<=eRowIdx)
        {
            //cout << "hello" << endl;
            return true;
        }
        else
            return false;
    }
    else
    {
        cout << "Route is not H or V." << endl;
        return false;
    }
}

char Route::direction()
{
    if(sRowIdx == eRowIdx && sColIdx == eColIdx) return 'C';
    else if(sRowIdx == eRowIdx && sColIdx != eColIdx)  return 'H';
    else if(sColIdx == eColIdx && sRowIdx != sColIdx) return 'V';
    else return 'F';
}

bool Route::contain(Pin* p)
{
    if(direction() == 'C')
    {
        if(p->getRowIdx() ==sRowIdx && p->getColIdx() == sColIdx )
            return true;
        else
        {
            return false;
        }
        
    }
    else if(direction() == 'H')
    {
        //cout<<p->getRowIdx()<<p->getColIdx()<<p->getLayIdx()<<endl;
        if(p->getRowIdx() == sRowIdx && Trelation(p))
            return true;
        else return false;
    }
    else
    {
        if(p->getColIdx() == sColIdx && Trelation(p))
            return true;
        else return false;
    }
}

Route Route::Tcreate(Pin* p)
{
    int sRidx, sCidx, eRidx, eCidx;
    sRidx = p->getRowIdx();
    sCidx = p->getColIdx();
    if(direction() == 'C' )
    {
        eRidx = sRowIdx;
        eCidx = sColIdx;
    }
    else if(direction() == 'H')
    {
        eRidx = this->sRowIdx;
        eCidx = sCidx;
    }
    else if(direction() == 'V')
    {
        eCidx = this->sColIdx;
        eRidx = sRidx;
    }
    else cout << "false" << endl;
    //cout << "in Tcreate: " << sRidx << " " << sCidx << " " << eRidx << " " << eCidx << endl;
    Route r(sRidx, sCidx, eRidx, eCidx);
    r.steiner=new Pin();
    r.steiner->setpos(eRidx, eCidx);
    return r;
}

Route& Route::operator=(const Route& r)
{
    sRowIdx = r.sRowIdx;
    sColIdx = r.sColIdx;
    sLayIdx = r.sLayIdx;
    eRowIdx = r.eRowIdx;
    eColIdx = r.eColIdx;
    eLayIdx = r.eLayIdx;
    return *this;
}

bool Route::operator==(const Route& r) //new
{
    if(sRowIdx == r.sRowIdx && sColIdx == r.sColIdx && sLayIdx == r.sLayIdx &&
       eRowIdx == r.eRowIdx && eColIdx == r.eColIdx && eLayIdx == r.eLayIdx)
        return true;
    else return false;
}


bool
Route::ggridOK(vector<vector<vector<Ggrid*>>>& gg)//new!
{
    cout<<"ggridok "<<sRowIdx<<" "<<sColIdx<<" "<<sLayIdx<<" "<<eRowIdx<<" "<<eColIdx<<" "<<eLayIdx<<endl;
    for(int i=min(sRowIdx,eRowIdx);i<=max(sRowIdx,eRowIdx);i++){
        for (int j=min(sColIdx,eColIdx);j<=max(sColIdx,eColIdx);j++){
            for(int k=min(sLayIdx,eLayIdx);k<=max(sLayIdx,eLayIdx);k++){
                if(!gg[i][j][k]->ggridok()){return false;}
            }
        }
    }
    return true;
}

bool
Route::demanding(vector<vector<vector<Ggrid*>>>& gg)//new!
{
    for(int i=min(sRowIdx,eRowIdx);i<=max(sRowIdx,eRowIdx);i++){
        for (int j=min(sColIdx,eColIdx);j<=max(sColIdx,eColIdx);j++){
            for(int k=min(sLayIdx,eLayIdx);k<=max(sLayIdx,eLayIdx);k++){
                gg[i][j][k]->demanding(1);
            }
        }
    }
    return true;
}

bool
Route::returning(vector<vector<vector<Ggrid*>>>& gg)//new!
{
    for(int i=min(sRowIdx,eRowIdx);i<=max(sRowIdx,eRowIdx);i++){
        for (int j=min(sColIdx,eColIdx);j<=max(sColIdx,eColIdx);j++){
            for(int k=min(sLayIdx,eLayIdx);k<=max(sLayIdx,eLayIdx);k++){
                gg[i][j][k]->returning(1);
            }
        }
    }
    return true;
}

int Route::getlength() //newnew
{
    int rl = abs(sRowIdx-eRowIdx);
    int cl = abs(sColIdx-eColIdx);
    int ll = abs(sLayIdx-eLayIdx);
    return  rl+cl+ll+1;
}


