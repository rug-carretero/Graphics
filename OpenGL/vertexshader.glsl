#version 120
varying vec3 N, L, E;

void main()
{	
	N = normalize(gl_NormalMatrix * gl_Normal);

	vec3 v = normalize(vec3(gl_ModelViewMatrix * gl_Vertex));
	
	L = vec3(gl_LightSource[0].position.xyz - v);
	E = normalize(-v);

	gl_Position = ftransform();		
}
