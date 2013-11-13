#ifndef WMEUTILS_H
#define WMEUTILS_H

#include <wme_plugin.h>

class CWmeUtils
{
public:
	CWmeUtils(void);
	~CWmeUtils(void);

	static int Random(int From, int To);
	
	static unsigned long MakeRGBA(unsigned char R, unsigned char G, unsigned char B, unsigned char A);
	static unsigned char GetR(unsigned long Color);	
	static unsigned char GetG(unsigned long Color);
	static unsigned char GetB(unsigned long Color);
	static unsigned char GetA(unsigned long Color);

	static void TransferString(IWmeSerialMgr* SerialMgr, char** Str);

private:
	static bool m_RandInit;
};


#endif // WMEUTILS_H
