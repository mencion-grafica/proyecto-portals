#version 330

uniform sampler2D textureColor;

in vec4 fColor;
in vec3 fPos;
in vec3 fNormal;
in vec2 fUv;

void main() {
	vec4 finalColor = fColor;

	gl_FragColor = finalColor;
}