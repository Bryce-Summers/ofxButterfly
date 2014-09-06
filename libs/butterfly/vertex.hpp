#ifndef __GFX_VERTEX_HPP
#define __GFX_VERTEX_HPP
#include <iostream>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace gfx
{

class Vertex
{
  GLfloat x;
  GLfloat y;
  GLfloat z;

public:
  Vertex() {}
  Vertex(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}

  GLfloat X() const { return x; }
  GLfloat Y() const { return y; }
  GLfloat Z() const { return z; }

  /* used to use Vertex as key in a map */
  bool operator<(const Vertex& v) const
  {
    if (x < v.X()) return true;
    else if (x > v.X()) return false;
    else if (y < v.Y()) return true;
    else if (y > v.Y()) return false;
    else if (z < v.Z()) return true;
    else if (z > v.Z()) return false;
    return false;
  }

  bool operator>(const Vertex& v) const
  {
    return (v < *this);
  }
  bool operator==(const Vertex& v) const
  {
    return (x == v.X() && y == v.Y() && z == v.Z());
  }

  bool operator!=(const Vertex& v) const
  {
    return !(*this == v);
  }

  /* arithmitic operators */

  friend Vertex operator/(const Vertex& v, GLfloat f)
  {
    return Vertex(v.X()/f, v.Y()/f, v.Z()/f);
  }

  friend Vertex operator*(const Vertex& v, GLfloat f)
  {
    return Vertex(v.X()*f, v.Y()*f, v.Z()*f);
  }

  friend Vertex operator+(const Vertex& v, GLfloat f)
  {
    return Vertex(v.X()+f, v.Y()+f, v.Z()+f);
  }

  friend Vertex operator-(const Vertex& v, GLfloat f)
  {
    return Vertex(v.X()-f, v.Y()-f, v.Z()-f);
  }

  friend Vertex operator+(const Vertex& v1, const Vertex& v2)
  {
    return Vertex(v1.X()+v2.X(), v1.Y()+v2.Y(), v1.Z()+v2.Z()); 
  }

  friend Vertex operator-(const Vertex& v1, const Vertex& v2)
  {
    return Vertex(v1.X()-v2.X(), v1.Y()-v2.Y(), v1.Z()-v2.Z()); 
  }

  friend std::ostream& operator<<(std::ostream& os, const Vertex& v)
  {
    os << "v " << v.X() << ", " << v.Y() << ", " << v.Z();
    return os;
  }
};


/* end */
}
#endif

