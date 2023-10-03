#version 410 core

layout(location = 0) out vec4 color;

uniform vec2 u_resolution;
uniform float u_iterations;
uniform vec2 u_offset;
uniform float u_zoom;

vec2 map(vec2 coord) {
    vec2 c = (2 * coord - u_resolution.xy) / u_resolution.y;
    c.x += -0.5;
    return c;

}

vec2 normalize_offset(vec2 offset) {
    return vec2(offset.x / u_resolution.x * 3.5, offset.y / u_resolution.y * 2.0);
}

float mandelbrot(vec2 c) {
    vec2 z = vec2(0.0, 0.0);
    float n = 0.0;
    for(int i = 0; i < int(u_iterations); i++) {
        z = vec2(z.x * z.x - z.y * z.y + c.x, 2.0 * z.x * z.y + c.y);
        n += 1.0;
        if(length(z) > 2.0) {
            break;
        }
    }
    return n;
}

void main() {
    vec2 coord = gl_FragCoord.xy;
    vec2 c = map(coord) / u_zoom + normalize_offset(u_offset);
    float n = mandelbrot(c);
    float v = n / u_iterations;
    color = vec4(v, v, v, 1.0);
}