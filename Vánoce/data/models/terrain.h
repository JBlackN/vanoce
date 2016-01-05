//----------------------------------------------------------------------------------------
/**
* \file       terrain.h
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains model of a terrain.
*
*  File contains terrain's number of attributes per vertex, number of vertices,
*  number of triangles, array of vertices and array of triangles.
*
*/
//----------------------------------------------------------------------------------------
const int terrainNAttribsPerVertex = 8;
const int terrainNVertices = 4;
const int terrainNTriangles = 2;
const float terrainVertices[] = {
  -1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,1.0f,
  1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
  1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,
  -1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,
}; // end terrainVertices

const unsigned terrainTriangles[] = {
  0, 1, 2,
  0, 2, 3,
}; // end terrainTriangles

