# 3D GRAPHIC ENGINE

## Description

## Example
``` C++
#include <render_class.hpp>

int main()
{          
          /*init render class (disable full-screen)*/
          RenderClass render("OpenGL", nullptr, false);

          /*set T R S matrix parameters*/
          render.add_object("../../../assets/monkey.obj",
                    glm::translate(glm::mat4(1), glm::vec3(0.5f, 0.5f, 0.75f)),
                    glm::mat4(1), 
                    glm::scale(glm::mat4(1), glm::vec3(0.5f, 0.5f, 0.5f))
          );
          render.add_object("../../../assets/cat.obj",
                    glm::translate(glm::mat4(1), glm::vec3(-0.9f, -0.9f, -0.75f)),
                    glm::mat4(1),
                    glm::mat4(1)
          );
          
          /*start opengl engine with flat mode(true for enable flat / false for enable smooth)*/
          render.start_display(/*enable flat mode = */ false);
          return 0;
}
```

## Developer Quick Start
### Platform Support
Windows, Linux, MacOS(Intel and M Serious Chip)
### Building 3D Graphic Engine
``` bash
git clone https://github.com/Liupeter01/3DGraphicEngine
cd 3DGraphicEngine
cmake -B build
cmake --build build --parallel x
```
