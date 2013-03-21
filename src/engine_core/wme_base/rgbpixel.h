#ifndef RGBPIXEL_H
#define RGBPIXEL_H

	struct RGBcolor {
		unsigned char red, green, blue;
		RGBcolor () : red(0), green(0), blue(0) {}
		RGBcolor (unsigned char r, unsigned char g, unsigned char b) : red(r), green(g), blue(b) {}
		void Set (unsigned char r, unsigned char g, unsigned char b){ red = r; green = g; blue = b; }
		bool operator == (const RGBcolor& c) const { return (c.red == red) && (c.green == green) && (c.blue == blue); }
		bool operator != (const RGBcolor& c) const { return !operator == (c); }
		RGBcolor operator + (const RGBcolor& c) const { return RGBcolor (c.red + red, c.green + green, c.blue + blue); }
	};

	struct RGBPixel {
		unsigned char red, green, blue, alpha;
		RGBPixel () : red(0), green(0), blue(0), alpha(255) {}
		RGBPixel (const RGBcolor& c) : red (c.red), green (c.green), blue (c.blue), alpha (255) {}
		RGBPixel (const RGBPixel& p) : red (p.red), green (p.green), blue (p.blue), alpha (p.alpha) {}
		bool operator == (const RGBcolor& c) const { return (c.red == red) && (c.green == green) && (c.blue == blue); }
		bool operator == (const RGBPixel& p) const { return (p.red == red) && (p.green == green) && (p.blue == blue); }
		bool operator != (const RGBcolor& c) const { return !operator == (c); }
		bool operator != (const RGBPixel& p) const { return !operator == (p); }
		operator RGBcolor () const { return RGBcolor (red, green, blue); }
	};

/// Red component sensivity
#define R_COEF		173
/// Green component sensivity
#define G_COEF		242
/// Blue component sensivity
#define B_COEF		107
/// Eye sensivity to different color components, squared
/// Red component sensivity, squared
#define R_COEF_SQ	299
/// Green component sensivity, squared
#define G_COEF_SQ	587
/// Blue component sensivity, squared
#define B_COEF_SQ	114


#endif
