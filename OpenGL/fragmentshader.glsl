#version 120
varying vec3 N, L, E;

void main (void)
{
	vec4 final_color = 
		(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
		(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
	
	float R0 = dot(N,L);
	
	if(R0 > 0.0)
	{
		final_color += gl_LightSource[0].diffuse * 
			gl_FrontMaterial.diffuse * R0;	
		
		vec3 R = reflect(-L, N);
		
		float specular = pow(max(dot(R, E), 0.0), 
			gl_FrontMaterial.shininess);
			
		final_color += gl_LightSource[0].specular * 
			gl_FrontMaterial.specular * specular;	
	}

	gl_FragColor = final_color;			
}
