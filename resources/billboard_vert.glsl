#version 120

attribute vec4 aPos; // in object space
attribute int aTexInd;
uniform mat4 P;
uniform mat4 MV;

uniform vec2 screenSize;

varying vec2 texCoords;
varying vec3 currPos;
varying flat int texIndex;

void main(){
    gl_Position = P * MV * aPos;
    
    currPos = gl_Position.xyz;

    texIndex = aTexInd;


    gl_PointSize = screenSize.y * P[1][1] * 1.0/ gl_Position.w;

}