//
//  marchingCubesGPU.h
//  marchingCubesGPU
//
//  Created by Chris Kiefer on 25/02/2012.


//ported to OF/OSX from http://www.icare3d.org/blog_techno/gpu/opengl_geometry_shader_marching_cubes.html


#ifndef marchingCubesGPU_marchingCubesGPU_h
#define marchingCubesGPU_marchingCubesGPU_h

#include "Vector.h"
#include "ofMain.h"
#include "simplexnoise1234.h"

using namespace NemoGraphics;

class GridCell{
public:
	float val[8];
	Vector3f pos[8];
};

class marchingCubesGPU {
public:
    
    void setup();
    void prepareToDraw();
    void draw();
    void initShader(GLhandleARB programObject, const char *filen, GLuint type);
    void toggleWireframe();
    void nextData();

	Vector3f sphereLightPos;
    
    
	//general flags
	bool pause;
	bool wireframe;
    
	GLuint edgeTableTex;
	GLuint triTableTex;
    
	GLuint dataFieldTex[3];
    
	Vector3f cubeSize;
	Vector3f cubeStep;
    
	Vector3i dataSize;
    
	float *dataField[3];
	float isolevel;
    
	bool animate;
	bool autoWay;
	bool enableVBO;
	bool enableSwizzledWalk;
    
	int mode;
    
	int curData;
    
	float *gridData;
	GLuint gridDataBuffId;
	GLuint gridDataSwizzledBuffId;
    
    GLhandleARB programObject;
    ofShader surfaceShader;

    SimplexNoise1234 simplexNoise;
    float noise1[96][96][96];
//    float noise2[64][64][64];
//    float noise3[64][64][64];
//    float noise4[64][64][64];
    
    uint densityFBO;
    uint textureID;
    float *densityData;
    GLint sliceAttrib, scaleAttrib;
    
    GLuint noiseTex, noiseTex2;
    float *noiseField, *noiseField2;
    GLuint noiseSize;
    float *surfaceGridData;
    GLuint surfaceGridVBO;
    


};

#endif
