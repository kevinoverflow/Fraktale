#version 410 core

layout(location = 0) out vec4 fragColor;

uniform vec2 u_resolution;
uniform float u_iterations;
uniform vec2 u_offset;
uniform float u_zoom;
uniform vec3 u_color;

// Consine gradient 
uniform vec3 u_a;
uniform vec3 u_b;
uniform vec3 u_c;
uniform vec3 u_d;

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

vec3 colorize_single(float n) {
    if(n == u_iterations) {
        return vec3(0.0, 0.0, 0.0);
    }
    float hue = n / u_iterations;
    return u_color * vec3(hue, hue, hue);
}

vec3 colorize_cosine(float n) {
    if(n == u_iterations) {
        return vec3(0.0, 0.0, 0.0);
    }

    float t = n / u_iterations + 0.5;

    return u_a.xyz + u_b.xyz * cos(6.28318 * (u_c.xyz * t + u_d.xyz));
}

void main() {
    vec2 coord = gl_FragCoord.xy;
    vec2 c = map(coord) / u_zoom + normalize_offset(u_offset);
    float n = mandelbrot(c);
    fragColor = vec4(colorize_cosine(n), 1.0);
}