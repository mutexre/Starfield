#version 330

layout (location = 0, index = 0) out vec4 fragmentColor;

//precision mediump float;
in float pointSize;
in float distToPov;
in vec3 vfColor;

void main()
{
    vec2 centerCoord = gl_PointCoord - vec2(0.5);
    float radius2 = dot(centerCoord, centerCoord) * 4.0;
    float falloff = clamp((1.0 - pow(radius2, 0.125)) * 2.0, 0.0, 1.0);
    float coeff = (1.0f / pow(max(distToPov, 1.0), 2.0)) * smoothstep(0.0,
                                   1.0,
                                   mix(0.0,
                                       mix(mix(0.4,
                                               0.5 * step(-1.0, -radius2),
                                               clamp(pointSize - 1.0, 0.0, 1.0)),
                                           falloff,
                                           clamp(pointSize - 4.0, 0.0, 1.0)),
                                        clamp(pointSize * 2.0, 0.0, 1.0)));
    vec3 color = vfColor;
/*
    float dampingFactor = 2.f;
    float lightToFragmentDamp = 1.0f / pow(distFromFragmentToLightSource, dampingFactor);
    float fragmentToPovDamp = 1.0f / pow(distFromFragmentToPov, dampingFactor);
*/
    fragmentColor = vec4(color.r, color.g, color.b, coeff);
    //fragmentColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
