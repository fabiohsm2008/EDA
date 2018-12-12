#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Mtree.h"

using namespace std;

vector<point*> valores;

void printvector(vector<point*> temp){
    for(int i = 0; i < temp.size(); ++i){
        cout << temp[i]->idx << " ";
    }
}

int main()
{
    ifstream archivo;
    archivo.open("YearPredictionMSD.txt");
    string linea;
    int tam = 0;
    int features = 0;
    getline(archivo, linea);
    string feature;
    point *a = new point();
    a->idx = tam;
    ++tam;
    stringstream stream(linea);
    while(getline(stream, feature, ',')){
        a->features.push_back(stod(feature));
        features++;
    }
    Mtree *arbol = new Mtree(5000,a);
    arbol->root = true;
    valores.push_back(a);
    while(getline(archivo, linea)){
        point *b = new point();
        b->idx = tam;
        string feature;
        stringstream stream(linea);
        while(getline(stream, feature, ',')){
            b->features.push_back(stod(feature));
        }
        arbol->insertar(b);
        valores.push_back(b);
        ++tam;
    }
    archivo.close();
    cout << tam << " " << features << endl;

    return 0;
}
