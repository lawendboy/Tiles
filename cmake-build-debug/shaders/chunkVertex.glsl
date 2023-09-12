#version 330 core
uniform mat4 viewMatrix;
uniform vec2 chunkVector;
uniform vec2 playerVector;

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 txtCrd;
layout (location = 2) in int txtId;

flat out int TextureId;
out vec2 TexCoord;

void main() {

    TextureId = txtId;

    TexCoord = txtCrd;

    gl_Position = viewMatrix * vec4(
        inPosition + vec2(
            (gl_InstanceID % 32) * 0.1,
            (gl_InstanceID / 32) * -0.1

        ) + chunkVector + playerVector,
        0.0, 1.0);
}