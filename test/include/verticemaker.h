#ifndef VERTICEMAKER_H
#define VERTICEMAKER_H

#include <QVector>
#include <QVector3D>
#include <QDebug>
#include <math.h>


/*Code pris sur http://goo.gl/PJixnL*/
struct sphereGeom
{

    sphereGeom( unsigned int subdiv = 4 ) {
        unsigned int perFaceVertexCount = subdiv * subdiv;
        float invSubDiv = 1.f / float(subdiv-1);

        // allocate the vertex buffer memory
        vertices.resize( subdiv * subdiv * 6);
        // fill the buffer
        for ( unsigned int face = 0; face != 3; ++face ) {
            // helper to redirect the vertex coordinate in the axis
            const struct { unsigned int xIdx, yIdx, zIdx; } faceIndex[] { {0,1,2}, {2,0,1}, {1,2,0} };
            auto const & indirect = faceIndex[face];
            for ( unsigned int v = 0; v < subdiv; ++v ) {
                float texV = float(v) * invSubDiv; // the v texcoord [0..1]
                for ( unsigned int u = 0; u < subdiv; ++u) {
                    float texU = float(u) * invSubDiv; // the u texcoord [0..1]
                    float posU = texU * 2.f - 1.f; // the position first value [-1..1]
                    float posV = texV * 2.f - 1.f; // the position second value [-1..1]
                    float invLen = 1.f / sqrtf( posU*posU + posV*posV + 1.f ); // pythagore
                    vertex result;
                    // set the vertex position
                    result.pos[ indirect.xIdx ] = posU * invLen;
                    result.pos[ indirect.yIdx ] = posV * invLen;
                    result.pos[ indirect.zIdx ] = invLen;
                    // set the vertex texcoord
                    result.uv[0] = texU;
                    result.uv[1] = texV;
                    // copy result to the right offset
                    vertices[ perFaceVertexCount * ( 2 * face + 0 ) + u + subdiv * v] = result;
                    // mirror x and z location for the other face
                    result.pos[ indirect.xIdx ] *= -1.f;
                    result.pos[ indirect.zIdx ] *= -1.f;
                    vertices[ perFaceVertexCount * ( 2 * face + 1 ) + u + subdiv * v] = result;
                }
            }
        }
        unsigned int perFaceQuadCount = (subdiv-1)*(subdiv-1); // number of quad per faces
        // start by filling an index buffer for one face, then we will duplicate it for each other face
        std::vector<unsigned int> faceIndices;
        faceIndices.resize( perFaceQuadCount * 2 * 3 ); // one quad is two triangle of three indices.
        for (unsigned int v = 0; v != subdiv - 1; ++v ) {
            for (unsigned int u = 0; u != subdiv - 1; ++u ) {
                // the four vertex corners of the quad
                unsigned int i0 = ( u + 0 ) + ( v + 0 ) * subdiv;
                unsigned int i1 = ( u + 0 ) + ( v + 1 ) * subdiv;
                unsigned int i2 = ( u + 1 ) + ( v + 1 ) * subdiv;
                unsigned int i3 = ( u + 1 ) + ( v + 0 ) * subdiv;

                // fill the two triangles, clockwise
                auto it = std::begin(faceIndices) + ( v * (subdiv-1) + u ) * 6;
                *it++ = i1; *it++ = i0; *it++ = i3;
                *it++ = i1; *it++ = i3; *it++ = i2;
            }
        }
        // allocate memory for the full index buffer
        indices.resize( perFaceQuadCount * 2 * 3 * 6 );
        // for each face, copy the per face index buffer with a index shift to map the correct range of vertices
        for( unsigned int face = 0; face != 6; ++face) {
            unsigned int vertexOffs = face * subdiv * subdiv;
            unsigned int indexOffs = face * 2 * 3 * perFaceQuadCount;
            std::transform( std::begin(faceIndices), std::end(faceIndices)
                , std::begin(indices) + indexOffs
                , [vertexOffs]( unsigned int idx) { return idx+vertexOffs; } );
        }
    }
    struct vertex { float pos[3]; float uv[2]; };
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
};


struct vertex
{
    QVector<QVector3D> vertices;
    QVector<QVector3D> color;
};



class VerticeMaker
{
public:
    static constexpr double MO_PI = 3.14159265359;
    static constexpr double MO_PI_2 = 1.57079632679;

    VerticeMaker();
    ~VerticeMaker();

    static vertex verticesCube(double a, QVector3D decalage=QVector3D(0.0,0.0,0.0));//retourne les vertices et la couleur d'un cube selon son arrête
    static vertex verticesSphere(sphereGeom s);
    static vertex verticesSphere(float radius, unsigned int rings, unsigned int sectors);
    static vertex verticesSphere(double a, int depth);
private:
    static QVector<QVector3D> subdivideTriangle(QVector3D v1, QVector3D v2, QVector3D v3, int depth);



};


#endif // VERTICEMAKER_H
