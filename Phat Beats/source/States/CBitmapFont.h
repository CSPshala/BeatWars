/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File			:	“CBitmapFont.h”
//
//	Author			:	Steven Seufert (SMS)
//	Last Modified	:	January 15, 2012
// 
//	Purpose			:	This class handles rendering passed strings in bitmap fonts taken from a single texture.
//	Notes			:	The bitmap (and associated XML parsing) is based on the font bitmap image created by AngelCode
//					:	Bitmap Font Generator v1.12 by Andreas Jonsson (www.angelcode.com)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CBITMAPFONT_H
#define CBITMAPFONT_H

#include <Windows.h>	

#include <string>
using std::string;
#include <vector>
using std::vector;

class CBitmapFont
{
private:
	// make this a singleton class
	//static CBitmapFont* sm_Instance;		// the single instance of this class

	// constructor
	CBitmapFont()			{ m_fScale = -1; m_nCurrentFont = -1; }
	// destructor
	~CBitmapFont();
	// copy constructor
	CBitmapFont(const CBitmapFont& bitmapfont);
	// assignment operator
	CBitmapFont& operator=(const CBitmapFont& bitmapfont);

	// this struct holds all the data that a single character contains
	struct tSingleCharInfo
	{
		int		m_nPosX;			// the x position of the character's left side on the bitmap
		int		m_nPosY;			// the y position of the character's top on the bitmap
		int		m_nCharWidth;		// the width of the character
		int		m_nCharHeight;		// the height of the character
		int		m_nOffsetX;			// how much to offset the character's x value
		int		m_nOffsetY;			// how much to offset the character's y value
		int		m_nXAdvance;		// horizontal padding to apply variable character widths		

		// initialize everything to zero
		tSingleCharInfo()
		{
			m_nPosX = m_nPosY = m_nCharWidth = m_nCharHeight = m_nOffsetX = m_nOffsetY = m_nXAdvance = 0;
		}
	};

	struct tFont
	{
		char*				m_szFontName;			// the name of your font
		tSingleCharInfo*	m_sciCharacters[256];	// an array that holds up to the first 256 individual characters
		int					m_nImageID;				// the image ID as held by the texture manager
		int					m_nLineHeight;			// the total height of a single line in pixels

		// initialize everything to default values
		tFont ()
		{
			m_szFontName = "";
			for (int index = 0; index < 256; index++)
			{
				m_sciCharacters[index] = NULL;
			}
			m_nImageID = -1;
			m_nLineHeight = -1;
		}
	};

	float			m_fScale;				// the scale to draw the text
	vector<tFont*>	m_vFonts;				// if we want to have more than one font, we can store them in a vector
	int				m_nCurrentFont;			// keeps track of which font is currently being used

	///////////////////////////////////////////////////
	//	Function	:	"LoadXMLFont"
	//	
	//	Last Modified:	January 15, 2012
	//
	//	Input		:	fontName		-	the relative path of the font bitmap; searches in the resources/fonts directory
	//
	//	Return		:	RECT around the desired letter
	//
	//	Purpose		:	To obtain the rect around a particular letter in the current font's bitmap
	//////////////////////////////////////////////////
	RECT CellAlgorithm (int id);

public:
	// obtain a pointer to this class's instance
	static CBitmapFont* GetInstance();

	// accessors 
	void SetScale(float scale)		{ m_fScale = scale; }
	//void SetFont(string font);

	// mutators
	float GetScale() const			{ return m_fScale; }

	///////////////////////////////////////////////////
	//	Function	:	"PrintText"
	//	
	//	Last Modified:	January 15, 2012
	//
	//	Input		:	textToPrint		-	this is the string you want to display
	//					nPosX			-	the x position to print the left side of the string of text
	//					nPosY			-	the position of the top of the string of text
	//					dwColor			-	the ARGB or XRBG color value of the text
	//
	//	Return		:	none
	//
	//	Purpose		:	To draw the passed string of text at a given position and color
	//////////////////////////////////////////////////
	void PrintText(string textToPrint, int nPosX, int nPosY, DWORD dwColor);

	///////////////////////////////////////////////////
	//	Function	:	"PrintInRect"
	//	
	//	Last Modified:	January 15, 2012
	//
	//	Input		:	textToPrint		-	this is the string you want to display
	//					paramRect		-	a pointer to the rect defining the box in which you want to draw the text
	//					alignment		-	the enumerated alignment for the text
	//										(ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT)
	//					dwColor			-	the ARGB or XRBG color value of the text
	//
	//	Return		:	none
	//
	//	Purpose		:	To draw the passed string of text within a rect and color
	//
	//	Notes		:	Returns if a null RECT pointer is passed
	//////////////////////////////////////////////////
	void PrintInRect(string textToPrint, RECT* paramRect, int alignment, DWORD dwColor);

	void PrintStrokedText(string szText, int nPosX, int nPosY, DWORD dBgColor, DWORD dFgColor);
	void PrintStrokedTextInRect(string szText, RECT* layoutRect, int nAlignment, DWORD dBgColor, DWORD dFgColor);

	///////////////////////////////////////////////////
	//	Function	:	"LoadXMLFont"
	//	
	//	Last Modified:	January 15, 2012
	//
	//	Input		:	fontName		-	the relative path of the font bitmap; searches in the resources/fonts directory
	//
	//	Return		:	true if successful
	//
	//	Purpose		:	To load in a particular font
	//////////////////////////////////////////////////
	bool LoadXMLFont(const char* fontName);

	///////////////////////////////////////////////////
	//	Function	:	"SetFont"
	//	
	//	Last Modified:	January 15, 2012
	//
	//	Input		:	fontName		-	the name of the font you wish to set the BitmapFont class to use
	//
	//	Return		:	true if successful
	//
	//	Purpose		:	To set which font the Bitmap Font class will use
	//////////////////////////////////////////////////
	bool SetFont (string fontName);
};
#endif