#version 120

varying vec2 texCoords;
varying vec3 currPos;
varying float texIndex;
uniform sampler2D texA;
uniform sampler2D texB;
uniform sampler2D texC;
uniform sampler2D texD;
uniform vec3 startingPos;

uniform vec3 kd;


void main(){
    //float dist = distance(currPos, startingPos);
    //float alpha = texture2D(tex, gl_PointCoord).r; //idea from L08 where you can
    //gl_FragColor = vec4(1.0, 0.0, 0.0, alpha); //uncomment for red squares
    if(texIndex >= 3.0){
        gl_FragColor = texture2D(texD, gl_PointCoord);
    }
    else if(texIndex >= 2.0){
        gl_FragColor = texture2D(texC, gl_PointCoord);
    }
    else{
        gl_FragColor = texture2D(texA, gl_PointCoord) * vec4(kd, 1.0);
    }


    
    //gl_FragColor = texture2D(tex, texCoords);
}