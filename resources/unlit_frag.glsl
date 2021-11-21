#version 120
uniform vec3 kd;
varying vec4 vPos;
void main()
{
	gl_FragColor = vec4(kd, 1.0);
}
