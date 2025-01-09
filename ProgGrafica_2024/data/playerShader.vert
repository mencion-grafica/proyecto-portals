#version 330 core

const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 4;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 jointTransforms[MAX_JOINTS];

in vec4 vPos;          
in vec4 vColor;        
in vec4 vNormal;       
in vec4 vUv;           
in ivec4 jointIndex;   
in vec4 weightJoints;  

out vec4 fColor;
out vec3 fPos;
out vec3 fNormal;
out vec2 fUv;

void main() {
    vec4 totalPos = vec4(0.0);
    vec3 totalNormal = vec3(0.0);

    for (int i = 0; i < MAX_WEIGHTS; i++) {
        if (jointIndex[i] == -1) {
            break;
        }
        if (jointIndex[i] >= MAX_JOINTS) {
            totalPos = vPos;
            break;
        }

        vec4 localPosition = (jointTransforms[jointIndex[i]] * vPos);
        totalPos += localPosition * weightJoints[i];

        mat3 normalMatrix = mat3(jointTransforms[jointIndex[i]]);
        vec3 localNormal = normalize(normalMatrix * vNormal.xyz);
        totalNormal += localNormal * weightJoints[i];
    }

    fPos = (M * totalPos).xyz;
    fNormal = normalize(mat3(inverse(transpose(M))) * totalNormal);

    fColor = vec4(totalPos.xyz, 1.0);
    fUv = vUv.xy;

    gl_Position = MVP * totalPos;
}
