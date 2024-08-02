/**
 * Author: Lucian Bicsi
 * Date: 2021-06-28
 * License: CC0
 * Source: Own
 * Description: 
 * Computes the set $P + Q = \lbrace p + q \mid p \in P, q \in Q \rbrace$,
 * where $P$, $Q$ are convex sets (described by polygons).
 * Status: Tested on shenyang 2018 regional problem D
 * Time: $O(N + M)$
 */
#pragma once

#include "Point.h"

vector<Point> MinkowskiSum(vector<Point> P, vector<Point> Q) {
  rotate(P.begin(), min_element(P.begin(), P.end()), P.end());
  rotate(Q.begin(), min_element(Q.begin(), Q.end()), Q.end());
  P.push_back(P[0]);
  P.push_back(P[1]);
  Q.push_back(Q[0]);
  Q.push_back(Q[1]);
  int n = P.size(), m = Q.size();
  vector<Point> R;
  for (int i = 0, j = 0; i < n - 2 || j < m - 2; ) {
    R.push_back(P[i] + Q[j]);
    auto c = cross(P[i + 1] - P[i], Q[j + 1] - Q[j]);
    if (c >= 0 && i < n - 2) ++i;
    if (c <= 0 && j < m - 2) ++j;
  }
  return R;
}
