#include "convexhull.h"


ConvexHull::ConvexHull (const Pt2i &lpt, const Pt2i &cpt, const Pt2i &rpt)
{
  CHVertex *cvert = new CHVertex (cpt);
  leftVertex = new CHVertex (lpt);
  rightVertex = new CHVertex (rpt);
  lastToLeft = false;

  if (lpt.toLeft (cpt, rpt))
  {
    leftVertex->setRight (cvert);
    cvert->setLeft (leftVertex);
    cvert->setRight (rightVertex);
    rightVertex->setLeft (cvert);
    rightVertex->setRight (leftVertex);
    leftVertex->setLeft (rightVertex);
  }
  else
  {
    leftVertex->setRight (rightVertex);
    rightVertex->setLeft (leftVertex);
    rightVertex->setRight (cvert);
    cvert->setLeft (rightVertex);
    cvert->setRight (leftVertex);
    leftVertex->setLeft (cvert);
  }

  aph.init (leftVertex, cvert, rightVertex);
  apv.setVertical ();
  apv.init (leftVertex, cvert, rightVertex);

  gbg.push_back (leftVertex);
  gbg.push_back (cvert);
  gbg.push_back (rightVertex);

  old_left = leftVertex;
  old_right = rightVertex;
  old_aph_vertex = aph.vertex ();
  old_aph_edge_start = aph.edgeStart ();
  old_aph_edge_end = aph.edgeEnd ();
  old_apv_vertex = apv.vertex ();
  old_apv_edge_start = apv.edgeStart ();
  old_apv_edge_end = apv.edgeEnd ();
}


ConvexHull::~ConvexHull ()
{
  for (int i = 0; i < (int) (gbg.size ()); ++i) delete gbg [i];
}


void ConvexHull::preserve ()
{
  old_aph_vertex = aph.vertex ();
  old_aph_edge_start = aph.edgeStart ();
  old_aph_edge_end = aph.edgeEnd ();
  old_apv_vertex = apv.vertex ();
  old_apv_edge_start = apv.edgeStart ();
  old_apv_edge_end = apv.edgeEnd ();
  old_left = leftVertex;
  old_right = rightVertex;
}


void ConvexHull::restore ()
{
  rconnect->setLeft (rdisconnect);
  lconnect->setRight (ldisconnect);
  leftVertex = old_left;
  rightVertex = old_right;
  aph.setVertexAndEdge (old_aph_vertex, old_aph_edge_start, old_aph_edge_end);
  apv.setVertexAndEdge (old_apv_vertex, old_apv_edge_start, old_apv_edge_end);
}


bool ConvexHull::addPoint (const Pt2i &pix, bool toleft)
{
  if (inHull (pix, toleft)) return false;
  CHVertex *pt = new CHVertex (pix);
  lastToLeft = toleft;
  gbg.push_back (pt);
  preserve ();
  insert (pt, toleft);
  aph.update (pt);
  apv.update (pt);
  return true;
}


bool ConvexHull::addPointDS (const Pt2i &pix, bool toleft)
{
  CHVertex *pt = new CHVertex (pix);
  lastToLeft = toleft;
  gbg.push_back (pt);
  preserve ();
  insertDS (pt, toleft);
  aph.update (pt);
  apv.update (pt);
  return true;
}


bool ConvexHull::moveLastPoint (const Pt2i &pix)
{
  restore ();
  if (inHull (pix, lastToLeft)) return false;
  gbg.pop_back ();
  preserve ();
  addPoint (pix, lastToLeft);
  return true;
}


AbsRat ConvexHull::rationalThickness () const
{
  AbsRat aphw = aph.rationalWidth ();
  AbsRat apvw = apv.rationalWidth ();
  return (apvw.lessThan (aphw) ? apvw : aphw);
}


void ConvexHull::antipodalEdgeAndVertex (Pt2i &s, Pt2i &e, Pt2i &v) const
{
  int n1, d1, n2, d2;
  aph.width (n1, d1);
  apv.width (n2, d2);
  const Antipodal *ap = ((n2 * d1 < n1 * d2) ? &apv : &aph);
  s.set (*(ap->edgeStart ()));
  e.set (*(ap->edgeEnd ()));
  v.set (*(ap->vertex ()));
}


bool ConvexHull::inHull (const Pt2i &pix, bool toleft) const
{
  CHVertex *ext = (toleft ? leftVertex : rightVertex);
  return (pix.toLeftOrOn (*ext, *(ext->right ()))
          && pix.toLeftOrOn (*(ext->left ()), *ext));
}


void ConvexHull::insert (CHVertex *pt, bool toleft)
{
  bool opIn = false; // Opposite polyline top in the new convex hull
  CHVertex *opVertex = NULL; // Opposite vertex

  if (toleft)
  {
    lconnect = leftVertex;
    rconnect = leftVertex;
    leftVertex = pt;
    opVertex = rightVertex;
  }
  else
  {
    lconnect = rightVertex;
    rconnect = rightVertex;
    rightVertex = pt;
    opVertex = leftVertex;
  }

  ldisconnect = lconnect->right ();
  while (pt->toLeftOrOn (*lconnect, *(lconnect->left ())))
  {
    if (lconnect == opVertex) opIn = true;
    ldisconnect = lconnect;
    lconnect = lconnect->left ();
  }
  if (opIn)
  {
    if (toleft) rightVertex = lconnect;
    else leftVertex = lconnect;
  }

  opIn = false;
  rdisconnect = rconnect->left ();
  while (! pt->toLeft (*rconnect, *(rconnect->right ())))
  {
    if (rconnect == opVertex) opIn = true;
    rdisconnect = rconnect;
    rconnect = rconnect->right ();
  }
  if (opIn)
  {
    if (toleft) rightVertex = rconnect;
    else leftVertex = rconnect;
  }
  
  lconnect->setRight (pt);
  pt->setLeft (lconnect);
  rconnect->setLeft (pt);
  pt->setRight (rconnect);
}


void ConvexHull::insertDS (CHVertex *pt, bool toleft)
{
  if (toleft)
  {
    lconnect = leftVertex;
    rconnect = leftVertex;
    leftVertex = pt;
  }
  else
  {
    lconnect = rightVertex;
    rconnect = rightVertex;
    rightVertex = pt;
  }

  ldisconnect = lconnect->right ();
  while (pt->toLeftOrOn (*lconnect, *(lconnect->left ())))
  {
    ldisconnect = lconnect;
    lconnect = lconnect->left ();
  }

  rdisconnect = rconnect->left ();
  while (! pt->toLeft (*rconnect, *(rconnect->right ())))
  {
    rdisconnect = rconnect;
    rconnect = rconnect->right ();
  }
  
  lconnect->setRight (pt);
  pt->setLeft (lconnect);
  rconnect->setLeft (pt);
  pt->setRight (rconnect);
}


/*
ostream& operator<< (ostream &os, const ConvexHull &ch)
{
  os << "APH = " << ch.aph << endl;
  os << "APV = " << ch.apv << endl;
  os << "FIRST " << *(ch.leftVertex);
  CHVertex *next = ch.leftVertex->right ();
  int i = 0;
  while (i++ < 20 && next != ch.leftVertex)
  {
    os << " - " << *next;
    next = next->right ();
  }
  if (i >= 20) os << " ---";
  os << endl;
  os << "LAST " << *(ch.rightVertex);
  next = ch.rightVertex->left ();
  i = 0;
  while (i++ < 20 && next != ch.rightVertex)
  {
    os << " - " << *next;
    next = next->left ();
  }
  if (i >= 20) os << " ---";

  return os;
}
*/
