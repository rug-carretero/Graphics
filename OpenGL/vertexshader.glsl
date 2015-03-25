#version 120

varying vec3 N;
varying vec4 v, E;

void main(void)
{
    v = normalize(gl_ModelViewMatrix * gl_Vertex);
    E = normalize(-v);
    N = normalize(gl_NormalMatrix * gl_Normal);

	gl_Position = ftransform();
}