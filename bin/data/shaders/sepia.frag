#version 330

// this is how we receive the texture
uniform sampler2DRect tex0;
uniform float factor;
uniform float alpha;

in vec2 texCoordVarying;

out vec4 outputColor;

vec4 Sepia( in vec4 color) {

    vec3 c1 = vec3(100.0, 196.0, 48.0);
    vec3 c2 = vec3(89.0, 175.0, 43.0);
    vec3 c3 = vec3(69.0, 136.0, 33.0);

    return vec4(clamp(color.r * c1.x/255.0 + color.g * c1.y/255.0 + color.b * c1.z/255.0, 0.0, 1.0),
                clamp(color.r * c2.x/255.0 + color.g * c2.y/255.0 + color.b * c2.z/255.0, 0.0, 1.0),
                clamp(color.r * c3.x/255.0 + color.g * c3.y/255.0 + color.b * c3.z/255.0, 0.0, 1.0),
                color.a * alpha);
}

void main (void) {
    outputColor = texture(tex0, texCoordVarying);
    outputColor = mix(outputColor, Sepia(outputColor), clamp(factor,0.0,1.0) );
}
