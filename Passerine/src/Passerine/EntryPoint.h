#pragma once

#ifdef PS_PLATFORM_WINDOWS

extern Passerine::Application *Passerine::CreateApplication();

int main(int argc, char **argv)
{ 
	auto app = Passerine::CreateApplication();
	app->Run();
	delete app;
}
#endif