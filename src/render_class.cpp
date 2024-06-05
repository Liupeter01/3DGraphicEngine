#include<render_class.hpp>

RenderClass::RenderClass(
          std::string window_name,
          GLFWwindow* window,
          bool enable_fullscreen)
          :shader_loader(),
          CameraControl()
{
          init_opengl_glfw();                  //glfw init
          set_version();                            //set the opengl version before init window

          /*Windows is not created yet*/
          if (window == nullptr) {
                    init_window(window_name, enable_fullscreen);         //after set version, create window
          }

          glfwMakeContextCurrent(m_window);

          init_opengl_func();
          set_advance_features();             //use glenable to activate some features

          registerCallBackFunctions();

#ifdef  _DEBUG
          printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
#endif // _DEBUG
}

RenderClass::~RenderClass()
{
          terminate_opengl();
}

void RenderClass::add_object(std::string obj_name)
{
          _loaded_objs.emplace(obj_name, obj_loader(obj_name));
}

void RenderClass::add_object(std::string obj_name,
          glm::mat4&& T,
          glm::mat4&& R,
          glm::mat4&& S)
{
          obj_loader obj = obj_loader(obj_name);
          obj.setObjectTranslateMatrix(std::move(T));
          obj.setObjectRotateMatrix(std::move(R));
          obj.setObjectScaleMatrix(std::move(S));

          _loaded_objs.emplace(obj_name, std::move(obj));
}

void RenderClass::set_object(std::string target_obj,
          glm::mat4&& T,
          glm::mat4&& R,
          glm::mat4&& S)
{
          decltype(_loaded_objs)::iterator it = _loaded_objs.find(target_obj);
          if (it == _loaded_objs.end() ){

#ifdef  _DEBUG
                    printf("[Error]: OpenGL Find Object Error\n");
#endif // _DEBUG

                    return;
          }

          (*it).second.setObjectTranslateMatrix(std::move(T));
          (*it).second.setObjectRotateMatrix(std::move(R));
          (*it).second.setObjectScaleMatrix(std::move(S));
}

void RenderClass::start_rendering()
{
          for (auto& object : _loaded_objs) 
          {
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

                   // Getting Uniform Variable From GLSL/VERT
                    auto vert_light = glGetUniformLocation(m_program, "uniLightPath");

                    //Getting In parameters from GLSL/VERT
                    auto vert_position = glGetAttribLocation(m_program, "position");
                    auto vert_normal = glGetAttribLocation(m_program, "normal");
                    auto vert_texture = glGetAttribLocation(m_program, "texCoord");
                    auto vert_color = glGetAttribLocation(m_program, "color");

                    glUniform3fv(vert_light, 1, glm::value_ptr(getCameraViewMatrix()));
                    glUniformMatrix4fv(glGetUniformLocation(m_program, "uniModel"), 1, GL_FALSE, glm::value_ptr(object.second.getObjectModelMatrix()));
                    glUniformMatrix4fv(glGetUniformLocation(m_program, "uniView"), 1, GL_FALSE, glm::value_ptr(getCameraViewMatrix()));
                    glUniformMatrix4fv(glGetUniformLocation(m_program, "uniProjection"), 1, GL_FALSE, glm::value_ptr(getProjectionMatrix()));

                    glUseProgram(m_program);
                    glBindVertexArray(object.second.cur_object_vao()); 

                    glDrawElements(GL_TRIANGLES, object.second.cur_object_element(), GL_UNSIGNED_INT, 0);
                    object.second.unbind_vao();
          }
}

void RenderClass::start_display()
{
          /*user enable smooth graphic mode*/
          glfwSwapInterval(1);

          while (!glfwWindowShouldClose(m_window)) {
                    start_rendering();

                    glfwSwapBuffers(m_window);
                    glfwPollEvents();
          }
}

void RenderClass::init_opengl_glfw()
{
          if (!glfwInit()) {
                    terminate_opengl();
                    throw std::runtime_error("Failed to Init GLFW");
          }
}

void RenderClass::init_opengl_func()
{
          if (!gladLoadGL()) {
                    terminate_opengl();
                    throw std::runtime_error("Failed to Load GL functions");
          }
}

void RenderClass::terminate_opengl()
{
          glfwDestroyWindow(m_window);
          glfwTerminate();
}

void  RenderClass::enable_fullscreen()
{
          GLFWmonitor* monitor = glfwGetPrimaryMonitor();
          if (monitor != nullptr) 
          {
                    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                    if (mode != nullptr) 
                    {
                              glfwSetWindowSize(m_window, mode->width, mode->height);
                              glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

                              /*update members' value*/
                              m_height = mode->height;
                              m_width = mode->width;
                    }
          }
}

void RenderClass::init_window(const std::string & windows_name, bool is_fullscreen)
{
          /*default screen size is 1024*768*/ 
          m_window = glfwCreateWindow(m_width, m_height, windows_name.c_str(), nullptr, nullptr);

          /*user needs full screen*/
          if (is_fullscreen) {
                    enable_fullscreen();
          }
}

void RenderClass::set_version()
{
          constexpr int version = 33;   //OpenGL 3.3
          glfwWindowHint(GL_SAMPLES, 4);          //MSAAx4
          glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
          glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
          glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version / 10);
          glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version % 10);
          if (version >= 33) {
                    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
                    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
          }

          glfwWindowHint(GLFW_DEPTH_BITS, 24);
          glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
}

void RenderClass::set_advance_features()
{
          glEnable(GL_DEPTH_TEST);
          glDepthFunc(GL_LESS);

          glEnable(GL_MULTISAMPLE);
          glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

          glEnable(GL_CULL_FACE);
          glCullFace(GL_BACK);
          glFrontFace(GL_CCW);
}