#ifndef __SHAPES_CUBE_HPP
#define __SHAPES_CUBE_HPP

#include "mesh.hpp"

namespace shapes
{

class Cube
{
  gfx::WingedEdge mesh;

public:
  Cube();

  void Draw();
  void Subdivide();
  void ButterflySubdivide() { mesh = mesh.ButterflySubdivide(); }
};

/* end */
}
#endif
