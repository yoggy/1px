uniform int    idx;
uniform ivec2  resolution;

void main()
{
	vec2 p = gl_FragCoord.xy / resolution.xy;
	vec4 c = vec4(0, 0, 0, 1);

	vec2 center = vec2(0.501, 0.501);
	
	float l = length(p - center);
	
	if (l <= 0.003) {
		switch(idx) {
			case 0:
				c = vec4(1, 0, 0, 1);
				break;
			case 1:
				c = vec4(0, 1, 0, 1);
				break;
			case 2:
				c = vec4(0, 0, 1, 1);
				break;
			default:
				break;
		}
	}
	gl_FragColor = c;
}