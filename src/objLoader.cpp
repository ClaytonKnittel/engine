
#include "objLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

using std::ifstream;
using std::istringstream;

#define RESOURCE_LOC "res/"
#define IMG_TYPE ".png"

struct single {
    const static uint size = 1;
    uint i;

    single(const vector<uint> &v): i(v[0]) {}

    uint operator[](const int ind) {
        if (ind == 0)
            return i;
        return 0xffffffff;
    }
};

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
void loadAll(vector<string> &f, vector<dataType> &vec, const string &key, void(*loadFunc)(istringstream&, vector<dataType>&)) {
    string test;
    for (auto it = f.begin(); it != f.end(); it++) {
        if (it->size() < key.size()) continue;
        istringstream is(*it);
        if (!(is >> test) || test != key)
            continue;
        loadFunc(is, vec);
    }
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

template<typename storage, typename T>
void assign(vector<storage> &indices, int index, unsigned int *mins, std::pair<vector<T>&, vector<T>&> vec) {
    for (int i = 0; i < indices.size(); i++)
        vec.second.push_back(vec.first[indices[i][index] - mins[index]]);
}

template<typename storage, typename T, typename...Rest>
void assign(vector<storage> &indices, int index, unsigned int *mins, std::pair<vector<T>&, vector<T>&> vec, std::pair<vector<Rest>&, vector<Rest>&> ...vecs) {
    assign(indices, index, mins, vec);
    assign(indices, index + 1, mins, vecs...);
}


/*
Takes a series of pairs of vectors of any type of vecmath vector,
with the first vector in the pair being the source and the second
being the destination. Using the index data from file, each of
the destination vectors is filled with corresponding data from
source so that indexing will not be necessary when rendering
(this is due to OpenGL not supporting multiple indexing)
*/
template<typename storage, typename...T>
void indexVectors(vector<string> &file, std::pair<vector<T>&, vector<T>&> ...vecs) {
    vector<storage> indices;
    loadAll<storage>(file, indices, "f", &loadIndex);

    constexpr size_t num = sizeof...(T);
    unsigned int mins[num];
    memset(mins, 0xffffffff, num);
    for (storage &i : indices) {
        for (int q = 0; q < num; q++) {
            if (i[q] < mins[q])
                mins[q] = i[q];
        }
    }
    assign(indices, 0, mins, vecs...);
}


void loadObj(const char* obj_file, vector<vec3> *vertices, vector<vec3> *normals, vector<vec2> *texCoords) {
    ifstream f;
    open_obj_file(f, obj_file);
    vector<string> file;
    string buf;
    while (getline(f, buf)) file.push_back(buf);

    vector<vec3> verts;
    vector<vec3> norms;
    vector<vec2> texs;
    vector<triple> indices;

    loadAll<vec3>(file, verts, "v", &loadVec3);
    if (normals)
        loadAll<vec3>(file, norms, "vn", &loadVec3);
    if (texCoords)
        loadAll<vec2>(file, texs, "vt", &loadVec2);

    printf("sizes: norms %lu, texs %lu\n", norms.size(), texs.size());
    printf("%s\n", obj_file);

    if (norms.size() > 0 && texs.size() > 0) {
        indexVectors<triple>(file, std::pair<vector<vec3>&, vector<vec3>&>(verts, *vertices),
                                   std::pair<vector<vec3>&, vector<vec3>&>(norms, *normals),
                                   std::pair<vector<vec2>&, vector<vec2>&>(texs, *texCoords));
    }
    else if (norms.size() > 0) {
        indexVectors<tuple>(file, std::pair<vector<vec3>&, vector<vec3>&>(verts, *vertices),
                                   std::pair<vector<vec3>&, vector<vec3>&>(norms, *normals));
    }
    else if (texs.size() > 0) {
        indexVectors<tuple>(file, std::pair<vector<vec3>&, vector<vec3>&>(verts, *vertices),
                                   std::pair<vector<vec2>&, vector<vec2>&>(texs, *texCoords));
    }
    else {
        indexVectors<single>(file, std::pair<vector<vec3>&, vector<vec3>&>(verts, *vertices));
    }
}
