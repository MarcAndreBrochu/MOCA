#version 330

uniform mat4 mvpMatrix;

in vec3 vertex;
in vec3 color;

out vec3 varyingColor;

void main(void) {
    gl_Position.xyz = vertex;
    gl_Position.w = 1.0;
    gl_Position = mvpMatrix * gl_Position;
    varyingColor.rgb = color;
}
