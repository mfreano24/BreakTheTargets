#version 120
uniform sampler2D tex;
void main()
{
	gl_FragColor = texture2D(tex, gl_PointCoord);
	
}
