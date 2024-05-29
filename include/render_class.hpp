#pragma once
#ifndef _RENDER_CLASS_HPP_
#define _RENDER_CLASS_HPP_
#include<obj_loader.hpp>
#include<camera_ctrl.hpp>

#ifdef _WIN32
#include <windows.h>
#include <cstdlib>
#include <cstdio>
#include <clocale>
#endif

struct KeyBindings {
          uint16_t keyboard_ctrl_button;          //control key on keyboard(ctrl, shift,alt)
          uint16_t mouse_operation;               //bind control key and mouse key press
};

class RenderClass : public CameraControl
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

          void start_display(bool isFlat);
          void terminate_opengl();

          static glm::vec3 perspective_divide(glm::vec4 vec);
          static glm::vec3 normal_calcualtion(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc);
          static glm::vec3 normal_calculation_with_weight(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc);

private:
          void init_opengl_glfw();
          void init_opengl_func();
          void init_window(const std::string& windows_name, bool is_fullscreen);
          void set_advance_features();
          void enable_fullscreen();
          void enable_smooth_mode();
          void start_rendering(bool isFlat, bool realTimeNormalCalcualtion);

          /*has to be deployed before creating window*/
          void set_version();                                                    

private:
          /*store object's name and it's obj_loader class*/
          std::multimap<std::string, obj_loader> _loaded_objs;
};

#endif // !_RENDER_CLASS_HPP_