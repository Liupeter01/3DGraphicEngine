#include<render_class.hpp>

RenderClass::RenderClass(
          std::string window_name, 
          GLFWwindow* window, 
          bool enable_fullscreen)
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

void RenderClass::start_rendering(bool isFlat, bool realTimeNormalCalcualtion)
{
          glBegin(GL_TRIANGLES);
          /*set color*/
          glColor3f(0.9f, 0.6f, 0.1f);

          for (auto& object : _loaded_objs) 
          {
                    for (const auto& index : object.second._faces) 
                    {
                              glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                              const auto VM = getCameraViewMatrix() * object.second.getObjectModelMatrix();   //VM
                              const auto VM_normal = glm::transpose(glm::inverse(glm::mat3x3(VM)));                 //VM transform
                              const auto PVM_vertex = getProjectionMatrix() * VM;                                                  //PVM

                              const auto p1_pos = object.second._vertexSet.at(index[0][0]);
                              const auto p2_pos = object.second._vertexSet.at(index[1][0]);
                              const auto p3_pos = object.second._vertexSet.at(index[2][0]);

                              const auto p1_uv = object.second._uvs.at(index[0][1]);
                              const auto p2_uv = object.second._uvs.at(index[1][1]);
                              const auto p3_uv = object.second._uvs.at(index[2][1]);

                              /*When Smooth mode is enabled!!!*/
                              if (!isFlat) {

                                        /*the normal of vertexes have already calculated previously*/
                                        const auto p1_vertex_normal = object.second._normals.at(index[0][0]);
                                        const auto p2_vertex_normal = object.second._normals.at(index[1][0]);
                                        const auto p3_vertex_normal = object.second._normals.at(index[2][0]);

                                        /* normal*/
                                        //glNormal3fv(glm::value_ptr(glm::transpose(glm::inverse(glm::mat3x3(_view * _model))) * p1_vertex_normal));
                                        //glVertex3fv(glm::value_ptr(RenderClass::perspective_divide(_projection * _view * _model * glm::vec4(p1_pos, 1))));
                                        
                                        glNormal3fv(glm::value_ptr(VM_normal * p1_vertex_normal));
                                        glVertex3fv(glm::value_ptr(perspective_divide(PVM_vertex * glm::vec4(p1_pos, 1))));

                                        glNormal3fv(glm::value_ptr(VM_normal * p2_vertex_normal));
                                        glVertex3fv(glm::value_ptr(perspective_divide(PVM_vertex * glm::vec4(p2_pos, 1))));

                                        glNormal3fv(glm::value_ptr(VM_normal * p3_vertex_normal));
                                        glVertex3fv(glm::value_ptr(perspective_divide(PVM_vertex * glm::vec4(p3_pos, 1))));
                              }
                              else {
                                        /*When Flat mode is enabled!!!*/
                                        glm::vec3 face_normal = normal_calcualtion(p1_pos, p2_pos, p3_pos);

                                         /*only need to specify a single face normal*/
                                        glNormal3fv(glm::value_ptr(face_normal));
                                        glVertex3fv(glm::value_ptr(perspective_divide(PVM_vertex * glm::vec4(p1_pos, 1))));
                                        glVertex3fv(glm::value_ptr(perspective_divide(PVM_vertex * glm::vec4(p2_pos, 1))));
                                        glVertex3fv(glm::value_ptr(perspective_divide(PVM_vertex * glm::vec4(p3_pos, 1))));
                              }
                    }
          }
          glEnd();
}

void RenderClass::start_display(bool isFlat)
{
          /*user enable smooth graphic mode*/
          if (!isFlat) {
                    enable_smooth_mode();
          }

          //glfwSwapInterval(1);

          while (!glfwWindowShouldClose(m_window)) {
                    //start_rendering(isFlat, true);
                    start_rendering(isFlat, false);

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

glm::vec3 RenderClass::perspective_divide(glm::vec4 vec)
{
          return glm::vec3(vec.x / vec.w, vec.y / vec.w, vec.z / vec.w);
}

glm::vec3 RenderClass::normal_calcualtion(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc)
{
          const glm::vec3 AB = pb - pa;
          const glm::vec3 AC = pc - pa;
          return glm::normalize(glm::cross(AB, AC));
}

glm::vec3 RenderClass::normal_calculation_with_weight(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc)
{
          const glm::vec3 AB = pb - pa;
          const glm::vec3 AC = pc - pa;
          glm::vec3 normal = glm::cross(AB, AC);     //calculate normal vector

          const float length = glm::length(normal);               //length of normal vector
          const float arc_sin_degree = length / (glm::length(AB) * glm::length(AC));

          /*length must not equal to zero*/
          if (!(-(1e-8) <= length && length <= 1e-8)) {
                    normal = normal * (glm::asin(arc_sin_degree) / length);
          }
          return normal;
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

void  RenderClass::enable_smooth_mode()
{
          for (auto& object : _loaded_objs) {
                    /*clean existing normal and calculate vertex normal*/
                    object.second._normals.resize(object.second._vertexSet.size());

                    for (const auto& index : object.second._faces) {
                              const auto p1_pos = object.second._vertexSet.at(index[0][0]);
                              const auto p2_pos = object.second._vertexSet.at(index[1][0]);
                              const auto p3_pos = object.second._vertexSet.at(index[2][0]);

                              auto face_normal_with_weight = normal_calculation_with_weight(p1_pos, p2_pos, p3_pos);

                              object.second._normals.at(index[0][0]) = face_normal_with_weight;
                              object.second._normals.at(index[1][0]) = face_normal_with_weight;
                              object.second._normals.at(index[2][0]) = face_normal_with_weight;
                    }

                    //do the normalize process(we only need the direction)
                    for (auto& _normal : object.second._normals) {
                              _normal = glm::normalize(_normal);
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
          constexpr int version = 20;   //OpenGL 2.0
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
          //glEnable(GL_DEPTH_TEST);
          //glDepthFunc(GL_LESS);

          glEnable(GL_STENCIL_TEST);

          glEnable(GL_MULTISAMPLE);
          glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

          glEnable(GL_LIGHTING);
          glEnable(GL_LIGHT0);
          glEnable(GL_COLOR_MATERIAL);

          glEnable(GL_CULL_FACE);
          glCullFace(GL_BACK);
          glFrontFace(GL_CCW);
}