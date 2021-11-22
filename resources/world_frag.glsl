#version 120
uniform vec3 kd;
varying vec4 vPos;
void main()
{
	float height = vPos.y; //range is -97 to +97 with the randomized noise?
	vec3 bottomColor = vec3(0.0, 0.0, 0.4);
	vec3 topColor = vec3(0.0, 0.5, 0.0);

	float alpha = (45 - height)/90;

	vec3 color = alpha * bottomColor + (1 - alpha) * topColor;

	gl_FragColor = vec4(color, 1.0);

}
