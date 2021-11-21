#version 120
uniform vec3 kd;
varying vec4 vPos;
void main()
{
	float height = vPos.y + 0.1;
	vec4 baseColor = vec4(0.2, 0.1, 0.0, 1.0);
	vec4 topColor = vec4(0.0, 0.7, 0.3, 1.0);
	gl_FragColor = height * topColor + (1 - height) * baseColor;
	

	if(height >= 0.98){
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
	else if(height >= 0.65){
		gl_FragColor = vec4(0.2, 0.1, 0.0, 1.0);
	}
	else if(height >= 0.05){
		gl_FragColor = vec4(0.0, 0.75, 0.0, 1.0);
	}
	else if(height >= 0.015){
		gl_FragColor = vec4(0.94, 0.9175, 0.612, 1.0);
	}
	else{
		gl_FragColor = vec4(0.0, 0.0, 0.85, 1.0);
	}
}
