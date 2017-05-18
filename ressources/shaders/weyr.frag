#version 330

// Input: tangent space matrix, position (view space) and uv coming from the vertex shader
in INTERFACE {
    mat3 tbn;
	vec3 position;
	vec3 objectColor;
	vec2 uv;
} In ;

// Uniform: the light structure (position in view space)
layout (std140) uniform Light {
  vec4 position;
  vec4 Ia;
  vec4 Id;
  vec4 Is;
  float shininess;
} light;


uniform sampler2D textureColor;
uniform sampler2D textureNormal;
uniform sampler2D textureEffects;
uniform bool useTexture;

// Output: the fragment color
out vec3 fragColor;

// Returns a random float in [0,1] based on the input vec4 seed.
float random(vec4 p){
	return fract(sin(dot(p, vec4(12.9898,78.233,45.164,94.673))) * 43758.5453);
}


void main(){
	// Compute the normal at the fragment using the tangent space matrix and the normal read in the normal map.
	vec3 n = texture(textureNormal,In.uv).rgb;
	n = normalize(n * 2.0 - 1.0);
	n = normalize(In.tbn * n);

	// Read the effects values
	vec3 effects = texture(textureEffects,In.uv).rgb;

	// Compute the direction from the point to the light
	// light.position.w == 0 if the light is directional, 1 else.
	vec3 l = normalize(light.position.xyz - light.position.w * In.position);
	vec3 diffuseColor = texture(textureColor, In.uv).rgb;
	
	if(useTexture){
		float luma = dot(diffuseColor, vec3(0.2126, 0.7152, 0.0722));
		diffuseColor = mix(In.objectColor, vec3(luma), 0.25);
	}
	// The ambient factor
	vec3 ambient = effects.r * 0.3 * diffuseColor;
	
	// Compute the diffuse factor
	float diffuse = max(0.0, dot(l,n));

	vec3 v = normalize(-In.position);

	// Compute the specular factor
	float specular = 0.0;
	if(diffuse > 0.0){
		vec3 r = reflect(-l,n);
		specular = pow(max(dot(r,v),0.0),light.shininess);
		specular *= effects.g;
	}
	
	vec3 lightShading = diffuse * diffuseColor + specular * light.Is.rgb;
	
	
	fragColor = ambient * light.Ia.rgb +  lightShading;
}
