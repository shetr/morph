#version 450 core

#type vert

layout(location = 0) in vec2 a_position;

uniform mat3 u_transform;

out vec2 f_position;

void main()
{
    f_position = (u_transform * vec3(a_position, 1)).xy;
    gl_Position = vec4(a_position, 0, 1);	
}

#type frag
			
layout(location = 0) out uint iterations;

in vec2 f_position;

uniform uint u_iterationsLimit;

vec2 complexSquare(vec2 z)
{
    return vec2(z.x*z.x - z.y*z.y, 2*z.x*z.y);
}

void main()
{
    uint i = 0;
    vec2 z = vec2(0);
    vec2 z2 = vec2(0);
    vec2 c = f_position;
    while(i <= u_iterationsLimit && z2.x + z2.y <= 4)
    {
        z = vec2(z2.x - z2.y, 2*z.x*z.y) + c;
        z2 = z * z;
        i++;
    }

    iterations = i;
}