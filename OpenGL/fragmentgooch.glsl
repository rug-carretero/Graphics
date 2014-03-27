#version 120
uniform vec3 WarmColour;
uniform vec3 CoolColour;
uniform float OutlineWidth;

varying vec3 N;
varying vec3 P;
varying vec3 V;
varying vec3 L;

void main()
{
	vec3 l = normalize(L);
	vec3 n = normalize(N);
	vec3 v = normalize(V);
	vec3 h = normalize(l+v);

	float diffuse = dot(l,n);
	float specular = pow(dot(n,h),gl_FrontMaterial.shininess);

	vec4 cool = min(vec4(CoolColour,1.0)+(0.2*gl_LightSource[0].diffuse * 
				gl_FrontMaterial.diffuse ),1.0);
	vec4 warm = min(vec4(WarmColour,1.0)+0.5*gl_LightSource[0].diffuse * 
				gl_FrontMaterial.diffuse,1.0);

	vec4 colour = min(mix(cool,warm,diffuse)+specular * gl_FrontMaterial.specular * gl_LightSource[0].specular,1.0);

	 colour = cool *(1 - diffuse)/2 + warm * (1 + diffuse)/2 + specular * gl_FrontMaterial.specular * gl_LightSource[0].specular;

	/*if (dot(n,v)<OutlineWidth) colour=vec4(0,0,0,1);*/

	gl_FragColor = colour;
}
