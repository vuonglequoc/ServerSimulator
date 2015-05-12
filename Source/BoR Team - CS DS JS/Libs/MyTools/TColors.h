//-------------------------------------------------------------------------------------------------------------------------------------------
#ifndef COLORS_H
#define COLORS_H
//-------------------------------------------------------------------------------------------------------------------------------------------

#include <windows.h>
//-------------------------------------------------------------------------------------------------------------------------------------------

class CColors
{
public:
	CColors() {};
	virtual ~CColors() {};
	// ----
	COLORREF	AliceBlue()				{ return 0xF0F8FF; };
	COLORREF	AntiqueWhite()			{ return 0xFAEBD7; };
	COLORREF	Aqua()					{ return 0x00FFFF; };
	COLORREF	Aquamarine()			{ return 0x7FFFD4; };
	COLORREF	Azure()					{ return 0xF0FFFF; };
	COLORREF	Beige()					{ return 0xF5F5DC; };
	COLORREF	Bisque()				{ return 0xFFE4C4; };
	COLORREF	Black()					{ return 0x000000; };
	COLORREF	BlanchedAlmond()		{ return 0xFFEBCD; };
	COLORREF	Blue()					{ return 0x0000FF; };
	COLORREF	BlueViolet()			{ return 0x8A2BE2; };
	COLORREF	Brown()					{ return 0xA52A2A; };
	COLORREF	BurlyWood()				{ return 0xDEB887; };
	COLORREF	CadetBlue()				{ return 0x5F9EA0; };
	COLORREF	Chartreuse()			{ return 0x7FFF00; };
	COLORREF	Chocolate()				{ return 0xD2691E; };
	COLORREF	Coral()					{ return 0xFF7F50; };
	COLORREF	CornflowerBlue()		{ return 0x6495ED; };
	COLORREF	Cornsilk()				{ return 0xFFF8DC; };
	COLORREF	Crimson()				{ return 0xDC143C; };
	COLORREF	Cyan()					{ return 0x00FFFF; };
	COLORREF	DarkBlue()				{ return 0x00008B; };
	COLORREF	DarkCyan()				{ return 0x008B8B; };
	COLORREF	DarkGoldenrod()			{ return 0xB8860B; };
	COLORREF	DarkGray()				{ return 0xA9A9A9; };
	COLORREF	DarkGreen()				{ return 0x006400; };
	COLORREF	DarkKhaki()				{ return 0xBDB76B; };
	COLORREF	DarkMagenta()			{ return 0x8B008B; };
	COLORREF	DarkOliveGreen()		{ return 0x556B2F; };
	COLORREF	DarkOrange()			{ return 0xFF8C00; };
	COLORREF	DarkOrchid()			{ return 0x9932CC; };
	COLORREF	DarkRed()				{ return 0x8B0000; };
	COLORREF	DarkSalmon()			{ return 0xE9967A; };
	COLORREF	DarkSeaGreen()			{ return 0x8FBC8B; };
	COLORREF	DarkSlateBlue()			{ return 0x483D8B; };
	COLORREF	DarkSlateGray()			{ return 0x2F4F4F; };
	COLORREF	DarkTurquoise()			{ return 0x00CED1; };
	COLORREF	DarkViolet()			{ return 0x9400D3; };
	COLORREF	DeepPink()				{ return 0xFF1493; };
	COLORREF	DeepSkyBlue()			{ return 0x00BFFF; };
	COLORREF	DimGray()				{ return 0x696969; };
	COLORREF	DodgerBlue()			{ return 0x1E90FF; };
	COLORREF	Firebrick()				{ return 0xB22222; };
	COLORREF	FloralWhite()			{ return 0xFFFAF0; };
	COLORREF	ForestGreen()			{ return 0x228B22; };
	COLORREF	Fuchsia()				{ return 0xFF00FF; };
	COLORREF	Gainsboro()				{ return 0xDCDCDC; };
	COLORREF	GhostWhite()			{ return 0xF8F8FF; };
	COLORREF	Gold()					{ return 0xFFD700; };
	COLORREF	Goldenrod()				{ return 0xDAA520; };
	COLORREF	Gray()					{ return 0x808080; };
	COLORREF	Green()					{ return 0x008000; };
	COLORREF	GreenYellow()			{ return 0xADFF2F; };
	COLORREF	Honeydew()				{ return 0xF0FFF0; };
	COLORREF	HotPink()				{ return 0xFF69B4; };
	COLORREF	IndianRed()				{ return 0xCD5C5C; };
	COLORREF	Indigo()				{ return 0x4B0082; };
	COLORREF	Ivory()					{ return 0xFFFFF0; };
	COLORREF	Khaki()					{ return 0xF0E68C; };
	COLORREF	Lavender()				{ return 0xE6E6FA; };
	COLORREF	LavenderBlush()			{ return 0xFFF0F5; };
	COLORREF	LawnGreen()				{ return 0x7CFC00; };
	COLORREF	LemonChiffon()			{ return 0xFFFACD; };
	COLORREF	LightBlue()				{ return 0xADD8E6; };
	COLORREF	LightCoral()			{ return 0xF08080; };
	COLORREF	LightCyan()				{ return 0xE0FFFF; };
	COLORREF	LightGoldenrodYellow()	{ return 0xFAFAD2; };
	COLORREF	LightGray()				{ return 0xD3D3D3; };
	COLORREF	LightGreen()			{ return 0x90EE90; };
	COLORREF	LightPink()				{ return 0xFFB6C1; };
	COLORREF	LightSalmon()			{ return 0xFFA07A; };
	COLORREF	LightSeaGreen()			{ return 0x20B2AA; };
	COLORREF	LightSkyBlue()			{ return 0x87CEFA; };
	COLORREF	LightSlateGray()		{ return 0x778899; };
	COLORREF	LightSteelBlue()		{ return 0xB0C4DE; };
	COLORREF	LightYellow()			{ return 0xFFFFE0; };
	COLORREF	Lime()					{ return 0x00FF00; };
	COLORREF	LimeGreen()				{ return 0x32CD32; };
	COLORREF	Linen()					{ return 0xFAF0E6; };
	COLORREF	Magenta()				{ return 0xFF00FF; };
	COLORREF	Maroon()				{ return 0x800000; };
	COLORREF	MediumAquamarine()		{ return 0x66CDAA; };
	COLORREF	MediumBlue()			{ return 0x0000CD; };
	COLORREF	MediumOrchid()			{ return 0xBA55D3; };
	COLORREF	MediumPurple()			{ return 0x9370DB; };
	COLORREF	MediumSeaGreen()		{ return 0x3CB371; };
	COLORREF	MediumSlateBlue()		{ return 0x7B68EE; };
	COLORREF	MediumSpringGreen()		{ return 0x00FA9A; };
	COLORREF	MediumTurquoise()		{ return 0x48D1CC; };
	COLORREF	MediumVioletRed()		{ return 0xC71585; };
	COLORREF	MidnightBlue()			{ return 0x191970; };
	COLORREF	MintCream()				{ return 0xF5FFFA; };
	COLORREF	MistyRose()				{ return 0xFFE4E1; };
	COLORREF	Moccasin()				{ return 0xFFE4B5; };
	COLORREF	NavajoWhite()			{ return 0xFFDEAD; };
	COLORREF	Navy()					{ return 0x000080; };
	COLORREF	OldLace()				{ return 0xFDF5E6; };
	COLORREF	Olive()					{ return 0x808000; };
	COLORREF	OliveDrab()				{ return 0x6B8E23; };
	COLORREF	Orange()				{ return 0xFFA500; };
	COLORREF	OrangeRed()				{ return 0xFF4500; };
	COLORREF	Orchid()				{ return 0xDA70D6; };
	COLORREF	PaleGoldenrod()			{ return 0xEEE8AA; };
	COLORREF	PaleGreen()				{ return 0x98FB98; };
	COLORREF	PaleTurquoise()			{ return 0xAFEEEE; };
	COLORREF	PaleVioletRed()			{ return 0xDB7093; };
	COLORREF	PapayaWhip()			{ return 0xFFEFD5; };
	COLORREF	PeachPuff()				{ return 0xFFDAB9; };
	COLORREF	Peru()					{ return 0xCD853F; };
	COLORREF	Pink()					{ return 0xFFC0CB; };
	COLORREF	Plum()					{ return 0xDDA0DD; };
	COLORREF	PowderBlue()			{ return 0xB0E0E6; };
	COLORREF	Purple()				{ return 0x800080; };
	COLORREF	Red()					{ return 0xFF0000; };
	COLORREF	RosyBrown()				{ return 0xBC8F8F; };
	COLORREF	RoyalBlue()				{ return 0x4169E1; };
	COLORREF	SaddleBrown()			{ return 0x8B4513; };
	COLORREF	Salmon()				{ return 0xFA8072; };
	COLORREF	SandyBrown()			{ return 0xF4A460; };
	COLORREF	SeaGreen()				{ return 0x2E8B57; };
	COLORREF	SeaShell()				{ return 0xFFF5EE; };
	COLORREF	Sienna()				{ return 0xA0522D; };
	COLORREF	Silver()				{ return 0xC0C0C0; };
	COLORREF	SkyBlue()				{ return 0x87CEEB; };
	COLORREF	SlateBlue()				{ return 0x6A5ACD; };
	COLORREF	SlateGray()				{ return 0x708090; };
	COLORREF	Snow()					{ return 0xFFFAFA; };
	COLORREF	SpringGreen()			{ return 0x00FF7F; };
	COLORREF	SteelBlue()				{ return 0x4682B4; };
	COLORREF	Tan()					{ return 0xD2B48C; };
	COLORREF	Teal()					{ return 0x008080; };
	COLORREF	Thistle()				{ return 0xD8BFD8; };
	COLORREF	Tomato()				{ return 0xFF6347; };
	COLORREF	Transparent()			{ return 0xFFFFFF; };
	COLORREF	Turquoise()				{ return 0x40E0D0; };
	COLORREF	Violet()				{ return 0xEE82EE; };
	COLORREF	Wheat()					{ return 0xF5DEB3; };
	COLORREF	White()					{ return 0xFFFFFF; };
	COLORREF	WhiteSmoke()			{ return 0xF5F5F5; };
	COLORREF	Yellow()				{ return 0xFFFF00; };
};
//-------------------------------------------------------------------------------------------------------------------------------------------

extern CColors g_Colors;
//-------------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------------
#endif /* COLORS_H */
//-------------------------------------------------------------------------------------------------------------------------------------------


