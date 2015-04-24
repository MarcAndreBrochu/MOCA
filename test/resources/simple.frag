#version 330

in vec3 varyingColor;

out vec3 fragColor;

void main(void) {
    fragColor = varyingColor;
}
