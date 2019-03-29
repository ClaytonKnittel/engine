
#include "objLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

using std::ifstream;
using std::istringstream;

#define RESOURCE_LOC "res/"
#define IMG_TYPE ".png"

struct tuple {
    const static uint size = 2;
    uint i, j;

    tuple(const vector<uint> &v): i(v[0]), j(v[1]) {}

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
    const static uint size = 3;
    uint i, j, k;

    triple(const vector<uint> &v): i(v[0]), j(v[1]), k(v[2]) {}

    uint operator[](const int ind) {
        switch (ind) {
        case 0:
            return i;
        case 1:
            return j;
        case 2:
            return k;
        }
        return 0xffffffff;
    }
};


void open_obj_file(ifstream &f, const char* obj_file) {
    f.open(obj_file);
    if (!f.is_open()) {
        f.open(string(RESOURCE_LOC) + obj_file);
        if (!f.is_open()) {
            fprintf(stderr, "Unable to open .obj file %s\n", obj_file);
            throw std::exception();
        }
    }
}

string get_texture_loc(const char* obj_file) {
    ifstream f;
    open_obj_file(f, obj_file);
    string obj_file_str(obj_file);
    string dir;
    auto find = obj_file_str.find_last_of("/\\");
    if (find != string::npos)
        dir = obj_file_str.substr(0, find + 1);

    string buf, first;
    while (getline(f, buf)) {
        istringstream is(buf);
        is >> first;
        if (first == "usemtl") {
            is >> buf;
            return dir + buf + IMG_TYPE;
        }
    }
    return "";
}


// read all data from the file which begins with the specified
// key and is in vec3 format (x, y, z)
template<typename dataType>
void loadAll(ifstream& f, vector<dataType> &vec, const string &key, void(*loadFunc)(istringstream&, vector<dataType>&)) {
    string buf, test;
    while (getline(f, buf) && (buf.size() <= key.size() || buf[key.size()] != ' ' || buf.substr(0, key.size()) != key));
    do {
        istringstream is(buf);
        if (!(is >> test) || test != key)
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
            v.push_back(in[i + j]);
        }
    }
}


void loadObj(const char* obj_file, vector<vec3> &vertices, vector<vec2> &texCoords) {
    ifstream f;
    open_obj_file(f, obj_file);

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
    open_obj_file(f, obj_file);

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
    open_obj_file(f, obj_file);

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
