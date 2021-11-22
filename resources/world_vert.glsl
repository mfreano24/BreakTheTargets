#version 120
attribute vec4 aPos; // in object space
attribute vec3 aNor; // in object space
uniform mat4 P;
uniform mat4 MV;
uniform mat4 V;

varying vec4 vPos;

void main()
{
	// World space:             M * aPos
	// Camera space:        V * M * aPos
	// Projected space: P * V * M * aPos
	vec4 posCamera = MV * aPos;
	vec4 norCamera = MV * vec4(aNor, 0.0);
	gl_Position = P * posCamera;
	vPos = aPos;
}
