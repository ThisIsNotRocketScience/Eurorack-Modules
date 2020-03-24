/*

	modified from 70s Wallpaper
	-------------
*/


void mainImage( out vec4 o, vec2 p )
{
    p*=0.14;
    p.y *= (sin(p.y))*0.01 + 1.0;
    p.x *= sign(cos(length(ceil(p /= 14.+floor(p.y/20.) ))*99.));
    o = o - o + cos(min(length(p = fract(p)), length(--p+sin(p)))*53.); // --p - Thanks, Coyote.
}

