#version 120

varying vec4 position;

uniform vec3 dataStep;
uniform sampler3D dataFieldTex;

const vec3 diffuseMaterial = vec3(0.7, 0.7, 0.7);
const vec3 specularMaterial = vec3(0.99, 0.99, 0.99);
const vec3 ambiantMaterial = vec3(0.1, 0.1, 0.1);


void main(void)
{

#if 1
    vec3 grad = vec3(
		texture3D(dataFieldTex, (position.xyz+vec3(dataStep.x, 0, 0)+1.0f)/2.0f).a - texture3D(dataFieldTex, (position.xyz+vec3(-dataStep.x, 0, 0)+1.0f)/2.0f).a, 
		texture3D(dataFieldTex, (position.xyz+vec3(0, dataStep.y, 0)+1.0f)/2.0f).a - texture3D(dataFieldTex, (position.xyz+vec3(0, -dataStep.y, 0)+1.0f)/2.0f).a, 
		texture3D(dataFieldTex, (position.xyz+vec3(0,0,dataStep.z)+1.0f)/2.0f).a - texture3D(dataFieldTex, (position.xyz+vec3(0,0,-dataStep.z)+1.0f)/2.0f).a);
    

    
    vec3 lightVec=normalize(gl_LightSource[0].position.xyz-position.xyz);
    
    vec3 normalVec = normalize(grad);

	vec3 color=gl_Color.rgb*0.5+abs(normalVec)*0.5;

    // calculate half angle vector
    vec3 eyeVec = vec3(0.0, 0.0, 1.0);
    vec3 halfVec = normalize(lightVec + eyeVec);
    
    // calculate diffuse component
    vec3 diffuse = vec3(abs(dot(normalVec, lightVec))) * color*diffuseMaterial;

    // calculate specular component
    vec3 specular = vec3(abs(dot(normalVec, halfVec)));
    specular = pow(specular.x, 32.0) * specularMaterial;
    
    // combine diffuse and specular contributions and output final vertex color
    gl_FragColor.rgb =gl_Color.rgb*ambiantMaterial + diffuse + specular;
    gl_FragColor.a = 1.0;
#else

	gl_FragColor=gl_Color;
#endif

    
}
