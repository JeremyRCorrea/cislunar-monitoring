#pragma once
#include "Math.h"
#define pi 3.14159265358979323846
#define twopi 6.28318530717958647692

struct OE {
    double a;
    double e;
    double i;
    double w;
    double O;
    double f;
};

OE rv2coe(double mu, const State& state)
{

    double a, ecc, incl, w, O, nu;

    double rad2deg = 180.0 / pi;

    double3 r = double3{ state[0], state[1], state[2] };
    double3 v = double3{ state[3], state[4], state[5] };

    double3 hbar;
    double3 nbar;
    double3 ebar;

    double small, magr, magv, magn, sme, rdotv, temp, c1, hk, magh, halfpi;




    double undefined = 99.9;

    halfpi = 0.5 * pi;
    small = 0.00000001;


    // -------------------------  implementation   -----------------
    magr = magnitude(r);
    magv = magnitude(v);

    // ------------------  find h n and e vectors   ----------------
    hbar = cross(r, v);
    magh = magnitude(hbar);


    nbar = double3{ -hbar[1], hbar[0], 0.0 };

    magn = magnitude(nbar);
    c1 = magv * magv - mu / magr;
    rdotv = dot(r, v);
    temp = 1.0 / mu;
    for (int i = 0; i <= 2; i++) {
        ebar[i] = (c1 * r[i] - rdotv * v[i]) * temp;
    }
        
    ecc = magnitude(ebar);

    // ------------  find a e and semi-latus rectum   ----------
    sme = (magv * magv * 0.5) - (mu / magr);
    if (fabs(sme) > small)
        a = -mu / (2.0 * sme);
    else
        a = 99.9;
    double p = magh * magh * temp;

    // -----------------  find inclination   -------------------
    hk = hbar[2] / magh;
    incl = acos(hk);


    // Determine orbit type based on eccentricity and inclination
    bool isCircular = (ecc < small);  // True if orbit is nearly circular
    bool isEquatorial = (fabs(incl) < small || fabs(incl - pi) < small);  // True if orbit is nearly equatorial

    // Right Ascension of Ascending Node (RAAN)
    if (magn > small) {
        temp = nbar[0] / magn;
        if (fabs(temp) > 1.0) temp = sgn(temp);
        O = acos(temp);
        if (nbar[1] < 0.0) O = twopi - O;
    }
    else {
        O = undefined;
    }

    // Argument of Perigee
    if (!isCircular) {
        w = angle(nbar, ebar);
        if (ebar[2] < 0.0) w = twopi - w;
    }
    else {
        w = undefined;
    }

    // True Anomaly
    if (!isCircular || !isEquatorial) {
        nu = angle(ebar, r);
        if (rdotv < 0.0) nu = twopi - nu;
    }
    else {
        nu = undefined;
    }

    OE orbital_elements = { a, ecc, incl * rad2deg, w * rad2deg, O * rad2deg, nu * rad2deg };
    return orbital_elements;
}