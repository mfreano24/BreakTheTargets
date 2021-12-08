#version 120

attribute vec4 aPos; // in object space
uniform mat4 P;
uniform mat4 MV;
uniform vec2 screenSize;
void main(){
    gl_Position = P * MV * aPos;

    gl_PointSize = screenSize.y / 12.5;
}
