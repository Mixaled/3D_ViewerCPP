#include <iostream>

#include "view.h"

int main(int /* argc */, char ** /* argv */) {
  try {
    nanogui::init();
    {
      nanogui::ref<s21::Viewer3D> app =
          new s21::Viewer3D(1220, 970, "3D_VIEWER");

      app->setVisible(true);
      nanogui::mainloop();
    }
    nanogui::shutdown();

  } catch (const std::runtime_error &error) {
    std::string message = std::string("ERR: ") + std::string(error.what());
    std::cerr << message << std::endl;
    return -1;
  }

  return 0;
}