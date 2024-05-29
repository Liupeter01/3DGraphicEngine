#pragma once
#ifndef _OBJ_LOADER_HPP_
#define _OBJ_LOADER_HPP_

#include<iostream>
#include<vector>
#include<tuple>
#include<glm/glm.hpp>

class RenderClass;

class obj_loader 
{
          friend class RenderClass;

public:
          using VerTexType = std::vector<glm::vec3>;
          using Normal =glm::vec3;
          using Uv = glm::vec2;    
          obj_loader(std::string obj_path) ;

public:
          void load_obj();

          const VerTexType& getVertexSet();
          const std::vector<glm::umat3x3>& getFacesMatrics();
          const std::vector < Normal >& getNormalsSet();
          const std::vector < Uv >& getUvSet();

          void setObjectTranslateMatrix(glm::mat4&& T);
          void setObjectRotateMatrix(glm::mat4&& R);
          void setObjectScaleMatrix(glm::mat4&& S);

          const glm::mat4x4& getObjectModelMatrix();

private:
          void updateObjectModelMatrix();

private:
          //translate, rotate, scale
          glm::mat4x4 _T, _R, _S;
          glm::mat4x4 _model;

          std::string _path;

          /*vertex coordinates for object*/
          VerTexType _vertexSet;   

          /*normals for object*/
          std::vector < Normal > _normals;

          /*UV for object*/
          std::vector < Uv > _uvs;

          /*include three triangles' indexes & nomals & uvs*/
          std::vector<glm::umat3x3> _faces;
};

#endif // !_OBJ_LOADER_HPP_