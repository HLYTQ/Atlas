#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

out vec2 TexCoord;
out vec3 FragPos;  
out vec3 Normal;
out mat3 TBN;


uniform mat4 Transform;
uniform mat4 viewProjection;

void main()
{
	TexCoord = a_TexCoord;
	FragPos = vec3(Transform * vec4(a_Position, 1.0));
	Normal = a_Normal;

	vec3 T = normalize(vec3(Transform * vec4(a_Tangent,   0.0)));
	vec3 B = normalize(vec3(Transform * vec4(a_Bitangent, 0.0)));
	vec3 N = normalize(vec3(Transform * vec4(a_Normal,    0.0)));

	TBN = mat3(T, B, N);

	gl_Position = viewProjection * Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core

layout(location = 0) out vec4 a_Color;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

struct Light {
	vec3 Color;
	vec3 Position;
	vec3 viewPos;
};

struct MaterialUniforms {
	vec3  AlbedoColor;
	float Metalness;
	float Roughness;

	bool UseNormalMap;
};

uniform sampler2D u_AlbedoTexture;
uniform	sampler2D u_NormalTexture;
uniform	sampler2D u_RoughnessTexture;
uniform	sampler2D u_MetalnessTexture;

uniform MaterialUniforms u_MaterialUniforms;
uniform Light u_Light;

vec4 AmbientLight() {
	vec3 ambient = u_MaterialUniforms.Roughness * u_Light.Color;
	return vec4(ambient, 1.0);
}

vec4 DiffuseLight(vec3 lightDir) {
	vec3 norm;
	if (u_MaterialUniforms.UseNormalMap) {
		norm = texture(u_NormalTexture, TexCoord).rgb;
		norm = normalize(norm * 2.0 - 1.0);
		norm = normalize(TBN * norm);
	} else {
		norm = normalize(Normal);
	}
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_Light.Color;
	return vec4(diffuse, 1.0);
}

vec4 SpecularLight(vec3 lightDir) {
	vec3 viewDir = normalize(u_Light.viewPos - FragPos);
	vec3 norm;
	if (u_MaterialUniforms.UseNormalMap) {
		norm = texture(u_NormalTexture, TexCoord).rgb;
		norm = normalize(norm * 2.0 - 1.0);
	} else {
		norm = normalize(Normal);
	}
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	// vec3 specular = u_MaterialUniforms.Metalness * spec * u_Light.Color;
	vec3 specular = 0.5 * spec * u_Light.Color;

	return vec4(specular, 1.0);
}

void main()
{
	vec3 lightDir = normalize(u_Light.Position - FragPos);
	a_Color = (AmbientLight() + DiffuseLight(lightDir) + SpecularLight(lightDir)) * texture(u_AlbedoTexture, TexCoord);
}
