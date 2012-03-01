#version 120

varying vec4 position;
varying float zpos;
uniform sampler3D noiseTex, noiseTex2;
float surfaceVal;
float mod;
uniform float scale;

void main(void) {
    surfaceVal = zpos;
    mod = (texture3D(noiseTex, vec3(position.x/800.0, position.y/800.0, zpos/800.0)).a * 9);
    mod += (texture3D(noiseTex, vec3(position.x/1000.0, position.y/400.0, zpos/600.0)).a * 16);
    mod += (texture3D(noiseTex, vec3((position.x + zpos)/700.0, (position.y-34)/1500.0, (zpos+12)/1000.0)).a * 12);
    mod += (texture3D(noiseTex, vec3(position.x/200.0, position.y/200.0, zpos/200.0)).a * 1.2);
    mod += (texture3D(noiseTex, vec3((position.x+zpos)/190.0, (position.y-zpos)/150.0, (zpos+50)/100.0)).a * 0.5);
    mod += (texture3D(noiseTex2, vec3(position.x/200.0, position.y/200.0, zpos/200.0)).a * 0.1);
    mod += (texture3D(noiseTex2, vec3(position.x/100.0, position.y/300.0, zpos/50.0)).a * 0.09);
    mod *= scale;
	gl_FragColor=vec4(0.0,0.0,0.0, (surfaceVal + mod) / 96.0);
}
