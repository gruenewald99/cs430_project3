//https://github.com/gruenewald99/cs430_project3.git
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef struct {
  int kind; // 0 = plane, 1 = sphere
  int width,height;
  union {
    struct {
      int position[3];
      int color[3];
      int normal[3];
    } plane;
    struct {
      int color[3];
      int position[3];
      int radius;
    } sphere;
  };
} Object;
Object** objects;


int line = 1;

// next_c() wraps the getc() function and provides error checking and line
// number maintenance
int next_c(FILE* json) {
  int c = fgetc(json);
#ifdef DEBUG
  printf("next_c: '%c'\n", c);
#endif
  if (c == '\n') {
    line += 1;
  }
  if (c == EOF) {
    fprintf(stderr, "Error: Unexpected end of file on line number %d.\n", line);
    exit(1);
  }
  return c;
}


// expect_c() checks that the next character is d.  If it is not it emits
// an error.
void expect_c(FILE* json, int d) {
  int c = next_c(json);
  if (c == d) return;
  fprintf(stderr, "Error: Expected '%c' on line %d.\n", d, line);
  exit(1);
}


// skip_ws() skips white space in the file.
void skip_ws(FILE* json) {
  int c = next_c(json);
  while (isspace(c)) {
    c = next_c(json);
  }
  ungetc(c, json);
}


// next_string() gets the next string from the file handle and emits an error
// if a string can not be obtained.
char* next_string(FILE* json) {
  char buffer[129];
  int c = next_c(json);
  if (c != '"') {
    fprintf(stderr, "Error: Expected string on line %d.\n", line);
    exit(1);
  }
  c = next_c(json);
  int i = 0;
  while (c != '"') {
    if (i >= 128) {
      fprintf(stderr, "Error: Strings longer than 128 characters in length are not supported.\n");
      exit(1);
    }
    if (c == '\\') {
      fprintf(stderr, "Error: Strings with escape codes are not supported.\n");
      exit(1);
    }
    if (c < 32 || c > 126) {
      fprintf(stderr, "Error: Strings may contain only ascii characters.\n");
      exit(1);
    }
    buffer[i] = c;
    i += 1;
    c = next_c(json);
  }
  buffer[i] = 0;
  return strdup(buffer);
}

int next_number(FILE* json) {
  int value;
  fscanf(json, "%d", &value);
  // Error check this..
  return value;
}

int* next_vector(FILE* json) {
  int* v = malloc(3*sizeof(int));
  expect_c(json, '[');
  skip_ws(json);
  v[0] = next_number(json);
  skip_ws(json);
  expect_c(json, ',');
  skip_ws(json);
  v[1] = next_number(json);
  skip_ws(json);
  expect_c(json, ',');
  skip_ws(json);
  v[2] = next_number(json);
  skip_ws(json);
  expect_c(json, ']');
  return v;
}


void read_scene(char* filename) {
  objects = malloc(sizeof(Object*)*2);
  objects[0] = malloc(sizeof(Object));
  objects[1] = malloc(sizeof(Object));
  int c;
  FILE* json = fopen(filename, "r");

  if (json == NULL) {
    fprintf(stderr, "Error: Could not open file \"%s\"\n", filename);
    exit(1);
  }

  skip_ws(json);

  // Find the beginning of the list
  expect_c(json, '[');

  skip_ws(json);

  // Find the objects

  while (1)
  {

    c = fgetc(json);
    if (c == ']') {
      fprintf(stderr, "Error: This is the worst scene file EVER.\n");
      fclose(json);
      return;
    }
    if (c == '{') {
      skip_ws(json);

      // Parse the object
      char* key = next_string(json);
      if (strcmp(key, "type") != 0) {
	       fprintf(stderr, "Error: Expected \"type\" key on line number %d.\n", line);
	        exit(1);
      }

      skip_ws(json);

      expect_c(json, ':');

      skip_ws(json);



      char* type = next_string(json);
      int i;
      if (strcmp(type, "camera") == 0) {
      } else if (strcmp(type, "sphere") == 0) {
        i=1;
        objects[1]->kind = 1;
      } else if (strcmp(type, "plane") == 0) {
        i = 0;
        objects[0]->kind =0;
      } else {
	fprintf(stderr, "Error: Unknown type, \"%s\", on line number %d.\n", type, line);
	exit(1);
      }

      skip_ws(json);

      while (1) {
	// , }
	c = next_c(json);
	if (c == '}') {
	  // stop parsing this object
	  break;
	} else if (c == ',') {
	  // read another field
	  skip_ws(json);
	  char* key = next_string(json);
    //printf("%s\n", key);
	  skip_ws(json);
	  expect_c(json, ':');
	  skip_ws(json);
	  if (strcmp(key, "width") == 0){
      int value = next_number(json);
      objects[0]->width= value;
    }
	  else if(strcmp(key, "height") == 0){
      int value = next_number(json);
      objects[0]->height = value;
    }
	  else if(strcmp(key, "radius") == 0) {
	    int value = next_number(json);
      objects[1]->sphere.radius =value;

	  } else if (strcmp(key, "color") == 0)
    {
      int* value = next_vector(json);

      if(strcmp(type, "plane") == 0)
      {
        objects[0]->plane.color[0] =value[0];
        objects[0]->plane.color[1] =value[1];
        objects[0]->plane.color[2] =value[2];
      }else if(strcmp(type, "sphere")==0)
      {
        objects[1]->sphere.color[0] =value[0];
        objects[1]->sphere.color[1] =value[1];
        objects[1]->sphere.color[2] =value[2];
      }

    }
		  else if(strcmp(key, "position") == 0)
    {
            //printf("type %s",type);
      int* value = next_vector(json);
      if(strcmp(type, "plane") == 0)
      {
        objects[0]->plane.position[0] =value[0];
        objects[0]->plane.position[1] =value[1];
        objects[0]->plane.position[2] =value[2];
      }else if(strcmp(type, "sphere") == 0)
      {
        objects[1]->sphere.position[0] =value[0];
        objects[1]->sphere.position[1] =value[1];
        objects[1]->sphere.position[2] =value[2];
        //printf("%d %d %d\n",objects[0]->sphere.position[0], objects[0]->sphere.position[1], objects[0]->sphere.position[2] );
      }

    }
		  else if(strcmp(key, "normal") == 0)
      {
	       int* value = next_vector(json);

           objects[0]->plane.normal[0] =value[0];
           objects[0]->plane.normal[1] =value[1];
           objects[0]->plane.normal[2] =value[2];
      }
      else {
	    fprintf(stderr, "Error: Unknown property, \"%s\", on line %d.\n",
		    key, line);
	    //char* value = next_string(json);
	  }
	  skip_ws(json);
	} else {
	  fprintf(stderr, "Error: Unexpected value on line %d\n", line);
	  exit(1);
	}
      }
      skip_ws(json);
      c = next_c(json);
      if (c == ',') {
	// noop
	skip_ws(json);
      } else if (c == ']') {
	       fclose(json);
	        return;
      } else {
	fprintf(stderr, "Error: Expecting ',' or ']' on line %d.\n", line);
	exit(1);
      }
    }
  }
}
static inline double sqr(double v) {
  return v*v;
}


static inline void normalize(double* v) {
  double len = sqrt(sqr(v[0]) + sqr(v[1]) + sqr(v[2]));
  v[0] /= len;
  v[1] /= len;
  v[2] /= len;
}

double sphere_intersection(double* Ro, double* Rd, int r) {
  // Step 1. Find the equation for the object you are
  // interested in..  (e.g., cylinder)
  //
  // R(t) = R0 + t * Rd , t > 0 with R0 = [X0, Y0, Z0] and Rd = [Xd, Yd, Zd]
  //
  //
  // Step 2. Parameterize the equation with a center point
  // if needed
  //
  // (x-Cx)^2 + (z-Cz)^2 = r^2
  //
  // Step 3. Substitute the eq for a ray into our object
  // equation.
  //
  // (Rox + t*Rdx - Cx)^2 + (Roz + t*Rdz - Cz)^2 - r^2 = 0
  //
  // Step 4. Solve for t.
  //
  // Step 4a. Rewrite the equation (flatten).
  //
  // -r^2 +
  // t^2 * Rdx^2 +
  // t^2 * Rdz^2 +
  // 2*t * Rox * Rdx -
  // 2*t * Rdx * Cx +
  // 2*t * Roz * Rdz -
  // 2*t * Rdz * Cz +
  // Rox^2 -
  // 2*Rox*Cx +
  // Cx^2 +
  // Roz^2 -
  // 2*Roz*Cz +
  // Cz^2 = 0
  //
  // Steb 4b. Rewrite the equation in terms of t.
  //
  // t^2 * (Rdx^2 + Rdz^2) +
  // t * (2 * (Rox * Rdx - Rdx * Cx + Roz * Rdz - Rdz * Cz)) +
  // Rox^2 - 2*Rox*Cx + Cx^2 + Roz^2 - 2*Roz*Cz + Cz^2 - r^2 = 0
  //
  // Use the quadratic equation to solve for t..
  double a = (sqr(Rd[0]) + sqr(Rd[1]) + sqr(Rd[3]));
  double b = (2 * (Ro[0] - Ro[0]*r) + Rd[1] * (Ro[1]-Ro[1]*r) + Rd[2] *(Ro[2] - Ro[2]*r));
  double c = sqr(Ro[0] - r*Ro[0]) + sqr(Ro[1] - r*Ro[1]) + sqr(Ro[2] - r*Ro[2]) - sqr(r);

  double det = sqr(b) - 4 * a * c;
  if (det < 0) return -1;

  det = sqrt(det);

  double t0 = (-b - det) / (2*a);
  if (t0 > 0) return t0;

  double t1 = (-b + det) / (2*a);
  if (t1 > 0) return t1;

  return -1;
}


int main(int c, char** argv) {
  read_scene(argv[3]);
  FILE * fh2 = fopen(argv[4], "w");
  int width = (int)argv[1];
  int height = (int)argv[2];
//   double cx = 0;
//   double cy = 0;
//   double h = 0.7;
//   double w = 0.7;
//   fprintf(fh2, "P3\n" );
//   fprintf(fh2, "%d %d\n",width,height);
//   fprintf(fh2, "%d\n", 255);
//
//   int M = 20;
//   int N = 20;
//
//   double pixheight = h / M;
//   double pixwidth = w / N;
// for (int y = 0; y < M; y += 1) {
//   for (int x = 0; x < N; x += 1) {
//     double Ro[3] = {0, 0, 0};
//     // Rd = normalize(P - Ro)
//     double Rd[3] = {
//       cx - (w/2) + pixwidth * (x + 0.5),
//       cy - (h/2) + pixheight * (y + 0.5),
//       1
//     };
//     normalize(Rd);
//
//     double best_t = INFINITY;
//     for (int i=0; objects[i] != 0; i += 1) {
//         double t = 0;
//
//         switch(objects[i]->kind) {
//           case 0:
//             t = sphere_intersection(Ro, Rd,
//             objects[i]->sphere.radius);
//           break;
//           default:
//           // Horrible error
//           exit(1);
//         }
//         if (t > 0 && t < best_t) best_t = t;
//   }
//   if (best_t > 0 && best_t != INFINITY) {
//     fprintf(fh2, "%u\n", objects[1]->sphere.color[0]);
//     fprintf(fh2, "%u\n", objects[1]->sphere.color[1]);
//     fprintf(fh2, "%u\n", objects[1]->sphere.color[2]);
//   } else {
//       printf(".");
//   }
//
//   }
//   printf("\n");
// }
//
//   return 0;
}
