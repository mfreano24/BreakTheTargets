#version 120
attribute vec4 aPos; // in object space
attribute vec3 aNor; // in object space
uniform mat4 P;
uniform mat4 MV;
uniform mat4 V;

varying vec3 vPos;
varying vec3 fragPos;
varying vec3 fragNor;

void main()
{
	// World space:             M * aPos
	// Camera space:        V * M * aPos
	// Projected space: P * V * M * aPos
	vec4 posCamera = MV * aPos;
	vec3 norCamera = mat3(V) * aNor;
	gl_Position = P * posCamera;
	vPos = aPos.xyz;
	fragPos = posCamera.xyz;
	fragNor = normalize(norCamera);
}
