#version 330

// Attributes
layout(location = 0) in vec3 v;
layout(location = 1) in vec3 n;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tang;
layout(location = 4) in vec3 binor;
layout(location = 5) in vec3 objectV;
layout(location = 6) in vec3 objectN;
layout(location = 7) in vec2 objectUV;

// Uniform: the VP, V and normal matrices
uniform mat4 vp;
uniform mat4 view;
uniform float scale;
uniform sampler2D textureObject;
uniform bool useTexture;
// Output: tangent space matrix, position in view space and uv.
out INTERFACE {
    mat3 tbn;
	vec3 position;
	vec3 objectColor;
	vec2 uv;
} Out ;


void main(){

	mat4 m = mat4(scale);
	m[3][3] = 1.0;
	
	// We want the surface normal to correspond to the Z+ axis of the primitive.
	vec3 objectNormal = normalize(objectN);
	vec3 primitiveAxis = normalize(vec3(0.0,0.0,1.0));
	// Avoid the case where the two vectors are opposed.
	if(primitiveAxis == -objectNormal){
		primitiveAxis.x += 0.2;
		primitiveAxis = normalize(primitiveAxis) ;
	}
	// Rotation axis.
	vec3 axis = cross(objectNormal,primitiveAxis);
	// Build cross matrix.
	mat3 crossMatrix = mat3(0.0);
	crossMatrix[0].y = axis.z;
	crossMatrix[0].z = -axis.y;
	crossMatrix[1].x = -axis.z;
	crossMatrix[1].z = axis.x;
	crossMatrix[2].x = axis.y;
	crossMatrix[2].y = -axis.x;
	// Rodrigues formula.
	mat3 r33 = mat3(1.0) + crossMatrix + 1.0/(1.0 + dot(objectNormal,primitiveAxis)) * crossMatrix * crossMatrix ;
	// Build the rotation matrix.
	mat4 r = mat4(r33);
	// Model matrix.
	m = r * m;
	// Add translation.
	m[3].xyz = objectV;
	
	// We multiply the coordinates by the MVP matrix, and ouput the result.
	gl_Position = vp * m * vec4(v, 1.0);
	
	mat4 mv = view * m;
	
	Out.position = (mv * vec4(v,1.0)).xyz;
	
	Out.uv = uv;
	//Out.objectNormal = normalize(transpose(inverse(mat3(view))) * objectV);
	
	mat3 normalMatrix = transpose(inverse(mat3(mv)));
	// Compute the TBN matrix (from tangent space to view space).
	vec3 T = normalize(normalMatrix * tang);
	vec3 B = normalize(normalMatrix * binor);
	vec3 N = normalize(normalMatrix * n);
	Out.tbn = mat3(T, B, N);
	
	if(useTexture){
		Out.objectColor = texture(textureObject, objectUV).rgb;
	}
	
}
