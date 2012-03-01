#version 120

varying vec4 position;
attribute float slice;
varying float zpos;

void main(void)
{
//    gl_TexCoord[0]=gl_MultiTexCoord0;
//
    zpos = slice;
	gl_Position=ftransform();
    position = gl_Position;
//	gl_Position=ftransform();

//	gl_FrontColor=gl_Color;

}
