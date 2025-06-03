#include <Passerine.h>

class Sandbox : public Passerine::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Passerine::Application *Passerine::CreateApplication()
{
	return new Sandbox();
}