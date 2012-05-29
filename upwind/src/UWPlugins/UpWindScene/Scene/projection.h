#ifndef PROJECTION_H
#define PROJECTION_H

#include <math.h>

#define D_R (M_PI / 180.0)
#define R_D (180.0 / M_PI)
#define R_MAJOR 6378137.0   //metres
#define R_MINOR 6356752.3142    //metres
#define RATIO (R_MINOR/R_MAJOR)
#define ECCENT (sqrt(1.0 - (RATIO * RATIO)))
#define COM (0.5 * ECCENT)
#define RADIUSAVERAGE 6371010    //metres

class Projection{
    public:
    Projection();




static double deg_rad (double ang) {
        return ang * D_R;
}

static double merc_x (double lon) {
    double temp = R_MAJOR * deg_rad (lon);      // for debugging porpose
        return temp;
}

static double merc_y (double lat) {
        lat = fmin (89.5, fmax (lat, -89.5));
        double phi = deg_rad(lat);
        double sinphi = sin(phi);
        double temp = R_MAJOR*log((tan((M_PI/4)+(phi/2)))*(pow((1-(ECCENT*sinphi))/(1+(ECCENT*sinphi)),(ECCENT/2)))); // for debugging porpose
        return temp;
}

static double rad_deg (double ang) {
        return ang * R_D;
}

static double merc_lon (double x) {
        return rad_deg(x) / R_MAJOR;
}

static double merc_lat (double y) {
        double ts = exp ( -y / R_MAJOR);
        double phi = M_PI_2 - 2 * atan(ts);
        double dphi = 1.0;
        int i = 0;
        while ((fabs(dphi) > 0.000000001) && (i < 15)) {
                double con = ECCENT * sin (phi);
                dphi = M_PI_2 - 2 * atan (ts * pow((1.0 - con) / (1.0 + con), COM)) - phi;
                phi += dphi;
                i++;
        }
        return rad_deg (phi);
}


};
#endif // PROJECTION_H
