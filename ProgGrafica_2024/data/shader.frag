#version 330

struct material_t {
	float Ka, Kd, Ks;
	int shiny;
	int textureEnabled;
};

uniform material_t material;
uniform sampler2D textureColor;

in vec4 fColor;
in vec3 fPos;
in vec3 fNormal;
in vec2 fUv;

void main() {
	vec4 finalColor = fColor;

	if (material.textureEnabled == 1)
	{
		finalColor = texture(textureColor, fUv);
		//finalColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	gl_FragColor = finalColor;
}