/**** Geometry Shader Marching Cubes
	* Copyright Cyril Crassin, Junuary 2007.
	* This code is partially based on the example of 
	* Paul Bourke "Polygonising a scalar field" located at :
	* http://local.wasp.uwa.edu.au/~pbourke/geometry/polygonise/
****/

//GLSL version 1.20
#version 120
//New G80 extensions
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable


//Volume data field texture
uniform sampler3D dataFieldTex;
//Edge table texture
uniform isampler2D edgeTableTex;
//Triangles table texture
uniform isampler2D triTableTex;

//Global iso level
uniform float isolevel;
//Marching cubes vertices decal
uniform vec3 vertDecals[8];

//Vertices position for fragment shader
varying vec4 position;

//Get vertex i position within current marching cube
vec3 cubePos(int i){
	return gl_PositionIn[0].xyz + vertDecals[i];
}

vec4 sampleFunction( vec3 p )
{
    return vec4(
        16.0*p.y*p.z +
        4.0*2.0*p.x,
        16.0*p.x*p.z +
        4.0*2.0*p.y,
        16.0*p.x*p.y +
        4.0*2.0*p.z,
        16.0*p.x*p.y*p.z +
        4.0*p.x*p.x +
        4.0*p.y*p.y +
        4.0*p.z*p.z - 1.0
        );
} 

//Get vertex i value within current marching cube
float cubeVal(int i){
	//return sampleFunction(cubePos(i)).a;

	return texture3D(dataFieldTex, (cubePos(i)+1.0)/2.0).a;
}

//Get edge table value
int edgeTableValue(int i){
	return texelFetch2D(edgeTableTex, ivec2(i, 0), 0).a;
}

//Get triangle table value
int triTableValue(int i, int j){
	return texelFetch2D(triTableTex, ivec2(j, i), 0).a;
}

//Compute interpolated vertex along an edge
vec3 vertexInterp(float isolevel, vec3 v0, float l0, vec3 v1, float l1){
	return mix(v0, v1, (isolevel-l0)/(l1-l0));
}

//Geometry Shader entry point
void main(void) {
	int cubeindex=0;

    //Determine the index into the edge table which
    //tells us which vertices are inside of the surface
	if (cubeVal(0) < isolevel) cubeindex = cubeindex | 1;
	if (cubeVal(1) < isolevel) cubeindex = cubeindex | 2;
	if (cubeVal(2) < isolevel) cubeindex = cubeindex | 4;
	if (cubeVal(3) < isolevel) cubeindex = cubeindex | 8;
	if (cubeVal(4) < isolevel) cubeindex = cubeindex | 16;
	if (cubeVal(5) < isolevel) cubeindex = cubeindex | 32;
	if (cubeVal(6) < isolevel) cubeindex = cubeindex | 64;
	if (cubeVal(7) < isolevel) cubeindex = cubeindex | 128;
	

	//Cube is entirely in/out of the surface
	if (edgeTableValue(cubeindex) == 0)
		return;
    
    vec3 vertlist[12];
    
	//Find the vertices where the surface intersects the cube
	if ((edgeTableValue(cubeindex) & 1)!=0 )
	  vertlist[0] =	 vertexInterp(isolevel, cubePos(0), cubeVal(0), cubePos(1), cubeVal(1));
	if ((edgeTableValue(cubeindex) & 2)!=0 )
	  vertlist[1] = vertexInterp(isolevel, cubePos(1), cubeVal(1), cubePos(2), cubeVal(2));
	if ((edgeTableValue(cubeindex) & 4)!=0)
	  vertlist[2] = vertexInterp(isolevel, cubePos(2), cubeVal(2), cubePos(3), cubeVal(3));
	if ((edgeTableValue(cubeindex) & 8)!=0)
	  vertlist[3] = vertexInterp(isolevel, cubePos(3), cubeVal(3), cubePos(0), cubeVal(0));
	if ((edgeTableValue(cubeindex) & 16)!=0)
	  vertlist[4] = vertexInterp(isolevel, cubePos(4), cubeVal(4), cubePos(5), cubeVal(5));
	if ((edgeTableValue(cubeindex) & 32)!=0)
	  vertlist[5] = vertexInterp(isolevel, cubePos(5), cubeVal(5), cubePos(6), cubeVal(6));
	if ((edgeTableValue(cubeindex) & 64)!=0)
	  vertlist[6] = vertexInterp(isolevel, cubePos(6), cubeVal(6), cubePos(7), cubeVal(7));
	if ((edgeTableValue(cubeindex) & 128)!=0)
	  vertlist[7] = vertexInterp(isolevel, cubePos(7), cubeVal(7), cubePos(4), cubeVal(4));
	if ((edgeTableValue(cubeindex) & 256)!=0)
	  vertlist[8] = vertexInterp(isolevel, cubePos(0), cubeVal(0), cubePos(4), cubeVal(4));
	if ((edgeTableValue(cubeindex) & 512)!=0)
	  vertlist[9] = vertexInterp(isolevel, cubePos(1), cubeVal(1), cubePos(5), cubeVal(5));
	if ((edgeTableValue(cubeindex) & 1024)!=0)
	  vertlist[10] = vertexInterp(isolevel, cubePos(2), cubeVal(2), cubePos(6), cubeVal(6));
	if ((edgeTableValue(cubeindex) & 2048)!=0)
	  vertlist[11] = vertexInterp(isolevel, cubePos(3), cubeVal(3), cubePos(7), cubeVal(7));

	
	// Create the triangle
	gl_FrontColor=gl_FrontColorIn[0]; //vec4(cos(isolevel*10.0-0.5), sin(isolevel*10.0-0.5), cos(1.0-isolevel),1.0);

	int i=0;		
	//for (i=0; triTableValue(cubeindex, i)!=-1; i+=3) { //Strange bug with this way, uncomment to test
	while(true){
	//for(i=0; i<16; i+=3){
		if(triTableValue(cubeindex, i)!=-1){
			//Generate first vertex of triangle//
			//Fill position varying attribute for fragment shader
			position= vec4(vertlist[triTableValue(cubeindex, i)], 1);
			//Fill gl_Position attribute for vertex raster space position
			gl_Position = gl_ModelViewProjectionMatrix* position;
			EmitVertex();
			
			//Generate second vertex of triangle//
			//Fill position varying attribute for fragment shader
			position= vec4(vertlist[triTableValue(cubeindex, i+1)], 1);
			//Fill gl_Position attribute for vertex raster space position
			gl_Position = gl_ModelViewProjectionMatrix* position;
			EmitVertex();

			//Generate last vertex of triangle//
			//Fill position varying attribute for fragment shader
			position= vec4(vertlist[triTableValue(cubeindex, i+2)], 1);
			//Fill gl_Position attribute for vertex raster space position
			gl_Position = gl_ModelViewProjectionMatrix* position;
			EmitVertex();
			
			//End triangle strip at firts triangle
			EndPrimitive();
		}else{
			break;
		}
		
		i=i+3; //Comment it for testing the strange bug
	}
	
}
