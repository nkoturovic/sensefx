#include <iostream>
#include <map>
#include "vec.h"
#include "config.h"

using namespace std;

int main() {

    /* vector class examples */
    vec3 u(2,8,-2);
    vec3 v(-4,11,-4);

    cout << "Norme u i v: " << u.norm() << " " << v.norm() << endl;

    if (u <= v)
        cout << "u <= v" << endl;

    if (u >= v)
        cout << "u >= u" << endl;

    vec3 w = u + v;
    cout << "u + v is:" << w << endl;

    vec3 w1 = u - v;
    cout << "u - v is:" << w1 << endl;

    double dotProduct = u * v;
    cout << "Dot product is: " << dotProduct << endl;

    vec3 vecProduct = u.operatorX(v);
    cout << "Cross product: " << vecProduct << endl;

    vec2 u2 = vec2(2, 2);
    vec2 v2 = vec2(-3, 4);

    cout << "Cross of 2-dim vecs u2 and v2 is: " << u2.operatorX(v2) << endl;

    vec3 x(2,-3,1);

    cout << "Triple product: " << v.operatorTriple(u,x) << endl;

    vec3 xNorm = x.normalize();
    cout << "Normalized x: " << xNorm << endl;

    /* config class examples */
    config appCfg("configs/application.cfg");
    cout << appCfg.getParameter("TITLE") << endl;

    std::map<string, config> confMap = config::importAll("configs");
    cout << confMap["application"].getParameter("AUTHOR") << endl;

    return 0;
}
