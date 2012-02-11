/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File			:	“CBitmapFont.cpp”
//
//	Author			:	Steven Seufert (SMS)
//	Last Modified	:	January 15, 2012
// 
//	Purpose			:	This class handles rendering passed strings in bitmap fonts taken from a single texture.
//	Notes			:	The bitmap (and associated XML parsing) is based on the font bitmap image created by AngelCode
//					:	Bitmap Font Generator v1.12 by Andreas Jonsson (www.angelcode.com)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CBitmapFont.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "Globals.h"
#include "xml\tinyxml.h"


//CBitmapFont* CBitmapFont::sm_Instance = NULL;

CBitmapFont* CBitmapFont::GetInstance()
{
	//	Lazy instantiation
	static CBitmapFont instance;
	return &instance;

	//// if the bitmap font singleton hasn't been initialzed yet
	//if (NULL == sm_Instance)
	//{
	//	sm_Instance = new CBitmapFont;
	//}

	//// return with the pointer to the object manager
	//return sm_Instance;
}

CBitmapFont::~CBitmapFont()
{

}

RECT CBitmapFont::CellAlgorithm(int id)
{
	RECT rCell = {0, 0, 0, 0};

	rCell.left		= m_vFonts[m_nCurrentFont]->m_sciCharacters[id]->m_nPosX;
	rCell.top		= m_vFonts[m_nCurrentFont]->m_sciCharacters[id]->m_nPosY;
	rCell.right		= m_vFonts[m_nCurrentFont]->m_sciCharacters[id]->m_nPosX + m_vFonts[m_nCurrentFont]->m_sciCharacters[id]->m_nCharWidth;
	rCell.bottom	= m_vFonts[m_nCurrentFont]->m_sciCharacters[id]->m_nPosY + m_vFonts[m_nCurrentFont]->m_sciCharacters[id]->m_nCharHeight;

	return rCell;
}

void CBitmapFont::PrintText(string textToPrint, int nPosX, int nPosY, DWORD dwColor)
{
		// keep track of where posX was when pass in to the function
		int nOriginalX = nPosX;

		//int length = strlen(szTextToPrint);	//if using const char*
		//the number of characters in the string
		int length = textToPrint.size();

		//	Go through string a letter at a time
		//	Calculate id by subtracting the char from FirstChar
		for (int i = 0; i < length; i++)			//strlen(szTextToPrint) if using const char*
		{
			// hold the letter
			char ch = textToPrint[i];

			// if there's a newline character, 
			if (ch == '\n')
			{
				// go down a line
				nPosY += (int)(m_vFonts[m_nCurrentFont]->m_nLineHeight * m_fScale); // one line's height times the scale
				// and reset to the left side
				nPosX = nOriginalX;
				continue;
			}
		
			// hold the ASCII value of the current letter
			int id = (int)(ch);

			// create a RECT around this letter on the font's bitmap
			RECT rLetter = CellAlgorithm(id);

			// add the x offset of this letter (scaled up or down) to the x position to draw the letter
			int drawX = nPosX + (int)(m_vFonts[m_nCurrentFont]->m_sciCharacters[id]->m_nOffsetX * m_fScale);
			// add the y offset of this letter (scaled up or down) to the y position to draw the letter
			int drawY = nPosY + (int)(m_vFonts[m_nCurrentFont]->m_sciCharacters[id]->m_nOffsetY * m_fScale);

			CSGD_TextureManager::GetInstance()->Draw(m_vFonts[m_nCurrentFont]->m_nImageID, drawX, drawY, m_fScale, m_fScale, &rLetter, 0, 0, 0, dwColor);

			// set the x position for the next letter in the string
			// the advance value holds how far to move over to the next character
			nPosX += (int)(m_vFonts[m_nCurrentFont]->m_sciCharacters[id]->m_nXAdvance * m_fScale);
		}
}

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
void CBitmapFont::PrintInRect(string textToPrint, RECT* paramRect, int alignment, DWORD dwColor)
{
	// check for a valid RECT pointer
	if (NULL == paramRect)
		return;

	//int length = strlen(szTextToPrint);	//if using const char*
	// the number of characters total in the passed string
	unsigned int stringSize = textToPrint.size();

	// determine number of lines of text
	int numLines = 0;
	for (unsigned int index = 0; index < stringSize; index++)
	{
		if (textToPrint[index] == '\n')
			numLines++;
	}

	// total height of all lines combined
	int heightOfTextBlock = (int)(m_vFonts[m_nCurrentFont]->m_nLineHeight * m_fScale);
	heightOfTextBlock += (int)(numLines *((float)m_vFonts[m_nCurrentFont]->m_nLineHeight * m_fScale));

	// the top of the block of text
	int nPosY = paramRect->top;
	int g = 1;
	/* vertically align the text */
	// Find the total vertical height of the rect.
	// Subtract the amount of space that the block of text will take up
	// this will give the amount of total empty space vertically.
	// Start the text with half that empty space from the top of the rect.
	nPosY += ((paramRect->bottom - paramRect->top) - heightOfTextBlock) / 2;
	
	int rectWidth = paramRect->right - paramRect->left;
	// the width the text takes up on this line 
	int sentenceWidth = 0;
	// the string on this line as we're building it
	string outputString = "";
	// which current letter we're on in the passed textToPrint string parameter (out of the total num chars)
	unsigned int i = 0;	
	i = 0;

	//	Go through string a letter at a time until you reach the end
	while (i < stringSize)
	{
		// new line of text; reset sentence width; clear out the output string
		sentenceWidth = 0;
		outputString.clear();

		// load up string until we hit a new line
		for (; i < stringSize; i++)
		{
			// as long as we're still on the same line
			if (textToPrint[i] != '\n')
			{
				int id = (int)textToPrint[i];
				outputString += textToPrint[i];
				if (m_vFonts[m_nCurrentFont]->m_sciCharacters[id] != NULL)
				{
					sentenceWidth += (int)(m_vFonts[m_nCurrentFont]->m_sciCharacters[id]->m_nXAdvance * m_fScale);
				}
				// if this character is registered in the character array for this font
				//if (m_vFonts[m_nCurrentFont]->m_sciCharacters[i] != NULL)
				//{
					
					//// add the letter to our output string
					//outputString += textToPrint[i];
					//sentenceWidth += (int)(m_vFonts[m_nCurrentFont]->m_sciCharacters[i]->m_nXAdvance * m_fScale);
				//}
			}
			// if you come to a newline character, go to the next character and break out of adding to this line
			else
			{
				i++;
				break;
			}
		}
		
		switch(alignment)
		{
		case ALIGN_LEFT:
			{
				PrintText(outputString, paramRect->left, nPosY, dwColor);
			}
			break;

		case ALIGN_CENTER:
			{
				int startX = paramRect->left;
				// make sure the sentence fits
				if (paramRect->right - sentenceWidth > 0)
				{
					startX += (rectWidth - sentenceWidth) / 2;
				}
				PrintText(outputString, startX, nPosY, dwColor);
			}
			break;

		case ALIGN_RIGHT:
			{
				PrintText(outputString, (paramRect->right - sentenceWidth), nPosY, dwColor);
			}
			break;
		}

		// go down to next line
		nPosY += (int)(m_vFonts[m_nCurrentFont]->m_nLineHeight * m_fScale);


	}


}

///////////////////////////////////////////////////
//	Function	:	"LoadXMLFont"
//	
//	Last Modified:	January 15, 2012
//
//	Input		:	fontName		-	the relative path of the font bitmap; searches in the resources/fonts directory
//
//	Return		:	none
//
//	Purpose		:	To load in a particular font
//////////////////////////////////////////////////
bool CBitmapFont::LoadXMLFont(const char* fontName)
{
	tFont* pNewFont = new tFont();

	int length = strlen(fontName) + 1;
	pNewFont->m_szFontName = new char[length];
	strcpy_s(pNewFont->m_szFontName, length, fontName);


	string szDirectory = "resources/fonts/";

	string szFullFileName = szDirectory + fontName;

	TiXmlDocument file;

	// Did the file open properly?
	if (false == file.LoadFile(szFullFileName.c_str()))
	{		
		return false;
	}

	// create a pointer to the root element
	TiXmlElement* pRoot = file.RootElement();

	// make sure this root is valid
	if (pRoot == NULL)
		return false;

	// record the first child element
	TiXmlElement* pInfo = pRoot->FirstChildElement("info");

	if (pInfo)
	{
		// Any of the required "info" elements get from here
	}

	TiXmlElement* pCommon = pRoot->FirstChildElement("common");

	if (pCommon)
	{
		// Any of the required "common" Element get from here
	}

	// record the next child element
	TiXmlElement* pPages = pRoot->FirstChildElement("pages");

	if (pPages)
	{
		TiXmlElement* pPage = pPages->FirstChildElement("page");

		if (pPage)
		{
			// get the file name from the XML document
			string szImgName;

			TiXmlAttribute* pAtt = pPage->FirstAttribute();

			while (pAtt)
			{
				if (strcmp(pAtt->Name(), "file") == 0)
				{
					szImgName = pAtt->Value();
				}
				pAtt = pAtt->Next();
			}

			string imgDirectory = szDirectory + szImgName;

			// send the file path to the texture manager
			pNewFont->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture(imgDirectory.c_str());

			// whoops
			if (pNewFont->m_nImageID == -1)
				return false;
		}
	}
	else
		return false;

	// record the element for the characters
	TiXmlElement* pChars = pRoot->FirstChildElement("chars");

	if (pChars)
	{
		TiXmlElement* pChar = pChars->FirstChildElement("char");

		int nID;
		tSingleCharInfo* pCI;

		// while there are more characters, get info for the character
		while (pChar)
		{
			pCI = new tSingleCharInfo();

			pChar->Attribute("id", &nID);	// char id

			pChar->Attribute("x", &pCI->m_nPosX);
			pChar->Attribute("y", &pCI->m_nPosY);
			pChar->Attribute("width", &pCI->m_nCharWidth);
			pChar->Attribute("height", &pCI->m_nCharHeight);
			pChar->Attribute("xoffset", &pCI->m_nOffsetX);
			pChar->Attribute("yoffset", &pCI->m_nOffsetY);
			pChar->Attribute("xadvance", &pCI->m_nXAdvance);
			
			pNewFont->m_nLineHeight = pCI->m_nCharHeight;
			
			// add the character to the array of sFont in the location of nID
			pNewFont->m_sciCharacters[nID] = pCI;

			// move on to the next element
			pChar = pChar->NextSiblingElement("char");
		}
	}
	else
		return false;

	// add the font ont the array
	m_vFonts.push_back(pNewFont);

	return true;
}

///////////////////////////////////////////////////
//	Function	:	"SetFont"
//	
//	Last Modified:	January 15, 2012
//
//	Input		:	fontName		-	the name of the font you wish to set the BitmapFont class to use
//
//	Return		:	none
//
//	Purpose		:	To set which font the Bitmap Font class will use
//////////////////////////////////////////////////
bool CBitmapFont::SetFont(string fontName)
{
	unsigned int size = m_vFonts.size();
	
	// iterate through the vector of loaded fonts
	for (unsigned int index = 0; index < size; index++)
	{
		// if the passed in font name is found in the loaded fonts
		if (strcmp(m_vFonts[index]->m_szFontName, fontName.c_str()) == 0)
		{
			// set the BitmapFont class to use this particular font for drawing text
			m_nCurrentFont = index;
			// You found a match, so break out of the search
			return true;
		}
	}

	// a match was not found, return with failure
	return false;
}
