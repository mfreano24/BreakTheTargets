#version 120

attribute vec4 aPos;
attribute vec3 aNor;

varying vec3 texCoords;

uniform mat4 P;
uniform mat4 MV;

void main(){
    texCoords = aPos.xyz;
    vec4 norCamera = MV * vec4(aNor, 0.0);
    vec4 pos = P * MV * aPos;
    gl_Position = pos.xyww;

    
}