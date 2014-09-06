#include <iostream>
#include "cube.hpp"
#include "vertex.hpp"

namespace shapes
{

Cube::Cube()
{
  using namespace gfx;
  Vertex v0 = mesh.AddVertex(-0.5f, -0.5f, -0.5f); // front top left
  Vertex v1 = mesh.AddVertex( 0.5f, -0.5f, -0.5f); // front top right
  Vertex v2 = mesh.AddVertex(-0.5f,  0.5f, -0.5f); // front bottom left
  Vertex v3 = mesh.AddVertex( 0.5f,  0.5f, -0.5f); // front bottom right
  Vertex v4 = mesh.AddVertex(-0.5f, -0.5f,  0.5f); // back top left
  Vertex v5 = mesh.AddVertex( 0.5f, -0.5f,  0.5f); // back top right
  Vertex v6 = mesh.AddVertex(-0.5f,  0.5f,  0.5f); // back bottom left
  Vertex v7 = mesh.AddVertex( 0.5f,  0.5f,  0.5f); // back bottom right

  // 0, 2, 3 - 0, 3, 1
  {
    Edge e1(v0, v2);
    Edge e2(v2, v3);
    Edge e3(v3, v0);
    mesh.AddFace(e1, e2, e3);
  }
  
  {
    Edge e1(v0, v3);
    Edge e2(v3, v1);
    Edge e3(v1, v0);
    mesh.AddFace(e1, e2, e3);
  }

  // 4, 6, 7 - 4, 7, 5
  {
    Edge e1(v4, v6);
    Edge e2(v6, v7);
    Edge e3(v7, v4);
    mesh.AddFace(e1, e2, e3);
  }

  {
    Edge e1(v4, v7);
    Edge e2(v7, v5);
    Edge e3(v5, v4);
    mesh.AddFace(e1, e2, e3);
  }

  // 4, 6, 2 - 4, 2, 0
  {
    Edge e1(v4, v6);
    Edge e2(v6, v2);
    Edge e3(v2, v4);
    mesh.AddFace(e1, e2, e3);
  }

  {
    Edge e1(v4, v2);
    Edge e2(v2, v0);
    Edge e3(v0, v4);
    mesh.AddFace(e1, e2, e3);
  }

  // 5, 3, 7 - 5, 1, 3
  {
    Edge e1(v5, v3);
    Edge e2(v3, v7);
    Edge e3(v7, v5);
    mesh.AddFace(e1, e2, e3);
  }

  {
    Edge e1(v5, v1);
    Edge e2(v1, v3);
    Edge e3(v3, v5);
    mesh.AddFace(e1, e2, e3);
  }

  // 0, 5, 4 - 0, 1, 5
  {
    Edge e1(v0, v5);
    Edge e2(v5, v4);
    Edge e3(v4, v0);
    mesh.AddFace(e1, e2, e3);
  }

  {
    Edge e1(v0, v1);
    Edge e2(v1, v5);
    Edge e3(v5, v0);
    mesh.AddFace(e1, e2, e3);
  }

  // 2, 7, 6 - 2, 3, 7
  {
    Edge e1(v2, v7);
    Edge e2(v7, v6);
    Edge e3(v6, v2);
    mesh.AddFace(e1, e2, e3);
  }

  {
    Edge e1(v2, v3);
    Edge e2(v3, v7);
    Edge e3(v7, v2);
    mesh.AddFace(e1, e2, e3);
  }

}

void Cube::Draw()
{
  mesh.Draw();  
}

void Cube::Subdivide()
{
  mesh = mesh.Subdivide();
}

/* end */
}
