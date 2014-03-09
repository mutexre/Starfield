#version 330

//precision mediump float;

uniform float aspectRatio;
uniform float z;
uniform float minZ;
uniform float zSize;
uniform float scale;
uniform mat4 transform;
uniform mat4 projection;

in float size;
in vec3 coord;
in vec3 color;
out float pointSize;
out float distToPov;
out vec3 vfColor;

void main()
{
    float dz = coord.z - z;
    float z = dz + (dz <= minZ ? zSize : 0.0f);
    
    gl_Position = transform * vec4(coord.x, coord.y, z, 1.0f);
    distToPov = length(gl_Position.xyz - vec3(0.0f, 0.0f, 0.0f));
    gl_Position = projection * gl_Position;
    gl_Position.x *= aspectRatio;

//    normalizedDistance = sqrt(_z * _z + coord.x * coord.x + coord.y * coord.y) / 1.74;

    pointSize = 1.5f * scale * size / max(pow(distToPov, 1.0f), 0.2f);
    gl_PointSize = pointSize;
    vfColor = color;

	//gl_Position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
