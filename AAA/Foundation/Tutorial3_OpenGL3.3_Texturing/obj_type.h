/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * Original Author: Damiano Vitulli
 * Porting to OpenGL3.3: Movania Muhammad Mobeen
 * Shaders Functions: Movania Muhammad Mobeen
 *
 * This program is released under the BSD licence
 * By using this program you agree to licence terms on spacesimulator.net copyright page
 *
 */

#ifndef OBJ_TYPE_INC
#define OBJ_TYPE_INC
#pragma once

#define MAX_VERTICES 2000 // Max number of vertices (for each object)
#define MAX_POLYGONS 2000 // Max number of polygons (for each object)



/**********************************************************
 *
 * TYPES DECLARATION
 *
 *********************************************************/

// Our vertex type
typedef struct{
    GLfloat x,y,z;
}vertex_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct{
    GLuint a,b,c;
}polygon_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct{
    GLfloat u,v;
}mapcoord_type;

// The object type
typedef struct { 
    vertex_type vertex[MAX_VERTICES]; 
    polygon_type polygon[MAX_POLYGONS];
    mapcoord_type mapcoord[MAX_VERTICES];
    GLuint id_texture;
} obj_type, *obj_type_ptr;
#endif