#ifndef MTREE_H
#define MTREE_H
#include <vector>
#include <math.h>
#include <utility>
#include <iostream>

using namespace std;

struct point{
    point(){};
    int idx;
    vector<double> features;
};

class Mtree
{
    public:
        Mtree();
        Mtree(int);
        Mtree(int,point*);
        void insertar(point*);
        vector<point*> K_nn(int k, point *p);
        pair<point*, point*> pickValues();
        pair<Mtree*, Mtree*> pickSons();
        void split();
        void refill(point *p);
        point* feature_value;
        double covering_radius = 0;
        vector<point*> values;
        vector<Mtree*> sons;
        Mtree* parent = nullptr;
        double distance_parent = 0;
        bool root = false;
        bool leaf = true;
        virtual ~Mtree();

    private:
        int M;
        int m;
};

#endif // MTREE_H
