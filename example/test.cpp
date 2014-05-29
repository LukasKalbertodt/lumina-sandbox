#include "lumina/lumina.hpp"
#include "lumina/util/Color.hpp"

#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

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
  // win.setVersionHint(1, 1);
  win.addEventCallback([&](const LInputEvent& e) {
    if(e.keyInput.key == LKeyCode::Escape) { 
      run = false; 
      return LEventResult::Processed;
    }
    return LEventResult::Skipped;
  });
  win.open();
  win.setVSync(true);

  auto cnt = win.getRenderContext();
  cnt->create();
  cnt->makeCurrent();

  Mesh mesh;
  mesh.create(3*3*2, 3);
  
  mesh.prime<Vec3f, Color32f>([](HotMesh<Vec3f, Color32f>& m) {
    // m.vertex[0] = Vec3f(-1.f, -1.f, 0.f), Color32f(1.f, 0.f, 0.f);
    // m.vertex[1] = Vec3f(1.f, -1.f, 0.f), Color32f(0.f, 1.f, 0.f);
    // m.vertex[2] = Vec3f(0.f, 1.f, 0.f), Color32f(0.f, 0.f, 1.f);
    m.vertex[0].set(Vec3f(-1.f, -1.f, 0.f), Color32f(1.f, 0.f, 0.f));
    m.vertex[1].set(Vec3f(1.f, -1.f, 0.f), Color32f(0.f, 1.f, 0.f));
    m.vertex[2].set(Vec3f(0.f, 1.f, 0.f), Color32f(0.f, 0.f, 1.f));
    m.index[0] = 0;
    m.index[1] = 1;
    m.index[2] = 2;
    m.applyVertexLayout();
  });


  auto cube = createBox<VChan::Position, VChan::Normal>(Vec3f(1,1,1)*0.8f);
  // auto cube = createBox<VChan::Normal, VChan::Position>(Vec3f(1,1,1));

  // Shader tests
  Shader<ShaderType::Vertex> vs;
  vs.compile(loadShaderFromFile("shader/test.vsh"));
  Shader<ShaderType::Fragment> fs;
  fs.compile(loadShaderFromFile("shader/test.fsh"));
  // PipelineContainer p(vs, fs);
  // p.use();
  Program p;
  p.create(vs, fs);
  // p.use();
  // p.setUniform("mul", 0.5f);

  auto last = chrono::system_clock::now();

  // glEnable(GL_CULL_FACE);

  while(win.isValid() && run) {
    glClear(GL_COLOR_BUFFER_BIT);
    win.update();

    auto now = chrono::system_clock::now();
    auto diff = now - last;
    float val
      = (chrono::duration_cast<chrono::milliseconds>(diff).count() / 1000.f);

    p.prime([&](HotProgram& hot) {
      hot.uniform["mul"] = sin(val*3)/2 + 0.5;
      // mesh.sendData();
      cube.sendData();
    });
    // cnt->execute(p, [&](HotProgram& hot) {
    //   hot.uniform["mul"] = sin(val)/2 + 0.5;
    //   mesh.sendData();
    // });

    cnt->swapBuffer();
  }
}