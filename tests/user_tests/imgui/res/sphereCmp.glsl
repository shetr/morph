#version 430

layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D u_textureImage;

uniform vec3 u_backgroundColor;
uniform vec3 u_sphereColor;

void main() {
    // base pixel colour for image
    vec4 pixel = vec4(0.0, 0.0, 0.0, 0.0);
    // get index in global work group i.e x,y position
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
    
    for(int yi = 0; yi < 2; ++yi) {
        for(int xi = 0; xi < 2; ++xi) {
            ivec2 pixel_coords_off = pixel_coords + ivec2(xi, yi);
            float max_x = 5.0;
            float max_y = 5.0;
            ivec2 dims = imageSize(u_textureImage); // fetch image dimensions
            float x = (float(pixel_coords_off.x * 2 - dims.x) / dims.x);
            float y = (float(pixel_coords_off.y * 2 - dims.y) / dims.y);
            vec3 ray_o = vec3(x * max_x, y * max_y, 0.0);
            vec3 ray_d = vec3(0.0, 0.0, -1.0); // ortho

            vec3 sphere_c = vec3(0.0, 0.0, -10.0);
            float sphere_r = 1.0;

            vec3 omc = ray_o - sphere_c;
            float b = dot(ray_d, omc);
            float c = dot(omc, omc) - sphere_r * sphere_r;
            float bsqmc = b * b - c;
            // hit one or both sides
            if (bsqmc >= 0.0) {
                pixel += vec4(u_sphereColor, 1.0);
            } else {
                pixel += vec4(u_backgroundColor, 1.0);
            }
        }
    }
    pixel /= 4;
    
    // output to a specific pixel in the image
    imageStore(u_textureImage, pixel_coords, pixel);
}