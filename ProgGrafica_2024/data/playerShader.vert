#version 330

const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 4;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 jointTransforms[MAX_JOINTS];

attribute vec4 vPos;
attribute vec4 vColor;
attribute vec4 vNormal;
attribute vec4 vUv;
attribute ivec4 jointIndex;
attribute vec4 weightJoints;

out vec4 fColor;
out vec3 fPos;
out vec3 fNormal;
out vec2 fUv;

void main() {
	for(int i = 0; i < MAX_WEIGHTS; i++){
		if(jointIndex[i] != -1){
			mat4 jointTransform = jointTransforms[jointIndex[i]];
			vec4 posePosition = jointTransform * vPos;
		}
	}

	fPos = (M * vPos).xyz;
	fNormal = (inverse(transpose(M)) * vNormal).xyz;
	fNormal = normalize(fNormal);
	
	fColor = vColor;
	fUv = vUv.xy;

	gl_Position = MVP * vPos;
}