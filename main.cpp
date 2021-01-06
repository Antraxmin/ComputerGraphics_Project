#include <iostream>
#include <fstream>
#include <gl/glut.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <cmath>
#include "OBJ_Loader.h"
#include "bmpfuncs.h"
#include "graphics.h"
using namespace std;

static inline void invert4x4(const GLfloat* src, GLfloat* dst)
{
    float det;

    dst[0] =
        +src[5] * src[10] * src[15]
        - src[5] * src[11] * src[14]
        - src[9] * src[6] * src[15]
        + src[9] * src[7] * src[14]
        + src[13] * src[6] * src[11]
        - src[13] * src[7] * src[10];

    dst[1] =
        -src[1] * src[10] * src[15]
        + src[1] * src[11] * src[14]
        + src[9] * src[2] * src[15]
        - src[9] * src[3] * src[14]
        - src[13] * src[2] * src[11]
        + src[13] * src[3] * src[10];

    dst[2] =
        +src[1] * src[6] * src[15]
        - src[1] * src[7] * src[14]
        - src[5] * src[2] * src[15]
        + src[5] * src[3] * src[14]
        + src[13] * src[2] * src[7]
        - src[13] * src[3] * src[6];

    dst[3] =
        -src[1] * src[6] * src[11]
        + src[1] * src[7] * src[10]
        + src[5] * src[2] * src[11]
        - src[5] * src[3] * src[10]
        - src[9] * src[2] * src[7]
        + src[9] * src[3] * src[6];

    dst[4] =
        -src[4] * src[10] * src[15]
        + src[4] * src[11] * src[14]
        + src[8] * src[6] * src[15]
        - src[8] * src[7] * src[14]
        - src[12] * src[6] * src[11]
        + src[12] * src[7] * src[10];

    dst[5] =
        +src[0] * src[10] * src[15]
        - src[0] * src[11] * src[14]
        - src[8] * src[2] * src[15]
        + src[8] * src[3] * src[14]
        + src[12] * src[2] * src[11]
        - src[12] * src[3] * src[10];

    dst[6] =
        -src[0] * src[6] * src[15]
        + src[0] * src[7] * src[14]
        + src[4] * src[2] * src[15]
        - src[4] * src[3] * src[14]
        - src[12] * src[2] * src[7]
        + src[12] * src[3] * src[6];

    dst[7] =
        +src[0] * src[6] * src[11]
        - src[0] * src[7] * src[10]
        - src[4] * src[2] * src[11]
        + src[4] * src[3] * src[10]
        + src[8] * src[2] * src[7]
        - src[8] * src[3] * src[6];

    dst[8] =
        +src[4] * src[9] * src[15]
        - src[4] * src[11] * src[13]
        - src[8] * src[5] * src[15]
        + src[8] * src[7] * src[13]
        + src[12] * src[5] * src[11]
        - src[12] * src[7] * src[9];

    dst[9] =
        -src[0] * src[9] * src[15]
        + src[0] * src[11] * src[13]
        + src[8] * src[1] * src[15]
        - src[8] * src[3] * src[13]
        - src[12] * src[1] * src[11]
        + src[12] * src[3] * src[9];

    dst[10] =
        +src[0] * src[5] * src[15]
        - src[0] * src[7] * src[13]
        - src[4] * src[1] * src[15]
        + src[4] * src[3] * src[13]
        + src[12] * src[1] * src[7]
        - src[12] * src[3] * src[5];

    dst[11] =
        -src[0] * src[5] * src[11]
        + src[0] * src[7] * src[9]
        + src[4] * src[1] * src[11]
        - src[4] * src[3] * src[9]
        - src[8] * src[1] * src[7]
        + src[8] * src[3] * src[5];

    dst[12] =
        -src[4] * src[9] * src[14]
        + src[4] * src[10] * src[13]
        + src[8] * src[5] * src[14]
        - src[8] * src[6] * src[13]
        - src[12] * src[5] * src[10]
        + src[12] * src[6] * src[9];

    dst[13] =
        +src[0] * src[9] * src[14]
        - src[0] * src[10] * src[13]
        - src[8] * src[1] * src[14]
        + src[8] * src[2] * src[13]
        + src[12] * src[1] * src[10]
        - src[12] * src[2] * src[9];

    dst[14] =
        -src[0] * src[5] * src[14]
        + src[0] * src[6] * src[13]
        + src[4] * src[1] * src[14]
        - src[4] * src[2] * src[13]
        - src[12] * src[1] * src[6]
        + src[12] * src[2] * src[5];

    dst[15] =
        +src[0] * src[5] * src[10]
        - src[0] * src[6] * src[9]
        - src[4] * src[1] * src[10]
        + src[4] * src[2] * src[9]
        + src[8] * src[1] * src[6]
        - src[8] * src[2] * src[5];

    /* Compute determinant: */

    det = +src[0] * dst[0] + src[1] * dst[4] + src[2] * dst[8] + src[3] * dst[12];

    /* Multiply adjoint with reciprocal of determinant: */

    det = 1.0f / det;

    dst[0] *= det;
    dst[1] *= det;
    dst[2] *= det;
    dst[3] *= det;
    dst[4] *= det;
    dst[5] *= det;
    dst[6] *= det;
    dst[7] *= det;
    dst[8] *= det;
    dst[9] *= det;
    dst[10] *= det;
    dst[11] *= det;
    dst[12] *= det;
    dst[13] *= det;
    dst[14] *= det;
    dst[15] *= det;
}

// bmp loader
uchar* readImageData(const char* filename, int* width, int* height, int* channels)
{
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    int dataPos;     // Position in the file where the actual data begins
    int imageSize;   // = width*height*3 or width*height
    // Actual RGB data
    uchar* imageData;

    // Open the file
    FILE* file = NULL;
    fopen_s(&file, filename, "rb");
    if (!file) { printf("bmpfunc.cpp : Image could not be opened\n"); return 0; }

    if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
        printf("bmpfunc.cpp : Not a correct BMP file\n");
        return 0;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        printf("bmpfunc.cpp : Not a correct BMP file\n");
        return 0;
    }

    // 바이트 배열에서 int 변수를 읽습니다. 
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    *width = *(int*)&(header[0x12]);
    *height = *(int*)&(header[0x16]);

    if (imageSize == 0)
    {
        *channels = 3;
        imageSize = (*width) * (*height) * 3; // Assume 3 channels
    }
    else {
        if (imageSize == (*width) * (*height)) {
            *channels = 1;
        }
        else if (imageSize == (*width) * (*height) * 3) {
            *channels = 3;
        }
        else { // Assume 3 channels
            *channels = 3;
        }
    }
    if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

    imageData = (uchar*)malloc(imageSize);

    fread(imageData, 1, imageSize, file);
    fclose(file);

    // flip R & B // BGR order -> RGB order
    if (*channels == 3) {
        for (int i = 0; i < (*height); i++)
        {
            for (int j = 0; j < (*width); j++)
            {
                uchar tmp = imageData[3 * (i * (*width) + j)]; // R
                imageData[3 * (i * (*width) + j)] = imageData[3 * (i * (*width) + j) + 2];
                imageData[3 * (i * (*width) + j) + 2] = tmp;
            }
        }
    }
    return imageData;
}

void droneTexture() {
    uchar* data;
    int w, h, c;
    glGenTextures(3, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    data = readImageData("texture/Flying-drone_col.bmp", &w, &h, &c);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    data = readImageData("texture/Flying-drone_col2.bmp", &w, &h, &c);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texture[2]);
    data = readImageData("texture/Flying-drone2.bmp", &w, &h, &c);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void lightTexture() {
    uchar* data;
    int w, h, c;
    glGenTextures(1, flash);
    glBindTexture(GL_TEXTURE_2D, flash[0]);
    data = readImageData("texture/white.bmp", &w, &h, &c);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void skyTexture(const char* fname) {
    uchar* data;
    int w, h, c;
    glGenTextures(1, skyTex);
    glBindTexture(GL_TEXTURE_2D, skyTex[0]);
    data = readImageData(fname, &w, &h, &c);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void groundTexture() {
    uchar* data;
    int w, h, c;
    glGenTextures(1, ground);

    glBindTexture(GL_TEXTURE_2D, ground[0]);
    data = readImageData("texture/ngtground.bmp", &w, &h, &c);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void buildingTexture() {
    uchar* data;
    int w, h, c;
    glGenTextures(2, buildTex);
    glBindTexture(GL_TEXTURE_2D, buildTex[0]);
    data = readImageData("texture/Building_side.bmp", &w, &h, &c);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, buildTex[1]);
    data = readImageData("texture/roof.bmp", &w, &h, &c);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Draw_city(int listIndex) {
    glNewList(listIndex, GL_COMPILE);
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    string m2 = "Mat.2";
    string m_2 = "Mat_2";
    glPushAttrib(GL_LIGHTING_BIT);
    for (int i = 0; i < CityLoader.LoadedMeshes.size(); i++) {
        objl::Mesh curMesh = CityLoader.LoadedMeshes[i];
        if (curMesh.MeshMaterial.name == m2 || curMesh.MeshMaterial.name == m_2) {
            glBindTexture(GL_TEXTURE_2D, buildTex[1]);
        }
        else glBindTexture(GL_TEXTURE_2D, buildTex[0]);
        for (int j = 0; j < curMesh.Indices.size(); j += 3) {
            glBegin(GL_TRIANGLES);
            glTexCoord2f(curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.Y);
            glNormal3f(curMesh.Vertices[curMesh.Indices[j]].Normal.X, curMesh.Vertices[curMesh.Indices[j]].Normal.Y, curMesh.Vertices[curMesh.Indices[j]].Normal.Z);
            glVertex3f(curMesh.Vertices[curMesh.Indices[j]].Position.X, curMesh.Vertices[curMesh.Indices[j]].Position.Y, curMesh.Vertices[curMesh.Indices[j]].Position.Z);
            glTexCoord2f(curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.Y);
            glNormal3f(curMesh.Vertices[curMesh.Indices[j + 1]].Normal.X, curMesh.Vertices[curMesh.Indices[j + 1]].Normal.Y, curMesh.Vertices[curMesh.Indices[j + 1]].Normal.Z);
            glVertex3f(curMesh.Vertices[curMesh.Indices[j + 1]].Position.X, curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y, curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
            glTexCoord2f(curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.Y);
            glNormal3f(curMesh.Vertices[curMesh.Indices[j + 2]].Normal.X, curMesh.Vertices[curMesh.Indices[j + 2]].Normal.Y, curMesh.Vertices[curMesh.Indices[j + 2]].Normal.Z);
            glVertex3f(curMesh.Vertices[curMesh.Indices[j + 2]].Position.X, curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y, curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);
            glEnd();
        }
    }
    glPopAttrib();
    glPopMatrix();
    glEndList();
}

void drawHelicopter(int i) {
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glScalef(10, 10, 10);
    objl::Mesh curMesh = Loader.LoadedMeshes[0];
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    for (int j = 0; j < curMesh.Indices.size(); j += 3) {
        glBegin(GL_TRIANGLES);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j]].Normal.X, curMesh.Vertices[curMesh.Indices[j]].Normal.Y, curMesh.Vertices[curMesh.Indices[j]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j]].Position.X, curMesh.Vertices[curMesh.Indices[j]].Position.Y, curMesh.Vertices[curMesh.Indices[j]].Position.Z);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j + 1]].Normal.X, curMesh.Vertices[curMesh.Indices[j + 1]].Normal.Y, curMesh.Vertices[curMesh.Indices[j + 1]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j + 1]].Position.X, curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y, curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j + 2]].Normal.X, curMesh.Vertices[curMesh.Indices[j + 2]].Normal.Y, curMesh.Vertices[curMesh.Indices[j + 2]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j + 2]].Position.X, curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y, curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);
        glEnd();
    }
    glPopMatrix();
}

void drawRotorL2(int i) { //프로펠러
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glScalef(10, 10, 10);
    objl::Mesh curMesh = Loader.LoadedMeshes[1];
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    for (int j = 0; j < curMesh.Indices.size(); j += 3) {
        glBegin(GL_TRIANGLES);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j]].Normal.X, curMesh.Vertices[curMesh.Indices[j]].Normal.Y, curMesh.Vertices[curMesh.Indices[j]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j]].Position.X, curMesh.Vertices[curMesh.Indices[j]].Position.Y, curMesh.Vertices[curMesh.Indices[j]].Position.Z);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j + 1]].Normal.X, curMesh.Vertices[curMesh.Indices[j + 1]].Normal.Y, curMesh.Vertices[curMesh.Indices[j + 1]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j + 1]].Position.X, curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y, curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j + 2]].Normal.X, curMesh.Vertices[curMesh.Indices[j + 2]].Normal.Y, curMesh.Vertices[curMesh.Indices[j + 2]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j + 2]].Position.X, curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y, curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);
        glEnd();
    }
    glPopMatrix();
}

void drawRotorL(int i) {
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    if (power == true) {
        glTranslatef(2.4855f, -0.00811f, -0.05415f);
        glRotatef(propeller, 0, 1, 0);
        glTranslatef(-2.4855f, 0.00811f, 0.05415f);
    }
    glScalef(10, 10, 10);
    objl::Mesh curMesh = Loader.LoadedMeshes[2];
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    for (int j = 0; j < curMesh.Indices.size(); j += 3) {
        glBegin(GL_TRIANGLES);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j]].Normal.X, curMesh.Vertices[curMesh.Indices[j]].Normal.Y, curMesh.Vertices[curMesh.Indices[j]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j]].Position.X, curMesh.Vertices[curMesh.Indices[j]].Position.Y, curMesh.Vertices[curMesh.Indices[j]].Position.Z);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j + 1]].Normal.X, curMesh.Vertices[curMesh.Indices[j + 1]].Normal.Y, curMesh.Vertices[curMesh.Indices[j + 1]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j + 1]].Position.X, curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y, curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j + 2]].Normal.X, curMesh.Vertices[curMesh.Indices[j + 2]].Normal.Y, curMesh.Vertices[curMesh.Indices[j + 2]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j + 2]].Position.X, curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y, curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);
        glEnd();
    }
    glPopMatrix();
}

void drawRotorR2(int i) {
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glScalef(10, 10, 10);
    objl::Mesh curMesh = Loader.LoadedMeshes[3];
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    for (int j = 0; j < curMesh.Indices.size(); j += 3) {
        glBegin(GL_TRIANGLES);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j]].Normal.X, curMesh.Vertices[curMesh.Indices[j]].Normal.Y, curMesh.Vertices[curMesh.Indices[j]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j]].Position.X, curMesh.Vertices[curMesh.Indices[j]].Position.Y, curMesh.Vertices[curMesh.Indices[j]].Position.Z);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j + 1]].Normal.X, curMesh.Vertices[curMesh.Indices[j + 1]].Normal.Y, curMesh.Vertices[curMesh.Indices[j + 1]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j + 1]].Position.X, curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y, curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j + 2]].Normal.X, curMesh.Vertices[curMesh.Indices[j + 2]].Normal.Y, curMesh.Vertices[curMesh.Indices[j + 2]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j + 2]].Position.X, curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y, curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);
        glEnd();
    }
    glPopMatrix();
}

void drawRotorR(int i) { //프로펠러
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    if (power == true) {
        glTranslatef(-2.4855f, -0.00132f, -0.04796f);
        glRotatef(propeller, 0, 1, 0);
        glTranslatef(2.4855f, 0.00132f, 0.04796f);
    }
    glScalef(10, 10, 10);
    objl::Mesh curMesh = Loader.LoadedMeshes[4];
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    for (int j = 0; j < curMesh.Indices.size(); j += 3) {
        glBegin(GL_TRIANGLES);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j]].Normal.X, curMesh.Vertices[curMesh.Indices[j]].Normal.Y, curMesh.Vertices[curMesh.Indices[j]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j]].Position.X, curMesh.Vertices[curMesh.Indices[j]].Position.Y, curMesh.Vertices[curMesh.Indices[j]].Position.Z);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j + 1]].Normal.X, curMesh.Vertices[curMesh.Indices[j + 1]].Normal.Y, curMesh.Vertices[curMesh.Indices[j + 1]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j + 1]].Position.X, curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y, curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
        glTexCoord2f(curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.Y);
        glNormal3f(curMesh.Vertices[curMesh.Indices[j + 2]].Normal.X, curMesh.Vertices[curMesh.Indices[j + 2]].Normal.Y, curMesh.Vertices[curMesh.Indices[j + 2]].Normal.Z);
        glVertex3f(curMesh.Vertices[curMesh.Indices[j + 2]].Position.X, curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y, curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);
        glEnd();
    }
    glPopMatrix();
}

void print_key() {
    printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ조작법ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
    printf("mouse scroll : zoom in, zoom out\n");
    printf("방향키 : 카메라 위치\n");
    printf("1, 2, 3, 4, 5 : 드론 조립\n");
    printf("b : 1인칭 / 3인칭 시점\n");
    printf("p : Power on/off\n");
    printf("c : Speed up\n");
    printf("v : Speed down\n");
    printf("r : Altitude up\n");
    printf("t : Altitude down\n");
    printf("w, s, a, d : Drone control\n");
    printf("q, e : Drone angle control\n");
    printf("l : Light on/off\n");
    printf("z : Drone clocking mode on/off\n");
    printf("b : Drone view 1D/3D\n");
    printf("F1 : Light default\n");
    printf("F2 : Camera default\n");
    printf("F3 : Position default\n");
    printf("F4 : Speed default\n");
    printf("F5 : Setting default\n");
    printf("F10 : 조작법 출력\n");
}

void Skybox_texture() {

    int w, h, c;
    glGenTextures(1, cubeTex);
    uchar* img0 = readImageData("texture/left.bmp", &w, &h, &c);
    uchar* img1 = readImageData("texture/right.bmp", &w, &h, &c);
    uchar* img2 = readImageData("texture/top.bmp", &w, &h, &c);
    uchar* img3 = readImageData("texture/bottom.bmp", &w, &h, &c);
    uchar* img4 = readImageData("texture/front.bmp", &w, &h, &c);
    uchar* img5 = readImageData("texture/back.bmp", &w, &h, &c);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex[0]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img1);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img2);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img3);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img4);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img5);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
}


void skyMapping() {
    glPushMatrix();
    glTranslatef(0, 147, 0);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex[0]);
    glBegin(GL_QUADS);
    //px
    glTexCoord3d(1, -1, -1);
    glVertex3f(skySize, -skySize, -skySize);
    glTexCoord3d(1, -1, 1);
    glVertex3f(skySize, -skySize, skySize);
    glTexCoord3d(1, 1, 1);
    glVertex3f(skySize, skySize, skySize);
    glTexCoord3d(1, 1, -1);
    glVertex3f(skySize, skySize, -skySize);
    //nx
    glTexCoord3d(-1, -1, -1);
    glVertex3f(-skySize, -skySize, -skySize);
    glTexCoord3d(-1, -1, 1);
    glVertex3f(-skySize, -skySize, skySize);
    glTexCoord3d(-1, 1, 1);
    glVertex3f(-skySize, skySize, skySize);
    glTexCoord3d(-1, 1, -1);
    glVertex3f(-skySize, skySize, -skySize);
    //py
    glTexCoord3d(1, 1, 1);
    glVertex3f(skySize, skySize, skySize);
    glTexCoord3d(-1, 1, 1);
    glVertex3f(-skySize, skySize, skySize);
    glTexCoord3d(-1, 1, -1);
    glVertex3f(-skySize, skySize, -skySize);
    glTexCoord3d(1, 1, -1);
    glVertex3f(skySize, skySize, -skySize);
    //ny
    glTexCoord3d(1, -1, 1);
    glVertex3f(skySize, -skySize, skySize);
    glTexCoord3d(-1, -1, 1);
    glVertex3f(-skySize, -skySize, skySize);
    glTexCoord3d(-1, -1, -1);
    glVertex3f(-skySize, -skySize, -skySize);
    glTexCoord3d(1, -1, -1);
    glVertex3f(skySize, -skySize, -skySize);
    //pz
    glTexCoord3d(1, -1, 1);
    glVertex3f(skySize, -skySize, skySize);
    glTexCoord3d(-1, -1, 1);
    glVertex3f(-skySize, -skySize, skySize);
    glTexCoord3d(-1, 1, 1);
    glVertex3f(-skySize, skySize, skySize);
    glTexCoord3d(1, 1, 1);
    glVertex3f(skySize, skySize, skySize);
    //nz
    glTexCoord3d(1, -1, -1);
    glVertex3f(skySize, -skySize, -skySize);
    glTexCoord3d(-1, -1, -1);
    glVertex3f(-skySize, -skySize, -skySize);
    glTexCoord3d(-1, 1, -1);
    glVertex3f(-skySize, skySize, -skySize);
    glTexCoord3d(1, 1, -1);
    glVertex3f(skySize, skySize, -skySize);
    glEnd();
    glPopMatrix();
}


void draw_ground(int ListIndex) {
    glNewList(ListIndex, GL_COMPILE);
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glScalef(400, 400, 400);
    glBindTexture(GL_TEXTURE_2D, ground[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glNormal3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glTexCoord2f(1, 0);
    glNormal3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glTexCoord2f(1, 1);
    glNormal3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glTexCoord2f(0, 1);
    glNormal3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glEnd();
    glPopMatrix();
    glEndList();
}

void Disk() {
    glPushMatrix();
    disk = gluNewQuadric();
    gluQuadricDrawStyle(disk, GLU_FILL);
    glTranslatef(-50, 0, 20);
    glRotatef(90, 1, 0, 0);
    glColor3f(1, 1, 1);
    glScalef(5, 5, 5);
    gluQuadricNormals(disk, GLU_SMOOTH);
    gluDisk(disk, 0, 5, 30, 30);
    glPopMatrix();
}

void draw_sky(int ListIndex)
{
    int lats = 500;
    int longs = 500;
    glNewList(ListIndex, GL_COMPILE);
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glRotatef(90, 1, 0, 0);
    glScalef(400, 400, 400);
    int i, j;
    int halfLats = lats / 2;
    glBindTexture(GL_TEXTURE_2D, skyTex[0]);
    for (i = 0; i <= halfLats; i++)
    {
        double lat0 = PI * (-0.5 + (double)(i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = PI * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs; j++)
        {
            double lng = 2 * PI * (double)(j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            double s1, s2, t;
            s1 = ((double)i) / halfLats;
            s2 = ((double)i + 1) / halfLats;
            t = ((double)j) / longs;

            glTexCoord2d(s1, t);
            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(x * zr0, y * zr0, z0);

            glTexCoord2d(s2, t);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1, y * zr1, z1);
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();
}

void Draw_string(void* font, const char* str, float x, float y, float red, float green, float blue) {
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-5, 5, -5, 5); //화면의 좌표는 (-5,-5)~(5,5)
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(red, green, blue);
    glRasterPos3i(x, y, 0);
    for (int i = 0; i < strlen(str); i++) {
        glutBitmapCharacter(font, str[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopAttrib();
}

void Draw_texture() {
    if (pick == false) {
        glPushAttrib(GL_LIGHTING_BIT);
        glDisable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f, g_nGLHeight / 2.0f, -100, 100);
        glMatrixMode(GL_MODELVIEW);
    }
    glLoadName(1);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1, 1, 1);
    glTranslatef(g_pPosition[0], g_pPosition[1], 0);
    glRotatef(texAngle, 0, 1, 0);
    sphere[0] = gluNewQuadric();
    gluQuadricDrawStyle(sphere[0], GLU_FILL);
    gluQuadricTexture(sphere[0], GL_TRUE);
    gluQuadricNormals(sphere[0], GLU_SMOOTH);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    gluSphere(sphere[0], 30, 30, 30);
    glPopMatrix();

    glLoadName(2);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1, 1, 1);
    glTranslatef(g_pPosition[2], g_pPosition[3], 0);
    glRotatef(texAngle, 0, 1, 0);
    sphere[1] = gluNewQuadric();
    gluQuadricDrawStyle(sphere[1], GLU_FILL);
    gluQuadricTexture(sphere[1], GL_TRUE);
    gluQuadricNormals(sphere[1], GLU_SMOOTH);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    gluSphere(sphere[1], 30, 30, 30);
    glPopMatrix();

    glLoadName(3);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1, 1, 1);
    glTranslatef(g_pPosition[4], g_pPosition[5], 0);
    glRotatef(texAngle, 0, 1, 0);
    sphere[2] = gluNewQuadric();
    gluQuadricDrawStyle(sphere[2], GLU_FILL);
    gluQuadricTexture(sphere[2], GL_TRUE);
    gluQuadricNormals(sphere[2], GLU_SMOOTH);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    gluSphere(sphere[2], 30, 30, 30);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopAttrib();
}

void Picking(int x, int y) {
    pick = true;
    GLuint selectBuf[256];
    glSelectBuffer(sizeof(selectBuf), selectBuf);
    //GL_SELECT 사용해 선택 모드 설정
    glRenderMode(GL_SELECT);
    //선택의 처리는 시점 좌표계에서 실시하므로 투영 행렬 설정
    glMatrixMode(GL_PROJECTION);
    //개체 이름을 등록해두면 이름 스택을 초기화
    //반드시 glRenderMode(GL_SELECT)를 호출 후 실행해야한다.
    glInitNames();
    //glInitNames()직후 이름 스택의 선두에 임시로 -1을 넣어둔다.
    glPushName(-1);
    glPushMatrix();
    glLoadIdentity();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluPickMatrix(x, y, 3, 3, viewport);

    glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f, g_nGLHeight / 2.0f, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    Draw_texture();
    pick = false;
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glFlush();

    GLint hits = glRenderMode(GL_RENDER);
    if (hits <= 0)return;
    int stack = selectBuf[0]; //object name id
    unsigned int zMin = selectBuf[1]; //zmin
    unsigned int zMax = selectBuf[2]; //zmax
    g_nSelect = selectBuf[3]; //선택된 object의 id
    int index = 3 + stack;
    int i;

    for (i = 1; i < hits; i++) {
        stack = selectBuf[index];
        if (zMin > selectBuf[index + 1]) {
            zMin = selectBuf[index + 1];
            g_nSelect = selectBuf[index + 3];
        }
        index += 3 + stack;
    }
    if (g_nSelect == 1) {
        printf("Picking black\n");
    }
    if (g_nSelect == 2) {
        printf("Picking purple\n");
    }
    if (g_nSelect == 3) {
        printf("Picking white\n");
    }
}

void resize(int width, int height) {
    g_nGLWidth = width;
    g_nGLHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)width / (float)height, 1, 500);
}

void dtos(double num, char val[]) {
    num *= 2;
    if (num <= 0)num = 0;
    snprintf(val, 4, "%f", num);
}

void draw_text(const char* str1, const char* str2) {
    Draw_string(GLUT_BITMAP_TIMES_ROMAN_24, str1, -5, 4, 1, 0, 0);
    Draw_string(GLUT_BITMAP_TIMES_ROMAN_24, h, -3.5, 4, 1, 0, 0);
    Draw_string(GLUT_BITMAP_TIMES_ROMAN_24, str2, -5, 3.5, 1, 0, 0);
    Draw_string(GLUT_BITMAP_TIMES_ROMAN_24, v, -3.5, 3.5, 1, 0, 0);
}

double getRadian(int num) {
    return num * PI / 180;
}

void CamPosition() {
    double radianTheta = getRadian(theta);
    double radianPhi = getRadian(phi);
    cam[0] = r * sin(radianTheta) * sin(radianPhi);
    cam[1] = r * cos(radianTheta);
    cam[2] = r * sin(radianTheta) * cos(radianPhi);
}

void LightSetting() {
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat specularMaterial[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat diffuseMaterial[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat ambientMaterial[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 5.0);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 5.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

    glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
    glMateriali(GL_FRONT, GL_SHININESS, 64);


    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    //glColorMaterial(GL_FRONT, GL_EMISSION);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
}

void EmissionClear() {
    mat_emission[0] = 0.0f;
    mat_emission[1] = 0.0f;
    mat_emission[2] = 0.0f;
    mat_emission[3] = 0.0f;
}

void EmissionSet() {
    mat_emission[0] = 1.0f;
    mat_emission[1] = 1.0f;
    mat_emission[2] = 1.0f;
    mat_emission[3] = 1.0f;
}

void DroneLight() {
    lightSphere = gluNewQuadric();
    gluQuadricDrawStyle(lightSphere, GLU_FILL);
    glColor3f(1, 1, 0);
    glScalef(0.08f, 0.08f, 0.08f);
    gluQuadricTexture(lightSphere, GL_TRUE);
    gluQuadricNormals(lightSphere, GLU_SMOOTH);
    glBindTexture(GL_TEXTURE_2D, *flash);
    gluSphere(lightSphere, 1, 30, 30);
}

void Draw_total() {
    GLfloat lightPosition[] = { 0, 0, 0, 1.0f };
    GLfloat spotlightPos[] = { 0,0,1,1 };
    glPushMatrix();
    glTranslatef(xlocation, 1.0 + altitude, zlocation);
    glRotated(droneAngle, 0, 1, 0);
    if (power == true && motorSpeed > 0) {
        if (state == 0) glRotatef(frontAngle, 1, 0, 0);
        else if (state == 1) glRotatef(backAngle, 1, 0, 0);
        else if (state == 2) {
            glRotatef(csideAngle, 0, 0, 1);
        }
        else if (state == 3) {
            glRotatef(sideAngle, 0, 0, 1);
        }
        else {
            if (frontAngle > 0) {
                glRotatef(frontAngle, 1, 0, 0);
            }
            else if (backAngle < 0) {
                glRotatef(backAngle, 1, 0, 0);
            }
            else if (csideAngle < 0) {
                glRotatef(csideAngle, 0, 0, 1);
            }
            else if (sideAngle > 0) {
                glRotatef(sideAngle, 0, 0, 1);
            }
        }
    }

    if (draw1 == true) drawHelicopter(dtexNum);
    if (draw2 == true) drawRotorL(dtexNum);
    if (draw3 == true) drawRotorL2(dtexNum); //초기화면 오른쪽
    if (draw4 == true) drawRotorR(dtexNum); //초기화면 왼쪽
    if (draw5 == true) drawRotorR2(dtexNum);

    //드론 라이트
    if (lightMode == true) {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glPushMatrix();
        glTranslatef(0.5346f, -0.01287f, 0.4585f);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotlightPos);
        EmissionSet();
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
        DroneLight();
        EmissionClear();
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.5346f, -0.01287f, 0.4585f);
        glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlightPos);
        EmissionSet();
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
        DroneLight();
        EmissionClear();
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
        glPopMatrix();
    }
    else {
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
    }
    glPopMatrix();
}

void draw_oasis() {
    float x, z, d1, d2, d3, d4, y1, y2, y3, y4;
    float step = 0.2;
    float freq = 5.0;
    glColor3f(0.1, 0.5, 1.0);
    for (x = -10; x < 10; x += step) {
        for (z = -10; z < 10; z += step) {
            d1 = sqrt(x * x + z * z);
            d2 = sqrt((x + step) * (x + step) + z * z);
            d3 = sqrt((x + step) * (x + step) + (z + step) * (z + step));
            d4 = sqrt(x * x + (z + step) * (z + step));

            if (d1 == 0)y1 = peak;
            else y1 = peak * sin(freq * d1) / (freq * d1);

            if (d2 == 0)y2 = peak;
            else y2 = peak * sin(freq * d2) / (freq * d2);

            if (d3 == 0)y3 = peak;
            else y3 = peak * sin(freq * d3) / (freq * d3);

            if (d4 == 0)y4 = peak;
            else y4 = peak * sin(freq * d4) / (freq * d4);

            glBegin(GL_TRIANGLES);
            glNormal3f(step, step, y3 - y1);
            glVertex3f(x, y1, z);
            glVertex3f(x + step, y2, z);
            glVertex3f(x + step, y3, z + step);

            glVertex3f(x + step, y3, z + step);
            glVertex3f(x, y4, z + step);
            glVertex3f(x, y1, z);
            glEnd();
        }
    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    //antialiasing 수준 결정
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
    bool loadout = Loader.LoadFile("obj/Drone_obj.obj");
    bool cityload = CityLoader.LoadFile("obj/City_Blocks.obj");
    print_key();
    groundTexture();
    buildingTexture();
    droneTexture();
    lightTexture();
    Skybox_texture();
    skyTexture("texture/ngtsky.bmp");
    listIndex = glGenLists(3);
    draw_sky(listIndex);
    Draw_city(listIndex + 1);
    draw_ground(listIndex + 2);
    LightSetting();
}

void draw() {
    //화면 지우기
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //이 함수가 호출되기 전에 resize()가 호출되므로 viewport TF와 projection TF는 이미 수행된 상태
    if (theta > 0 && theta <= 180)up[1] = 1;
    if (theta > 180 && theta <= 360 || theta == 0)up[1] = -1;
    CamPosition();
    if (droneView == 0) gluLookAt(cam[0] + xlocation, cam[1] + altitude, cam[2] + zlocation, center[0] + xlocation, center[1] + altitude, center[2] + zlocation, up[0], up[1], up[2]);
    else gluLookAt(droneEyex + xlocation, droneEyey + altitude, droneEyez + zlocation, droneEyex + xlocation + sin(getRadian(droneAngle)), droneEyey + altitude, droneEyez + zlocation + cos(getRadian(droneAngle)), 0, 1, 0);
    //텍스쳐 띄우기
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //Polygon의 원래 색상은 무시하고 texture로 덮음
    glEnable(GL_TEXTURE_2D);
    Draw_texture();
    //야간 모드
    if (ngt == true) {
        glDisable(GL_LIGHT2);
        glCallList(listIndex);
        glPushMatrix();
        glScalef(0.2f, 0.2f, 0.2f);
        glCallList(listIndex + 1);
        glPopMatrix();
        glCallList(listIndex + 2);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        Draw_total();
        glEnable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(-86, 156, 56);
        glutSolidSphere(6, 30, 30);
        glPopMatrix();
    }
    //낮 모드
    else {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        skyMapping();
        glEnable(GL_LIGHT2);
        GLfloat lightpos[] = { 0,200,0,1 };
        glLightfv(GL_LIGHT2, GL_POSITION, lightpos);
        if (clocking == true) {
            glMatrixMode(GL_TEXTURE);
            glPushMatrix();
            glGetFloatv(GL_MODELVIEW_MATRIX, m);
            invert4x4(m, invertm);
            glMultMatrixf(invertm);
            glMatrixMode(GL_MODELVIEW);
            Draw_total();
            glMatrixMode(GL_TEXTURE);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);
            glDisable(GL_TEXTURE_GEN_R);
            glDisable(GL_TEXTURE_CUBE_MAP);
        }
        else {
            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);
            glDisable(GL_TEXTURE_GEN_R);
            glDisable(GL_TEXTURE_CUBE_MAP);
            Draw_total();
        }
        glEnable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(0, 0, 100);
        glScalef(3, 1, 3);
        draw_oasis();
        glPopMatrix();
    }

    draw_text("Altitude : ", "Velocity : ");
    glFlush(); //gl함수들을 버퍼에 넣어주기 위한 함수
    glutSwapBuffers(); //Back buffer를 front buffer로 바꿈
}

void mouse(int button, int state, int x, int y) {
    if (state == 0) {
        if (button == 3) {
            if (r > 3) {
                r--;
                printf("Zoom in ! (radius %lf)\n", r);
            }
            else printf("Unavailable\n");
        }
        if (button == 4) {
            r++;
            printf("Zoom out ! (radius %lf)\n", r);
        }
    }
    y = g_nGLHeight - y;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        Picking(x, y);
        g_nX = x;
        g_nY = y;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if ((g_nX >= 190 && g_nX <= 510) && (g_nGLHeight - g_nY <= 350 && g_nGLHeight - g_nY >= 280)) {
            if (g_nSelect == 1) {
                dtexNum = 0;
            }
            if (g_nSelect == 2) {
                dtexNum = 1;
            }
            if (g_nSelect == 3) {
                dtexNum = 2;
            }
            g_pPosition[0] = 300.0f;
            g_pPosition[1] = 300.0f;
            g_pPosition[2] = 300.0f;
            g_pPosition[3] = 200.0f;
            g_pPosition[4] = 300.0f;
            g_pPosition[5] = 100.0f;
        }
        g_nSelect = 0;

    }
    glutPostRedisplay();
}

void motion(int x, int y) {
    y = g_nGLHeight - y;
    if (g_nSelect > 0) {
        g_pPosition[(g_nSelect - 1) * 2 + 0] += x - g_nX;
        g_pPosition[(g_nSelect - 1) * 2 + 1] += y - g_nY;
        g_nX = x;
        g_nY = y;
        glutPostRedisplay();
    }
}

void clear() {
    r = 20;
    theta = 65, phi = 180;
    altitude = 0;
    xlocation = 0;
    zlocation = 0;
    motorSpeed = 0;
    droneAngle = 0;
    h[0] = 0;
    h[1] = 0;
    h[2] = 0;
    lightMode = false;
    power = false;
    state = -1;
}

void special_keyboard(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        phi -= 5;
        if (phi < 0) {
            phi = 355;
        }
    }
    if (key == GLUT_KEY_RIGHT) {
        phi += 5;
        if (phi > 355) {
            phi = 0;
        }
    }
    if (key == GLUT_KEY_DOWN) {
        theta += 5;
        if (theta > 360) {
            theta = 5;
        }
    }
    if (key == GLUT_KEY_UP) {
        theta -= 5;
        if (theta < 5) {
            theta = 360;
        }
    }
    if (key == GLUT_KEY_F1) {
        lightMode = false;
    }
    if (key == GLUT_KEY_F2) {
        r = 20;
        theta = 65, phi = 180;
        droneView = false;
    }
    if (key == GLUT_KEY_F3) {
        altitude = 0;
        xlocation = 0;
        zlocation = 0;
    }
    if (key == GLUT_KEY_F4) {
        motorSpeed = 5;
    }
    if (key == GLUT_KEY_F5) {
        clear();
    }
    if (key == GLUT_KEY_F10) {
        print_key();
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == '1') draw1 = not(draw1);
    if (key == '3') draw2 = not(draw2);
    if (key == '2') draw3 = not(draw3);
    if (key == '5') draw4 = not(draw4);
    if (key == '4') draw5 = not(draw5);
    if (key == 'p') {
        power = not(power);
        if (power == true) {
            printf("Power On!\n");
        }
        else {
            printf("Power Off!\n");
            ddown = true;
        }
    }
    if (key == 'c') {
        if (power == false) {
            printf("Power on please\n");
        }
        else if (motorSpeed == 20) {
            printf("Max speed!\n");
        }
        else {
            motorSpeed += 5;
            printf("Now speed : %d\n", motorSpeed);
        }
    }
    if (key == 'v') {
        if (power == false) {
            printf("Power on please\n");
        }
        else if (motorSpeed == 0) {
            printf("Min speed!\n");
        }
        else {
            motorSpeed -= 5;
            printf("Now speed : %d\n", motorSpeed);
        }
    }
    if (key == 'r') {
        if (power != true) printf("Power on please!\n");
        else {
            dup = true;
            ddown = false;
        }
    }
    if (key == 't') {
        if (altitude == 0) printf("Collide with the ground\n");
        else {
            ddown = true;
            dup = false;
        }
    }
    if (key == 'q') {
        droneAngle += 1;
        phi += 1;
        if (droneAngle > 360)droneAngle -= 360;
    }
    if (key == 'e') {
        droneAngle -= 1;
        phi -= 1;
        if (droneAngle < -360)droneAngle += 360;
    }
    if (key == 'z') {
        clocking = not(clocking);
    }
    if (key == 'b') {
        droneView = not(droneView);
    }
    if (key == 'w') {
        state = 0;
    }
    if (key == 's') {
        state = 1;
    }
    if (key == 'a') {
        state = 2;
    }
    if (key == 'd') {
        state = 3;
    }
    if (key == 'f') {
        state = -1;
        velo = 0;
        dup = false;
        ddown = false;
    }
    if (key == 'l') {
        lightMode = not(lightMode);
    }
    glutPostRedisplay();
}

void Drone_angle() {
    if (state == 0 && frontAngle <= 20) {
        frontAngle += 0.4;
    }
    if (state == 3 && sideAngle <= 20) {
        sideAngle += 0.4;
    }
    if (state == 1 && backAngle >= -20) {
        backAngle -= 0.4;
    }
    if (state == 2 && csideAngle >= -20) {
        csideAngle -= 0.4;
    }
    if (state == -1) {
        if (frontAngle > 0) {
            frontAngle -= 0.4;
        }
        if (backAngle < 0) {
            backAngle += 0.4;
        }
        if (sideAngle > 0) {
            sideAngle -= 0.4;
        }
        if (csideAngle < 0) {
            csideAngle += 0.4;
        }
    }
}

void Drone_location() {
    if (state == 0) {
        xlocation += (motorSpeed * 0.01) * sin(getRadian(droneAngle));
        zlocation += (motorSpeed * 0.01) * cos(getRadian(droneAngle));
    }
    else if (state == 1) {
        xlocation -= (motorSpeed * 0.01) * sin(getRadian(droneAngle));
        zlocation -= (motorSpeed * 0.01) * cos(getRadian(droneAngle));
    }
    else if (state == 2) {
        xlocation += (motorSpeed * 0.01) * cos(getRadian(droneAngle));
        zlocation += (motorSpeed * 0.01) * sin(getRadian(droneAngle));
    }
    else if (state == 3) {
        xlocation -= (motorSpeed * 0.01) * cos(getRadian(droneAngle));
        zlocation -= (motorSpeed * 0.01) * sin(getRadian(droneAngle));
    }
    if (state != -1) {
        if (motorSpeed == 5) {
            if (velo < 20) {
                velo += (motorSpeed * 0.05);
            }
        }
        else if (motorSpeed == 10) {
            if (velo < 30) {
                velo += (motorSpeed * 0.05);
            }
        }
        else if (motorSpeed == 15) {
            if (velo < 40) {
                velo += (motorSpeed * 0.05);
            }
        }
        else if (motorSpeed == 20) {
            if (velo < 50) {
                velo += (motorSpeed * 0.05);
            }
        }
    }
    else velo = 0;
    dtos(velo, v);
}

void Oasis_move() {
    if (peak > 10) {
        oasisState = 1;
    }
    if (peak < 1) {
        oasisState = 2;
    }
    if (oasisState == 2) {
        peak += 0.2;
    }
    else if (oasisState == 1) {
        peak -= 0.2;
    }
}

void Drone_altitude() {
    if (dup) {
        altitude += (motorSpeed * 0.02);
    }
    if (ddown == true) {
        if (power == false) {
            motorSpeed = 0;
            if (altitude >= 1.0) {
                altitude -= 1;
            }
            else ddown = false;
        }
        else {
            if (altitude > 0) {
                altitude -= (motorSpeed * 0.02);
            }
            else ddown = false;
        }
    }
    dtos(altitude, h);
}

inline void idle(int value) {
    propeller += motorSpeed;
    if (propeller > 360)propeller -= 360;
    texAngle += 1;
    if (texAngle > 360)texAngle -= 360;
    Oasis_move();
    Drone_altitude();
    Drone_angle();
    Drone_location();
    glutTimerFunc(10, idle, 1); //10msec
    glutPostRedisplay();
}

void main_menu_function(int option) {
    if (option == 999) {
        exit(0);
    }
    if (option == 998) {
        clear();
        glutPostRedisplay();
    }

}

void background_menu(int option) {
    if (option == 3) {
        ngt = true;
    }
    if (option == 4) {
        skyMapping();
        ngt = false;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    //Window 초기화
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(g_nGLWidth, g_nGLHeight); //창의 폭과 높이
    //창의 왼쪽 위 모서리의 위치
    //첫번째 인자=가로, 두번째 인자=세로
    glutInitWindowPosition(500, 50);
    glutCreateWindow("12151570_석진웅");
    init(); //사용자 초기화 함수

    int submenul1;
    submenul1 = glutCreateMenu(background_menu); //submenul은 id
    glutAddMenuEntry("Night", 3);
    glutAddMenuEntry("Morning", 4);

    glutCreateMenu(main_menu_function);
    glutAddSubMenu("Background", submenul1);
    glutAddMenuEntry("Clear", 998);
    glutAddMenuEntry("Quit", 999);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutReshapeFunc(&resize);
    glutMouseFunc(&mouse); //mouse call back
    glutMotionFunc(&motion); //mouse motion call back
    glutKeyboardFunc(&keyboard); //keyboard motion call back
    glutSpecialFunc(&special_keyboard);

    glutDisplayFunc(draw); //draw() -> 실제 그리기 함수
    glutTimerFunc(10, idle, 1);
    glutMainLoop();
    return 0;
}