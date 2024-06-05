#include <render_class.hpp>

int main()
{          
          /*init render class (disable full-screen)*/
          RenderClass render("OpenGL", nullptr, false);

          /*set shaders(vertex & fragment)*/
          render.add_vertex_shader(GRAPHICENGINE_HOME"assets/vertex_shader.vert");
          render.add_fragment_shader(GRAPHICENGINE_HOME"assets/fragment_shader.frag");
          render.create_shader_program();

          /*set T R S matrix parameters*/
          render.add_object(GRAPHICENGINE_HOME"assets/monkey.obj", glm::mat4(1), glm::mat4(1), glm::mat4(1));
          render.add_object(GRAPHICENGINE_HOME"assets/cat.obj", glm::mat4(1), glm::mat4(1), glm::mat4(1));

          /*start opengl engine with flat mode(true for enable flat / false for enable smooth)*/
          render.start_display();
          return 0;
}