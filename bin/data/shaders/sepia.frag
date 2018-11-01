#version 330

// this is how we receive the texture
uniform sampler2DRect tex0;
uniform float factor;

in vec2 texCoordVarying;

out vec4 outputColor;

vec4 Sepia( in vec4 color )
{
    return vec4(
                clamp(color.r * 0.393 + color.g * 0.769 + color.b * 0.189, 0.0, 1.0)
                , clamp(color.r * 0.349 + color.g * 0.686 + color.b * 0.168, 0.0, 1.0)
                , clamp(color.r * 0.272 + color.g * 0.534 + color.b * 0.131, 0.0, 1.0)
                , color.a
                );
}

void main (void){
    outputColor = texture(tex0, texCoordVarying);
    outputColor = mix(outputColor, Sepia(outputColor), clamp(factor,0.0,1.0) );
}
