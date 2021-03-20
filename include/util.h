#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256

#define CUBE_SIZE 0.5f

#define ping fprintf(stderr, "ping from %d\n", __LINE__);

#define create_coord(size, name) \
	typedef struct {             \
		size x;                  \
		size y;                  \
		size z;                  \
	} name##coord;

#define coord_cmp(type, a, b) \
	a.x == b.x &&a.y == b.y &&a.z == b.z

create_coord(int64_t, i64_)

typedef uint8_t blockstate;

typedef struct {
	i64_coord position;
	blockstate state;
} block;

#define mod(a, b) ((a % b) + b) % b


#endif