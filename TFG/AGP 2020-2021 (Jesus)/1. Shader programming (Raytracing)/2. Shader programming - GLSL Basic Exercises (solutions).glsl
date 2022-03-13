
////////////////////////////////////////////////////////////////////////////
// Ex1: Horizontal gradient from black to white

vec3 ex1(in vec2 fragCoord)
{
    return vec3(fragCoord.x/iResolution.x);
}


////////////////////////////////////////////////////////////////////////////
// Ex2: Smooth color interpolation between corners

vec3 ex2(in vec2 fragCoord)
{
    vec2 uv = fragCoord/iResolution.xy;
    vec3 black = vec3(0.0);
    vec3 white = vec3(1.0);
    vec3 red = vec3(1.0, 0.0, 0.0);
    vec3 green = vec3(0.0, 1.0, 0.0);
    return mix(
        mix(black, red, uv.y),
        mix(green, white, uv.y),
        uv.x);
}


////////////////////////////////////////////////////////////////////////////
// Ex3: Smooth color interpolation between corners (5x5 tiles)

vec3 ex3(in vec2 fragCoord)
{
    return ex2(mod(fragCoord * 5.0, iResolution.xy));
}


////////////////////////////////////////////////////////////////////////////
// Ex: Checkerboard (vertical lines, horizontal lines, checkerboard)

vec3 ex4(in vec2 fragCoord)
{
    vec2 id = floor(2.0 * mod(fragCoord, vec2(100.0)) / vec2(100.0));
    vec3 col = vec3(id.x);
    return col;
}

vec3 ex5(in vec2 fragCoord)
{
    vec2 id = floor(2.0 * mod(fragCoord, vec2(100.0)) / vec2(100.0));
    vec3 col = vec3(id.y);
    return col;
}

vec3 ex6(in vec2 fragCoord)
{
    vec2 id = floor(2.0 * mod(fragCoord, vec2(100.0)) / vec2(100.0));
    vec3 col = vec3(mod(id.y + id.x, 2.0));
    return col;
}


////////////////////////////////////////////////////////////////////////////
// Ex: Japan flag

vec3 ex7(in vec2 fragCoord)
{
    vec2 center = iResolution.xy * 0.5;
    vec2 centerToFragCoord = fragCoord - center;
    float d = sqrt(dot(centerToFragCoord, centerToFragCoord));
    float factor = smoothstep(50.0, 100.0, d);
    vec3 color = mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), factor);
    return color;
}


////////////////////////////////////////////////////////////////////////////
// Ex: Repeated japan flags

vec3 ex8(in vec2 fragCoord)
{
    return ex7(mod(fragCoord * 5.0, iResolution.xy));
}


////////////////////////////////////////////////////////////////////////////
// Ex: Per-pixel noise

float hash(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

vec3 ex9(in vec2 fragCoord)
{
    return vec3(hash(fragCoord));
}


////////////////////////////////////////////////////////////////////////////
// Ex: Per-tile noise

vec3 ex10(in vec2 fragCoord)
{
    vec2 gridCoord = floor(fragCoord / 64.0);
    return vec3(hash(gridCoord));
}


////////////////////////////////////////////////////////////////////////////
// Ex: Colored per-tile noise

vec3 ex11(in vec2 fragCoord)
{
    vec2 gridCoord = floor(fragCoord / 64.0);
    return vec3(hash(gridCoord),
                hash(gridCoord + 1234.1234),
                hash(gridCoord - 1234.1234));
}


////////////////////////////////////////////////////////////////////////////
// Ex:

vec3 ex12(in vec2 fragCoord)
{
    vec2 gridCoord = floor(fragCoord / 64.0);
    vec2 repeCoord = mod(fragCoord, 64.0);
    vec2 randVec = vec2(hash(gridCoord),
                        hash(gridCoord+33.0));
    vec2 disp = 2.0 * randVec - 1.0;
    vec2 center = vec2(32.0) - 10.0 * disp;
    vec2 r = repeCoord - center;
    float d = sqrt(dot(r, r));
    float f = smoothstep(10.0, 12.0, d);
    return vec3(f);
}


////////////////////////////////////////////////////////////////////////////
// Ex: Smooth noise primitive

float noise(in vec2 coord)
{
    vec2 delta = vec2(1.0, 0.0);
    vec2 gridCoord = floor(coord);
    vec2 fraction = fract(coord);
    fraction = smoothstep(0.0, 1.0, fraction);
    float c00 = hash(gridCoord);
    float c10 = hash(gridCoord + delta.xy);
    float c01 = hash(gridCoord + delta.yx);
    float c11 = hash(gridCoord + delta.xx);
    float n = mix(
        mix(c00, c10, fraction.x),
        mix(c01, c11, fraction.x),
        fraction.y);
    return n;
}

vec3 ex13(in vec2 fragCoord)
{
    return vec3(noise(fragCoord/64.0));
}


////////////////////////////////////////////////////////////////////////////
// Ex: Fractional Brownian Motion

float fbm(in vec2 coord)
{
    float col = 0.0;
    float asum = 0.0;
    float a = 0.5;
    float f = 1.0;
    for (int i = 0; i < 8; ++i)
    {
    	col += a * noise(coord * f);
        asum += a;
        a *= 0.5;
        f *= 2.0;
    }
    col /= asum;
    return col;
}

vec3 ex14(in vec2 fragCoord)
{
    return vec3(fbm(fragCoord/64.0));
}


////////////////////////////////////////////////////////////////////////////
// Ex: Ridge

vec3 ex15(in vec2 fragCoord)
{
    float val = fbm(fragCoord/64.0);
    val = 2.0 * abs(val - 0.5);
    return vec3(val);
}


////////////////////////////////////////////////////////////////////////////
// Main function

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Output to screen
    fragColor = vec4(ex1(fragCoord),1.0);
}
