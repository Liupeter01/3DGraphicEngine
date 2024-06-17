#pragma once
#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_
#include<string>
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

namespace GraphicEngine {
          struct Tools
          {
                    /*normal math calculation*/
                    static glm::vec3 perspective_divide(glm::vec4 vec);
                    static glm::vec3 normal_calcualtion(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc);
                    static glm::vec3 normal_calculation_with_weight(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc);

                    /*compile and link shader*/
                    static std::string read_glsl_file(const std::string& path);
                    static int load_shader_source(const int shader, const std::string& glsl_code);
                    static int link_shader(const int program);

                    /*check shader & shader program error info log*/
                    static std::string get_shader_error_info(const int shader);
                    static std::string get_link_error_info(const int program);
          };
};

#endif // !_TOOLS_HPP_
