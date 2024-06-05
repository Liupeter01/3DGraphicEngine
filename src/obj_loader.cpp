#include<obj_loader.hpp>
#include<fstream>
#include<string>
#include<sstream>

obj_loader::obj_loader(std::string obj_path)
          :_path(obj_path), virtual_array_object()
{
          /*load object from file*/
          this->load_obj();

#ifdef  _DEBUG
          printf("ObjLoader loaded asset %s: %llu vertex, %llu faces Loaded!\n", obj_path.c_str(), _vertexSet.size(), _faces.size());
#endif // _DEBUG

          /*calculate vertex normal in order to generate OBJ_AVO*/
          this->calculate_vertex_normal(); 

          /*Bind vao structure*/
          bind_vao();
          bind_vbo(this->_vertices);
          bind_ebo(this->_faces);
          unbind_vao();
}

void obj_loader::load_obj() 
{
          std::ifstream in(_path);

          if (!in.is_open()) {
                    std::cout << "Obj File Read Error!" << std::endl;
                    throw std::runtime_error("Obj File Read Error!");
          }

          while (!in.eof()) {
                    std::string buffer;
                    std::getline(in, buffer);
                    /*Search for Vertex*/
                    if (buffer.substr(0, 2) == "v ") {
                              std::istringstream ss(buffer.substr(2));
                              glm::vec3 vertex;
                              ss >> vertex.x >> vertex.y >> vertex.z;
                              _vertexSet.push_back(std::move(vertex));
                    }

                    /*Search for Faces(Triangle Indexes)*/
                    if (buffer.substr(0, 2) == "f ") {      //seperate ?/?/? ?/?/? ?/?/? ?/?/?
                              std::istringstream ss(buffer.substr(2));
                              std::string splitted;
                              std::vector<unsigned int> faces_vec;
                              while (std::getline(ss, splitted, ' ')) {  //handle ?/?/? each time
                                        unsigned int index = 0;
                                        std::istringstream(splitted) >> index;
                                        faces_vec.push_back(index - 1);
                              }
                              for (std::size_t i = 2; i < faces_vec.size() ; ++i) {
                                        glm::vec3 face(faces_vec[0], faces_vec[i - 1], faces_vec[i]);
                                        _faces.push_back(std::move(face));
                              }
                    }

                    /*Search for Normals */
                    if (buffer.substr(0, 3) == "vn ") {
                              std::istringstream ss(buffer.substr(3));
                              glm::vec3 normal;
                              ss >> normal.x >> normal.y >> normal.z;
                              _normals.push_back(std::move(normal));
                    }

                    /*Search for UV*/
                    if (buffer.substr(0, 3) == "vt ") {
                              std::istringstream ss(buffer.substr(3));
                              glm::vec2 uv;
                              ss >> uv.x >> uv.y;
                              _uvs.push_back(std::move(uv));
                    }
          }
          in.close();
}

void obj_loader::calculate_vertex_normal() 
{
          /*calibrate the size of VAO*/
          _vertices.resize(_vertexSet.size());    

          for (const auto& face_ind : _faces) {
                    MultiAttributeSOA  obj1, obj2, obj3;
                    obj1.position = std::move(_vertexSet.at(face_ind.x));
                    obj2.position = std::move(_vertexSet.at(face_ind.y));
                    obj3.position = std::move(_vertexSet.at(face_ind.z));

                    obj1.normal = glm::normalize(Tools::normal_calculation_with_weight(obj1.position, obj2.position, obj3.position));
                    obj2.normal = glm::normalize(Tools::normal_calculation_with_weight(obj2.position, obj3.position, obj1.position));
                    obj3.normal = glm::normalize(Tools::normal_calculation_with_weight(obj3.position, obj1.position, obj2.position));

                    _vertices.at(face_ind.x) = std::move(obj1);
                    _vertices.at(face_ind.y) = std::move(obj2);
                    _vertices.at(face_ind.z) = std::move(obj3);
          }
}

void obj_loader::updateObjectModelMatrix()
{
          _model = _T * _R * _S;
}

const glm::mat4x4& obj_loader::getObjectModelMatrix()
{
          updateObjectModelMatrix();
          return _model;
}

void obj_loader::setObjectTranslateMatrix(glm::mat4&& T)
{
          _T = T;
}

void obj_loader::setObjectRotateMatrix(glm::mat4&& R)
{
          _R = R;
}

void obj_loader::setObjectScaleMatrix(glm::mat4&& S)
{
          _S = S;
}