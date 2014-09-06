#ifndef __GFX_EDGE_HPP
#define __GFX_EDGE_HPP
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include "vertex.hpp"

namespace gfx
{

class Edge
{
  Vertex v1;
  Vertex v2;

public:
  Edge() {}
  Edge(const Vertex& v1, const Vertex& v2)
  {
    /* guarantee some order */
    if (v1 < v2) 
    {
      this->v1 = v1;
      this->v2 = v2;
    }
    else 
    {
      this->v1 = v2;
      this->v2 = v1;
    }
  }

  const Vertex& V1() const { return v1; }
  const Vertex& V2() const { return v2; }

  void Draw() const
  {
    glBegin(GL_LINES);
    glVertex3f(v1.X(), v1.Y(), v1.Z());
    glVertex3f(v2.X(), v2.Y(), v2.Z());
    glEnd();
  }

  /* used to use Edge as key in map */
  bool operator<(const Edge& e) const
  {
    /* check they are not the same with the points switched */
    if (v1 == e.V2() && v2 == e.V1()) return false;  
    if (v1 == e.V1() && v2 == e.V2()) return false;

    if (v1 < e.V1()) return true;
    else if (v1 > e.V1()) return false;
    else if (v2 < e.V2()) return true;
    else if (v2 > e.V2()) return false;
    return false;
  }

  bool operator>(const Edge& e) const
  {
    return e < *this;
  }

  bool operator==(const Edge& e) const
  {
    return (v1 == e.V1() && v2 == e.V2());
  }

  bool operator!=(const Edge& e) const
  {
    return !(*this == e);
  }

  Edge& operator=(const Edge& e)
  {
    if (*this == e) return *this;

    v1 = e.v1;
    v2 = e.v2;

    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const Edge& v)
  {
    os << "e" << std::endl << v.V1() << std::endl << v.V2();
    return os;
  }
  
};

/* end */
}
#endif
