#version 430 core

vec3 L, N, H, E;
uniform mat4 M;
in mat4 ModelView;
in vec3 pos;
uniform vec3 light_position[2];
smooth in vec3 outVert;
smooth in vec3 outNormal;
smooth in vec2 outUV;
in vec4 ShadowCoord;


// this is set to the spec map (texture unit 1)
uniform sampler2D depthTex;


layout (location=0) out vec4 fragColour;

struct DirectionalLight
{
    vec4 ambient;
    vec3 position;
    float intensity;
    vec4 colour;
};


uniform DirectionalLight top_light = {vec4(0.2), vec3(0.0, 4.5, 0.0) , 2.0, vec4(0.96f, 0.96f, 0.95f, 0.8f)};
uniform DirectionalLight key_light = {vec4(0.1), vec3(-2.0, -1.3, 6.0) , 1.5, vec4(1.0f)};


vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

///algorithm for calculating and antialiasing shadow from http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
float shadowCalculation(float _bias) {
    // perform perspective divide
    vec3 projCoords = ShadowCoord.xyz / ShadowCoord.w;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthTex, 0);
    for(int x=-1; x<=1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(depthTex, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z - _bias > pcfDepth ? 0.5 : 0.0;
        }
    }

    for (int i=0;i<4;i++){
      if ( texture( depthTex, projCoords.xy + poissonDisk[i]/500.0 ).z  <  projCoords.z-_bias ){
        shadow-=0.1;
      }
    }
    return shadow /= 9.0;
}

vec4 toneMap(in vec4 Kd) {
    vec4 mask1 = smoothstep(0.47,0.5, Kd);
    vec4 mask2 = smoothstep(0.69,0.7, Kd);
    vec4 mask3 = smoothstep(0.85,0.86, Kd);
    vec4 mask4 = smoothstep(0.98,0.99, Kd);

    vec4 tone = mix(vec4(0.1), vec4(0.5), mask1);
    tone = mix(tone, vec4(0.84), mask2);
    tone = mix(tone, vec4(0.93), mask3);
    tone = mix(tone, vec4(1.0), mask4);

    return tone;
}

///Phong shading model
vec4 ToonShade(in DirectionalLight light)
{
    //float shadow = shadowCalculation(-0.000001);

    L = normalize(light.position); //light vector
    E = normalize(-pos);
    float falloff = inversesqrt((distance(light.position, pos))*2.0);

    //distribution of light over the object
    float Kd = light.intensity * max(dot(L,normalize(outNormal)),0.1f) * falloff;
    vec4 diffuse = vec4(Kd);
    L = normalize(light.position-pos);
    H = normalize(L+E);

    float Ks = pow(max(dot(L,N), 0.0), Shininess); //cal specular highlight

    return light.ambient +diffuse +Ks;
}


/*********************************************************************************/

void main()
{

    vec3 pos = outVert;
    vec4 ColourMap;

    mat4 MVNormal = transpose(inverse(ModelView));
    N = normalize(vec3(MVNormal) * outNormal);
    ColourMap = vec4(0.8,0.8,0.5,1.0f);

    vec4 tone = ToonShade(top_light) + ToonShade(key_light) ;
    vec4 Toon_tone = toneMap(tone);
    vec4 diffuse = Toon_tone * ColourMap;


    fragColour = diffuse;
}
