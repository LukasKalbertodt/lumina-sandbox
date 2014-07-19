#include "lumina/lumina.hpp"
#include "lumina/util/Color.hpp"
#include "lumina/video/Texture.hpp"
#include "lumina/service/StaticLogger.hpp"
#include "lumina/io/ImageJPEG.hpp"

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

  getLoggerService(config::defaultServiceContext);
  Logger::setGlobalStdLevelFilter(LogLevel::Info);

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
  win.open();
  win.setVSync(true);

  auto cnt = win.getRenderContext();
  cnt->create();
  cnt->makeCurrent();

  FrameBuffer framebuf;
  framebuf.create();

  VertexSeq mesh;
  mesh.create(3 + 3 + 2, 3);
  
  // mesh.prime<Vec3f, Color32f>([](HotVertexSeq<Vec3f, Color32f>& m) {
  //   m.vertex[0].set(Vec3f(-1.f, -1.f, 0.f),
  //                   Color32f(1.f, 0.f, 0.f));
  //   m.vertex[1].set(Vec3f(1.f, -1.f, 0.f),
  //                   Color32f(0.f, 1.f, 0.f));
  //   m.vertex[2].set(Vec3f(0.f, 1.f, 0.f),
  //                   Color32f(0.f, 0.f, 1.f));
  //   m.applyVertexLayout();
  // });

  mesh.prime([](HotVertexSeq<>& hot) {
    hot.vertex[0] = {-1.f, -1.f, 0.f, 0.0f, 0.0f, 0.0f,  0.5f,  0.5f};
    hot.vertex[1] = { 1.f, -1.f, 0.f, 0.0f, 0.0f, 0.0f, -0.5f,  0.5f};
    hot.vertex[2] = { 0.f,  1.f, 0.f, 0.0f, 0.0f, 0.0f,  0.5f, -0.5f};

    hot.applyVertexLayout<Vec3f, Vec3f, Vec2f>();
  });


  auto cube = createBox<VChan::Position, VChan::Normal, VChan::TexUV>(
    Vec3f(1, 1, 1) * 0.8f);

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

  Vec2i dim(16, 16);
  Image<Color32f> i;
  i.create(dim);
  for(auto it : dim) {
    i[it] = Color32f((it.x + it.y) / (dim.x * 2.f),
                     (it.x == it.y ? 1.f : 0.f),
                     (it.x == it.y ? 1.f : 0.f));
  }
  // cout << i[Vec2i(1,0)] << endl;

  auto pic = loadJPEGImage("gfx/ava.jpg");

  // ImageBox box = i;

  Tex2D tex;
  // tex.create(dim, TexFormat::RGB32F, i.data());
  tex.create(pic.dimension(), TexFormat::RGB8);

  tex.prime(0, [&](HotTex2D& hot) {
    hot.param.filterMode = TexFilterMode::Linear;
    hot.param.wrapMode = TexWrapMode::MirrorRepeat;
    hot.fill(pic.data());
  });

  // glActiveTexture(GL_TEXTURE0);
  // tex.bindTexture();






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
      hot.uniform["mul"] = 1.f;
      
      tex.prime(0, [&](HotTex2D&) {
        hot.draw(mesh, PrimitiveType::Triangle);
        hot.draw(cube, PrimitiveType::TriangleStrip);
      });
    });
    // cnt->execute(p, [&](HotProgram& hot) {
    //   hot.uniform["mul"] = sin(val)/2 + 0.5;
    //   mesh.sendData();
    // });

    cnt->swapBuffer();
  }
}