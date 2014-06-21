#pragma once

#include "../video/Mesh.hpp"
#include "../video/VChan.hpp"
#include "../util/Vector.hpp"

namespace lumina {

template <VChan... Chans>
Mesh createBox(Vec3f size);

}

#include "Geometry.tpp"