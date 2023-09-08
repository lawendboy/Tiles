#version 330 core

in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D textureSampler; // The texture atlas
flat in int TextureId; // The texture ID

void main() {
    // Calculate the texture coordinates for the selected texture
    float atlasWidth = 1.0 / 32.0;  // Assuming 4 textures horizontally
    float atlasHeight = 1.0 / 32.0; // Assuming 4 textures vertically
    float xOffset = float(TextureId % 32) * atlasWidth;
    float yOffset = float(TextureId / 32) * atlasHeight;

    vec2 atlasCoords = vec2(TexCoord.x * atlasWidth + xOffset, -TexCoord.y * atlasHeight + yOffset);

    // Sample the texture from the atlas using the calculated coordinates
    FragColor = texture(textureSampler, atlasCoords);
//    FragColor = vec4(TextureId);
//    FragColor = vec4(0.5);
}