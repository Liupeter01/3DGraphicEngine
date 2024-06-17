#include<buffer_object.hpp>

GraphicEngine::buffer_object::buffer_object()
{
          glGenBuffers(1, &_handle_obj);
}

GraphicEngine::buffer_object::~buffer_object()
{
          glDeleteBuffers(1, &_handle_obj);
}

GraphicEngine::virtual_buffer_object::virtual_buffer_object()
          :buffer_object()
{
}

void GraphicEngine::virtual_buffer_object::setupMultiAttribute(const std::vector< MultiAttributeSOA>& vertics, GLenum target)
{
          glBufferData(target, sizeof(MultiAttributeSOA) * vertics.size(), vertics.data(), GL_STATIC_DRAW);

          glEnableVertexAttribArray(0);
          glVertexAttribPointer(0, glm::vec3::length(), GL_FLOAT, GL_FALSE, sizeof(MultiAttributeSOA),
                    reinterpret_cast<const void*>(offsetof(MultiAttributeSOA, position))
          );

          glEnableVertexAttribArray(1);
          glVertexAttribPointer(1, glm::vec3::length(), GL_FLOAT, GL_FALSE, sizeof(MultiAttributeSOA),
                    reinterpret_cast<const void*>(offsetof(MultiAttributeSOA, normal))
          );
}

GraphicEngine::virtual_buffer_object& GraphicEngine::virtual_buffer_object::bind(GLenum target)
{
          glBindBuffer(target, _handle_obj);
          return *this;
}

void  GraphicEngine::virtual_buffer_object::unbind(GLenum target)
{
          glBindBuffer(target, 0);
}

GraphicEngine::element_buffer_object::element_buffer_object()
          :buffer_object()
{
}

void GraphicEngine::element_buffer_object::setupTriangleFaces(const std::vector<glm::uvec3>& faces, GLenum target)
{
          glBufferData(target, sizeof(glm::uvec3) * faces.size(), faces.data(), GL_STATIC_DRAW);
}

GraphicEngine::element_buffer_object& GraphicEngine::element_buffer_object::bind(GLenum target)
{
          glBindBuffer(target, _handle_obj);
          return *this;
}

void GraphicEngine::element_buffer_object::unbind(GLenum target)
{
          glBindBuffer(target, 0);
}

GraphicEngine::virtual_array_object::virtual_array_object()
          :_vbo(virtual_buffer_object()),
          _ebo(element_buffer_object())
{
          glGenVertexArrays(1, &_handle_obj);
}

unsigned int GraphicEngine::virtual_array_object::cur_object_vao() const
{
          return this->_handle_obj;
}

std::size_t GraphicEngine::virtual_array_object::cur_object_element() const
{
          return this->_element;
}

void GraphicEngine::virtual_array_object::bind_vao()
{
          glBindVertexArray(_handle_obj);
}

void  GraphicEngine::virtual_array_object::bind_vbo(const std::vector< MultiAttributeSOA>& vertics, GLenum target)
{
          _vbo.bind(target).setupMultiAttribute(vertics, target);
}

void  GraphicEngine::virtual_array_object::bind_ebo(const std::vector<glm::uvec3>& faces, GLenum target)
{
          /*do the triangle faces setup*/
          _ebo.bind(target).setupTriangleFaces(faces, target);

          /*setup element sizes*/
          this->_element = sizeof(glm::uvec3) * faces.size();
}

void GraphicEngine::virtual_array_object::unbind_vao()
{
          glBindVertexArray(0);
}