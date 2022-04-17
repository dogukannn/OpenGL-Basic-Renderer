#version 460 core

in vec4 color;
in vec2 outTexCoord;
out vec4 fragColor;



void main(void)
{
	// Set the color of this fragment to the interpolated color
	// value computed by the rasterizer.

	//fragColor =  0.5 * color + texture(ourTexture, outTexCoord);
	fragColor = vec4(1,1,1,1);
	//fragColor = vec4(outTexCoord.x, outTexCoord.y,1,1);
}
