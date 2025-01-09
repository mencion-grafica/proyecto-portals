#version 330 core

const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 4;

uniform mat4 MVP; // Model-View-Projection matrix
uniform mat4 M;   // Model matrix
uniform mat4 jointTransforms[MAX_JOINTS]; // Transformations for joints

in vec4 vPos;          // Vertex position
in vec4 vColor;        // Vertex color
in vec4 vNormal;       // Vertex normal
in vec4 vUv;           // Vertex UV
in ivec4 jointIndex;   // Indices of joints affecting this vertex
in vec4 weightJoints;  // Weights of the joints affecting this vertex

out vec4 fColor;
out vec3 fPos;
out vec3 fNormal;
out vec2 fUv;

void main() {
    vec4 totalPos = vec4(0.0);
    vec3 totalNormal = vec3(0.0);

    // Iterate over the weights and joints
    for (int i = 0; i < MAX_WEIGHTS; i++) {
        if (jointIndex[i] == -1) {
            break;
        }
        if (jointIndex[i] >= MAX_JOINTS) {
            totalPos = vPos;
            break;
        }

        // Apply joint transformation to the vertex position
        vec4 localPosition = (jointTransforms[jointIndex[i]] * vPos);
        totalPos += localPosition * weightJoints[i];

        // Apply joint transformation to the vertex normal
        mat3 normalMatrix = mat3(jointTransforms[jointIndex[i]]);
        vec3 localNormal = normalize(normalMatrix * vNormal.xyz);
        totalNormal += localNormal * weightJoints[i];
    }

    // Final position and normal
    fPos = (M * totalPos).xyz;
    fNormal = normalize(mat3(inverse(transpose(M))) * totalNormal);

    // Output final color and UV
    fColor = vec4(totalPos.xyz, 1.0);
    fUv = vUv.xy;

    // Final clip space position
    gl_Position = MVP * totalPos;
}