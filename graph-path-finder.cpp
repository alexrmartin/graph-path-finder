#include <string>
#include <fstream>
#include <iomanip>
#include "ArgumentManager.h"

using namespace std;

int global_p[10001];

struct point {
    int v;
    double scale;
    struct point *next;
};


struct dmap {
    point *pts[10001];
    int deg[10001];
    int num_points;
    int num_verts;
};

class MapAlgo {
private:
    dmap *m;

public:
    MapAlgo() {
        m = new dmap;
        m->num_points = 0;
        m->num_verts = 0;

        for (int i = 1; i < 10001; i++) {
            m->deg[i] = 0;
        }
        for (int i = 1; i < 10001; i++) {
            m->pts[i] = NULL;
        }
    }
    
    void insert_helper(dmap *m, int x, int v, double s) {
        point *p = new point;

        p->v = v;
        p->scale = s;
        p->next = m->pts[x];

        m->pts[x] = p;
        m->num_points++;
        m->deg[x]++;
    }
    
    void insert(int *v1_arr, int *v2_arr, double *scale_arr, int idx, int largestV) {
        m->num_verts = largestV;
        for (int i = 0; i < idx; i++) {
            insert_helper(m, v1_arr[i], v2_arr[i], scale_arr[i]);
        }
    }
    
    double map_algo(int begin, int end) {
        point *p;
        bool intree[10001];
        double dist_arr[10001];
        int w, v;
        double dist, scale;

        for (int i = 1; i <= m->num_verts; i++) {
            intree[i] = false;
            global_p[i] = -1;
            dist_arr[i] = 99999;
        }

        dist_arr[begin] = 0;
        v = begin;
        while (!intree[v]) {
            intree[v] = !intree[v];
            p = m->pts[v];

            while (p != NULL) {
                w = p->v;
                scale = p->scale;
                if (dist_arr[w] > (dist_arr[v] + scale)) {
                    dist_arr[w] = dist_arr[v] + scale;
                    global_p[w] = v;
                }
                p = p->next;
            }
            v = 1;
            dist = 99999;

            for (int i = 1; i <= m->num_verts; i++) {
                if (!intree[i] && (dist > dist_arr[i])) {
                    dist = dist_arr[i];
                    v = i;
                }
            }
        }

        return dist_arr[end];
    }
};

int main(int argc, char* argv[]) {
    ArgumentManager am(argc, argv);

    ifstream pathFile;
    pathFile.open(am.get("path"));

    ifstream inputFile;
    inputFile.open(am.get("input"));

    ofstream outputFile;
    outputFile.open(am.get("output"));
    
    int m, x, y;
    double s;

    int *v1_arr = new int[10000];
    int *v2_arr = new int[10000];
    double *scale_arr = new double[10000];
    
    int idx = 0;
    int largestV = 0;
    
    int begin, end;
    
    pathFile >> begin;
    pathFile >> end;
    
    while(inputFile >> x >> y >> s) {
        v1_arr[idx] = x;
        v2_arr[idx] = y;
        scale_arr[idx] = s;
        if (largestV < y) {
            largestV = y;
        }
        if (largestV < x) {
            largestV = x;
        }
        idx++;
    }
    
    MapAlgo *a = new MapAlgo();
    
    if(largestV < begin || largestV < end) {
        outputFile << "Infinite" << endl;
        return -1;
    }
    
    a->insert(v1_arr, v2_arr, scale_arr, idx, largestV);
    double output = a->map_algo(begin, end);

    if(output >= 99999)
        outputFile << "Infinite" << endl;
    else {
        outputFile << setprecision(1) << fixed;
        outputFile << output;
    }
    
    return 0;
}
