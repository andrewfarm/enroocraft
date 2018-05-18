//
//  indexedmesh.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/18/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include "indexedmesh.h"

IndexedMesh::IndexedMesh(
        const VertexAttrib attribList[],
        GLuint attribCount,
        GLenum vertexUsage,
        GLenum indexUsage,
        GLenum drawMode) :
Mesh(attribList, attribCount, vertexUsage, drawMode),
indexBuffer(GL_ELEMENT_ARRAY_BUFFER, indexUsage)
{}

void IndexedMesh::setIndices(const void *data, GLsizei size, GLenum type,
        GLsizei typeSize) {
    indexType = type;
    indexCount = size / typeSize;
    indexBuffer.setData(data, size);
}

void IndexedMesh::draw() {
    bindVAO();
    indexBuffer.bind();
    glDrawElements(getDrawMode(), indexCount, indexType, (void *) 0);
}
