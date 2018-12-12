#include "Mtree.h"

double euclidean_distance(point *a, point *b){
    double rpta = 0;
    for(int i = 0; i < a->features.size(); ++i){
        rpta += pow((a->features[i] - b->features[i]),2);
    }
    return sqrt(rpta);
}

Mtree::Mtree()
{

}

Mtree::Mtree(int a){
    this->M = a;
    this->m = ceil(M/2);
}

Mtree::Mtree(int a, point* p){
    this->M = a;
    this->m = ceil(M/2);
    values.push_back(p);
    feature_value = p;
}

pair<point*, point*> Mtree::pickValues(){
    double d = 0.0;
    int first = 0;
    int last = 1;
    for(int i = 0; i < values.size(); ++i){
        for(int j = i; j < values.size(); ++j){
            double temp = euclidean_distance(values[i], values[j]);
            if(temp > d){
                first = i;
                last = j;
            }
        }
    }
    pair<point*,point*> rpta = make_pair(values[first], values[last]);
    values.erase(values.begin()+last);
    values.erase(values.begin()+first);
    return rpta;
}

pair<Mtree*, Mtree*> Mtree::pickSons(){
    double d = 0.0;
    int first = 0;
    int last = 1;
    for(int i = 0; i < sons.size(); ++i){
        for(int j = i; j < sons.size(); ++j){
            double temp = euclidean_distance(sons[i]->feature_value, sons[j]->feature_value);
            if(temp > d){
                first = i;
                last = j;
            }
        }
    }
    pair<Mtree*,Mtree*> rpta = make_pair(sons[first], sons[last]);
    sons.erase(sons.begin()+last);
    sons.erase(sons.begin()+first);
    return rpta;
}

void Mtree::split(){
    if(root){
        Mtree *new_root = new Mtree(M);
        new_root->feature_value = feature_value;
        Mtree *brother = new Mtree(M);
        if(values.size()!=0){
            pair<point*, point*> seeds = pickValues();
            vector<point*> temp = values;
            values.clear();
            feature_value = seeds.first;
            for(int i = 0; i < temp.size()/2; ++i){
                insertar(temp[i]);
            }
            brother->feature_value = seeds.second;
            for(int i = temp.size()/2; i < temp.size(); ++i){
                brother->insertar(temp[i]);
            }
        }
        else{
            pair<Mtree*, Mtree*> seeds = pickSons();
            vector<Mtree*> temp = sons;
            sons.clear();
            feature_value = seeds.first->feature_value;
            for(int i = 0; i < temp.size()/2; ++i){
                sons.push_back(temp[i]);
            }
            brother->feature_value = seeds.second->feature_value;
            for(int i = temp.size()/2; i < temp.size(); ++i){
                brother->sons.push_back(temp[i]);
            }
        }
        root = false;
        new_root->root = true;
        new_root->leaf = false;
        parent = new_root;
        brother->parent = new_root;
        distance_parent = euclidean_distance(feature_value, parent->feature_value);
        brother->distance_parent = euclidean_distance(brother->feature_value, brother->parent->feature_value);
        new_root->sons.push_back(this);
        new_root->sons.push_back(brother);
        if(parent->sons.size() > M){
            parent->split();
        }
    }
    else{
        Mtree *brother = new Mtree(M);
        if(values.size()!=0){
            pair<point*, point*> seeds = pickValues();
            vector<point*> temp = values;
            values.clear();
            feature_value = seeds.first;
            for(int i = 0; i < temp.size()/2; ++i){
                insertar(temp[i]);
            }
            brother->feature_value = seeds.second;
            for(int i = temp.size()/2; i < temp.size(); ++i){
                brother->insertar(temp[i]);
            }
        }
        else{
            pair<Mtree*, Mtree*> seeds = pickSons();
            vector<Mtree*> temp = sons;
            sons.clear();
            feature_value = seeds.first->feature_value;
            for(int i = 0; i < temp.size()/2; ++i){
                sons.push_back(temp[i]);
            }
            brother->feature_value = seeds.second->feature_value;
            for(int i = temp.size()/2; i < temp.size(); ++i){
                brother->sons.push_back(temp[i]);
            }
        }
        brother->parent = parent;
        brother->distance_parent = euclidean_distance(brother->feature_value, brother->parent->feature_value);
        parent->sons.push_back(brother);
        parent->leaf = false;
        if(parent->sons.size()>M){
            parent->split();
        }
    }
}

void Mtree::refill(point *p){
    double new_distance = euclidean_distance(feature_value, p);
    if(new_distance > covering_radius){
        covering_radius = new_distance;
        if(parent != nullptr){
            parent->refill(p);
        }
    }
}

void Mtree::insertar(point *p){
    if(!leaf){
        int idx = 0;
        double next = euclidean_distance(sons[0]->feature_value, p);
        for(int i = 1; i < sons.size(); ++i){
            double possibly_next = euclidean_distance(sons[i]->feature_value, p);
            if(possibly_next < next){
                idx = i;
                possibly_next = next;
            }
        }
        sons[idx]->insertar(p);
    }
    else{
        values.push_back(p);
        refill(p);
        if(values.size() > M){
            cout << p->idx << endl;
            split();
        }
    }
}

Mtree::~Mtree()
{

}
