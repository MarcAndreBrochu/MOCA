#include "verticemaker.h"
#include <iterator>

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
//    normals.resize(rings * sectors * 3);
//    texcoords.resize(rings * sectors * 2);
    QVector<QVector3D>::iterator v = lesverts.vertices.begin();
    QVector<QVector3D>::iterator c = lesverts.color.begin();
//    std::vector<GLfloat>::iterator n = normals.begin();
//    std::vector<GLfloat>::iterator t = texcoords.begin();
    for(r = 0; r < rings; r++)
        for(s = 0; s < sectors; s++) {

            float const y = sin( -MO_PI_2 + MO_PI * r * R );
            float const x = cos(2*MO_PI * s * S) * sin( MO_PI * r * R );
            float const z = sin(2*MO_PI * s * S) * sin( MO_PI * r * R );


            qDebug() << x << "/" << y << "/" << z;

            *v++ = QVector3D(x*radius, y*radius, z*radius);
            *c++ = QVector3D(0.0,0.0,0.0);



//            t++ = s*S;
//            *t++ = r*R;

//            *v++ = x * radius;
//            *v++ = y * radius;
//            *v++ = z * radius;

//            n++ = x;
//            *n++ = y;
//            *n++ = z;
    }

//    indices.resize(rings * sectors * 4);
//    std::vector<GLushort>::iterator i = indices.begin();
//    for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
//            *i++ = r * sectors + s;
//            *i++ = r * sectors + (s+1);
//            *i++ = (r+1) * sectors + (s+1);
//            *i++ = (r+1) * sectors + s;
//    }

    return lesverts;
}

vertex VerticeMaker::verticesSphere(double a, int depth)
{
    vertex levert;
    QVector<QVector3D> vertsIcosahedron;

    double c = a/2;
    c = c;
    const double X = .525731112119133606;
    const double Z = .850650808352039932;
    double vdata[12][3] = {
       {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
       {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
       {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
    };

    uint tindices[20][3] = {
        {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
        {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
        {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
        {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

    for(int i=19; i >= 0; i--){
        QVector3D v1 = QVector3D(vdata[tindices[i][2]][0], vdata[tindices[i][2]][1], vdata[tindices[i][2]][2]);
        QVector3D v2 = QVector3D(vdata[tindices[i][1]][0], vdata[tindices[i][1]][1], vdata[tindices[i][1]][2]);
        QVector3D v3 = QVector3D(vdata[tindices[i][0]][0], vdata[tindices[i][0]][1], vdata[tindices[i][0]][2]);

        levert.vertices << VerticeMaker::subdivideTriangle(v1, v2, v3, depth);

    }

    for(int i = 0; i<levert.vertices.size(); i++){
        if(i % 2 == 0)
            levert.color << QVector3D(1.0, 0.0, 0.0);
        else
            levert.color << QVector3D(0.0, 0.0, 1.0);
    }

        /*for(int j=2; j>=0; j--){
            levert.vertices << QVector3D(vdata[tindices[i][j]][0],vdata[tindices[i][j]][1], vdata[tindices[i][j]][2]);
            if(i < 5)
                levert.color << QVector3D(1.0, 1.0, 0.0);
            else if(5 <= i && i < 10)
                levert.color << QVector3D(0.0, 1.0, 1.0);
            else if(10 <= i && i < 15)
                levert.color << QVector3D(1.0, 0.0, 1.0);
            else
                levert.color << QVector3D(1.0, 0.0, 0.0);
        }*/


    /*vertsOctahedron << QVector3D(c, 0, c) << QVector3D(0, c, 0) << QVector3D(-c, 0, c)
                    << QVector3D(-c, 0, c) << QVector3D(0, c, 0) << QVector3D(-c, 0, -c)
                    << QVector3D(-c, 0, -c) << QVector3D(0, c, 0) << QVector3D(c, 0, -c)
                    << QVector3D(c, 0 , -c) << QVector3D(0, c, 0) << QVector3D(c, 0, c)
                    << QVector3D(c, 0, c) << QVector3D(0, -c, 0) << QVector3D(-c, 0, c)
                    << QVector3D(-c, 0, c) << QVector3D(0, -c, 0) << QVector3D(-c, 0, -c)
                    << QVector3D(-c, 0, -c) << QVector3D(0, -c, 0) << QVector3D(c, 0, -c)
                    << QVector3D(c, 0 , -c) << QVector3D(0, -c, 0) << QVector3D(c, 0, c);*/

    return levert;
}
QVector<QVector3D> VerticeMaker::subdivideTriangle(QVector3D v1, QVector3D v2, QVector3D v3, int depth){
    QVector<QVector3D> vLst;

    if(depth == 0){
            vLst << v1 << v2 << v3;
            return vLst;
    }


    QVector3D v12 = v1 + v2;
    QVector3D v23 = v2 + v3;
    QVector3D v31 = v3 + v1;
    v12.normalize();v23.normalize();v31.normalize();

    vLst << subdivideTriangle(v1, v12, v31, depth-1);
    vLst << subdivideTriangle(v2, v23, v12, depth-1);
    vLst << subdivideTriangle(v3, v31, v23, depth-1);
    vLst << subdivideTriangle(v12, v23, v31, depth-1);

    return vLst;
}
