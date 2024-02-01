#include "view.h"

#include <algorithm>
// #include <future>
// #include <thread>

namespace s21 {

Viewer3D::Viewer3D(int width, int height, std::string title)
    : nanogui::Screen(Eigen::Vector2i(width, height), title),
      var_width(width),
      var_height(height) {
  m_window = new Window(this, "");
  m_window->setPosition(Vector2i(15, 15));
  m_window->setLayout(new GroupLayout());
  init_config("config.conf", config);

  Label *filenameLabel = new Label(m_window, " ");
  Button *opobj = new Button(m_window, "Open obj");

  new Label(m_window, "Vertices", "sans-bold");
  IntBox<int> *vertInput = new IntBox<int>(m_window);
  vertInput->setEditable(false);

  new Label(m_window, "Faces", "sans-bold");
  IntBox<int> *faceInput = new IntBox<int>(m_window);
  faceInput->setEditable(false);
  std::string wireframe_val = config["Wireframe"];

  std::cout << "CONF:" << wireframe_val << std::endl;

  if (config["Wireframe"] == "true") {
    m_wireframe = true;
    CheckBox *wireframeCheck =
        new CheckBox(m_window, "Wireframe", [this](bool value) {
          config["Wireframe"] = value ? "true" : "false";
          m_control->m_view->m_wireframe = value;
        });
    wireframeCheck->setChecked(true);
  } else {
    m_wireframe = false;
    CheckBox *wireframeCheck =
        new CheckBox(m_window, "Wireframe", [this](bool value) {
          config["Wireframe"] = value ? "true" : "false";
          m_control->m_view->m_wireframe = value;
        });
    wireframeCheck->setChecked(false);
  }

  if (config["EdgeThikness"] == "true") {
    m_EdgeThikness = true;
    CheckBox *EdgeThiknessCheck =
        new CheckBox(m_window, "EdgeThikness", [this](bool value) {
          config["EdgeThikness"] = value ? "true" : "false";
          m_control->m_view->m_EdgeThikness = value;
        });
    EdgeThiknessCheck->setChecked(true);
  } else {
    m_EdgeThikness = false;
    CheckBox *EdgeThiknessCheck =
        new CheckBox(m_window, "EdgeThikness", [this](bool value) {
          config["EdgeThikness"] = value ? "true" : "false";
          m_control->m_view->m_EdgeThikness = value;
        });
    EdgeThiknessCheck->setChecked(false);
  }

  // new Label(m_window, "Edge Thickness", "sans-bold");
  // Slider *edgeThicknessSlider = new Slider(m_window);
  // edgeThicknessSlider->setRange(std::make_pair(0.1f, 100.0f));
  // edgeThicknessSlider->setValue(m_edgeThickness);
  // edgeThicknessSlider->setCallback(
  //     [this](float value) { m_edgeThickness = value; });

  new Label(m_window, "Translation");
  Label *m_sliderLabel = new Label(m_window, "X: 0.0");
  Slider *xTranslate = new Slider(m_window);
  Label *m_sliderLabel2 = new Label(m_window, "Y: 0.0");
  Slider *yTranslate = new Slider(m_window);
  Label *m_sliderLabel3 = new Label(m_window, "Z: 0.0");
  Slider *zTranslate = new Slider(m_window);

  new Label(m_window, "Rotation");
  Label *m_sliderLabelRotate = new Label(m_window, "X: 0.0");
  Slider *xRotate = new Slider(m_window);
  Label *m_sliderLabelRotate2 = new Label(m_window, "Y: 0.0");
  Slider *yRotate = new Slider(m_window);
  Label *m_sliderLabelRotate3 = new Label(m_window, "Z: 0.0");
  Slider *zRotate = new Slider(m_window);

  new Label(m_window, "Scaling");
  Label *m_sliderLabelScaleX = new Label(m_window, "X: 1.0");
  Slider *xScale = new Slider(m_window);
  Label *m_sliderLabelScaleY = new Label(m_window, "Y: 1.0");
  Slider *yScale = new Slider(m_window);
  Label *m_sliderLabelScaleZ = new Label(m_window, "Z: 1.0");
  Slider *zScale = new Slider(m_window);

  // std::string camMode = get_value_from_config("config.conf",
  // "ProjectionType");

  std::string camMode = config["ProjectionType"];
  std::vector<std::string> cameras_panel;
  if (camMode == "1") {
    cameras_panel = {"Central", "Parallel"};
    m_camera.projectionType = ProjectionType::Central;
    new Label(m_window, "Camera mode:", "sans-bold");
    ComboBox *cobo = new ComboBox(m_window, cameras_panel);
    cobo->setFontSize(16);
    cobo->setFixedSize(Vector2i(100, 20));
    cobo->setCallback([this](int index) {
      if (index == 1) {
        m_camera.projectionType = ProjectionType::Parallel;
        config["ProjectionType"] = "0";
      } else {
        m_camera.projectionType = ProjectionType::Central;
        config["ProjectionType"] = "1";
      }
    });
  } else {
    cameras_panel = {"Parallel", "Central"};
    m_camera.projectionType = ProjectionType::Parallel;
    new Label(m_window, "Camera mode:", "sans-bold");
    ComboBox *cobo = new ComboBox(m_window, cameras_panel);
    cobo->setFontSize(16);
    cobo->setFixedSize(Vector2i(100, 20));
    cobo->setCallback([this](int index) {
      if (index == 0) {
        m_camera.projectionType = ProjectionType::Parallel;
        config["ProjectionType"] = "0";
      } else {
        m_camera.projectionType = ProjectionType::Central;
        config["ProjectionType"] = "1";
      }
    });
  }
  // solid or dashed
  std::string edgeMode = config["EdgesType"];
  std::vector<std::string> edge_panel;
  if (edgeMode == "solid") {
    edge_panel = {"Solid", "Dashed"};
    is_stripple = 0;
    new Label(m_window, "Edge type:", "sans-bold");
    ComboBox *cobo2 = new ComboBox(m_window, edge_panel);
    cobo2->setFontSize(16);
    cobo2->setFixedSize(Vector2i(100, 20));
    cobo2->setCallback([this](int index) {
      if (index == 0) {
        is_stripple = 0;
        config["EdgesType"] = "solid";
      } else {
        is_stripple = 1;
        config["EdgesType"] = "dashed";
      }
    });
  } else {
    edge_panel = {"Dashed", "Solid"};
    is_stripple = 1;
    // m_camera.projectionType = ProjectionType::Parallel;
    new Label(m_window, "Edges mode:", "sans-bold");
    ComboBox *cobo2 = new ComboBox(m_window, edge_panel);
    cobo2->setFontSize(16);
    cobo2->setFixedSize(Vector2i(100, 20));
    cobo2->setCallback([this](int index) {
      if (index == 0) {
        is_stripple = 1;
        // m_camera.projectionType = ProjectionType::Parallel;
        config["EdgesType"] = "dashed";
      } else {
        is_stripple = 0;
        // m_camera.projectionType = ProjectionType::Central;
        config["EdgesType"] = "solid";
      }
    });
  }

  second_window = new Window(this, "");
  second_window->setPosition(Vector2i(1040, 15));
  second_window->setLayout(new GroupLayout());

  // COLORS TAB

  TabWidget *tabWidget = second_window->add<TabWidget>();

  Widget *layer = tabWidget->createTab("Background");
  layer->setLayout(new GroupLayout());
  layer->add<Label>("Set color", "sans-bold");
  ColorWheel *colorWheel = layer->add<ColorWheel>();
  std::string bgColorStr = config["BackgroundColor"];
  std::stringstream ss(bgColorStr);
  ss << std::fixed << std::setprecision(6);
  float r, g, b, a;
  ss >> r >> g >> b >> a;
  std::cout << "Colors:" << r << " " << g << " " << b << " " << a << '\n';
  m_background_color = nanogui::Color(Eigen::Vector4f(r, g, b, a));
  colorWheel->setCallback([this](const nanogui::Color &color) {
    m_background_color = color;
    std::stringstream ss2;
    ss2 << std::fixed << std::setprecision(6);
    ss2 << color.r() << " " << color.g() << " " << color.b() << " "
        << color.w();
    config["BackgroundColor"] = ss2.str();
  });

  // Edges color
  TabWidget *tabWidget2 = second_window->add<TabWidget>();

  Widget *edgesLayer = tabWidget2->createTab("Edges");
  edgesLayer->setLayout(new GroupLayout());
  edgesLayer->add<Label>("Set color", "sans-bold");
  ColorWheel *edgesColorWheel = edgesLayer->add<ColorWheel>();

  std::string edgesColorStr = config["EdgesColor"];
  std::stringstream edgesColorStream(edgesColorStr);
  edgesColorStream << std::fixed << std::setprecision(6);
  edgesColorStream >> edgesRed >> edgesGreen >> edgesBlue >> edgesAlpha;
  std::cout << "Edges Colors: " << edgesRed << " " << edgesGreen << " "
            << edgesBlue << " " << edgesAlpha << '\n';
  COLORS = Eigen::Vector4f(edgesRed, edgesGreen, edgesBlue, edgesAlpha);
  m_edges_color = nanogui::Color(
      Eigen::Vector4f(edgesRed, edgesGreen, edgesBlue, edgesAlpha));

  edgesColorWheel->setCallback([this](const nanogui::Color &color2) {
    m_edges_color = color2;
    COLORS = Eigen::Vector4f(color2.r(), color2.g(), color2.b(), color2.w());
    std::stringstream colorStream;
    colorStream << std::fixed << std::setprecision(6);
    colorStream << color2.r() << " " << color2.g() << " " << color2.b() << " "
                << color2.w();
    config["EdgesColor"] = colorStream.str();
  });

  setupVertexDisplay();

  std::vector<std::string> sceenshot_panel = {"BMP", "JPEG"};
  new Label(second_window, "Screenshot type:", "sans-bold");
  ComboBox *cobo3 = new ComboBox(second_window, sceenshot_panel);
  cobo3->setFontSize(16);
  cobo3->setFixedSize(Vector2i(100, 20));
  cobo3->setCallback([this](int index) {
    if (index == 0) {
      conv_type = 0;
    } else {
      conv_type = 1;
    }
  });
  Button *shot = new Button(second_window, "SCREENSHOT");
  shot->setCallback([this]() {
    int i = 0;
    std::string baseFilename = "filik";
    std::string filename;
    do {
      filename = baseFilename + std::to_string(i) + ".tga";
      i++;
    } while (fileExists(filename));
    saveScreenshotToFile(filename, var_width, var_height);
    if (conv_type == 0)
      system("bash converter.sh bmp");
    else
      system("bash converter.sh jpg");
  });
  Button *gif = new Button(second_window, "GIF");
  gif->setCallback([this]() {
    // std::thread gifThread([this]() {
    int i = 0;
    std::string baseFilename = "filik";
    std::string filename;
    std::string folder = "gif_make/";
    std::filesystem::create_directory(folder);
    while (i != 50) {
      do {
        filename = folder + baseFilename + std::to_string(i) + ".tga";
        i++;
      } while (fileExists(filename));
      saveScreenshotToFile(filename, var_width, var_height);
    }
    std::cout << "Finish gif\n";
    system("bash gif_converter.sh");
    // system(
    //     "byzanz-record --duration=10 --x=640 --width=640 "
    //     "--height=480 "
    //     "test.gif");
    //});
    // gifThread.detach();
  });

  setupSlider(xTranslate, m_sliderLabel, "X", std::make_pair(-15.0f, 15.0f),
              [this](float value) { m_camera.modelTranslation.x() = value; });

  setupSlider(yTranslate, m_sliderLabel2, "Y", std::make_pair(-15.0f, 15.0f),
              [this](float value) { m_camera.modelTranslation.y() = value; });

  setupSlider(zTranslate, m_sliderLabel3, "Z", std::make_pair(-15.0f, 15.0f),
              [this](float value) { m_camera.modelTranslation.z() = value; });

  setupSlider(xRotate, m_sliderLabelRotate, "X",
              std::make_pair(-180.0f, 180.0f), [this](float value) {
                float angleRad = value * M_PI / 180.0f;
                Eigen::Quaternionf q(
                    Eigen::AngleAxisf(angleRad, Eigen::Vector3f::UnitX()));
                m_camera.modelRotation = q;
                refresh_mesh();
              });
  setupSlider(yRotate, m_sliderLabelRotate2, "Y",
              std::make_pair(-180.0f, 180.0f), [this](float value) {
                float angleRad = value * M_PI / 180.0f;
                Eigen::Quaternionf q(
                    Eigen::AngleAxisf(angleRad, Eigen::Vector3f::UnitY()));
                m_camera.modelRotation = q;
                refresh_mesh();
              });
  setupSlider(zRotate, m_sliderLabelRotate3, "Z",
              std::make_pair(-180.0f, 180.0f), [this](float value) {
                float angleRad = value * M_PI / 180.0f;
                Eigen::Quaternionf q(
                    Eigen::AngleAxisf(angleRad, Eigen::Vector3f::UnitZ()));
                m_camera.modelRotation = q;
                refresh_mesh();
              });

  setupSlider(xScale, m_sliderLabelScaleX, "X", std::make_pair(0.1f, 2.0f),
              [this](float value) { m_camera.modelScale.x() = value; });

  setupSlider(yScale, m_sliderLabelScaleY, "Y", std::make_pair(0.1f, 2.0f),
              [this](float value) { m_camera.modelScale.y() = value; });

  setupSlider(zScale, m_sliderLabelScaleZ, "Z", std::make_pair(0.1f, 2.0f),
              [this](float value) { m_camera.modelScale.z() = value; });

  m_control = new Controller(this);
  opobj->setCallback([this, vertInput, faceInput, filenameLabel]() {
    std::string filename =
        nanogui::file_dialog({{"obj", "Wavefront OBJ"}}, false);

    if (filename != "") {
      mProcessEvents = false;
      m_control->loadMesh(filename);
      this->refresh_mesh();
      this->refresh_trackball_center();
      vertInput->setValue(m_control->m_mesh->m_num_vertices);
      faceInput->setValue(m_control->m_mesh->m_num_faces);
      std::filesystem::path filePath(filename);
      std::string FileNameOnly = filePath.filename().string();
      filenameLabel->setCaption(FileNameOnly);
      mProcessEvents = true;
    }
  });

  performLayout();

  m_control->loadShaders();
  initVertexShader();

  m_control->loadMesh("cube.obj");
  filenameLabel->setCaption("cube.obj");
  this->refresh_mesh();
  this->refresh_trackball_center();
  vertInput->setValue(m_control->m_mesh->m_num_vertices);
  faceInput->setValue(m_control->m_mesh->m_num_faces);
}

void Viewer3D::initVertexShader() {
  bool loading = m_vertex_shader.initFromFiles(
      "points_shader", "vertex_point.vert", "points.frag");
}

void Viewer3D::setupVertexDisplay() {
  new Label(second_window, "Vertex Display", "sans-bold");
  ComboBox *vertexDisplayCombo =
      new ComboBox(second_window, {"None", "Circle", "Square"});
  vertexDisplayCombo->setCallback([this](int index) {
    m_vertexDisplay = static_cast<VertexDisplay>(index);
  });

  new Label(second_window, "Vertex Color", "sans-bold");
  ColorWheel *vertexColorWheel = new ColorWheel(second_window);
  vertexColorWheel->setCallback(
      [this](const nanogui::Color &color) { m_vertexColor = color; });

  new Label(second_window, "Vertex Size", "sans-bold");
  Slider *vertexSizeSlider = new Slider(second_window);
  vertexSizeSlider->setRange(std::make_pair(1.0f, 35.0f));
  vertexSizeSlider->setValue(m_vertexSize);
  vertexSizeSlider->setCallback([this](float value) { m_vertexSize = value; });
}

bool Viewer3D::fileExists(const std::string &filename) {
  std::ifstream file(filename);
  return file.good();
}

void Viewer3D::remake_config(const std::string &filename) {
  std::ofstream cFile(filename, std::ios::out);
  if (cFile.is_open()) {
    for (std::map<std::string, std::string>::iterator it = config.begin();
         it != config.end(); ++it) {
      std::cout << it->first << "=" << it->second << "\n";
      cFile << it->first << "=" << it->second << "\n";
    }
  } else {
    std::cerr << "ERR: Didn't found " << filename
              << " file. Make sure it is in the "
                 "build folder";
  }
}

void Viewer3D::saveScreenshotToFile(std::string filename, int windowWidth,
                                    int windowHeight) {
  int numberOfPixels = windowWidth * windowHeight * 3;
  std::vector<char> pixels(numberOfPixels);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadBuffer(GL_BACK);
  glReadPixels(0, 0, windowWidth, windowHeight, GL_BGR, GL_UNSIGNED_BYTE,
               pixels.data());

  FILE *outputFile = fopen(filename.c_str(), "wb");
  if (!outputFile) {
    std::cerr << "ERR: can't open file: " << filename << std::endl;
    return;
  }

  char header[18] = {0};
  header[2] = 2;
  header[12] = windowWidth & 0xFF;
  header[13] = (windowWidth >> 8) & 0xFF;
  header[14] = windowHeight & 0xFF;
  header[15] = (windowHeight >> 8) & 0xFF;
  header[16] = 24;
  header[17] = 0;

  fwrite(header, sizeof(header), 1, outputFile);
  fwrite(pixels.data(), numberOfPixels, 1, outputFile);
  fclose(outputFile);
}

void Viewer3D::setupSlider(Slider *slider, Label *label,
                           const std::string &axis,
                           const std::pair<float, float> &range,
                           std::function<void(float)> callback) {
  slider->setRange(range);
  if (range == std::make_pair(0.1f, 2.0f))
    slider->setValue(1.0f);
  else
    slider->setValue(0.0f);
  slider->setCallback([=](float value) {
    callback(value);
    label->setCaption(axis + ":" + std::to_string(value));
  });
}

void Viewer3D::init_config(const std::string &filename,
                           std::map<std::string, std::string> &config) {
  std::ifstream cFile(filename);
  if (cFile.is_open()) {
    std::string line;
    while (getline(cFile, line)) {
      // line.erase(std::remove_if(line.begin(), line.end(), isspace),
      // line.end());
      if (line[0] == '#' || line.empty()) continue;
      auto delimiterPos = line.find("=");
      if (delimiterPos != std::string::npos) {
        auto name = line.substr(0, delimiterPos);
        auto value = line.substr(delimiterPos + 1);
        config.insert(std::make_pair(name, value));
      }
    }
  } else {
    std::cerr << "ERR: Didn't find " << filename
              << " file. Make sure it is in the "
                 "build folder.\n";
  }
}

bool Viewer3D::keyboardEvent(int key, int scancode, int action, int modifiers) {
  if (Screen::keyboardEvent(key, scancode, action, modifiers)) {
    return true;
  }
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    setVisible(false);
    return true;
  }
  return false;
}

void Viewer3D::draw(NVGcontext *ctx) {
  drawContents();
  Screen::draw(ctx);
}

Vector2f Viewer3D::getScreenCoord() {
  return Vector2f(2.0f * (float)mousePos().x() / width() - 1.0f,
                  1.0f - 2.0f * (float)mousePos().y() / height());
}

void Viewer3D::drawContents() {
  using namespace nanogui;

  glClearColor(m_background_color.r(), m_background_color.g(),
               m_background_color.b(), m_background_color.w());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (m_control->m_mesh == nullptr) return;

  Eigen::Matrix4f model, view, proj;
  computeCameraMatrices(model, view, proj);

  Matrix4f mv = view * model;
  Matrix4f p = proj;

  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  // Save the current polygon mode
  GLint polygonMode[2];
  glGetIntegerv(GL_POLYGON_MODE, polygonMode);

  if (m_wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (m_EdgeThikness) {
      glEnable(GL_LINE_SMOOTH);
    } else {
      glDisable(GL_LINE_SMOOTH);
    }
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  m_phong_shader.bind();
  m_phong_shader.setUniform("MV", mv);
  m_phong_shader.setUniform("P", p);
  m_phong_shader.setUniform("C", COLORS);
  m_phong_shader.setUniform("is_stripple", is_stripple);
  m_phong_shader.drawIndexed(GL_TRIANGLES, 0,
                             m_control->m_mesh->get_number_of_face());

  glPolygonMode(GL_FRONT_AND_BACK, polygonMode[0]);

  int u_is_circle = 0;
  if (m_vertexDisplay == VertexDisplay::Square ||
      m_vertexDisplay == VertexDisplay::Circle) {
    if (m_vertexDisplay == VertexDisplay::Circle) {
      u_is_circle = 1;
    }
    m_vertex_shader.bind();
    m_vertex_shader.setUniform("MV", mv);
    m_vertex_shader.setUniform("P", p);
    m_vertex_shader.setUniform("vert_color", m_vertexColor);
    m_vertex_shader.setUniform("pointSize", m_vertexSize);
    m_vertex_shader.setUniform("u_is_circle", u_is_circle);

    glPointSize(m_vertexSize);
    // std::cout << "vertices:" << m_control->m_mesh->get_number_of_vertices()
    //           << "\n";
    m_vertex_shader.drawIndexed(
        GL_POINTS, 0, m_control->m_mesh->get_number_of_vertices() * 5);
  }
}

bool Viewer3D::scrollEvent(const Vector2i &p, const Vector2f &rel) {
  if (!Screen::scrollEvent(p, rel)) {
    m_camera.zoom = std::max(0.1, m_camera.zoom * (rel.y() > 0 ? 1.1 : 0.9));
  }
  return true;
}

bool Viewer3D::mouseMotionEvent(const Vector2i &p, const Vector2i &rel,
                                int button, int modifiers) {
  if (!Screen::mouseMotionEvent(p, rel, button, modifiers)) {
    if (m_camera.arcball.motion(p)) {
    } else if (m_translate) {
      Eigen::Matrix4f model, view, proj;
      computeCameraMatrices(model, view, proj);
      Eigen::Vector3f mesh_center = m_control->m_mesh->get_mesh_center();
      float zval = nanogui::project(Vector3f(mesh_center.x(), mesh_center.y(),
                                             mesh_center.z()),
                                    view * model, proj, mSize)
                       .z();
      Eigen::Vector3f pos1 =
          nanogui::unproject(Eigen::Vector3f(p.x(), mSize.y() - p.y(), zval),
                             view * model, proj, mSize);
      Eigen::Vector3f pos0 = nanogui::unproject(
          Eigen::Vector3f(m_translateStart.x(),
                          mSize.y() - m_translateStart.y(), zval),
          view * model, proj, mSize);
      m_camera.modelTranslation =
          m_camera.modelTranslation_start + (pos1 - pos0);
    }
  }
  return true;
}

bool Viewer3D::mouseButtonEvent(const Vector2i &p, int button, bool down,
                                int modifiers) {
  if (!Screen::mouseButtonEvent(p, button, down, modifiers)) {
    if (button == GLFW_MOUSE_BUTTON_1 && modifiers == 0) {
      m_camera.arcball.button(p, down);
    } else if (button == GLFW_MOUSE_BUTTON_2 ||
               (button == GLFW_MOUSE_BUTTON_1 && modifiers == GLFW_MOD_SHIFT)) {
      m_camera.modelTranslation_start = m_camera.modelTranslation;
      m_translate = true;
      m_translateStart = p;
    }
  }
  if (button == GLFW_MOUSE_BUTTON_1 && !down) {
    m_camera.arcball.button(p, false);
  }
  if (!down) {
    m_translate = false;
  }
  return true;
}

void Viewer3D::refresh_trackball_center() {
  Eigen::Vector3f mesh_center = m_control->m_mesh->get_mesh_center();
  m_camera.arcball = Arcball();
  m_camera.arcball.setSize(mSize);
  m_camera.modelZoom = 2 / m_control->m_mesh->get_dist_max();
  m_camera.modelTranslation =
      -Vector3f(mesh_center.x(), mesh_center.y(), mesh_center.z());
}

void Viewer3D::refresh_mesh() {
  m_phong_shader.bind();
  m_phong_shader.uploadIndices(*(m_control->m_mesh->get_indices()));
  m_phong_shader.uploadAttrib("position", *(m_control->m_mesh->get_points()));
  m_phong_shader.uploadAttrib("normal", *(m_control->m_mesh->get_normals()));
  m_vertex_shader.bind();
  m_vertex_shader.uploadIndices(*(m_control->m_mesh->get_indices()));
  m_vertex_shader.uploadAttrib("position", *(m_control->m_mesh->get_points()));
  m_vertex_shader.uploadAttrib("normal", *(m_control->m_mesh->get_normals()));
}

void Viewer3D::computeCameraMatrices(Eigen::Matrix4f &model,
                                     Eigen::Matrix4f &view,
                                     Eigen::Matrix4f &proj) {
  Eigen::Matrix3f rot = m_camera.modelRotation.toRotationMatrix();
  Eigen::Matrix4f rot4;
  rot4.setIdentity();  // fill by 1
  rot4.block<3, 3>(0, 0) =
      rot;  // Copy the 3x3 rotation into the top-left corner

  view = nanogui::lookAt(m_camera.eye, m_camera.center, m_camera.up);

  float fH, fW;
  if (m_camera.projectionType == ProjectionType::Parallel) {
    fH = m_camera.zoom;
    fW = fH * (float)mSize.x() / (float)mSize.y();
    proj = nanogui::ortho(-fW, fW, -fH, fH, m_camera.dnear, m_camera.dfar);
  } else {
    fH = std::tan(m_camera.viewAngle / 360.0f * M_PI) * m_camera.dnear;
    fW = fH * (float)mSize.x() / (float)mSize.y();
    proj = nanogui::frustum(-fW, fW, -fH, fH, m_camera.dnear, m_camera.dfar);
  }
  model = m_camera.arcball.matrix();
  model = model * rot4;  // interface rotation
  model = model * nanogui::scale(Eigen::Vector3f::Constant(m_camera.zoom *
                                                           m_camera.modelZoom));
  model = model * nanogui::scale(m_camera.modelScale);  // interface scaling
  model = model * nanogui::translate(m_camera.modelTranslation);

  // model = model * m_camera.modelRotation.toRotationMatrix();
}

Viewer3D::~Viewer3D() {
  remake_config("config.conf");
  m_phong_shader.free();
  m_vertex_shader.free();
  delete m_control->m_mesh;
  delete m_control;
}
}  // namespace s21