#ifndef ADAPTIVE_SCANNER_O7_H
#define ADAPTIVE_SCANNER_O7_H

#include "directionalscanner.h"

using namespace std;



/** 
 * @class AdaptiveScannerO7 adaptivescannero7.h
 * \brief Adaptive directional scanner for the 7th octant.
 * \author {P. Even}
 */
class AdaptiveScannerO7 : public DirectionalScanner
{

public:
  
  /**
   * @fn AdaptiveScanner07(int xmin, int ymin, int xmax, int ymax,
   *                       int a, int b, int mu2,
   *                       int nbs, bool *steps, int sx, int sy)
   * \brief Creates an adaptive DS from pattern, start and upper bound.
   * The scan strip is composed of parallel scan lines, the first one being
   *   defined by a start point, a line pattern, and an upper bound.
   * @param xmin left border of the scan area
   * @param ymin low border of the scan area
   * @param xmax right border of the scan area
   * @param ymax up border of the scan area
   * @param a x coefficient of the discrete support line
   * @param b y coefficient of the discrete support line
   * @param c y intercept parameter of the upper bound
   * @param nbs size of the support line pattern
   * @param steps support line pattern
   * @param sx abscissae of the central scan start point
   * @param sy ordinate of the central scan start point
   */
  AdaptiveScannerO7 (int xmin, int ymin, int xmax, int ymax,
                     int a, int b, int c,
                     int nbs, bool *steps, int sx, int sy);

  /**
   * @fn AdaptiveScanner07(int xmin, int ymin, int xmax, int ymax,
   *                       int a, int b, int c1, int c2,
   *                       int nbs, bool *steps, int cx, int cy)
   * \brief Creates an adaptive DS from pattern, center and bounds.
   * The scan strip is composed of parallel scan lines, the first one being
   *   defined by a center, a line pattern, upper and lower bounds.
   * @param xmin left border of the scan area
   * @param ymin low border of the scan area
   * @param xmax right border of the scan area
   * @param ymax up border of the scan area
   * @param a x coefficient of the discrete support line
   * @param b y coefficient of the discrete support line
   * @param c1 y intercept parameter of one of the bounds
   * @param c2 y intercept parameter of the other bound
   * @param nbs size of the support line pattern
   * @param steps support line pattern
   * @param cx abscissae of the central scan center
   * @param cy ordinate of the central scan center
   */
  AdaptiveScannerO7 (int xmin, int ymin, int xmax, int ymax,
                     int a, int b, int c1, int c2,
                     int nbs, bool *steps, int cx, int cy);

  /**
   * @fn AdaptiveScanner07(int xmin, int ymin, int xmax, int ymax,
   *                       int a, int b,
   *                       int nbs, bool *steps, int cx, int cy, int length)
   * \brief Creates an adaptive DS from pattern, center and length.
   * The scan strip is composed of parallel scan lines, the first one being
   *   defined by a center, a line pattern, and a length value.
   * @param xmin left border of the scan area
   * @param ymin low border of the scan area
   * @param xmax right border of the scan area
   * @param ymax up border of the scan area
   * @param a x coefficient of the discrete support line
   * @param b y coefficient of the discrete support line
   * @param nbs size of the support line pattern
   * @param steps support line pattern
   * @param cx abscissae of the central scan center
   * @param cy ordinate of the central scan center
   * @param length length of a scan strip
   */
  AdaptiveScannerO7 (int xmin, int ymin, int xmax, int ymax,
                     int a, int b,
                     int nbs, bool *steps,
                     int cx, int cy, int length);

  /**
   * @fn first(vector<Pt2i> &scan)
   * \brief Returns the central scan.
   * Fills in the vector with the central scan and returns its size.
   * @param scan vectors of points to be filled in.
   */
  int first (vector<Pt2i> &scan);

  /**
   * @fn nextOnLeft(vector<Pt2i> &scan)
   * \brief Returns the next scan on the left.
   * Fills in the vector with the next scan on left and returns its size.
   * @param scan vectors of points to be filled in.
   */
  int nextOnLeft (vector<Pt2i> &scan);

  /**
   * @fn nextOnRight(vector<Pt2i> &scan)
   * \brief Returns the next scan on the right.
   * Fills in the vector with the next scan on right and returns its size.
   * @param scan vectors of points to be filled in.
   */
  int nextOnRight (vector<Pt2i> &scan);

  /**
   * @fn bindTo(int a, int b, int c)
   * \brief Binds the scan stripe to wrap the given digital line.
   * Resets the bounding lines parameters to center on the given line.
   * @param a New value for the 'a' parameter of the current scan stripe.
   * @param b New value for the 'b' parameter of the current scan stripe.
   * @param c New value for the medial axis of the current scan stripe.
   */
  void bindTo (int a, int b, int c);


protected :

  AdaptiveScannerO7 () { }

  /** Coefficients of the template discrete support lines */
  int templ_a, templ_b, templ_nu;

  /** Shift coefficient of the discrete lower support line */
  int dlc1;
};

#endif
