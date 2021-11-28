#version 120
uniform vec2 h;
varying vec4 vPos;
void main()
{
	float diff = h.y - h.x;

	float alpha = (vPos.y - h.x) / diff;
	
	//use alpha to calculate color by step
	vec3 color = vec3(0.05, 0.0, 0.2); //deep blue
	if(alpha >= 0.6){
		//grass
		color = vec3(0.0, 0.4, 0.0);
	}
	else if(alpha >= 0.55){
		//beach
		color = vec3(0.902, 0.847, 0.494); //sand
	}
	else if(alpha >= 0.2){
		color = vec3(0.0, 0.0, 0.5);
	}

	gl_FragColor = vec4(color, 1.0);

}
