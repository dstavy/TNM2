#version 330

// this is how we receive the texture
uniform sampler2DRect tex0;
uniform float factor;
uniform float alpha;

in vec2 texCoordVarying;

out vec4 outputColor;

vec4 Sepia( in vec4 color )
{
    vec3 c1 = vec3(0.393, 0.769, 0.189);
    vec3 c2 = vec3(0.349, 0.686, 0.168);
    vec3 c3 = vec3(0.272, 0.534, 0.131);

    return vec4(clamp(color.r * c1.x + color.g * c1.y + color.b * c1.z, 0.0, 1.0),
                clamp(color.r * c2.x + color.g * c2.y + color.b * c2.z, 0.0, 1.0),
                clamp(color.r * c3.x + color.g * c3.y + color.b * c3.z, 0.0, 1.0),
                color.a * alpha);
}

void main (void){
    outputColor = texture(tex0, texCoordVarying);
    outputColor = mix(outputColor, Sepia(outputColor), clamp(factor,0.0,1.0) );
}
