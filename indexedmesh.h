//
//  indexedmesh.h
//  enroocraft
//
//  Created by Andrew Farm on 5/18/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef indexedmesh_h
#define indexedmesh_h

#include "mesh.h"

class IndexedMesh : public Mesh {
    VBO indexBuffer;
    GLsizei indexCount;
    GLenum indexType;
    
public:
    IndexedMesh(
            const VertexAttrib attribList[],
            GLuint attribCount,
            GLenum vertexUsage,
            GLenum indexUsage,
            GLenum drawMode);
    
    /* Sets the contents of the index buffer. Can be called more than once. */
    void setIndices(const void *data, GLsizei size, GLenum type,
            GLsizei typeSize);
    
    /* Draws the mesh. It is undefined to call draw() before setData() and
     setIndices() have both been called. */
    void draw();
};

#endif /* indexedmesh_h */
