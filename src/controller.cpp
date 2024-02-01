#include "controller.h"

#include "model.h"
#include "view.h"

namespace s21 {

Controller::Controller(Viewer3D *view) : m_view(view), m_mesh(nullptr) {}

void Controller::loadShaders() {
  m_view->m_phong_shader.initFromFiles("a_simple_shader", "vertex_shader.vert",
                                       "fragment_shader.frag");
}

void Controller::loadMesh(const std::string &filename) {
  if (m_mesh != nullptr) {
    delete m_mesh;
  }
  // new mesh
  m_mesh = new Mesh(filename);
  m_view->refresh_mesh();
  m_view->refresh_trackball_center();
}

}  // namespace s21