#include "lumina/lumina.hpp"
#include "lumina/util/Color.hpp"

#include <iostream>
#include <vector>

using namespace lumina;
using namespace std;

void windowTest();

int main() {
  windowTest();
}

void windowTest() {
  bool run = true;

  Window win("Hai :3");
  win.setVersionHint(3, 3);
  win.setVSync(true);
  win.addEventCallback([&](const LInputEvent& e) {
    if(e.keyInput.key == LKeyCode::Escape) {
      run = false;
      return LEventResult::Processed;
    }
    return LEventResult::Skipped;
  });

  Vec3f position = Vec3f(0.f, 0.f, 0.f);
  win.addEventCallback([&](const LInputEvent& e) {
    if (e.keyInput.type == LKeyEventType::KeyPressed) {
      switch (e.keyInput.key) {
      case LKeyCode::Up:
        position += Vec3f(0.f, 0.1f, 0.f);
        break;
      case LKeyCode::Down:
        position += Vec3f(0.f, -0.1f, 0.f);
        break;
      case LKeyCode::Left:
        position += Vec3f(-0.1f, 0.f, 0.f);
        break;
      case LKeyCode::Right:
        position += Vec3f(0.1f, 0.f, 0.f);
        break;
      default:
        return LEventResult::Skipped;
      }
      return LEventResult::Processed;
    }
    return LEventResult::Skipped;
  });

  float scaling = 1.f;
  win.addEventCallback([&](const LInputEvent& e) {
    if (e.keyInput.type == LKeyEventType::KeyPressed) {
      switch (e.keyInput.key) {
      case LKeyCode::Add:
        scaling += 0.1f;
        break;
      case LKeyCode::Subtract:
        scaling -= 0.1f;
        break;
      default:
        return LEventResult::Skipped;
      }
      return LEventResult::Processed;
    }
    return LEventResult::Skipped;
  });

  Quaternion<float> rotation = Quaternion<float>(1.f, 0.f, 0.f, 0.f);
  win.addEventCallback([&](const LInputEvent& e) {
    if (e.keyInput.type == LKeyEventType::KeyPressed) {
      switch (e.keyInput.key) {
      case LKeyCode::W:
        rotation *= quaternionFromAxisAngle(Vec3f(1.f, 0.f, 0.f), 0.05f);
        break;
      case LKeyCode::S:
        rotation *= quaternionFromAxisAngle(Vec3f(1.f, 0.f, 0.f), -0.05f);
        break;
      case LKeyCode::A:
        rotation *= quaternionFromAxisAngle(Vec3f(0.f, 1.f, 0.f), -0.05f);
        break;
      case LKeyCode::D:
        rotation *= quaternionFromAxisAngle(Vec3f(0.f, 1.f, 0.f), 0.05f);
        break;
      default:
        return LEventResult::Skipped;
      }
      std::cout << rotation << std::endl;
      return LEventResult::Processed;
    }
    return LEventResult::Skipped;
  });

  win.open();
  win.setVSync(true);

  auto cnt = win.getRenderContext();
  cnt->create();
  cnt->makeCurrent();

  auto cube = createBox<VChan::Position, VChan::Normal>(Vec3f(1,1,1)*0.8f);

  // Shader tests
  Shader<ShaderType::Vertex> vs;
  vs.compile(loadShaderFromFile("shader/transform.vsh"));
  Shader<ShaderType::Fragment> fs;
  fs.compile(loadShaderFromFile("shader/transform.fsh"));
  Program p;
  p.create(vs, fs);

  //m.setToIdentity();

  Mat4f viewingPipeline = viewMatrix(Vec3f(0.f, 0.f, 5.f),
                                     Vec3f(0.f, 0.f, -1.f),
                                     Vec3f(0.f, 1.f, 0.f));
  viewingPipeline = projectionMatrix(22.f * static_cast<float>(M_PI) / 180.f, 4.f / 3.f, 0.1f, 100.f) * viewingPipeline;
  while(win.isValid() && run) {
    win.update();
    glClear(GL_COLOR_BUFFER_BIT);

    p.prime([&](HotProgram& hot) {
        Mat4f m = translationMatrix(position) * rotationMatrix(rotation) * scalingMatrix(Vec3f(scaling, scaling, scaling));
      hot.uniform["transformation"] = viewingPipeline * m;
      cube.sendData();
    });

    cnt->swapBuffer();
  }
}