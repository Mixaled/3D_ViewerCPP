#ifndef VIEWER_H
#define VIEWER_H

#include <nanogui/button.h>
#include <nanogui/checkbox.h>
#include <nanogui/colorpicker.h>
#include <nanogui/colorwheel.h>
#include <nanogui/combobox.h>
#include <nanogui/common.h>
#include <nanogui/glutil.h>
#include <nanogui/label.h>
#include <nanogui/layout.h>
#include <nanogui/opengl.h>
#include <nanogui/popupbutton.h>
#include <nanogui/screen.h>
#include <nanogui/slider.h>
#include <nanogui/tabwidget.h>
#include <nanogui/textbox.h>
#include <nanogui/widget.h>
#include <nanogui/window.h>
// #include <nanogui/graph.h>
#include <nanogui/tabwidget.h>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>

#include "controller.h"
#include "model.h"

using namespace nanogui;

namespace s21 {
class Controller;
}

namespace s21 {

class Viewer3D : public nanogui::Screen {
 public:
  enum class ProjectionType { Parallel, Central };
  struct CameraParameters {
    nanogui::Arcball arcball;
    float zoom = 1.0f, viewAngle = 45.0f;
    float dnear = 0.05f, dfar = 100.0f;
    Eigen::Vector3f eye = Eigen::Vector3f(0.0f, 0.0f, 5.0f);
    Eigen::Vector3f center = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
    Eigen::Vector3f up = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
    Eigen::Vector3f modelTranslation = Eigen::Vector3f::Zero();
    Eigen::Vector3f modelTranslation_start = Eigen::Vector3f::Zero();
    Eigen::Vector3f modelScale = Eigen::Vector3f::Ones();
    Eigen::Quaternionf modelRotation = Eigen::Quaternionf::Identity();
    float modelZoom = 1.0f;
    ProjectionType projectionType;
  };

 public:
  Viewer3D(int width, int height, std::string title);
  ~Viewer3D();

  void loadShaders();
  void computeCameraMatrices(Eigen::Matrix4f &model, Eigen::Matrix4f &view,
                             Eigen::Matrix4f &proj);

  virtual void draw(NVGcontext *ctx);
  virtual void drawContents();

  void refresh_mesh();
  void refresh_trackball_center();

  virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);

  Vector2f getScreenCoord();
  bool scrollEvent(const Vector2i &p, const Vector2f &rel);
  bool mouseMotionEvent(const Vector2i &p, const Vector2i &rel, int button,
                        int modifiers);
  bool mouseButtonEvent(const Vector2i &p, int button, bool down,
                        int modifiers);

  void setupSlider(nanogui::Slider *slider, nanogui::Label *label,
                   const std::string &axis,
                   const std::pair<float, float> &range,
                   std::function<void(float)> callback);
  void init_config(const std::string &filename,
                   std::map<std::string, std::string> &config);

  void remake_config(const std::string &filename);

  void saveScreenshotToFile(std::string filename, int windowWidth,
                            int windowHeight);
  bool fileExists(const std::string &filename);
  void setupVertexDisplay();
  void initVertexShader();
  void renderScene();

  CameraParameters m_camera;
  float edgesRed, edgesGreen, edgesBlue, edgesAlpha;
  Vector4f COLORS;
  bool m_translate = false;
  bool m_wireframe = false;
  bool m_EdgeThikness = false;
  int is_stripple = 0;
  Vector2i m_translateStart = Vector2i(0, 0);
  nanogui::GLShader m_phong_shader;
  nanogui::GLShader m_vertex_shader;
  nanogui::Window *m_window;
  nanogui::Window *second_window;
  nanogui::Color m_background_color;
  nanogui::Color m_edges_color;
  float m_edgeThickness = 1.0f;
  Mesh *m_mesh;
  int var_width;
  int var_height;
  int conv_type = 0;

  enum class VertexDisplay { None, Circle, Square };
  VertexDisplay m_vertexDisplay = VertexDisplay::None;
  nanogui::Color m_vertexColor = nanogui::Color(1.0f, 1.0f, 1.0f, 1.0f);
  float m_vertexSize = 1.0f;

 private:
  Controller *m_control;
  std::map<std::string, std::string> config;
};
}  // namespace s21
#endif  // VIEWER_H