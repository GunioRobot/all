typedef struct{
	float r,g,b;
}color_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct{
	uint a,b,c;
}polygon_type;

struct vertex_type {
	float x,y,z;
};

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct{
	float u,v;
}mapcoord_type;

#define MAX_VERTICES 2000
#define MAX_POLYGONS 2000

// The object type
typedef struct {
    vertex_type vertex[MAX_VERTICES];
    polygon_type polygons[MAX_POLYGONS];
    mapcoord_type mapcoord[MAX_VERTICES];
    color_type colors[MAX_VERTICES];
} obj_type;

obj_type cube = {
	{
		{-1, -1, -1},
		{1, -1, -1},
		{-1, 1, -1},
		{1, 1, -1},

		{-1, -1, 1},
		{1, -1, 1},
		{-1, 1, 1},
		{1, 1, 1}
	},
	{
		{0, 1, 3}, // back
		{0, 2, 3},

		{0, 4, 5}, // bottom
		{0, 1, 5},

		{0, 4, 6}, // left
		{0, 2, 6},

		{1, 5, 7}, // right
		{1, 3, 7},

		{2, 6, 7}, // top
		{2, 3, 7},

		{4, 5, 7}, // front
		{4, 6, 7}
	},
	{
		{0, 0},
		{1, 0},
		{0, 1},
		{1, 1},

		{0, 0},
		{1, 0},
		{0, 1},
		{1, 1}
	},
	{
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1},
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1},
		{1, 0, 0},
		{0, 1, 0}
	}
};
const uint n_indices = sizeof(cube.polygons) / sizeof(cube.polygons[0]);
