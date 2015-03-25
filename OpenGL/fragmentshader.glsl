varying vec3 N;
varying vec3 v;

void main()
{
	vec3 L = normalize(gl_LightSource[0].position.xyz - v);
    vec3 E = normalize(-v);
    vec3 R = normalize(-reflect(L,N));
    vec4 color = gl_FrontLightModelProduct.sceneColor;

    color += gl_FrontLightProduct[0].ambient;
    float angle = dot(N,L);

    if (angle > 0.0)
        color += gl_FrontLightProduct[0].diffuse * angle;
    
    float light_angle = dot(R,E);

    if (light_angle > 0.0)
        color += gl_FrontLightProduct[0].specular * pow(light_angle, gl_FrontMaterial.shininess);
    
    gl_FragColor = color;
}
