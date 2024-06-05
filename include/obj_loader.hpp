#pragma once
#ifndef _OBJ_LOADER_HPP_
#define _OBJ_LOADER_HPP_

#include<iostream>
#include<vector>
#include<buffer_object.hpp>

class RenderClass;

class obj_loader :public virtual_array_object
{
          friend class RenderClass;

public:
          obj_loader(std::string obj_path);

public:
          void load_obj();
          void calculate_vertex_normal();

          void setObjectTranslateMatrix(glm::mat4&& T);
          void setObjectRotateMatrix(glm::mat4&& R);
          void setObjectScaleMatrix(glm::mat4&& S);

          const glm::mat4x4& getObjectModelMatrix();

private:
          void updateObjectModelMatrix();

private:
          std::string _path;

          //translate, rotate, scale
          glm::mat4x4 _T = glm::mat4(1);
          glm::mat4x4 _R = glm::mat4(1);
          glm::mat4x4 _S = glm::mat4(1);

          glm::mat4x4 _model = glm::mat4(1);

          /*retrieve all data stored in  _vertexSet, _normals and _uvs*/
          std::vector<MultiAttributeSOA> _vertices;

          /*include three triangles' indexes & nomals & uvs*/
          std::vector<glm::uvec3> _faces;

          /*vertex coordinates for object*/
          std::vector<glm::vec3> _vertexSet;

          /*Face normals for object*/
          std::vector < glm::vec3 > _normals;

          /*UV for object*/
          std::vector < glm::vec2 > _uvs;
};

#endif // !_OBJ_LOADER_HPP_