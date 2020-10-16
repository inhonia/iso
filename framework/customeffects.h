#include "main.h"

class chroma : public singleton<chroma>, public CChromaSDKImpl
{
public:
	void initialize();
	void kill();
	void on_inject();
	void in_game();
private:

protected:

};