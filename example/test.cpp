#include "lumina/lumina.hpp"
#include "lumina/util/Color.hpp"
#include "lumina/video/Texture.hpp"
#include "lumina/service/StaticLogger.hpp"
#include "lumina/io/ImageJPEG.hpp"
#include "lumina/util/VariadicTools.hpp"

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

  auto winSize = win.getSize();
  Tex2D drawTex;
  drawTex.create(winSize, TexFormat::RGB8);
  // drawTex.prime(1, [](HotTex2D& hot) {
    // hot.param.filterMode = TexFilterMode::Nearest;
  // });
  // framebuf.colors[0] = drawTex;
  framebuf.attachColor(0, drawTex);

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
    Vec3f(1, 1, 1));

  // Shader tests
  Shader<ShaderType::Vertex> vs;
  vs.compile(loadShaderFromFile("shader/test.vsh"));
  Shader<ShaderType::Fragment> fs;
  fs.compile(loadShaderFromFile("shader/test.fsh"));

  Program p;
  p.create(vs, fs);

  Vec2i dim(16, 16);
  Image<Color32f> i;
  i.create(dim);
  for(auto it : dim) {
    i[it] = Color32f((it.x + it.y) / (dim.x * 2.f),
                     (it.x == it.y ? 1.f : 0.f),
                     (it.x == it.y ? 1.f : 0.f));
  }

  auto pic = loadJPEGImage("gfx/ava.jpg");

  Tex2D tex;
  tex.create(pic.dimension(), TexFormat::RGB8);

  tex.prime(0, [&](HotTex2D& hot) {
    hot.param.filterMode = TexFilterMode::Linear;
    hot.param.wrapMode = TexWrapMode::MirrorRepeat;
    hot.fill(pic.data());
  });


  // glEnable(GL_CULL_FACE);

  

  while(win.isValid() && run) {
    win.update();

    framebuf.prime([&](HotFrameBuffer& hotFrame) {
      // hotFrame.colors[0].clear(Color32A());
      tex.prime(0, [&](HotTex2D& hotTex) {
        TexCont cont(hotTex);
        glClear(GL_COLOR_BUFFER_BIT);

        p.prime([&](HotProgram& hot) {        
          hot.draw(cont, mesh, PrimitiveType::Triangle);
        });
      });
    });

    cnt->getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFrame){
      drawTex.prime(0, [&](HotTex2D& hotTex) {
        TexCont cont(hotTex);
        glClear(GL_COLOR_BUFFER_BIT);

        p.prime([&](HotProgram& hot) {
          // hot.draw(cont, mesh, PrimitiveType::Triangle);
          hot.draw(cont, cube, PrimitiveType::TriangleStrip);
        });
      });
    });

    cnt->swapBuffer();
  }
}