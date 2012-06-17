#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "rays.h"

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define vsub(a, b) {a.x - b.x, a.y - b.y, a.z - b.z}
#define vadd(a, b) {a.x + b.x, a.y + b.y, a.z + b.z}
#define vscale(a, b) {b*a.x, b*a.y, b*a.z}

double vdot(vec *a, vec *b) { return a->x*b->x + a->y*b->y + a->z*b->z; }

double ray_sphere_hit(ray *r, sphere *s)
{
  vec so = vsub(s->o, r->o);
  double so_dot_d = vdot(&r->d, &so), so2 = vdot(&so, &so), 
    rd2 = vdot(&r->d, &r->d);
  double descr = (s->r*s->r - so2)*rd2 + so_dot_d*so_dot_d;
  if (descr < 0) return -1.0;
  descr = sqrt(descr);
  return MIN(so_dot_d + descr, so_dot_d - descr) / rd2;
}

sphere * find_hit(sphere spheres[], int n, ray *r, double *out_s)
{
  sphere *minsphere = NULL;
  double min_s = -1.0;
  int i;
  for (i=0; i<n; i++) {
    double s = ray_sphere_hit(r, &spheres[i]);
    if (s>0 && (s < min_s || min_s < 0)) {
      min_s = s;
      minsphere = &spheres[i];
    }
  }
  *out_s = min_s;
  return minsphere;
}

void normalize(vec *v)
{
  double mag = sqrt(vdot(v, v));
  v->x /= mag; v->y /= mag; v->z /= mag;
}

void jitter(vec *v)
{
  v->x += ((double) rand() / RAND_MAX)*2 - 1;
  v->y += ((double) rand() / RAND_MAX)*2 - 1;
  v->z += ((double) rand() / RAND_MAX)*2 - 1;
}

unsigned int find_color(ray *r, sphere spheres[], int nspheres, 
                        light lights[], int nlights)
{
  double s, out=0;
  sphere *hit = find_hit(spheres, nspheres, r, &s);
  if (hit) {
    vec tmp = vscale(r->d, s);
    vec hit_point = vadd(tmp, r->o);
    int i;
    double s2;

    for (i=0; i<nlights; i++) {
      ray shadow_ray = {hit_point, vsub(lights[i].o, hit_point)};
      sphere *shadow = find_hit(spheres, nspheres, &shadow_ray, &s2);
      if (!shadow || s2 > 1) {
        vec normal = vsub(hit_point, hit->o);
        double l = vdot(&shadow_ray.d, &normal);
        if (l>0) {
          out += lights[i].strength * l / 
            vdot(&shadow_ray.d, &shadow_ray.d) / sqrt(vdot(&normal, &normal));
        }
      }
    }
  }
  unsigned int val = (out > 1.0 ? 1.0 : out)*255;
  val |= (val << 8);
  val |= (val << 8);
  return val;
}

// Camera is at 0,0,0, pointing in the 0,1,0 direction.
// Viewing plane is at 0,1,0
void dotrace(int w, int h, sphere spheres[], int nspheres,
             light lights[], int nlights,
             unsigned int *pixels)
{
  double aspect = (double)h/w;
#pragma omp parallel for schedule(dynamic, 1)
  for (int j=0; j<h; j++) {
    ray r = {{0,0,0}, {0,1.3,0}};
    r.d.z = ((double)j*2/(h-1) - 1.0)*aspect*-1;
    for (int i=0; i<w; i++) {
      r.d.x = ((double)i*2/(w-1) - 1.0);
      pixels[j*w+i] = find_color(&r, spheres, nspheres, lights, nlights);
    }
  }
}

struct tgatag {
  unsigned char stuff[12];
  unsigned short width;
  unsigned short height;
  unsigned char depth;
  unsigned char junk;
};
