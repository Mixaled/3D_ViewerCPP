#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Eigen/Dense>
namespace s21 {

class Viewer3D;
class Mesh;

class Controller {
 public:
  Mesh* m_mesh;
  Viewer3D* m_view;
  Eigen::Matrix4f modelMatrix_;
  Eigen::Matrix4f viewMatrix_;
  Eigen::Matrix4f projectionMatrix_;

 public:
  Controller(Viewer3D* view);
  void loadShaders();
  void loadMesh(const std::string& filename);
};
}  // namespace s21

#endif  // CONTROLLER_H