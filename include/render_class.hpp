#pragma once
#ifndef _RENDER_CLASS_HPP_
#define _RENDER_CLASS_HPP_
#include<obj_loader.hpp>
#include<camera_ctrl.hpp>
#include<shader_loader.hpp>

#ifdef _WIN32
#include <windows.h>
#include <cstdlib>
#include <cstdio>
#include <clocale>
#endif

class RenderClass 
          : public CameraControl, public shader_loader
{
public:
          RenderClass(
                    std::string window_name,
                    GLFWwindow* window = nullptr,
                    bool enable_fullscreen = false
          );

          virtual ~RenderClass();

public:
          void add_object(std::string obj_name);
          void add_object(std::string obj_name,
                    glm::mat4&& T,
                    glm::mat4&& R,
                    glm::mat4&& S
          );
          void set_object(std::string target_obj,
                    glm::mat4&& T,
                    glm::mat4&& R,
                    glm::mat4&& S
          );

          void start_display();
          void terminate_opengl();

private:
          void init_opengl_glfw();
          void init_opengl_func();
          void init_window(const std::string& windows_name, bool is_fullscreen);
          void set_advance_features();
          void enable_fullscreen();
          void start_rendering();

          /*has to be deployed before creating window*/
          void set_version();                                                    

private:
          shader_loader _shader;

          /*store object's name and it's obj_loader class*/
          std::multimap<std::string, obj_loader> _loaded_objs;
};

#endif // !_RENDER_CLASS_HPP_