#include "verticemaker.h"

VerticeMaker::VerticeMaker()
{

}

VerticeMaker::~VerticeMaker()
{

}

vertex VerticeMaker::verticesCube(double a, QVector3D decalage)
{
    vertex levert;

    levert.vertices << QVector3D(-(a/2), -(a/2),  (a/2)) << QVector3D( (a/2), -(a/2),  (a/2)) << QVector3D( (a/2),  (a/2),  (a/2)) // Front
     << QVector3D( (a/2),  (a/2),  (a/2)) << QVector3D(-(a/2),  (a/2),  (a/2)) << QVector3D(-(a/2), -(a/2),  (a/2))
     << QVector3D( (a/2), -(a/2), -(a/2)) << QVector3D(-(a/2), -(a/2), -(a/2)) << QVector3D(-(a/2),  (a/2), -(a/2)) // Back
     << QVector3D(-(a/2),  (a/2), -(a/2)) << QVector3D( (a/2),  (a/2), -(a/2)) << QVector3D( (a/2), -(a/2), -(a/2))
     << QVector3D(-(a/2), -(a/2), -(a/2)) << QVector3D(-(a/2), -(a/2),  (a/2)) << QVector3D(-(a/2),  (a/2),  (a/2)) // Left
     << QVector3D(-(a/2),  (a/2),  (a/2)) << QVector3D(-(a/2),  (a/2), -(a/2)) << QVector3D(-(a/2), -(a/2), -(a/2))
     << QVector3D( (a/2), -(a/2),  (a/2)) << QVector3D( (a/2), -(a/2), -(a/2)) << QVector3D( (a/2),  (a/2), -(a/2)) // Right
     << QVector3D( (a/2),  (a/2), -(a/2)) << QVector3D( (a/2),  (a/2),  (a/2)) << QVector3D( (a/2), -(a/2),  (a/2))
     << QVector3D(-(a/2),  (a/2),  (a/2)) << QVector3D( (a/2),  (a/2),  (a/2)) << QVector3D( (a/2),  (a/2), -(a/2)) // Top
     << QVector3D( (a/2),  (a/2), -(a/2)) << QVector3D(-(a/2),  (a/2), -(a/2)) << QVector3D(-(a/2),  (a/2),  (a/2))
     << QVector3D(-(a/2), -(a/2), -(a/2)) << QVector3D( (a/2), -(a/2), -(a/2)) << QVector3D( (a/2), -(a/2),  (a/2)) // Bottom
     << QVector3D( (a/2), -(a/2),  (a/2)) << QVector3D(-(a/2), -(a/2),  (a/2)) << QVector3D(-(a/2), -(a/2), -(a/2));

    levert.color    << QVector3D(1.0, 0.0, 0.0) << QVector3D(1.0, 0.0, 0.0) << QVector3D(1.0, 0.0, 0.0) // Front
                << QVector3D(1.0, 0.0, 0.0) << QVector3D(1.0, 0.0, 0.0) << QVector3D(1.0, 0.0, 0.0)
                << QVector3D(1.0, 0.0, 0.0) << QVector3D(1.0, 0.0, 0.0) << QVector3D(1.0, 0.0, 0.0) // Back
                << QVector3D(1.0, 0.0, 0.0) << QVector3D(1.0, 0.0, 0.0) << QVector3D(1.0, 0.0, 0.0)
                << QVector3D(0.0, 1.0, 0.0) << QVector3D(0, 1.0, 0) << QVector3D(0, 1.0, 0) // Left
                << QVector3D(0, 1.0, 0) << QVector3D(0, 1.0, 0) << QVector3D(0, 1.0, 0)
                << QVector3D(0, 1.0, 0) << QVector3D(0, 1.0, 0) << QVector3D(0, 1.0, 0) // Right
                << QVector3D(0, 1.0, 0) << QVector3D(0, 1.0, 0) << QVector3D(0, 1.0, 0)
                << QVector3D(0, 0, 1.0) << QVector3D(0, 0, 1.0) << QVector3D(0, 0, 1.0) // Top
                << QVector3D(0, 0, 1.0) << QVector3D(0, 0, 1.0) << QVector3D(0, 0, 1.0)
                << QVector3D(0, 0, 1.0) << QVector3D(0, 0, 1.0) << QVector3D(0, 0, 1.0) // Bottom
                << QVector3D(0, 0, 1.0) << QVector3D(0, 0, 1.0) << QVector3D(0, 0, 1.0);

    for(int i=0; i < levert.vertices.size(); i++){
        levert.vertices[i] = levert.vertices[i] + decalage;
    }


    return levert;
}
vertex VerticeMaker::verticesSphere(sphereGeom s){
    vertex vertS;


    std::for_each( std::begin(s.vertices), std::end(s.vertices)
                , [&vertS]( sphereGeom::vertex &v) {
        vertS.vertices << QVector3D(v.pos[0], v.pos[1], v.pos[2]);
        vertS.color << QVector3D(0.0, 0.0, 0.0);
        //qDebug() << v.pos[0] << " " <<  v.pos[1] << " "  << v.pos[2];
    });

    return vertS;
}
vertex VerticeMaker::verticesSphere(float radius, unsigned int rings, unsigned int sectors)
{
    vertex lesverts;
    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);
    uint r, s;

    lesverts.vertices.resize(rings * sectors * 3);
    lesverts.color.resize(rings * sectors * 3);
    /*normals.resize(rings * sectors * 3);
    texcoords.resize(rings * sectors * 2);*/
    QVector<QVector3D>::iterator v = lesverts.vertices.begin();
    QVector<QVector3D>::iterator c = lesverts.color.begin();
    /*std::vector<GLfloat>::iterator n = normals.begin();
    std::vector<GLfloat>::iterator t = texcoords.begin();*/
    for(r = 0; r < rings; r++)
        for(s = 0; s < sectors; s++) {
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            /**t++ = s*S;
            *t++ = r*R;*/

            *v++ = QVector3D(x*radius, y*radius, z*radius);
            *c++ = QVector3D(0.0,0.0,0.0);
            /*
            *v++ = x * radius;
            *v++ = y * radius;
            *v++ = z * radius;*/

            /**n++ = x;
            *n++ = y;
            *n++ = z;*/
    }

    /*indices.resize(rings * sectors * 4);
    std::vector<GLushort>::iterator i = indices.begin();
    for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
            *i++ = r * sectors + s;
            *i++ = r * sectors + (s+1);
            *i++ = (r+1) * sectors + (s+1);
            *i++ = (r+1) * sectors + s;
    }*/

    return lesverts;

}
