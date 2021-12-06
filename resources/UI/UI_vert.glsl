#version 120

attribute vec4 aPos; // in object space
uniform mat4 MV;
uniform vec2 screenSize;
void main(){
    gl_Position = MV * aPos;

    gl_PointSize = 20.0;
}
