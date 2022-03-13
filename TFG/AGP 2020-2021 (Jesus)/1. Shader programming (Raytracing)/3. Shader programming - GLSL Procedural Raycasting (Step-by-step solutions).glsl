
#define TAU 6.28318530718

////////////////////////////////////////////////////////////////////////////
// Ex1 Rays

vec3 ex1(in vec2 fragCoord)
{
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    vec3 o = vec3(0.0, 0.0, 2.0);
    vec3 d = normalize(nearCoord - o);
    vec3 col = d;
    return col;
}


////////////////////////////////////////////////////////////////////////////
// Ex2: Background gradient

vec3 ex2(in vec2 fragCoord)
{
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    vec3 o = vec3(0.0, 0.0, 2.0);
    vec3 d = normalize(nearCoord - o);
    
    // BG
    vec3 horiCol = vec3(0.85, 0.9, 1.0);
    vec3 topCol = vec3(0.2, 0.4, 1.0);
    float bgFactor = clamp(d.y, 0.0, 1.0);
    vec3 col = mix(horiCol, topCol, bgFactor);
    return col;
}


////////////////////////////////////////////////////////////////////////////
// Ex3: Intersection with floor

vec3 ex3(in vec2 fragCoord)
{
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    vec3 o = vec3(0.0, 0.0, 2.0);
    vec3 d = normalize(nearCoord - o);
    
    // BG
    vec3 horiCol = vec3(0.85, 0.9, 1.0);
    vec3 topCol = vec3(0.2, 0.4, 1.0);
    float bgFactor = clamp(d.y, 0.0, 1.0);
    vec3 col = mix(horiCol, topCol, bgFactor);
    
    // floor
    // x = o + t * d
    // t = (x - o) / d
    float t = (-1.0 - o.y) / d.y;
    vec3 p = t * d;
    if (t > 0.0)
        col = vec3(0.8, 0.8, 0.8);
    
    return col;
}


////////////////////////////////////////////////////////////////////////////
// Ex4: Sphere intersection

vec3 ex4(in vec2 fragCoord)
{
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    vec3 o = vec3(0.0, 0.0, 2.0);
    vec3 d = normalize(nearCoord - o);
    
    // BG
    vec3 horiCol = vec3(0.85, 0.9, 1.0);
    vec3 topCol = vec3(0.2, 0.4, 1.0);
    float bgFactor = clamp(d.y, 0.0, 1.0);
    vec3 col = mix(horiCol, topCol, bgFactor);
    
    // floor
    // x = o + t * d
    // t = (x - o) / d
    float t = (-1.0 - o.y) / d.y;
    vec3 p = o + t * d;
    if (t > 0.0)
        col = vec3(0.8, 0.8, 0.8);
    
    // sphere
    vec3 c = vec3(0.0, 0.0, 0.0);
    float r = 0.5;
    vec3 oc = c - o;
    t = dot(oc, d);
    if (t > 0.0)
    {
    	p = o + t * d;
    	float r2 = length(p - c);
  		if (r2 < r)
        {
		    col = vec3(1.0);
        }
    }
    
    return col;
}


////////////////////////////////////////////////////////////////////////////
// Ex5: Surface Normals

vec3 ex5(in vec2 fragCoord)
{
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    vec3 o = vec3(0.0, 0.0, 2.0);
    vec3 d = normalize(nearCoord - o);
    
    // BG
    vec3 horiCol = vec3(0.85, 0.9, 1.0);
    vec3 topCol = vec3(0.2, 0.4, 1.0);
    float bgFactor = clamp(d.y, 0.0, 1.0);
    vec3 col = mix(horiCol, topCol, bgFactor);
    
    // floor
    // x = o + t * d
    // t = (x - o) / d
    float t = (-1.0 - o.y) / d.y;
    vec3 p = o + t * d;
    if (t > 0.0)
        col = vec3(0.0, 1.0, 0.0);
    
    // sphere
    vec3 c = vec3(0.0, 0.0, 0.0);
    float r = 0.5;
    vec3 oc = c - o;
    t = dot(oc, d);
    if (t > 0.0)
    {
    	p = o + t * d;
    	float r2 = length(p - c);
  		if (r2 < r)
        {
            t = sqrt(r*r - r2*r2);
            p = p - t * d;
		    vec3 n = vec3(normalize(p - c));
            col = n;
        }
    }
    
    return col;
}


////////////////////////////////////////////////////////////////////////////
// Ex6: Arrays

struct Sphere {
    vec3  p;   // position
    float r;   // radius
    vec3  col; // color
    float R0;  // base reflectance
};
    
Sphere spheres[3] = Sphere[3](
    Sphere(vec3(-0.7, 0.0, -0.2), 0.5, vec3(0.9, 0.2, 0.1), 0.01),
    Sphere(vec3(0.7, 0.0, 0.2),   0.5, vec3(0.0, 0.8, 0.2), 0.04),
    Sphere(vec3(0.0, 0.0, -0.7),  0.5, vec3(0.2, 0.0, 0.7), 0.24)
    );

struct Hit { // If castRay hits a surface, this contains the hit info
    float t; // distance from ray origin to first intersection
    vec3 p;  // hit surface position
    vec3 n;  // hit surface normal
    vec3 col;// hit surface color
    float R0;// hit surface base reflectance
};
    
vec3 ex6(in vec2 fragCoord)
{
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    vec3 o = vec3(0.0, 0.0, 2.0);
    vec3 d = normalize(nearCoord - o);
    
    // BG
    vec3 horiCol = vec3(0.85, 0.9, 1.0);
    vec3 topCol = vec3(0.2, 0.4, 1.0);
    float bgFactor = clamp(d.y, 0.0, 1.0);
    vec3 col = mix(horiCol, topCol, bgFactor);
    
    // floor
    // x = o + t * d
    // t = (x - o) / d
    float t = (-1.0 - o.y) / d.y;
    vec3 p = o + t * d;
    if (t > 0.0)
        col = vec3(0.0, 1.0, 0.0);
    
    // spheres
    for (int i = 0; i < 3; ++i)
    {
        vec3 c = spheres[i].p;
        float r = spheres[i].r;
        vec3 oc = c - o;
        t = dot(oc, d);
        if (t > 0.0)
        {
            p = o + t * d;
            float r2 = length(p - c);
            if (r2 < r)
            {
                t = sqrt(r*r - r2*r2);
                p = p - t * d;
                vec3 n = vec3(normalize(p - c));
                col = n;
            }
        }
    }
    
    return col;
}


////////////////////////////////////////////////////////////////////////////
// Ex7: Surface - sort intersections

vec3 ex7(in vec2 fragCoord)
{
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    vec3 o = vec3(0.0, 0.0, 2.0);
    vec3 d = normalize(nearCoord - o);
    
    // BG
    vec3 horiCol = vec3(0.85, 0.9, 1.0);
    vec3 topCol = vec3(0.2, 0.4, 1.0);
    float bgFactor = clamp(d.y, 0.0, 1.0);
    vec3 col = mix(horiCol, topCol, bgFactor);
    
    // surface
    Hit hit;
    hit.t = 9999.9;
    
    // floor
    // x = o + t * d
    // t = (x - o) / d
    float t = (-1.0 - o.y) / d.y;
    vec3 p = o + hit.t * d;
    vec3 n = vec3(1.0);
    if (t > 0.0)
    {
        hit.t = t;
        hit.p = p;
        hit.n = vec3(0.0, 1.0, 0.0);
    }
    
    // spheres
    for (int i = 0; i < 3; ++i)
    {
        vec3 c = spheres[i].p;
        float r = spheres[i].r;
        vec3 oc = c - o;
        float t = dot(oc, d);
        if (t > 0.0)
        {
            p = o + t * d;
            float r2 = length(p - c);
            if (r2 < r)
            {
                t -= sqrt(r*r - r2*r2);
                if (t < hit.t)
                {
                	p = o + t * d;
                	n = vec3(normalize(p - c));
                	hit.p = p;
                    hit.t = t;
                    hit.n = n;
                }
            }
        }
    }
    
    if (hit.t < 9999.0)
    {
    	col = hit.n;
    }
    
    return col;
}


////////////////////////////////////////////////////////////////////////////
// Ex8: Diffuse shading, shading computed by shadeHit() function


vec3 shadeHit(in Hit hit, in vec3 d)
{
    if (hit.t < 9999.0)
    {
        vec3 l = normalize(vec3(1.0));
        //vec3 i = fresnel(surf.n, d, vec3(hit.R0)); // fresnel factor

        vec3 col = vec3(0.0);
        col += hit.col * 0.2; // ambient contribution
        col += hit.col * 0.7 * max(dot(l, hit.n), 0.0); // direct light diffuse
        return col;
        //return floor(col * 3.0)/3.0; // cell shading example
    }
    else // if the ray did not hit, shade the sky
    {
        vec3 horiCol = vec3(0.85, 0.9, 1.0);
        vec3 topCol = vec3(0.2, 0.4, 1.0);
        float bgFactor = clamp(d.y, 0.0, 1.0);
        vec3 col = mix(horiCol, topCol, bgFactor);
        return col;
    }
}

vec3 ex8(in vec2 fragCoord)
{
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    vec3 o = vec3(0.0, 0.0, 2.0);
    vec3 d = normalize(nearCoord - o);
    
    // surface
    Hit hit;
    hit.t = 9999.9;
    
    // floor
    // x = o + t * d
    // t = (x - o) / d
    float t = (-1.0 - o.y) / d.y;
    vec3 p = o + hit.t * d;
    vec3 n = vec3(1.0);
    if (t > 0.0)
    {
        hit.t = t;
        hit.p = p;
        hit.n = vec3(0.0, 1.0, 0.0);
        hit.col = vec3(1.0);
        hit.R0  = 0.02;
    }
    
    // spheres
    for (int i = 0; i < 3; ++i)
    {
        vec3 c = spheres[i].p;
        float r = spheres[i].r;
        vec3 oc = c - o;
        float t = dot(oc, d);
        if (t > 0.0)
        {
            p = o + t * d;
            float r2 = length(p - c);
            if (r2 < r)
            {
                t -= sqrt(r*r - r2*r2);
                if (t < hit.t)
                {
                	p = o + t * d;
                	n = vec3(normalize(p - c));
                	hit.p = p;
                    hit.t = t;
                    hit.n = n;
                    hit.col = spheres[i].col;
                    hit.R0  = spheres[i].R0;
                }
            }
        }
    }
    
    vec3 col = shadeHit(hit, d);
    
    return col;
}

////////////////////////////////////////////////////////////////////////////
// Ex9: Hard shadows (ray casting happens in its own function castRay, and
//     there is a new version of shadeHit that checks light visibility)

bool castRay(in vec3 o, in vec3 d, out Hit hit)
{
    hit.t = 9999.9;
    
    // floor
    // x = o + t * d
    // t = (x - o) / d
    float t = (-0.5 - o.y) / d.y;
    if (t > 0.0)
    {
        hit.t = t;
        hit.p = o + t * d;
        hit.n = vec3(0.0, 1.0, 0.0);
        hit.col = vec3(1.0);
        hit.R0  = 0.02;
    }
    
    // spheres
    for (int i = 0; i < 3; ++i)
    {
        vec3 c = spheres[i].p;
        float r = spheres[i].r;
        vec3 oc = c - o;
        float t = dot(oc, d);
        vec3 k = o + t * d;
        float r2 = length(k - c);
        if (r2 < r)
        {
            t -= sqrt(r*r - r2*r2);
            if (t < hit.t && t > 0.0)
            {
                hit.t = t;
                hit.p = o + t * d;
                hit.n = vec3(normalize(hit.p - c));
                hit.col = spheres[i].col;
                hit.R0  = spheres[i].R0;
            }
        }
    }
    
    return hit.t < 9999.0;
}

vec3 shadeHitWithShadows(in Hit hit, in vec3 d)
{
    if (hit.t < 9999.0)
    {
        vec3 l = normalize(vec3(1.0, 1.0, 1.0)); // hardcoded light direction
        
        vec3 col = vec3(0.0);
        col += hit.col * 0.2; // ambient contribution
            
        Hit hit2;
        if (!castRay(hit.p + hit.n*0.0001, l, hit2))
        {
            col += hit.col * 0.7 * max(dot(l, hit.n), 0.0); // direct light diffuse
        }
        
        return col;
    }
    else // if the ray did not hit, shade the sky
    {
        vec3 horiCol = vec3(0.85, 0.9, 1.0);
        vec3 topCol = vec3(0.2, 0.4, 1.0);
        float bgFactor = clamp(d.y, 0.0, 1.0);
        vec3 col = mix(horiCol, topCol, bgFactor);
        return col;
    }
}

vec3 ex9(in vec2 fragCoord)
{
    // per-pixel camera ray
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    vec3 o = vec3(0.0, 0.0, 2.0);
    vec3 d = normalize(nearCoord - o);
    
    // Ray iteration 1
    Hit hit1;
    bool b1 = castRay(o, d, hit1);
    vec3 col = shadeHitWithShadows(hit1, d); // diffuse shaded color (unused in this exercise)
    //vec3 f1 = fresnel(hit1.n, d, vec3(hit.R0));

    //vec3 col = f1; // we output the fresnel of the surface
    
    return col;
}

////////////////////////////////////////////////////////////////////////////
// Ex10: Fresnel factor (like a reflection ration)

vec3 fresnel(in vec3 n, in vec3 d, in vec3 R0)
{
    return R0 + (1.0 - R0) * pow(1.0 - dot(n, -d), 5.0);
}

vec3 ex10(in vec2 fragCoord)
{
    // per-pixel camera ray
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    vec3 o = vec3(0.0, 0.0, 2.0);
    vec3 d = normalize(nearCoord - o);
    
    // Ray iteration 1
    Hit hit1;
    bool b1 = castRay(o, d, hit1);
    vec3 c1 = shadeHitWithShadows(hit1, d);
    vec3 f1 = float(b1)*fresnel(hit1.n, d, vec3(hit1.R0));
        
    vec3 col = f1;
    
    return col;
}

////////////////////////////////////////////////////////////////////////////
// Ex11: 2-bounce reflections weighted by the fresnel factor

vec3 ex11(in vec2 fragCoord)
{
    // per-pixel camera ray
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    vec3 o = vec3(0.0, 0.0, 2.0);
    vec3 d = normalize(nearCoord - o);
    
    // Ray iteration 1
    Hit hit1;
    bool b1 = castRay(o, d, hit1);
    vec3 c1 = shadeHitWithShadows(hit1, d);
    vec3 f1 = float(b1)*fresnel(hit1.n, d, vec3(hit1.R0));

    // Ray iteration 2
    Hit hit2;
    vec3 d2 = reflect(d, hit1.n);
    bool b2 = castRay(hit1.p + d2*0.0001, d2, hit2);
    vec3 c2 = shadeHitWithShadows(hit2, d2);
        
    vec3 col = mix(c1, c2, f1);
    
    return col;
}

////////////////////////////////////////////////////////////////////////////
// Ex12: camera position

vec3 ex12(in vec2 fragCoord)
{
    // per-pixel position in camera space
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    
    // camera origin and target in world space
    vec3 o = vec3(-1.0, 1.0, 2.0);
    vec3 tgt = vec3(0.0, 0.0, 0.0);
    
    // camera to world matrix vectors
    vec3 zz  = normalize(o - tgt);
    vec3 xx  = normalize(cross(vec3(0.0, 1.0, 0.0), zz));
    vec3 yy  = cross(zz, xx);
    vec3 d = normalize(xx*nearCoord.x + yy*nearCoord.y - zz*1.8);
    
    
    
    // Ray iteration 1
    Hit hit1;
    bool b1 = castRay(o, d, hit1);
    vec3 c1 = shadeHitWithShadows(hit1, d);
    vec3 f1 = float(b1)*fresnel(hit1.n, d, vec3(hit1.R0));

    // Ray iteration 2
    Hit hit2;
    vec3 d2 = reflect(d, hit1.n);
    bool b2 = castRay(hit1.p + d2*0.0001, d2, hit2);
    vec3 c2 = shadeHitWithShadows(hit2, d2);
        
    vec3 col = mix(c1, c2, f1);
    
    return col;
}

////////////////////////////////////////////////////////////////////////////
// Ex13: camera rotation (just change 'o', the camera origin)

vec3 ex13(in vec2 fragCoord)
{
    // per-pixel position in camera space
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    
    // camera origin and target in world space (rotation happens here)
    vec3 o = 3.0*vec3(cos(1.0-iMouse.y/iResolution.y)*cos(TAU*iMouse.x/iResolution.x),
                      sin(1.0-iMouse.y/iResolution.y),
                      cos(1.0-iMouse.y/iResolution.y)*sin(TAU*iMouse.x/iResolution.x));
    vec3 tgt = vec3(0.0, 0.0, 0.0);
    
    // camera to world matrix vectors
    vec3 zz  = normalize(o - tgt);
    vec3 xx  = normalize(cross(vec3(0.0, 1.0, 0.0), zz));
    vec3 yy  = cross(zz, xx);
    vec3 d = normalize(xx*nearCoord.x + yy*nearCoord.y - zz*1.8);
    
    
    
    // Ray iteration 1
    Hit hit1;
    bool b1 = castRay(o, d, hit1);
    vec3 c1 = shadeHitWithShadows(hit1, d);
    vec3 f1 = float(b1)*fresnel(hit1.n, d, vec3(hit1.R0));

    // Ray iteration 2
    Hit hit2;
    vec3 d2 = reflect(d, hit1.n);
    bool b2 = castRay(hit1.p + hit1.n*0.0001, d2, hit2);
    vec3 c2 = shadeHitWithShadows(hit2, d2);
        
    vec3 col = mix(c1, c2, f1);
    
    return col;
}

////////////////////////////////////////////////////////////////////////////
// Ex14: Anti-aliasing

#define AA 2 // Don't abuse the AA factor...

vec3 ex14(in vec2 fragCoord)
{
    // per-pixel position in camera space
    vec3 nearCoord = vec3(2.0 * fragCoord / iResolution.xy - 1.0, 0.0);
    nearCoord.x *= iResolution.x / iResolution.y;
    
    vec3 col = vec3(0.0);
    
    for (int i = 0; i < AA; ++i)
    for (int j = 0; j < AA; ++j)
    {
        // camera origin and target in world space (rotation happens here)
        vec3 o = 3.0*vec3(cos(1.0-iMouse.y/iResolution.y)*cos(TAU*iMouse.x/iResolution.x),
                          sin(1.0-iMouse.y/iResolution.y),
                          cos(1.0-iMouse.y/iResolution.y)*sin(TAU*iMouse.x/iResolution.x));
        vec3 tgt = vec3(0.0, 0.0, 0.0);
        
        // Sub-pixel displacement for AA
        vec3 c = nearCoord + float(i)*vec3(dFdx(nearCoord.x), dFdy(nearCoord.y), 0.0)/float(AA);

        // camera to world matrix vectors
        vec3 zz  = normalize(o - tgt);
        vec3 xx  = normalize(cross(vec3(0.0, 1.0, 0.0), zz));
        vec3 yy  = cross(zz, xx);
        vec3 d = normalize(xx*c.x + yy*c.y - zz*1.8);

        // Ray iteration 1
        Hit hit1;
        bool b1 = castRay(o, d, hit1);
        vec3 c1 = shadeHitWithShadows(hit1, d);
        vec3 f1 = float(b1)*fresnel(hit1.n, d, vec3(hit1.R0));

        // Ray iteration 2
        Hit hit2;
        vec3 d2 = reflect(d, hit1.n);
        bool b2 = castRay(hit1.p + hit1.n*0.0001, d2, hit2);
        vec3 c2 = shadeHitWithShadows(hit2, d2);

        col += mix(c1, c2, f1);
    }
    
    col = col/float(AA*AA);
    
    return col;
}

////////////////////////////////////////////////////////////////////////////
// Main function

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Output to screen
    //fragColor = vec4(ex1(fragCoord),1.0);
    //fragColor = vec4(ex2(fragCoord),1.0);
    //fragColor = vec4(ex3(fragCoord),1.0);
    //fragColor = vec4(ex4(fragCoord),1.0);
    //fragColor = vec4(ex5(fragCoord),1.0);
    //fragColor = vec4(ex6(fragCoord),1.0);
    //fragColor = vec4(ex7(fragCoord),1.0);
    //fragColor = vec4(ex8(fragCoord),1.0);
    //fragColor = vec4(ex9(fragCoord),1.0);
    //fragColor = vec4(ex10(fragCoord),1.0);
    //fragColor = vec4(ex11(fragCoord),1.0);
    //fragColor = vec4(ex12(fragCoord),1.0);
    //fragColor = vec4(ex13(fragCoord),1.0);
    fragColor = vec4(ex14(fragCoord),1.0);
}
