#include<obj_loader.hpp>
#include<fstream>
#include<string>
#include<sstream>

obj_loader::obj_loader(std::string obj_path) 
          :_path(obj_path),_T(glm::mat4(1)), _R(glm::mat4(1)), _S(glm::mat4(1))
{
          this->load_obj();
}

void obj_loader::load_obj() 
{
          std::ifstream in(_path);

          if (!in.is_open()) {
                    std::cout << "Obj File Read Error!" << std::endl;
                    in.close();
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

                    /*Search for Triangle */
                    if (buffer.substr(0, 2) == "f ") {      //seperate ?/?/? ?/?/? ?/?/? ?/?/?
                              std::istringstream ss(buffer.substr(2));
                              std::string splitted;
                              std::vector<glm::uvec3> faces_vec;
                              while (std::getline(ss, splitted, ' ')) {  //handle ?/?/? each time
                                        std::istringstream split_ss(splitted);
                                        std::string slashsplitter;
                                        glm::uvec3 vertex_vt_vn(1);          //store v/vt/vn data in a vec
                                        std::size_t index = 0;
                                        while (std::getline(split_ss, slashsplitter, '/') && index < 3) {  //handle ?/ each time
                                                  std::istringstream(slashsplitter) >> vertex_vt_vn[index];
                                                  --vertex_vt_vn[index];
                                                  index++;
                                        }
                                        faces_vec.push_back(std::move(vertex_vt_vn));
                              }
                              for (std::size_t i = 1; i < faces_vec.size() - 1; ++i) {
                                        glm::umat3x3 face = glm::umat3x3(faces_vec.at(0), faces_vec.at(i), faces_vec.at(i + 1));
                                        _faces.push_back(face);
                              }
                    }

                    /*Search for Normals */
                    if (buffer.substr(0, 3) == "vn ") {
                              std::istringstream ss(buffer.substr(3));
                              Normal normal;
                              ss >> normal.x >> normal.y >> normal.z;
                              _normals.push_back(std::move(normal));
                    }

                    /*Search for UV*/
                    if (buffer.substr(0, 3) == "vt ") {
                              std::istringstream ss(buffer.substr(3));
                              Uv uv;
                              ss >> uv.x >> uv.y;
                              _uvs.push_back(std::move(uv));
                    }
          }
          in.close();
}

const  obj_loader::VerTexType& obj_loader::getVertexSet()
{
          return _vertexSet;
}

const std::vector<glm::umat3x3>& obj_loader::getFacesMatrics()
 {
          return _faces;
 }

const std::vector < typename obj_loader::Normal >& obj_loader::getNormalsSet()
 {
          return _normals;
 }

const std::vector <  typename obj_loader::Uv>& obj_loader::getUvSet()
{
          return _uvs;
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