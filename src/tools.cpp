#include<tools.hpp>
#include<fstream>

glm::vec3 GraphicEngine::Tools::perspective_divide(glm::vec4 vec)
{
          return glm::vec3(vec.x / vec.w, vec.y / vec.w, vec.z / vec.w);
}

glm::vec3 GraphicEngine::Tools::normal_calcualtion(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc)
{
          const glm::vec3 AB = pb - pa;
          const glm::vec3 AC = pc - pa;
          return glm::normalize(glm::cross(AB, AC));
}

glm::vec3 GraphicEngine::Tools::normal_calculation_with_weight(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc)
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

std::string GraphicEngine::Tools::read_glsl_file(const std::string& path)
{
          std::ifstream inf(path);
          if (!inf.is_open()) {
                    std::cout << "GLSL File Read Error!" << std::endl;
                    return std::string{};
          }
          return std::string{
                    std::istreambuf_iterator<char>(inf),
                    std::istreambuf_iterator<char>()
          };
}

int GraphicEngine::Tools::load_shader_source(const int shader, const std::string& glsl_code)
{
          int status;         //compile status

          const char* glsl_string = glsl_code.c_str();
          //int glsl_length[] = { static_cast<int>(glsl_code.size()) };
          //glShaderSource(shader, 1, glsl_string, glsl_length);
          glShaderSource(shader, 1, &glsl_string, nullptr);

          glCompileShader(shader);

          /*get compile status, is success or not*/
          glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
          return status;
}

int  GraphicEngine::Tools::link_shader(const int program)
{
          int status;         //compile status
          glLinkProgram(program);

          glGetProgramiv(program, GL_LINK_STATUS, &status);
          return status;
}

std::string GraphicEngine::Tools::get_shader_error_info(const int shader)
{
          /*get shader info log length*/
          int log_length;                //using glGetShaderiv and get log length
          int return_log_length;    //after using  glGetShaderInfoLog, the actual size of the error log

          glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

          /*The Return value*/
          std::string error_info;
          error_info.resize(log_length);
          glGetShaderInfoLog(shader, log_length, &return_log_length, error_info.data());
          error_info.resize(return_log_length);   //shrink the size
          return error_info;
}

std::string GraphicEngine::Tools::get_link_error_info(const int program)
{
          /*get shader info log length*/
          int log_length;                //using glGetShaderiv and get log length
          int return_log_length;    //after using  glGetShaderInfoLog, the actual size of the error log

          glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

          /*The Return value*/
          std::string error_info;
          error_info.resize(log_length);
          glGetProgramInfoLog(program, log_length, &return_log_length, error_info.data());
          error_info.resize(return_log_length);   //shrink the size
          return error_info;
}