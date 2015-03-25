#version 120

varying vec3 N;
varying vec4 v;

float b = 0.55;
float y = 0.3;

float alpha = 0.25;
float beta = 0.5;
float shininess = gl_FrontMaterial.shininess;

void main()
{
    gl_FragColor = vec4(0,0,0,0);
    vec3 L = normalize(gl_LightSource[0].position.xyz - v.xyz);
    vec3 R = normalize(L - 2 * dot(L, N) * N);

    vec4 kd = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;

    vec4 kCool = vec4(0, 0, b, 0) * alpha * kd;
    vec4 kWarm = vec4(y, y, 0, 0) * beta * kd;

    gl_FragColor += kCool * (1.0 - dot(L,N)) / 2 + kWarm * (1.0 + dot(L,N)) / 2;
    gl_FragColor += gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(max(0.0, dot(R, v.xyz)), shininess);
}
