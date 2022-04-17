#version 460 core

// All of the following variables could be defined in the OpenGL
// program and passed to this shader as uniform variables. This
// would be necessary if their values could change during runtim.
// However, we will not change them and therefore we define them 
// here for simplicity.

vec3 I = vec3(1, 1, 1);          // point light intensity
vec3 Iamb = vec3(0.8, 0.2, 0.8); // ambient light intensity
vec3 kd = vec3(1.0, 0.0, 1.0);     // diffuse reflectance coefficient
vec3 ka = vec3(0.3, 0.3, 0.3);   // ambient reflectance coefficient
vec3 ks = vec3(0.8, 0.8, 0.8);   // specular reflectance coefficient
vec3 lightPos1 = vec3(0, 0, -20);   // light position in world coordinates
vec3 lightPos2 = vec3(3, 3, 3);   // light position in world coordinates
vec3 lightPos3 = vec3(1, 23, 5);   // light position in world coordinates

uniform mat4 modelingMatrix;
uniform mat4 viewingMatrix;
uniform mat4 projectionMatrix;
uniform vec3 eyePos;

layout(location=0) in vec3 inVertex;
layout(location=1) in vec3 inNormal;
layout(location=2) in vec2 inTexCoord;

out vec2 outTexCoord;



out vec4 color;



vec3 CalcPointLight(vec3 lightPos)
{
    
	vec4 pWorld = modelingMatrix * vec4(inVertex, 1);
	vec3 nWorld = inverse(transpose(mat3x3(modelingMatrix))) * inNormal;

	// Compute lighting. We assume lightPos and eyePos are in world
	// coordinates.

	vec3 L = normalize(lightPos - vec3(pWorld));
	vec3 V = normalize(eyePos - vec3(pWorld));
	vec3 H = normalize(L + V);
	vec3 N = normalize(nWorld);

	float NdotL = dot(N, L); // for diffuse component
	float NdotH = dot(N, H); // for specular component

	vec3 diffuseColor = I * kd * max(0, NdotL) * 0.3;
	vec3 specularColor = I * ks * pow(max(0, NdotH), 100);
	//vec3 ambientColor = Iamb * ka;

	return diffuseColor + specularColor;
} 
void main(void)
{
	// First, convert to world coordinates. This is where
	// lighting computations must be performed. inVertex
	// is NOT in homogeneous coordinates. inNormal has three
	// components. For computing the normal transformation
	// matrix we use the upper 3x3 part of the modeling
	// matrix.
	 
	
	vec3 ambientColor = Iamb * ka;


	vec3 total = ambientColor;
	total += CalcPointLight(lightPos1);
	//total += CalcPointLight(lightPos2);
	//total += CalcPointLight(lightPos3);

	//outTexCoord = vec2(inTexCoord.x,1-inTexCoord.y);

	// We update the front color of the vertex. This value will be sent
	// to the fragment shader after it is interpolated at every fragment.
	// Front color specifies the color of a vertex for a front facing
	// primitive.

	color = vec4(total, 1);

	// Transform the vertex with the product of the projection, viewing, and
	// modeling matrices.

    gl_Position = projectionMatrix * viewingMatrix * modelingMatrix * vec4(inVertex, 1);
	//gl_Position = vec4(inVertex, 1);
}

