#version 330

uniform mat4 MVP;
//uniform mat4 M;

attribute vec4 vPos;
attribute vec4 vColor;
attribute vec4 vNormal;

out vec4 fColor;
//out vec4 fPos; // Pos Absoluta
//out vec4 fNormal; // Normal Absoluta

void main() {
	//fPos = M * vPos;
	//fNormal = inverse(transpose(M)) * vNormal;
	gl_Position = MVP * vPos;
	fColor = vColor;
}