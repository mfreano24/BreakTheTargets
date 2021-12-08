#version 120

uniform vec2 h;

varying vec3 vPos;
varying vec3 fragPos;
varying vec3 fragNor;

void main()
{
	float diff = h.y - h.x;

	float alpha = (vPos.y - h.x) / diff;
	float mountain_grassborder = 0.45;
	vec3 kd = vec3(0.0, 0.3, 0.0);
	if(alpha >= 0.675){
		kd = vec3(212.0/255.0, 206.0/255.0, 199.0/255.0);
	}
	else if(alpha >= mountain_grassborder){
		kd = vec3(77.0/255.0, 52.0/255.0, 18.0/255.0);
	}
	else if(alpha >= 0.1){
		float dist = (1.0 - (mountain_grassborder - alpha)) / 2.0;
		
		kd = vec3(0.0, dist, 0.0);
	}

	//gl_FragColor = vec4(color, 1.0);

	vec3 n = normalize(fragNor);
	vec3 lightPos = vec3(0.0, 5.0, 0.0);
	vec3 l = normalize(lightPos - fragPos);
	vec3 e = -normalize(fragPos);
	vec3 h = normalize(l + e);
	vec3 colorA = kd - vec3(0.1, 0.1, 0.1);
	vec3 colorD = max(dot(l, n), 0.0) * kd;
	vec3 colorS = pow(max(dot(h, n), 0.0), 200.0) * vec3(0.8, 0.8, 0.4);
	vec3 color = colorA + colorD + colorS;
	gl_FragColor = vec4(color.r, color.g, color.b, 1.0);



}
