#version 330

// this is how we receive the texture
uniform sampler2DRect tex0;
uniform float factor;
uniform float alpha;

in vec2 texCoordVarying;

out vec4 outputColor;

//vec4 Sepia( in vec4 color) {
//
//    //    vec3 c1 = vec3(100.0, 196.0, 48.0);
//    //    vec3 c2 = vec3(89.0, 175.0, 43.0);
//    //    vec3 c3 = vec3(69.0, 136.0, 33.0);
//
//    vec3 c1 = vec3(116.0*0.9, 191.0*0.7, 45.0*0.7);
//    vec3 c2 = vec3(105.0*0.9, 170.0*0.7, 40.0*0.7);
//    vec3 c3 = vec3(86.0*0.9, 131.0*0.7, 30.0*0.7);
//    vec3 gray = vec3(229.0, 221.0, 208.0);
////    vec3 gray = vec3(221.0, 215.0, 206.0);
//
//    vec4 sepia = vec4(clamp(color.r * c1.x/255.0 + color.g * c1.y/255.0 + color.b * c1.z/255.0, 0.0, 1.0),
//                clamp(color.r * c2.x/255.0 + color.g * c2.y/255.0 + color.b * c2.z/255.0, 0.0, 1.0),
//                clamp(color.r * c3.x/255.0 + color.g * c3.y/255.0 + color.b * c3.z/255.0, 0.0, 1.0),
//                color.a * alpha);
//
//    return vec4(clamp(sepia.r * gray.r/255.0, 0.0, 1.0),
//                clamp(sepia.g * gray.g/255.0, 0.0, 1.0),
//                clamp(sepia.b * gray.b/255.0, 0.0, 1.0),
//                sepia.a);
//}
//
//void sepia1( inout vec3 color, float adjust ) {
////    color.r = min(1.0, (color.r * (1.0 - (0.607 * adjust))) + (color.g * (0.769 * adjust)) + (color.b * (0.189 * adjust)));
////    color.g = min(1.0, (color.r * (0.349 * adjust)) + (color.g * (1.0 - (0.314 * adjust))) + (color.b * (0.168 * adjust)));
////    color.b = min(1.0, (color.r * (0.272 * adjust)) + (color.g * (0.534 * adjust)) + (color.b * (1.0 - (0.869 * adjust))));
//
//    color.r = min(1.0, (color.r * (1.0 - (0.607 * adjust))) + (color.g * (0.769 * adjust)) + (color.b * (0.189 * adjust)));
//    color.g = min(1.0, (color.r * (0.349 * adjust)) + (color.g * (1.0 - (0.314 * adjust))) + (color.b * (0.168 * adjust)));
//    color.b = min(1.0, (color.r * (0.272 * adjust)) + (color.g * (0.534 * adjust)) + (color.b * (1.0 - (0.869 * adjust))));
//}
//
//void main (void) {
//    outputColor = texture(tex0, texCoordVarying);
//    outputColor = mix(outputColor, Sepia(outputColor), clamp(factor,0.0,1.0) );
//
////    vec3 color = texture(tex0, texCoordVarying).rgb;
////    sepia1(color, 1.0);
////    outputColor = vec4(color, 1.0);
//}

#define boxradius 0.9
#define boxdarkness 0.95
#define desat 0.95
#define black vec3(0.32,0.3,0.28)
#define sepia vec3(0.84,0.76,0.62)

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
    
    outputColor = ch0 * 1.02;// add some grit, overexposure
//    outputColor = ch0 * 0.98;// add some grit, overexposure
//    outputColor = ch0 * ch0 * 1.4;// add some grit, overexposure
    float lum = (outputColor.r + outputColor.g + outputColor.b) /3.0;
    outputColor.rgb = outputColor.rgb * (1.0 - desat) + sepia * vec3(lum,lum,lum) * desat;
    
//    // shadowbox it
//    float r = distance(vec2(0.5,0.5),texCoordVarying)*2.0;
//    // r is 0 to 1 for the radius from center
//    if (r > boxradius){
//        outputColor.rgb = mix( outputColor.rgb, black, (r - boxradius)*boxdarkness);
//    }
}
