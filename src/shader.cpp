//
//  shader.cpp
//  dungeon_mapper
//
//  Created by Clayton Knittel on 9/20/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::stringstream;


const char* SHADER_PATH = "res/shaders/";


program::program(const char *v_path, const char *f_path) {
    shader vert(v_path, GL_VERTEX_SHADER);
    shader frag(f_path, GL_FRAGMENT_SHADER);
    
    prog = glCreateProgram();
    glAttachShader(prog, vert.tag());
    glAttachShader(prog, frag.tag());
    glLinkProgram(prog);
    
    // print any linking errors
    GLint success;
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(prog, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
    
    glDeleteShader(vert.tag());
    glDeleteShader(frag.tag());
}

GLuint program::programLoc() {
    return prog;
}

GLuint program::uniformLoc(const char *name) {
    return glGetUniformLocation(prog, name);
}

void program::use() {
    glUseProgram(prog);
}


shader::shader(const char *code_loc, GLint type) {
    
    string codestr;
    ifstream file;
    file.exceptions(ifstream::badbit);
    
    try {
        std::ostringstream full_path;
        full_path << SHADER_PATH << code_loc;
        file.open(full_path.str());
        if (!file.is_open()) {
            fprintf(stderr, "could not open path to shader: %s", full_path.str().c_str());
            throw ifstream::failure("");
        }
        stringstream s;
        s << file.rdbuf();
        file.close();
        codestr = s.str();
    }
    catch(ifstream::failure e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
    }
    const GLchar * code = codestr.c_str();
    
    
    GLint success;
    
    self = glCreateShader(type);
    glShaderSource(self, 1, &code, NULL);
    glCompileShader(self);
    // print any compile errors
    glGetShaderiv(self, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(self, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        else if (type == GL_FRAGMENT_SHADER)
            cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }
}

GLuint shader::tag() {
    return self;
}

