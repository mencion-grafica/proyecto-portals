#version 330

struct light_t {
	vec4 position;
	vec4 color;
	vec4 direction;
	float Ia;
	float Id;
	float Is;
	int type;
	int enable;
};

struct material_t {
	float Ka, Kd, Ks;
	int shiny;
	int textureEnabled;
};

uniform light_t light;
uniform vec4 eyePos;
uniform material_t material;
uniform sampler2D textureColor;

in vec4 fColor;
in vec3 fPos;
in vec3 fNormal;
in vec2 fUv;

vec4 calculateColor(light_t light) {
	vec4 finalColor = fColor;

   	if (material.textureEnabled == 1) {
		finalColor = fColor * texture(textureColor, fUv);
	}
	
	float ambiental = 1;
	float diffuse = 1;
	float specular = 1;

	if (light.enable == 1) {
		ambiental = light.Ia * material.Ka;
		
		vec3 L = vec3(0, 0, 0);
		vec3 N = fNormal;

		if (light.type == 0) L = light.position.xyz - fPos;
		if (light.type == 1) L = light.direction.xyz;

		L = normalize(L);
		N = normalize(N);
		
		diffuse = light.Id * material.Kd * max(dot(L, N), 0);

		vec3 R = normalize(reflect(L, N));
		vec3 V = normalize(fPos - eyePos.xyz);

		specular = light.Is * material.Ks * pow(max(dot(R, V), 0), material.shiny);
	}

	return finalColor * ((ambiental + diffuse + specular) * light.color); 
}

void main() {
	gl_FragColor = calculateColor(light);
}