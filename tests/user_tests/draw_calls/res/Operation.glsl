

vec4 operation(vec4 color)
{
    return vec4(vec3(1) - 2 * color.xyz, color.w);
}