
#include "objLoader.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

using std::ifstream;
using std::string;
using std::istringstream;

struct tuple {
    const static uint size = 2;
    uint i, j;
    uint operator[](const int ind) {
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
    const static uint size = 2;
    uint i, j, k;
    uint operator[](const int ind) {
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


const char* get_texture_loc(const char* obj_file) {
    ifstream f;
    f.open(obj_file);
    if (!f.is_open()) {
        printf("Unable to open .obj file %s\n", obj_file);
        return;
    }
    string buf, first;
    while (getline(f, buf)) {
        istringstream is(buf);
        is >> first;
        if (first == "usemtl") {
            is >> buf;
            return buf.c_str();
        }
    }
}


// read all data from the file which begins with the specified
// key and is in vec3 format (x, y, z)
template<typename dataType>
void loadAll(ifstream& f, vector<dataType> &vec, const string &key, void(*loadFunc)(istringstream&, vector<dataType>&)) {
    string buf, test;
    while (getline(f, buf) && (buf.size() <= key.size() || buf[key.size()] != ' ' || buf.substr(0, key.size()) != key));
    do {
        istringstream is(buf);
        is >> test;
        if (test != key)
            break;
        loadFunc(is, vec);
    } while (getline(f, buf));
}

void loadVec2(istringstream &is, vector<vec2> &v) {
    float x, y;
    if (!(is >> x >> y)) {
        printf("line containing \"%s\" corrupted\n", is.str().c_str());
        return;
    }
    x = x < 0 ? -x : x;
    y = y < 0 ? -y : y;
    v.push_back({x, y});
}

void loadVec3(istringstream &is, vector<vec3> &v) {
    float x, y, z;
    if (!(is >> x >> y >> z)) {
        printf("line containing \"%s\" corrupted\n", is.str().c_str());
        return;
    }
    v.push_back({x, y, z});
}

template<typename storage>
void loadIndex(istringstream &is, vector<storage> &v) {
    vector<string> inds;
    string buf, b2;
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
        if (in[in.size() - 1].size() != storage::size) {
            printf("expect %u indices, but found %lu in \"%s\"\n", storage::size, in[in.size() - 1].size(), st.c_str());
            return;
        }
    }
    for (int i = 0; i < in.size() - 2; i++) {
        for (int j = 0; j < 3; j++) {
            unsigned int* f = &in[i + j][0];
            v.push_back({f[0], f[1]});
        }
    }
}


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

    loadAll<vec3>(f, verts, "v", &loadVec3);
    loadAll<vec2>(f, texs, "vt", &loadVec2);
    loadAll<tuple>(f, indices, "f", &loadIndex);

    unsigned int mins[2] = {0xffffffff, 0xffffffff};
    for (tuple &i : indices) {
        for (int q = 0; q < 2; q++) {
            if (i[q] < mins[q])
                mins[q] = i[q];
        }
    }

    for (int i = 0; i < indices.size(); i++) {
        vertices.push_back(verts[indices[i][0] - mins[0]]);
        texCoords.push_back(texs[indices[i][1] - mins[1]]);
    }
}

void loadObj(const char* obj_file, vector<vec3> &vertices, vector<vec3> &normals) {
    ifstream f;
    f.open(obj_file);
    if (!f.is_open()) {
        printf("Unable to open .obj file %s\n", obj_file);
        return;
    }

    vector<vec3> verts;
    vector<vec3> norms;
    vector<tuple> indices;

    loadAll<vec3>(f, verts, "v", &loadVec3);
    loadAll<vec3>(f, norms, "vn", &loadVec3);
    loadAll<tuple>(f, indices, "f", &loadIndex);

    unsigned int mins[2] = {0xffffffff, 0xffffffff};
    for (tuple &i : indices) {
        for (int q = 0; q < 2; q++) {
            if (i[q] < mins[q])
                mins[q] = i[q];
        }
    }

    for (int i = 0; i < indices.size(); i++) {
        vertices.push_back(verts[indices[i][0] - mins[0]]);
        normals.push_back(norms[indices[i][1] - mins[1]]);
    }
}

void loadObj(const char* obj_file, vector<vec3> &vertices, vector<vec3> &normals, vector<vec2> &texCoords) {
    ifstream f;
    f.open(obj_file);
    if (!f.is_open()) {
        printf("Unable to open .obj file %s\n", obj_file);
        return;
    }

    vector<vec3> verts;
    vector<vec3> norms;
    vector<vec2> texs;
    vector<triple> indices;

    loadAll<vec3>(f, verts, "v", &loadVec3);
    loadAll<vec3>(f, norms, "vn", &loadVec3);
    loadAll<vec2>(f, texs, "vt", &loadVec2);
    loadAll<triple>(f, indices, "f", &loadIndex);

    unsigned int mins[3] = {0xffffffff, 0xffffffff, 0xffffffff};
    for (triple &i : indices) {
        for (int q = 0; q < 3; q++) {
            if (i[q] < mins[q])
                mins[q] = i[q];
        }
    }

    for (int i = 0; i < indices.size(); i++) {
        vertices.push_back(verts[indices[i][0] - mins[0]]);
        normals.push_back(norms[indices[i][1] - mins[1]]);
        texCoords.push_back(texs[indices[i][2] - mins[2]]);
    }
}
