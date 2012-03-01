#version 120

//Vertices position for fragment shader
varying vec4 position;

void main(void)
{
    gl_TexCoord[0]=gl_MultiTexCoord0;

	gl_Position=ftransform();

	position=gl_Vertex;

	gl_FrontColor=gl_Color;

}
