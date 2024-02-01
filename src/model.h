#ifndef MODEL_H
#define MODEL_H

#include <Eigen/Sparse>
#include <string>
#include <vector>

#include "controller.h"
#include "tiny_obj_loader.h"

namespace s21 {
typedef Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic> MatrixXu;

class Mesh {
 public:
  Mesh(const std::string &filename);

  ~Mesh();

  bool load_obj(const std::string &filename);

  unsigned int get_number_of_face();

  const Eigen::Vector3f get_mesh_center();

  const Eigen::MatrixXf *get_points();

  const MatrixXu *get_indices();

  const Eigen::MatrixXf *get_normals();

  unsigned int get_number_of_vertices();

  float get_dist_max();

 private:
  std::vector<tinyobj::shape_t> m_shapes;
  std::vector<tinyobj::material_t> m_materials;
  Eigen::Vector3f m_bmin;
  Eigen::Vector3f m_bmax;
  Eigen::Vector3f m_mesh_center;
  float m_dist_max = 0.0f;
  Eigen::MatrixXf m_points;
  MatrixXu m_indices;
  Eigen::MatrixXf m_normals;

 public:
  size_t m_num_vertices;
  size_t m_num_faces;
};

}  // namespace s21
#endif  // MODEL_H