#ifndef __GFX_FACE_HPP
#define __GFX_FACE_HPP

#include <set>
#include "edge.hpp"

namespace gfx
{

/* for ease assuming it's a triangle */
class Face
{
  Edge e1;
  Edge e2;
  Edge e3;

public:
  Face(const Edge& e1, const Edge& e2, const Edge& e3)
  {
    /* guarantee order for comparison */
    std::set<Edge> edges;
    edges.insert(e1);    
    edges.insert(e2);    
    edges.insert(e3);    
    auto it = edges.begin();
    this->e1 = *it;
    this->e2 = *++it;
    this->e3 = *++it;
  }
  
  const Edge& E1() const { return e1; }
  const Edge& E2() const { return e2; }
  const Edge& E3() const { return e3; }

  /* used to use Edge as key in map */
  bool operator<(const Face& f) const
  {
    if (e1 < f.E1()) return true;
    else if (e1 > f.E1()) return false;
    else if (e2 < f.E2()) return true;
    else if (e2 > f.E2()) return false;
    else if (e3 < f.E3()) return true;
    else if (e3 > f.E3()) return false;
    return false;
  }

  bool operator>(const Face& f) const
  {
    return (f < *this);
  }

  bool operator==(const Face& f) const
  {
    return (!(f < *this) && !(*this < f));
  }

  bool operator!=(const Face& f) const
  { 
    return !(*this == f);
  }

  friend std::ostream& operator<<(std::ostream& os, const Face& f)
  {
    os << "f" << std::endl;
    os << f.E1() << std::endl;
    os << f.E2() << std::endl;
    os << f.E3();
    return os;
  }
};

/* end */
}
#endif
