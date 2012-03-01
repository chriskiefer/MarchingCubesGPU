//
//  marchingCubesGPU.cpp
//  marchingCubesGPU
//
//  Created by Chris Kiefer on 25/02/2012.

#include <iostream>
#include "marchingCubesGPU.h"
#include "mctables.cpp"

void marchingCubesGPU::initShader(GLhandleARB programObject, const char *filen, GLuint type){
    
	//Source file reading
	std::string buff;
	std::ifstream file;
	std::string filename=filen;
	std::cerr.flush();
	file.open(filename.c_str());
	std::string line;
	while(std::getline(file, line))
		buff += line + "\n";
    
	const GLcharARB *txt=buff.c_str();
    
	//Shader object creation
	GLhandleARB object = glCreateShaderObjectARB(type);
	
	//Source code assignment
	glShaderSourceARB(object, 1, &txt, NULL);
    
	//Compile shader object
	glCompileShaderARB(object);
    
	//Check if shader compiled
	GLint ok = 0;
	glGetObjectParameterivARB(object, GL_OBJECT_COMPILE_STATUS_ARB, &ok);
	if (!ok){
		GLint maxLength=4096;
		char *infoLog = new char[maxLength];
		glGetInfoLogARB(object, maxLength, &maxLength, infoLog);
		std::cout<<"Compilation error: "<<infoLog<<"\n";
		delete []infoLog;
	}
    
	// attach shader to program object
	glAttachObjectARB(programObject, object);
    
	// delete object, no longer needed
	glDeleteObjectARB(object);
    
	//Global error checking
	std::cout<<"InitShader: "<<filen<<" Errors:"<<gluErrorString(glGetError())<<"\n";
}

void swizzledWalk(int &n, float *gridData, Vector3i pos, Vector3i size, const Vector3f &cubeSize){
	if(size.x>1){
		Vector3i newSize=size/2;
        
		swizzledWalk(n, gridData, pos, newSize, cubeSize);
		swizzledWalk(n, gridData, pos+Vector3i(newSize.x, 0, 0), newSize, cubeSize);
		swizzledWalk(n, gridData, pos+Vector3i(0, newSize.y,0), newSize, cubeSize);
		swizzledWalk(n, gridData, pos+Vector3i(newSize.x, newSize.y, 0), newSize, cubeSize);
        
		swizzledWalk(n, gridData, pos+Vector3i(0, 0, newSize.z), newSize, cubeSize);
		swizzledWalk(n, gridData, pos+Vector3i(newSize.x, 0, newSize.z), newSize, cubeSize);
		swizzledWalk(n, gridData, pos+Vector3i(0, newSize.y, newSize.z), newSize, cubeSize);
		swizzledWalk(n, gridData, pos+Vector3i(newSize.x, newSize.y, newSize.z), newSize, cubeSize);
	}else{
		gridData[n]=(pos.x/cubeSize.x)*2.0f-1.0f;
		gridData[n+1]=(pos.y/cubeSize.y)*2.0f-1.0f;
		gridData[n+2]=(pos.z/cubeSize.z)*2.0f-1.0f;
		n+=3;
	}
}

void marchingCubesGPU::setup() {
    enableVBO = false;
	pause=false;
    
	this->wireframe=false;;
    
	cubeSize=Vector3f(32,32,32);
	cubeStep=Vector3f(2.0f, 2.0f, 2.0f)/cubeSize;
    
	dataSize=Vector3i(96,96,96);
    
	isolevel=0.5f;
	animate=true;
	autoWay=true;
	curData=1;
	mode=1;
	enableSwizzledWalk=true;

    srand(time(0));
	glShadeModel (GL_SMOOTH);
    
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
    
	//Form multi-face view
	glDisable(GL_CULL_FACE);
    
    for(int i=0; i < 96; i++) {
        for(int j=0; j < 96; j++) {
            for(int k=0; k < 96; k++) {
                noise1[i][j][k] = simplexNoise.noise(i/100.0,j/100.0,k/100.0);
//                noise2[i][j][k] = simplexNoise.noise(i/50.0,j/50.0,k/50.0);
//                noise3[i][j][k] = simplexNoise.noise(i/10.0,j/10.0,k/10.0);
//                noise4[i][j][k] = simplexNoise.noise(i/5.0,j/5.0,k/5.0);
            }
        }
    }
    
    //create noise textures
    noiseSize = 256;
	glGenTextures(1, &noiseTex);
	glActiveTexture(GL_TEXTURE4);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, noiseTex);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
    //Generate a simplex noise field
	noiseField = new float[noiseSize*noiseSize*noiseSize];
	for(int k=0; k<noiseSize; k++)
        for(int j=0; j<noiseSize; j++)
            for(int i=0; i<noiseSize; i++){
                noiseField[i+ j*noiseSize + k*noiseSize*noiseSize] = simplexNoise.noise(i/50.0, j/50.0, k/50.0);
            }
	glTexImage3D( GL_TEXTURE_3D, 0, GL_ALPHA16F_ARB, noiseSize, noiseSize, noiseSize, 0, GL_ALPHA, GL_FLOAT, noiseField);
    
    glDisable(GL_TEXTURE_3D);

    glGenTextures(1, &noiseTex2);
	glActiveTexture(GL_TEXTURE5);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, noiseTex);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    //Generate a simplex noise field
	noiseField2 = new float[noiseSize*noiseSize*noiseSize];
	for(int k=0; k<noiseSize; k++)
        for(int j=0; j<noiseSize; j++)
            for(int i=0; i<noiseSize; i++){
                noiseField2[i+ j*noiseSize + k*noiseSize*noiseSize] = simplexNoise.noise(i/10.0, j/10.0, k/10.0);
            }
	glTexImage3D( GL_TEXTURE_3D, 0, GL_ALPHA16F_ARB, noiseSize, noiseSize, noiseSize, 0, GL_ALPHA, GL_FLOAT, noiseField2);
    
    glDisable(GL_TEXTURE_3D);

	glDepthMask(true);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
    
    surfaceShader.load("surface");
    surfaceShader.begin();
    sliceAttrib = surfaceShader.getAttributeLocation("slice");
    surfaceShader.setUniform1i("noiseTex", 4);
    surfaceShader.setUniform1i("noiseTex2", 5);
    surfaceShader.end();
    
    std::cout<<"InitShader: "<<gluErrorString(glGetError())<<"\n";
    
    programObject = glCreateProgramObjectARB();
    
	////Shaders loading////
	//Geometry Shader loading
	initShader(programObject, ofToDataPath("TestG80_GS2.geom").c_str(), GL_GEOMETRY_SHADER_EXT);
	//Geometry Shader require a Vertex Shader to be used
	initShader(programObject, ofToDataPath("TestG80_VS.vert").c_str(), GL_VERTEX_SHADER_ARB);
	//Fragment Shader for per-fragment lighting
	initShader(programObject, ofToDataPath("TestG80_FS.frag").c_str(), GL_FRAGMENT_SHADER_ARB);
	////////
    
	//Get max number of geometry shader output vertices
	GLint temp;
	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
	std::cout<<"Max GS output vertices:"<<temp<<"\n";
	
	////Setup Geometry Shader////
	//Set POINTS primitives as INPUT
    glProgramParameteriEXT(programObject,GL_GEOMETRY_INPUT_TYPE_EXT , GL_POINTS );
	//Set TRIANGLE STRIP as OUTPUT
	glProgramParameteriEXT(programObject,GL_GEOMETRY_OUTPUT_TYPE_EXT , GL_TRIANGLE_STRIP);
	//Set maximum number of vertices to be generated by Geometry Shader to 16
	//16 is the maximum number of vertices a marching cube configuration can own
	//This parameter is very important and have an important impact on Shader performances
	//Its value must be chosen closer as possible to real maximum number of vertices
	glProgramParameteriEXT(programObject,GL_GEOMETRY_VERTICES_OUT_EXT, 16);
    
    glLinkProgramARB(programObject);
    //	//Test link success
	GLint ok = false;
    glGetObjectParameterivARB(programObject, GL_OBJECT_LINK_STATUS_ARB, &ok);
	if (!ok){
		GLint maxLength=4096;
		char *infoLog = new char[maxLength];
		glGetInfoLogARB(programObject, maxLength, &maxLength, infoLog);
		std::cout<<"Link error: "<<infoLog<<"\n";
		delete []infoLog;
	}
    
    //Program validation
    glValidateProgramARB(programObject);
    ok = false;
    glGetObjectParameterivARB(programObject, GL_OBJECT_VALIDATE_STATUS_ARB, &ok);
    if (!ok){
		int maxLength=4096;
		char *infoLog = new char[maxLength];
		glGetInfoLogARB(programObject, maxLength, &maxLength, infoLog);
		std::cout<<"Validation error: "<<infoLog<<"\n";
		delete []infoLog;
	}
    
	//Bind program object for parameters setting
	glUseProgramObjectARB(programObject);
    
    //Edge Table texture//
	//This texture store the 256 different configurations of a marching cube.
	//This is a table accessed with a bitfield of the 8 cube edges states 
	//(edge cut by isosurface or totally in or out).
	//(cf. MarchingCubes.cpp)
	glGenTextures(1, &(this->edgeTableTex));
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
    
	glBindTexture(GL_TEXTURE_2D, this->edgeTableTex);
	//Integer textures must use nearest filtering mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
	//We create an integer texture with new GL_EXT_texture_integer formats
	glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA16I_EXT, 256, 1, 0, GL_ALPHA_INTEGER_EXT, GL_INT, &edgeTable);
    
    glDisable(GL_TEXTURE_2D);
    
	//Triangle Table texture//
	//This texture store the vertex index list for 
	//generating the triangles of each configurations.
	//(cf. MarchingCubes.cpp)
	glGenTextures(1, &(this->triTableTex));
	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
    
	glBindTexture(GL_TEXTURE_2D, this->triTableTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
	glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA16I_EXT, 16, 256, 0, GL_ALPHA_INTEGER_EXT, GL_INT, &triTable);
    
    glDisable(GL_TEXTURE_2D);
    
	//Datafield//
	//Store the volume data to polygonise
	glGenTextures(3, (this->dataFieldTex));
    
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, this->dataFieldTex[0]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    //Generate a distance field to the center of the cube
    Vector3f vectTo = Vector3f(dataSize.x/2.0,dataSize.y/2.0,dataSize.z/2.0);
	dataField[0]=new float[dataSize.x*dataSize.y*dataSize.z];
	for(int k=0; k<dataSize.z; k++)
        for(int j=0; j<dataSize.y; j++)
            for(int i=0; i<dataSize.x; i++){
                float d=Vector3f(i, j, k).distance(vectTo)/(float)(dataSize.length()*0.4);
                dataField[0][i+j*dataSize.x+k*dataSize.x*dataSize.y]=d;//+(rand()%100-50)/200.0f*d;
            }
    
	glTexImage3D( GL_TEXTURE_3D, 0, GL_ALPHA32F_ARB, dataSize.x, dataSize.y, dataSize.z, 0, GL_ALPHA, GL_FLOAT, dataField[0]);
    
    
	//Datafield Perturbated//
	dataField[1]=new float[dataSize.x*dataSize.y*dataSize.z];
	//perturb
	for(int k=0; k<dataSize.z; k++)
        for(int j=0; j<dataSize.y; j++)
            for(int i=0; i<dataSize.x; i++){
                float d=dataField[0][i+j*dataSize.x+k*dataSize.x*dataSize.y];
                dataField[1][i+j*dataSize.x+k*dataSize.x*dataSize.y]=d+(rand()%100-50)/100.0f*d;
            }
    
	//Smooth
	for(int l=0; l<4; l++)
        for(int k=1; k<dataSize.z-1; k++)
            for(int j=1; j<dataSize.y-1; j++)
                for(int i=1; i<dataSize.x-1; i++){
                    dataField[1][i+j*dataSize.x+k*dataSize.x*dataSize.y]=(dataField[1][i+1+j*dataSize.x+k*dataSize.x*dataSize.y]+dataField[1][i-1+j*dataSize.x+k*dataSize.x*dataSize.y]+dataField[1][i+(j+1)*dataSize.x+k*dataSize.x*dataSize.y]+dataField[1][i+(j-1)*dataSize.x+k*dataSize.x*dataSize.y]+dataField[1][i+j*dataSize.x+(k+1)*dataSize.x*dataSize.y]+dataField[1][i+j*dataSize.x+(k-1)*dataSize.x*dataSize.y])/6.0f;
                }
    
	//Store the volume data to polygonise
	glBindTexture(GL_TEXTURE_3D, this->dataFieldTex[1]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
	glTexImage3D( GL_TEXTURE_3D, 0, GL_ALPHA32F_ARB, dataSize.x, dataSize.y, dataSize.z, 0, GL_ALPHA, GL_FLOAT, dataField[1]);
    
    
	//Cayley-polynomial//
	dataField[2]=new float[dataSize.x*dataSize.y*dataSize.z];
    
//	for(int k=0; k<dataSize.z; k++)
//        for(int j=0; j<dataSize.y; j++)
//            for(int i=0; i<dataSize.x; i++){
//                float x=2.0f/dataSize.x*i-1.0f;
//                float y=2.0f/dataSize.y*j-1.0f;
//                float z=2.0f/dataSize.z*k-1.0f;
//                dataField[2][i+j*dataSize.x+k*dataSize.x*dataSize.y]= 16.0f*x*y*z + 4.0f*x*x + 4.0f*y*y + 4.0f*z*z - 1.0f;
//            }
//    
	for(int k=0; k<dataSize.z; k++)
        for(int j=0; j<dataSize.y; j++)
            for(int i=0; i<dataSize.x; i++){
                float x=2.0f/dataSize.x*i-1.0f;
                float y=2.0f/dataSize.y*j-1.0f;
                float z=2.0f/dataSize.z*k-1.0f;
                dataField[2][i+j*dataSize.x+k*dataSize.x*dataSize.y]= z + (sin(i * 0.1) * 0.5 ) + (cos(j*0.2) * 0.2) + (cos(k*0.3) * 0.4);
            }
//   	for(int l=0; l<4; l++)
//        for(int k=1; k<dataSize.z-1; k++)
//            for(int j=1; j<dataSize.y-1; j++)
//                for(int i=1; i<dataSize.x-1; i++){
//                    dataField[2][i+j*dataSize.x+k*dataSize.x*dataSize.y]=(dataField[2][i+1+j*dataSize.x+k*dataSize.x*dataSize.y]+dataField[2][i-1+j*dataSize.x+k*dataSize.x*dataSize.y]+dataField[2][i+(j+1)*dataSize.x+k*dataSize.x*dataSize.y]+dataField[2][i+(j-1)*dataSize.x+k*dataSize.x*dataSize.y]+dataField[2][i+j*dataSize.x+(k+1)*dataSize.x*dataSize.y]+dataField[2][i+j*dataSize.x+(k-1)*dataSize.x*dataSize.y])/6.0f;
//                }
 

	glBindTexture(GL_TEXTURE_3D, this->dataFieldTex[2]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
	glTexImage3D( GL_TEXTURE_3D, 0, GL_ALPHA32F_ARB, dataSize.x, dataSize.y, dataSize.z, 0, GL_ALPHA, GL_FLOAT, dataField[2]);
    
    
	//Set current texture//
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, this->dataFieldTex[curData]);
    
    glDisable(GL_TEXTURE_3D);
    
	////Samplers assignment///
	glUniform1iARB(glGetUniformLocationARB(programObject, "dataFieldTex"), 0);
	glUniform1iARB(glGetUniformLocationARB(programObject, "edgeTableTex"), 1); 
    glUniform1iARB(glGetUniformLocationARB(programObject, "triTableTex"), 2); 
    
	////Uniforms parameters////
	//Initial isolevel
	glUniform1fARB(glGetUniformLocationARB(programObject, "isolevel"), isolevel); 
	//Step in data 3D texture for gradient computation (lighting)
	glUniform3fARB(glGetUniformLocationARB(programObject, "dataStep"), 1.0f/dataSize.x, 1.0f/dataSize.y, 1.0f/dataSize.z); 
    
	//Decal for each vertex in a marching cube
	glUniform3fARB(glGetUniformLocationARB(programObject, "vertDecals[0]"), 0.0f, 0.0f, 0.0f); 
	glUniform3fARB(glGetUniformLocationARB(programObject, "vertDecals[1]"), cubeStep.x, 0.0f, 0.0f); 
	glUniform3fARB(glGetUniformLocationARB(programObject, "vertDecals[2]"), cubeStep.x, cubeStep.y, 0.0f); 
	glUniform3fARB(glGetUniformLocationARB(programObject, "vertDecals[3]"), 0.0f, cubeStep.y, 0.0f); 
	glUniform3fARB(glGetUniformLocationARB(programObject, "vertDecals[4]"), 0.0f, 0.0f, cubeStep.z); 
	glUniform3fARB(glGetUniformLocationARB(programObject, "vertDecals[5]"), cubeStep.x, 0.0f, cubeStep.z); 
	glUniform3fARB(glGetUniformLocationARB(programObject, "vertDecals[6]"), cubeStep.x, cubeStep.y, cubeStep.z); 
	glUniform3fARB(glGetUniformLocationARB(programObject, "vertDecals[7]"), 0.0f, cubeStep.y, cubeStep.z); 
    
    
    GLfloat LightAmbient[]= { 0.01f, 0.01f, 0.01f, 1.0f };
	GLfloat LightDiffuse[]= { 0.1f, 0.1f, 0.1f, 1.0f };	
	GLfloat LightPosition[]= { 5.0f, 5.0f, 5.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);	
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightDiffuse);	
	glLightfv(GL_LIGHT0, GL_POSITION,LightPosition);	
	glEnable(GL_LIGHT0);
    
    
    
	//////Grid data construction
	//Linear Walk
	gridData=new float[static_cast<int>(cubeSize.x*cubeSize.y*cubeSize.z*3.0)];
	int ii=0;
	for(float k=-1; k<1.0f; k+=cubeStep.z)
        for(float j=-1; j<1.0f; j+=cubeStep.y)
            for(float i=-1; i<1.0f; i+=cubeStep.x){
                gridData[ii]= i;	
                gridData[ii+1]= j;
                gridData[ii+2]= k;
                
                ii+=3;
            }
    
    
	//VBO configuration for marching grid linear walk
	glGenBuffersARB(1, &gridDataBuffId);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, gridDataBuffId);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, cubeSize.x*cubeSize.y*cubeSize.z*3*4, gridData, GL_STATIC_DRAW_ARB);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
	
	//Swizzled Walk 
	int n=0;
	swizzledWalk(n, gridData, Vector3i(0,0,0), Vector3i(cubeSize.x, cubeSize.y, cubeSize.z), cubeSize);
    
    
	//VBO configuration for marching grid Swizzled walk
	glGenBuffersARB(1, &gridDataSwizzledBuffId);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, gridDataSwizzledBuffId);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, cubeSize.x*cubeSize.y*cubeSize.z*3*4, gridData, GL_STATIC_DRAW_ARB);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
    
    
    
    //////Grid data construction
	//Linear Walk
	surfaceGridData=new float[static_cast<int>(dataSize.x*dataSize.y*3.0)];
	int idx=0;
    for(float j=0; j<dataSize.y; j++)
        for(float i=0; i<dataSize.x; i++) {
            surfaceGridData[idx]= i;	
            surfaceGridData[idx+1]= j;	
            idx+=2;
        }
	//VBO configuration for linear walk
	glGenBuffersARB(1, &surfaceGridVBO);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, surfaceGridVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, dataSize.x*dataSize.y*2*sizeof(float), surfaceGridData, GL_STATIC_DRAW_ARB);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
    
    
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    densityData = new float[dataSize.x * dataSize.y * dataSize.z * 4];
//    for(int i=0; i < 64*64*64*4; i++) {
////        densityData[i] = (i) % 256;
//        densityData[i] = rand() % 133;
//    }
    
	for(int k=0; k<dataSize.z; k++)
        for(int j=0; j<dataSize.y; j++)
            for(int i=0; i<dataSize.x; i++){
                float x=2.0f/dataSize.x*i-1.0f;
                float y=2.0f/dataSize.y*j-1.0f;
                float z=2.0f/dataSize.z*k-1.0f;
                float val = z + (sin(i * 0.1) * 0.5 ) + (cos(j*0.2) * 0.2) + (cos(k*0.3) * 0.4);
//                densityData[i+j*dataSize.x+k*dataSize.x*dataSize.y]= (k + rand() % 3) * 4;
                densityData[(i+j*dataSize.x+k*dataSize.x*dataSize.y)*4]= 0;
                densityData[(i+j*dataSize.x+k*dataSize.x*dataSize.y)*4+1]= 0;
                densityData[(i+j*dataSize.x+k*dataSize.x*dataSize.y)*4+2]= 0;
//                densityData[(i+j*dataSize.x+k*dataSize.x*dataSize.y)*4+3]= (k + rand() % 3) * 3;
                densityData[(i+j*dataSize.x+k*dataSize.x*dataSize.y)*4+3]= rand() % 256;
            }
    
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16, dataSize.x, dataSize.y, dataSize.z, 0, GL_RGBA, GL_FLOAT, densityData);
    
    glGenFramebuffersEXT(1, &densityFBO);	
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, densityFBO);
	for(int i=0; i < 64; i++) 
        glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_3D, textureID, 0 ,i);
    
    int retval =glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    cout << "Framebuffer status: " << retval << endl;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, this->dataFieldTex[curData]);
	glDisable(GL_TEXTURE_3D);
    

}

void marchingCubesGPU::prepareToDraw() {
    if (3 == curData) {
        ofPushView();

        ofViewport(0, 0, dataSize.x, dataSize.y, false);
        ofSetupScreenPerspective(dataSize.x, dataSize.y, ofGetOrientation(), false);
        glDisable(GL_TEXTURE_3D);
        glEnable(GL_DEPTH_TEST);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, densityFBO);	
        
        surfaceShader.begin();
        float scale1=sin(ofGetFrameNum() / 50.0) * 2;
        scale1 *= cos(ofGetFrameNum() / 29.0);
        scale1 += cos(ofGetFrameNum() / 2.0) * 0.2;
        surfaceShader.setUniform1f("scale", scale1);
        glBindBuffer(GL_ARRAY_BUFFER_ARB, surfaceGridVBO);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0,  NULL);

        for(int z=0; z < dataSize.z; z++) {
            glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_3D, textureID, 0 ,z);
            glClearColor(0.0,0.0,0.0,0);
            glClear(GL_COLOR_BUFFER_BIT);
            surfaceShader.setAttribute1f(sliceAttrib, z);
            glDrawArrays(GL_POINTS, 0, dataSize.x*dataSize.y);
        }
        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);    
        surfaceShader.end();

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        ofPopView();
    }
}


void marchingCubesGPU::draw() {

    //Shader program binding
    glUseProgramObjectARB(programObject);

	glDepthMask(GL_TRUE);
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
	//States setting
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_ALPHA_TEST);
    glDisable(GL_TEXTURE_2D);
    
    
    glScalef(200, 200, 200);
    
    
	glColor4f(cosf(isolevel*10.0-0.5), sinf(isolevel*10.0-0.5), cosf(1.0-isolevel),1.0);
    
    //    //Current isolevel uniform parameter setting
    glUniform1f(glGetUniformLocationARB(programObject, "isolevel"), isolevel); 
    //Switch to wireframe or solid rendering mode
    if(wireframe)
        glPolygonMode(GL_FRONT_AND_BACK , GL_LINE );
    else
        glPolygonMode(GL_FRONT_AND_BACK , GL_FILL );
    
    
    if(enableSwizzledWalk)
        glBindBuffer(GL_ARRAY_BUFFER_ARB, gridDataSwizzledBuffId);
    else
        glBindBuffer(GL_ARRAY_BUFFER_ARB, gridDataBuffId);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0,  NULL);
    glDrawArrays(GL_POINTS, 0, cubeSize.x*cubeSize.y*cubeSize.z);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
    
    
    
    //Disable shader program
    glUseProgramObjectARB(NULL);
    
//    if(autoWay){
//        if(isolevel<1.5)
//            isolevel+=0.005;
//        else
//            autoWay=!autoWay;
//    }else{
//        if(isolevel>0.0)
//            isolevel-=0.005;
//        else
//            autoWay=!autoWay;
//    }
    
    
}

void marchingCubesGPU::toggleWireframe() {
    wireframe = !wireframe;
}
void marchingCubesGPU::nextData() {
    if (3==curData) curData=0; else curData++;
    if (curData < 3) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, this->dataFieldTex[curData]);
    }else{
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, textureID);
    }
    
}
