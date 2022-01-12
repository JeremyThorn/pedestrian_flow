#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct{
    double x;
    double y;
}point;

typedef struct{
    point r;
    point v;
    point a_p;
    point a_f;
    point desired_v;
    double d0;
    double m;
    int idx;
    int red;
    int grn;
    int blu;
}pedestrian;

point force(pedestrian ped, pedestrian* peds, int num_peds, double upper, double lower);

const point gravity = {0, -9.81};

const double tau = 1.0;

point box_muller(point uniform_pair){
  double mag = sqrt(-2*log(uniform_pair.x));
  double phase = 2*M_PI*uniform_pair.y;
  point normal_pair = {mag*cos(phase), mag*sin(phase)};
  return normal_pair;
}

double uniform01(){
  double t = (double)rand()/RAND_MAX;
  return t;
}

void update_pedestrian(pedestrian* ped, pedestrian* peds, int num_peds, double upper, double lower,double dt){
    ped->a_p.x = force(*ped, peds, num_peds, upper, lower).x/ped->m;
    ped->a_p.y = force(*ped, peds, num_peds, upper, lower).y/ped->m;
    //printf("ped->a_p.y  %lf\n", ped->a_p.y);

    ped->r.x = ped->r.x + ped->v.x * dt + 0.5 * ped->a_p.x * dt * dt;
    ped->r.y = ped->r.y + ped->v.y * dt + 0.5 * ped->a_p.y * dt * dt;
    //printf("ped->r.y  %lf\n", ped->r.y);

    ped->a_f.x = force(*ped, peds, num_peds, upper, lower).x/ped->m;
    ped->a_f.y = force(*ped, peds, num_peds, upper, lower).y/ped->m;
    //printf("ped->a_f.y  %lf\n", ped->a_f.y);

    ped->v.x = ped->v.x + 0.5 * (ped->a_p.x + ped->a_f.x) * dt;
    ped->v.y = ped->v.y + 0.5 * (ped->a_p.y + ped->a_f.y) * dt;
    //printf("ped->v.y  %lf\n", ped->v.y);
}

point force(pedestrian ped, pedestrian* peds, int num_peds, double upper, double lower){

    point total_force = {0.0, 0.0};

    point selfish = {ped.m * (ped.desired_v.x-ped.v.x)/tau,
                    ped.m * (ped.desired_v.y-ped.v.y)/tau};

    total_force.x = total_force.x + selfish.x;
    total_force.y = total_force.y + selfish.y;

    for(int i = 0; i < num_peds; i++){
      if(i != ped.idx){
        double dx = peds[i].r.x - ped.r.x; double dy = peds[i].r.y - ped.r.y;
        double mag = 100.0 * exp(-(dx*dx+dy*dy)/ped.d0);
        double angle = atan2(dy, dx);

        point avoid = {-mag*cos(angle), -mag*sin(angle)};
        /*if(mag >= 1.0){
          printf("dx, dy %lf %lf\n", dx, dy);
          printf("mag %lf\n", mag);
          printf("avoid %lf, %lf\n", avoid.x, avoid.y);
        }*/
        total_force.x = total_force.x + avoid.x;
        total_force.y = total_force.y + avoid.y;
      }
    }

    double dy_wall_up = ped.r.y - upper;
    double mag_wall_up = 1000.0 * exp(-(dy_wall_up*dy_wall_up)/ped.d0);
    point upper_wall_force = {0.0, mag_wall_up};

    total_force.x = total_force.x + upper_wall_force.x;
    total_force.y = total_force.y + upper_wall_force.y;

    double dy_wall_low = ped.r.y - lower;
    double mag_wall_low = 1000.0 * exp(-(dy_wall_low*dy_wall_low)/ped.d0);
    point lower_wall_force = {0.0, -mag_wall_low};

    total_force.x = total_force.x + lower_wall_force.x;
    total_force.y = total_force.y + lower_wall_force.y;

    point uni_point = {uniform01(), uniform01()}, random = box_muller(uni_point);

    total_force.x = total_force.x + random.x;
    total_force.y = total_force.y + random.y;

    return total_force;
}
