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
  // Logger::setGlobalStdLevelFilter(LogLevel::Info);

  // configure window
  m_win.setVersionHint(3, 3);
  m_win.setVSync(true);

  // register event handler
  m_win.addEventCallback([&](InputEvent e) {
    if(e.type == InputType::Key && e.keyInput.key == KeyCode::Escape) { 
      m_gameRunning = false; 
      return EventResult::Processed;
    }
    if(e.type == InputType::Mouse) {
      // slogDebug("x: ", e.mouseInput.x, " \ty:", e.mouseInput.y);
      if(e.mouseInput.type == MouseInputType::LButtonPressed) {
        m_win.setCursorMode(CursorMode::Free);
        slog("Set to free!");
      }
      else if(e.mouseInput.type == MouseInputType::LButtonReleased) {
        m_win.setCursorMode(CursorMode::Normal);
        slog("Set to normal!");
      }
      else if(e.mouseInput.type == MouseInputType::Scroll) {
        slog("x: ", e.mouseInput.scrollX, " \ty: ", e.mouseInput.scrollY);
      }
    }
    return EventResult::Skipped;
  });

  m_win.addWindowCallback([](WindowEvent e) {
    if(e.type == WindowEventType::Close) {
      cout << "Bye :'(" << endl;
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
  // drawTex.params
  framebuf.attachColor(0, drawTex);

  Tex2D depthTex;
  depthTex.create(m_win.getSize(), TexFormat::D32);
  framebuf.attachDepth(depthTex);


  // create a VertexSeq and fill it with data to represent a triangle
  VertexSeq triangle;
  triangle.create(3 + 3 + 2, 3);

  triangle.prime([](HotVertexSeq<>& hot) {
    hot.vertex[0] = {-1.f, -1.f, 1.f,  0.0f, 0.0f, 0.0f,  0.5f,  0.5f};
    hot.vertex[1] = { 1.f, -1.f, -1.f, 0.0f, 0.0f, 0.0f, -0.5f,  0.5f};
    hot.vertex[2] = { 0.f,  1.f, 0.f,  0.0f, 0.0f, 0.0f,  0.5f, -0.5f};

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

  tex.params.filterMode = TexFilterMode::Linear;
  tex.params.wrapMode = TexWrapMode::MirrorRepeat;
  tex.params.useMipMaps = true;
  tex.prime(0, [&](HotTex2D& hot) {
    hot.fill(pic.data());
  });

  p.perFragProc.enableDepthTest();
  p.perFragProc.setDepthFunction(DepthFunction::Less);


  // loop until the game is stopped
  while(m_win.isValid() && m_gameRunning) {
    // poll events
    m_win.update();

    // draw the first pass
    framebuf.prime([&](HotFrameBuffer& hotFrame) {
      hotFrame.clearColor(0, Color32fA(1.0f, 0, 0));
      hotFrame.clearDepth(1.f);
      TexCont container;
      container.addTexture(0, tex);

      container.prime([&](HotTexCont& hotCont){
        p.prime([&](HotProgram& hot) {        
          hot.draw(hotCont, triangle, PrimitiveType::Triangle);
        });
      });

      // tex.prime(0, [&](HotTex2D& hotTex) {
      //   HotTexCont cont(hotTex);

      //   p.prime([&](HotProgram& hot) {        
      //     hot.draw(cont, triangle, PrimitiveType::Triangle);
      //   });
      // });
    });

    // draw second pass
    cnt.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFrame){
      hotFrame.clearColor(0, Color32fA(0, 1.0, 0));
      hotFrame.clearDepth(1.f);
      depthTex.prime(0, [&](HotTex2D& hotTex) {
        HotTexCont cont(hotTex);

        p.prime([&](HotProgram& hot) {
          hot.draw(cont, cube, PrimitiveType::TriangleStrip);
        });
      });
    });

    cnt.swapBuffer();
  }
}