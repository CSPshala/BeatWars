///////////////////////////////////////////////////////
// File Name	:	"CObjectFactory.h"
//
// Purpose		:	To take in and dispense objects
//////////////////////////////////////////////////////

#ifndef C_OBJECTFACTORY_H
#define C_OBJECTFACTORY_H

#include <map>
using std::map;

template <typename ClassID, typename BaseClass>
class CObjectFactory
{
public:	
	

private:
	// Construct / Deconstruct / Operators / Instance
	CObjectFactory();
	CObjectFactory(const CObjectFactory&);
	CObjectFactory& operator=(const CObjectFactory&);
	~CObjectFactory();
	static CObjectFactory<ClassID,BaseClass> sm_Instance;

	map<ClassID,ObjectCreator> m_ObjectCreators;

};


#endif