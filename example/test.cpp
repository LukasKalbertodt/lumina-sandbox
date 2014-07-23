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


class Test {
private:
  bool m_gameRunning;
  Window m_win;

  void execute(HotRenderContext& cnt);

public:
  Test();

  void run();
};


int main() {
  Test t;
  t.run();
}

Test::Test()
  : m_gameRunning(true), m_win("Hai :3") {
  // configure log service and start timer
  getLoggerService(config::defaultServiceContext);
  Logger::setGlobalStdLevelFilter(LogLevel::Info);

  // configure window
  m_win.setVersionHint(3, 3);
  m_win.setVSync(true);

  // register event handler
  m_win.addEventCallback([&](const InputEvent& e) {
    if(e.keyInput.key == KeyCode::Escape) { 
      m_gameRunning = false; 
      return EventResult::Processed;
    }
    return EventResult::Skipped;
  });
}

void Test::run() {
  // open window
  m_win.open();

  // obtain render context and use it to run the game
  auto& cnt = m_win.getRenderContext();
  cnt.create();
  cnt.prime([this](HotRenderContext& hot) { 
    this->execute(hot);
  });
}

void Test::execute(HotRenderContext& cnt) {

  // create new framebuffer
  FrameBuffer framebuf;
  framebuf.create();

  // create a texture of the size of the window and attach it to the framebuffer
  Tex2D drawTex;
  drawTex.create(m_win.getSize(), TexFormat::RGB8);
  framebuf.attachColor(0, drawTex);


  // create a VertexSeq and fill it with data to represent a triangle
  VertexSeq triangle;
  triangle.create(3 + 3 + 2, 3);

  triangle.prime([](HotVertexSeq<>& hot) {
    hot.vertex[0] = {-1.f, -1.f, 0.f, 0.0f, 0.0f, 0.0f,  0.5f,  0.5f};
    hot.vertex[1] = { 1.f, -1.f, 0.f, 0.0f, 0.0f, 0.0f, -0.5f,  0.5f};
    hot.vertex[2] = { 0.f,  1.f, 0.f, 0.0f, 0.0f, 0.0f,  0.5f, -0.5f};

    hot.applyVertexLayout<Vec3f, Vec3f, Vec2f>();
  });

  // generate cube
  auto cube = createBox<VChan::Position, VChan::Normal, VChan::TexUV>(
    Vec3f(1, 1, 1) * 0.8);

  // create two shaders and link them together
  Shader<ShaderType::Vertex> vs;
  vs.compile(loadShaderFromFile("shader/test.vsh"));
  Shader<ShaderType::Fragment> fs;
  fs.compile(loadShaderFromFile("shader/test.fsh"));

  Program p;
  p.create(vs, fs);
  p.primitiveProc.enableCulling();
  p.primitiveProc.setCullFace(CullFace::Back);


  // load a picture from file and create a texture of it
  auto pic = loadJPEGImage("gfx/ava.jpg");

  Tex2D tex;
  tex.create(pic.dimension(), TexFormat::RGB8);

  tex.prime(0, [&](HotTex2D& hot) {
    hot.param.filterMode = TexFilterMode::Linear;
    hot.param.wrapMode = TexWrapMode::MirrorRepeat;
    hot.fill(pic.data());
  });


  // loop until the game is stopped
  while(m_win.isValid() && m_gameRunning) {
    // poll events
    m_win.update();

    // draw the first pass
    framebuf.prime([&](HotFrameBuffer& hotFrame) {
      hotFrame.clearColor(0, Color32fA(1.0f, 0, 0));
      tex.prime(0, [&](HotTex2D& hotTex) {
        TexCont cont(hotTex);

        p.prime([&](HotProgram& hot) {        
          hot.draw(cont, triangle, PrimitiveType::Triangle);
        });
      });
    });

    // draw second pass
    cnt.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFrame){
      hotFrame.clearColor(0, Color32fA(0, 1.0, 0));
      drawTex.prime(0, [&](HotTex2D& hotTex) {
        TexCont cont(hotTex);

        p.prime([&](HotProgram& hot) {
          hot.draw(cont, cube, PrimitiveType::TriangleStrip);
        });
      });
    });

    cnt.swapBuffer();
  }
}