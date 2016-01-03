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

