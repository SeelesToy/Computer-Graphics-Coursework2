#version 450 core

layout (location = 0) out vec4 fColour;

in vec4 col;
in vec3 nor;
in vec3 FragPosWorldSpace;
in vec4 FragPosProjectedLightSpace;
in vec2 UV;

uniform sampler2D shadowMap;
uniform sampler2D diffuseTexture;
uniform bool useTexture = false;

uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform vec3 camPos;

#define SH_MAP_WIDTH 2048


float shadowOnFragment(vec4 FragPosProjectedLightSpace)
{
	vec3 ndc = FragPosProjectedLightSpace.xyz / FragPosProjectedLightSpace.w;
	vec3 ss = (ndc + 1) * 0.5;

	float fragDepth = ss.z;

	float shadow = 0.f;
	float samples = 0.f;
	float texelSize = 1.0 / SH_MAP_WIDTH;
	

	float litDepth = texture(shadowMap, ss.xy).r;
	vec3 Nnor = normalize(nor);
	vec3 Ntolight = normalize(-lightDirection);
	float bias = max(0.05 * (1.0 - dot(Nnor, Ntolight)), 0.005);

	for(int x=-1; x<=1; ++x){
		for(int y=-1; y<=1; ++y){
			float d = texture(shadowMap, ss.xy + vec2(x,y)*texelSize).r;
			shadow += fragDepth - bias > d ? 1.0 : 0.0;
			samples += 1.0;
		}
	}
	shadow /= samples;
	//shadow = fragDepth > (litDepth + bias) ? 1.0f : 0.0f;

	if(fragDepth > 1)
		shadow = 0.f;
	

	return shadow;
}

float CalculateDirectionalIllumination() {
	float ambient = 0.1f;
	vec3 Nnor = normalize(nor);
	vec3 Nto_light = normalize(-lightDirection);
	float diffuse = dot(Nnor, Nto_light);

	vec3 Nform_light = normalize(lightDirection);
	vec3 NrefLight = reflect(Nform_light, Nnor);
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);
	float specular = pow(abs(dot(NcamDirection, NrefLight)), 128);

	float shadow = shadowOnFragment(FragPosProjectedLightSpace);
	float phong = ambient + ((1.f - shadow) * (diffuse + specular));

	return phong;
}

float CalculatePositionalIllumination(){
	float ambient = 0.1f;
	vec3 Nnor = normalize(nor);
	vec3 Nto_light = normalize(lightPos - FragPosWorldSpace);
	float diffuse = dot(Nnor, Nto_light);

	vec3 Nform_light = - Nto_light;
	vec3 NrefLight = reflect(Nform_light, Nnor);
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);
	float specular = pow(abs(dot(NcamDirection, NrefLight)), 128);

	float attc = 1.0f;
	float attl = 0.05f;
	float attq = 0.002f;
	float d = distance(lightPos, FragPosWorldSpace);
	float attenuation = 1/(attc + attl * d + attq * pow(d, 2));

	return (ambient + diffuse + specular) * attenuation;
}

void main()
{
	float phong = CalculateDirectionalIllumination();
	if(useTexture){
		vec3 texcol = texture(diffuseTexture, UV).rgb;       // ²ÉÑù
		vec3 baseColor = texcol;
		fColour = vec4(phong * baseColor * lightColour, 1.f);
	}
	else{
		vec3 baseColor = col.rgb;
		fColour = vec4(phong * baseColor * lightColour, col.a);
	}
	


}
