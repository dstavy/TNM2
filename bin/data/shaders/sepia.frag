#version 330

// this is how we receive the texture
uniform sampler2DRect tex0;
uniform float factor;
uniform float alpha;

in vec2 texCoordVarying;

out vec4 outputColor;

#define desat 0.92
#define sepia vec3(0.70,0.66,0.50)
//#define sepia vec3(0.66,0.62,0.54) grey one
//#define halfsies

void main(void)
{
    vec4 ch0 = texture(tex0, texCoordVarying);
    
#ifdef halfsies
    
    if (uv.x < 0.5){
        outputColor = ch0;
        return;
    }
    
#endif
    
    outputColor = ch0 * 1;// add some grit, overexposure
    float lum = (outputColor.r + outputColor.g + outputColor.b) /2.2;
    outputColor.rgb = outputColor.rgb * (1.0 - desat) + sepia * vec3(lum,lum,lum) * desat;
    outputColor.a = alpha;
}
