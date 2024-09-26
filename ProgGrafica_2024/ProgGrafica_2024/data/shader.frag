#version 330

#define MAX_LIGHTS 4 // Define el máximo número de luces que deseas soportar

struct light_t {
	vec4 pos[MAX_LIGHTS];
	vec4 color[MAX_LIGHTS];
	vec4 direction[MAX_LIGHTS];
	float Ia[MAX_LIGHTS];
	float Id[MAX_LIGHTS];
	float Is[MAX_LIGHTS];
	int type[MAX_LIGHTS];
	int enable[MAX_LIGHTS];
};

struct material_t {
	float Ka, Kd, Ks;
	int shinny;
	int textureEnable;
};

uniform light_t lights; // Ahora light es un arreglo
uniform vec4 eyePos;
uniform material_t material;
uniform sampler2D textureColor;

in vec4 fColor;
in vec3 fPos;
in vec3 fNormal;
in vec2 fUv;

void main() {
	vec4 finalColor = fColor;

	if(material.textureEnable == 1) {
		finalColor = fColor * texture(textureColor, fUv);
	}

	float ambiental = 0;
	float diffuse = 0;
	float specular = 0;

	for (int i = 0; i < MAX_LIGHTS; ++i) {
		if (lights.enable[i] == 1) {
			vec3 L = vec3(0, 0, 0);
			vec3 N = fNormal;

			if (lights.type[i] == 0)
				L = lights.pos[i].xyz - fPos;
			if (lights.type[i] == 1)
				L = lights.direction[i].xyz;

			L = normalize(L);
			N = normalize(N);

			diffuse += lights.Id[i] * material.Kd * max(dot(L, N), 0);

			vec3 R = normalize(reflect(L, N));
			vec3 V = normalize(fPos - eyePos.xyz);
			specular += lights.Is[i] * material.Ks * pow(max(dot(R, V), 0), material.shinny);
		}
	}

	ambiental = material.Ka; // La luz ambiental no depende de la luz

	gl_FragColor = finalColor * (ambiental + diffuse + specular);
}
