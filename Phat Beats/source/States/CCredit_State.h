#ifndef _CCREDIT_STATE_H_
#define _CCREDIT_STATE_H_
#include "IGameState.h"
#include "../CGame.h"
#include "..\SGD Wrappers\CSGD_Direct3D.h"
#include "..\SGD Wrappers\CSGD_DirectInput.h"
#include "..\SGD Wrappers\CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_FModManager.h"
#include "CBitmapFont.h"

// STL
#include <vector>
#include <string>
using std::vector;
using std::string;

// Forward Declarations
class CBitmapFont;

class CCredit_State : public IGameState
{
private:
	// Proper singleton
	CCredit_State(const CCredit_State&);
	//		Assignment op
	CCredit_State& operator=(const CCredit_State&);
	//		Destructor
	~CCredit_State();
	//		Constructor
	CCredit_State();

	// for the bitfont
	CBitmapFont* m_bFont;

	// image id's and sound id's
	int m_nBackgroundID;
	int m_nLogo;
	int m_nSoundID;
	
	// vector for the names resposible
	vector<string> m_vNames;
	IDirect3DTexture9* m_RtText;
	LPDIRECT3DTEXTURE9 m_TexCredits;
	LPDIRECT3DTEXTURE9 LoadTexture(const char* szFileName, DWORD dwColorkey = 0);
	
	struct CUSTOMVERTEX {FLOAT X, Y, Z; DWORD COLOR; FLOAT U, V;};

	IDirect3DVertexBuffer9  *quadbuff;
	IDirect3DVertexDeclaration9 *cubedecl;
	IDirect3DIndexBuffer9* IndexBuffer;

	int m_nSwitchState;

public:
	static CCredit_State* GetInstance();
	void Enter(void); // Enters the Game State
	bool Input(void); // Input
	void Update(); // Update
	void Render(void); // Draw
	void Exit(void); // Leaves the Game State	

};
#endif