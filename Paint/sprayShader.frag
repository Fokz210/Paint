#extension GL_EXT_gpu_shader4 : enable

uniform vec2 coords;
uniform float thickness;
uniform vec4 color;
uniform sampler2D texture;
uniform int random;
uniform vec2 textureSize;

int rand(vec2 co);

void main()
{
	vec2 textureCoordinates = gl_TexCoord[0].xy;
	vec4 pixel = texture2D(texture, textureCoordinates);
	
	float range = length(textureCoordinates - vec2(coords.x / textureSize.x, coords.y / textureSize.y));

	if (range < thickness / textureSize.x) gl_FragColor = color;
	else gl_FragColor = pixel;
}

int rand(vec2 co)
{
    return int(fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453));
}