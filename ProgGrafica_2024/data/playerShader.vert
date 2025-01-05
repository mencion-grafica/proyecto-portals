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
	vec4 totalLocalPos = vec4(0.0);
	vec4 totalNormal = vec4(0.0);

	for(int i = 0; i < MAX_WEIGHTS; i++){
		if(jointIndex[i] != -1){
			mat4 jointTransform = jointTransforms[jointIndex[i]];
			vec4 posePosition = jointTransform * vPos;
			totalLocalPos += posePosition * weightJoints[i];

			mat4 normalMatrix = transpose(inverse(jointTransform));
            vec4 worldNormal = normalMatrix * vec4(vNormal.xyz, 0.0);
            totalNormal += worldNormal * weightJoints[i];
		}
	}

	fPos = (M * totalLocalPos).xyz;
	fNormal = normalize((inverse(transpose(M)) * totalNormal).xyz);
	
	fColor = vColor;
	fUv = vUv.xy;

	gl_Position = MVP * totalLocalPos;
}