#pragma once
#ifndef _BUFFER_OBJECT_HPP_
#define  _BUFFER_OBJECT_HPP_
#include<vector>
#include<tools.hpp>

typedef struct MultiAttributeSOA {
          glm::vec3 position; //offsetof(MultiAttributeSOA, position);
          glm::vec3 normal;   //offsetof(MultiAttributeSOA, normal);
}MultiAttributeSOA;

class virtual_array_object;

struct buffer
{
protected:
          unsigned int _handle_obj = 0;
};

struct buffer_object :public buffer {
          buffer_object();
          virtual ~buffer_object(); 
};

struct virtual_buffer_object :public buffer_object
{
          friend class virtual_array_object;
          virtual_buffer_object();
          virtual ~virtual_buffer_object() {}

protected:
          void setupMultiAttribute( const std::vector< MultiAttributeSOA>& vertics, GLenum target);
          virtual virtual_buffer_object& bind(GLenum target) final;
          virtual void unbind(GLenum target) final;
};

struct element_buffer_object :public buffer_object
{
          friend class virtual_array_object;
          element_buffer_object();
          virtual ~element_buffer_object() {}

protected:
          void setupTriangleFaces(const std::vector<glm::uvec3>& faces, GLenum target);
          virtual element_buffer_object& bind(GLenum target) final;
          virtual void unbind(GLenum target) final;
};

class virtual_array_object :public buffer
{
public:
          virtual_array_object();
          virtual ~virtual_array_object() {}
          
public:
          unsigned int cur_object_vao() const;
          std::size_t cur_object_element() const;

protected:
          virtual void bind_vao() final;
          virtual void unbind_vao() final;

          virtual void bind_vbo(const std::vector< MultiAttributeSOA>& vertics, GLenum target = GL_ARRAY_BUFFER) final;
          virtual void bind_ebo(const std::vector<glm::uvec3>& faces, GLenum target = GL_ELEMENT_ARRAY_BUFFER) final;

private:
          std::size_t _element = 0;
          virtual_buffer_object _vbo;
          element_buffer_object _ebo;
};

#endif // !_BUFFER_OBJECT_HPP_
