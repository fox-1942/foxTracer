//
// Created by fox-1942 on 3/26/20.
//

#include <GL/glew.h>
#include "Shader.h"

#ifndef OPENGL_SHADERPROGRAM_H
#define OPENGL_SHADERPROGRAM_H

#endif //OPENGL_SHADERPROGRAM_H

class ShaderProgram {


public:
    GLuint shaderProgram_id;
    bool isLinked = false;

    void CreateShaderProgram();

    bool addShaderToProgram(const Shader &shader);

    bool linkShaderProgram();

    void useProgram();

    void deleteProgram();

    int getShaderProgram_id() const;

    void setUniform1i(const std::string &name, int v0);

    void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3);

    int getUniformLocation(const std::string &name);

    void setUniformMat4f(const std::string &name, glm::mat4 &matrix);

    void setUniformVec3f(const std::string &name, glm::vec3 &vec3);

    static void GLClearError();

    static void GLCheckError();


};