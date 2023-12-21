#version 410 core

layout(location = 0) out vec4 fragColor;

uniform vec2 u_resolution;
uniform float u_iterations;
uniform vec2 u_offset;
uniform float u_zoom;

uniform float u_fractal;

uniform float u_color_scheme;

// Komplexe Zahl für Julia-Menge
uniform vec2 u_julia_c;

// Farbe für Einfärbung
uniform vec3 u_color;

// Parameter für Farbverlauf
uniform vec3 u_a;
uniform vec3 u_b;
uniform vec3 u_c;
uniform vec3 u_d;

// Pixelkoordinaten in komplexe Zahlen umwandeln
vec2 map(vec2 coord) {
    vec2 c = (2 * coord - u_resolution.xy) / u_resolution.y;
    c.x += -0.5;
    return c;
}

// Offset in [-3.5, 3.5] x [-2.0, 2.0] normalisieren
vec2 normalize_offset(vec2 offset) {
    return vec2(offset.x / u_resolution.x * 3.5, offset.y / u_resolution.y * 2.0);
}

// Mandelbrot-Menge
float mandelbrot(vec2 c) {
    vec2 z = vec2(0.0, 0.0);
    float n = 0.0;
    for(int i = 0; i < int(u_iterations); i++) {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y);
        z += c;
        n += 1.0;
        if(length(z) > 2.0) {
            break;
        }
    }
    return n;
}

// Julia-Menge
float julia(vec2 c) {
    vec2 z = c;
    float n = 0.0;
    for(int i = 0; i < int(u_iterations); i++) {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y);
        z += u_julia_c;
        n += 1.0;
        if(length(z) > 2.0) {
            break;
        }
    }
    return n;
}

// Burning Ship
float burning_ship(vec2 c) {
    vec2 z = vec2(0.0, 0.0);
    float n = 0.0;
    for(int i = 0; i < int(u_iterations); i++) {
        float xtemp = z.x * z.x - z.y * z.y + c.x;
        z.y = -abs(2.0 * z.x * z.y) + c.y; // Negate the imaginary part here
        z.x = abs(xtemp);
        n += 1.0;
        if(length(z) > 2.0) {
            break;
        }
    }
    return n;
}

// Einfärbung
vec3 colorize_single(float n) {
    if(n == u_iterations) {
        return vec3(0.0, 0.0, 0.0);
    }
    float hue = n / u_iterations;
    return u_color * vec3(hue, hue, hue);
}

// Farbverlauf
vec3 colorize_gradient(float n) {
    if(n == u_iterations) {
        return vec3(0.0, 0.0, 0.0);
    }
    float t = n / u_iterations + 0.5;
    return u_a.xyz + u_b.xyz * cos(6.28318 * (u_c.xyz * t + u_d.xyz));
}

void main() {
    vec2 coord = gl_FragCoord.xy;
    vec2 c = map(coord) / u_zoom + normalize_offset(u_offset);
    float n;

    switch(int(u_fractal)) {
        case 0:
            n = mandelbrot(c);
            break;
        case 1:
            n = julia(c);
            break;
        case 2:
            n = burning_ship(c);
            break;
    }

    switch(int(u_color_scheme)) {
        case 0:
            fragColor = vec4(colorize_single(n), 1.0);
            break;
        case 1:
            fragColor = vec4(colorize_gradient(n), 1.0);
            break;
    }
}