#pragma once

#include "GLObject.hpp"
#include "HotTexture.fpp"
#include "TexFormat.hpp"
#include "TextureUnits.hpp"
#include "TexParam.hpp"
#include "Texture.fpp"
#include "TexType.hpp"
#include "../config/BaseProxy.hpp"
#include "../util/Vector.hpp"

#include <GL/glew.h>
#include <functional>
#include <vector>

namespace lumina {

class TextureInterface : public GLObject {
public:
  virtual ~TextureInterface() = default;

  // virtual void prime(int, std::function<void(HotTexture<TT>&)>) = 0;
  virtual void bind(int texUnit) = 0;
  virtual void unbind(int texUnit) = 0;
};

template <TexType TT>
class Texture : public TextureInterface {
public:
  Texture();
  ~Texture();

  void create(Vec2i dimension, TexFormat format, void *data = nullptr);
  void prime(int texUnit, std::function<void(HotTexture<TT>&)> func);

  GLuint nativeHandle() const;
  TexFormat getFormat() const;

  void bind(int texUnit) final;
  void unbind(int texUnit) final;

private:
  GLuint m_handle;
  Vec2i m_dimension;
  TexParam m_params;
  TexFormat m_format;

  GLenum glType() const;
  void createStorage(Vec2i dim,
                     GLint internalFormat,
                     GLenum format,
                     GLenum type,
                     void* data);
  void applyParams();

  friend HotTexture<TT>;
};

} // namespace lumina

#include "Texture.tpp"