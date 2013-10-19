typedef float vec __attribute__ ((vector_size (16)));
typedef struct { vec o; float r; } sphere ;
typedef struct { vec o, d; } ray;
typedef struct { vec o; float strength; } light;

//FILE: rays.c
// ==============================
float ray_sphere_hit(ray *r, sphere *s); // Line 17-26
sphere * find_hit(sphere spheres[], int n, ray *r, float *out_s); // Line 28-42
unsigned int find_color(ray *r, sphere spheres[], int nspheres, light lights[], int nlights); // Line 57-82
void dotrace(int w, int h, sphere spheres[], int nspheres, light lights[], int nlights, unsigned int *pixels); // Line 86-100
