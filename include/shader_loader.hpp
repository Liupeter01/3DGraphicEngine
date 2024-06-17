#pragma once
#ifndef _SHADER_LOADER_HPP_
#define _SHADER_LOADER_HPP_
#include<iostream>
#include<vector>
#include<tools.hpp>

namespace GraphicEngine {

          class RenderClass;

          class shader_loader {
                    friend class RenderClass;
                    std::vector<unsigned int> m_shader;

          public:
                    shader_loader();
                    virtual ~shader_loader();

          public:
                    void add_vertex_shader(const std::string& path);
                    void add_fragment_shader(const std::string& path);
                    void create_shader_program();

          private:
                    void add_shader_source(const unsigned int shader, const std::string& path);

          protected:
                    unsigned int m_program;
                    std::vector<unsigned int> m_shaders;
          };

};
#endif // !_SHADER_LOADER_HPP_
