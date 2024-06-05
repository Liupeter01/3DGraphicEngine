#include<shader_loader.hpp>

shader_loader::shader_loader()
          :m_program(-1) 
{}

shader_loader::~shader_loader()
{
          /*delete shader*/
          for (const auto shader : m_shader) {
                    glDeleteShader(shader);
          }

          /*shader program is not created at all!*/
          if (m_program == -1) {
                    glDeleteProgram(m_program);
          }
}

void shader_loader::add_vertex_shader(const std::string& path)
{
          unsigned int shader = glCreateShader(GL_VERTEX_SHADER);
          add_shader_source(shader, path);
}

void shader_loader::add_fragment_shader(const std::string& path)
{
          unsigned int shader = glCreateShader(GL_FRAGMENT_SHADER);
          add_shader_source(shader, path);
}

void shader_loader::create_shader_program()
{
          m_program = glCreateProgram();
          for (const auto& shader : m_shader) {
                    glAttachShader(m_program, shader);
          }
          
          int status = Tools::link_shader(m_program);
          if (status != GL_TRUE) {
                    /*something went wrong*/
                    std::cout << Tools::get_link_error_info(m_program) << std::endl;
                    throw std::runtime_error("SHADER PROGRAM LINK FAILED!");
          }

;          for (const auto& shader : m_shader) {
                    glDeleteShader(shader);
          }
}

void  shader_loader::add_shader_source(const unsigned int shader, const std::string& path)
{
          std::string glsl = Tools::read_glsl_file(path);
          auto status = Tools::load_shader_source(shader, glsl);
          if (status != GL_TRUE) {
                    /*something went wrong*/
                    std::cout << Tools::get_shader_error_info(shader) << std::endl;
                    throw std::runtime_error("SHADER COMPILE FAILED!");
          }
          m_shader.push_back(shader);
}