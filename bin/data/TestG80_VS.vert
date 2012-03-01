#version 120

void main(void)
{
    gl_TexCoord[0]=gl_MultiTexCoord0;

	gl_Position=gl_Vertex;//ftransform();

	gl_FrontColor=gl_Color;

}
