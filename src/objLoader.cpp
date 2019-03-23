
#include "objLoader.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

using std::ifstream;
using std::string;
using std::istringstream;

struct tuple {
    unsigned int i, j;
    unsigned int operator[](const int ind) {
        switch (ind) {
        case 0:
            return i;
        case 1:
            return j;
        }
        return 0xffffffff;
    }
};

struct triple {
    unsigned int i, j, k;
    unsigned int operator[](const int ind) {
        switch (ind) {
        case 0:
            return i;
        case 1:
            return j;
        case 2:
            return j;
        }
        return 0xffffffff;
    }
};

void loadObj(const char* obj_file, vector<vec3> &vertices, vector<vec2> &texCoords) {
    ifstream f;
    f.open(obj_file);
    if (!f.is_open()) {
        printf("Unable to open .obj file %s\n", obj_file);
        return;
    }

    vector<vec3> verts;
    vector<vec2> texs;
    vector<tuple> indices;
    unsigned int mins[2] = {0xffffffff, 0xffffffff};

    string buf, b;
    while (getline(f, buf)) {
        istringstream is(buf);
        if (!(is >> b))
            continue;
        if (b == "v") {
            float x, y, z;
            if (!(is >> x >> y >> z)) {
                printf("line \"%s\" corrupted\n", buf.c_str());
                continue;
            }
            verts.push_back({x, y, z});
        }
        else if (b == "vn") {

        }
        else if (b == "vt") {
            float x, y;
            if (!(is >> x >> y)) {
                printf("line \"%s\" corrupted\n", buf.c_str());
                continue;
            }
            x = x < 0 ? -x : x;
            y = y < 0 ? -y : y;
            texs.push_back({x, y});
        }
        else if (b == "f") {
            vector<string> inds;
            string b2;
            while (is >> buf)
                inds.push_back(buf);
            vector<vector<unsigned int>> in;
            for (const string &st : inds) {
                in.push_back(vector<unsigned int>());
                istringstream is2(st);
                string b3;
                while (getline(is2, b3, '/')) {
                    unsigned int num;
                    istringstream ii(b3);
                    ii >> num;
                    in[in.size() - 1].push_back(num);
                }
            }
            for (int i = 0; i < in.size() - 2; i++) {
                for (int j = 0; j < 3; j++) {
                    unsigned int* f = &in[i + j][0];
                    // printf("%u/%u\n", f[0], f[1]);
                    indices.push_back({f[0], f[1]});

                    for (int q = 0; q < 2; q++) {
                        if (f[q] < mins[q])
                            mins[q] = f[q];
                    }
                }
            }
        }
        else if (b == "stop")
            break;
    }

    for (int i = 0; i < indices.size(); i++) {
        vertices.push_back(verts[indices[i][0] - mins[0]]);
        texCoords.push_back(texs[indices[i][1] - mins[1]]);
    }
}
