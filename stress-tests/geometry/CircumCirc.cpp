#include <vector>

#include "../utilities/template.h"

#include "../../content/geometry/CircumCirc.h"

signed main()
{
    Point a{0, 0}, b{0, 1}, c{1, 0};
    Point inside{0.1, 0.1};
    Point on_circle{1, 1};
    Point outside{4, 4};

    Circle circum_circle = CircumCircle(a, b, c);

    assert(sgn(circum_circle.r * 2 - sqrt(2)) == 0);
    assert(abs(circum_circle.c - Point{0.5, 0.5}) < EPS);

    assert(InsideCircum(inside, a, b, c) == 1);
    assert(InsideCircum(inside, a, c, b) == 1);
    
    assert(InsideCircum(outside, a, b, c) == -1);
    assert(InsideCircum(outside, a, c, b) == -1);

    assert(InsideCircum(on_circle, a, b, c) == 0);
    assert(InsideCircum(on_circle, a, c, b) == 0);

    cout << "All tests passed!" << endl;
    return 0;
}
