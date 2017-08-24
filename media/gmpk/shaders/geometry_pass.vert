#version 330

uniform mat4 GM_shadow_matrix;
uniform mat4 GM_view_matrix;
uniform mat4 GM_model_matrix;
uniform mat4 GM_projection_matrix;

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec4 tangent;
layout (location = 4) in vec4 bitangent;
layout (location = 5) in vec2 lightmapuv;

out vec4 _shadowCoord;
out vec4 _normal;
out vec2 _uv;
out vec4 _tangent;
out vec4 _bitangent;
out vec2 _lightmapuv;
out vec4 _position_world;

void calcCoords()
{
    gl_Position = GM_projection_matrix * GM_view_matrix * GM_model_matrix * position;
    _shadowCoord = GM_shadow_matrix * GM_model_matrix * _position_world;
    
    _position_world = position;
    _normal = normal;
    _tangent = tangent;
    _bitangent = bitangent;
    _uv = uv;
    _lightmapuv = lightmapuv;
}

void main(void)
{
    calcCoords();
}