/**
 * Author: Victor Lecomte, chilli
 * Date: 2019-10-31
 * License: CC0
 * Source: https://vlecomte.github.io/cp-geo.pdf
 * Description: Finds the external tangents of two circles, or internal if r2 is negated.
 * Can return 0, 1, or 2 tangents -- 0 if one circle contains the other (or overlaps it, in the internal case, or if the circles are the same);
 * 1 if the circles are tangent to each other (in which case .first = .second and the tangent line is perpendicular to the line between the centers).
 * .first and .second give the tangency points at circle 1 and 2 respectively.
 * To find the tangents of a circle with a point set r2 to 0.
 * Status: tested
 */
#pragma once

#include "Point.h"
#include "Circle.h"

int Tangents(Circle c1, Circle c2, 
             vector<pair<Point, Point>>& out) {
  Point d = c2.c - c1.c;
  double dr = c1.r - c2.r, d2 = norm(d), h2 = d2 - dr * dr;
  if (sgn(d2) == 0 || sgn(h2) == -1) return 0;
  for (double z : {-1, 1}) {
    Point v = (d * dr + perp(d) * sqrt(h2) * z) / d2;
    out.emplace_back(c1.c + v * c1.r, c2.c + v * c2.r);
    if (sgn(h2) == 0) return 1;
  }
  return 2;
}
