/**
 * Author: Victor Lecomte, chilli
 * Date: 2019-04-27
 * License: CC0
 * Source: https://vlecomte.github.io/cp-geo.pdf
 * Description:\\
\begin{minipage}{75mm}
If a unique intersection point between the line segments going from $a$ to $b$ and from $c$ to $d$ exists then it is returned.
If no intersection point exists an empty vector is returned. If infinitely many exist a vector with 2 elements is returned, containing the endpoints of the common line segment.
Does products up to $O(X^3)$.
\end{minipage}
\begin{minipage}{15mm}
\includegraphics[width=\textwidth]{content/geometry/SegmentIntersection}
\end{minipage}
 * Status: stress-tested, tested on kattis:intersection
 */
#pragma once

#include "Point.h"
#include "OnSegment.h"

vector<Point> SegInter(Point a, Point b, Point c, Point d) {
  auto oa = det(c, d, a), ob = det(c, d, b),
       oc = det(a, b, c), od = det(a, b, d);
  // Checks if intersection is single non-endpoint point.
  if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0)
    return {(a * ob - b * oa) / (ob - oa)};
  set<Point> s;
  if (OnSegment(c, d, a)) s.insert(a);
  if (OnSegment(c, d, b)) s.insert(b);
  if (OnSegment(a, b, c)) s.insert(c);
  if (OnSegment(a, b, d)) s.insert(d);
  return {s.begin(), s.end()};
}