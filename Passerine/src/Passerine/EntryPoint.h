#pragma once

#ifdef PS_PLATFORM_WINDOWS

extern Passerine::Application *Passerine::CreateApplication();

int main(int argc, char **argv)
{ 
	Passerine::Log::Init();
	PS_CORE_WARN("Initialize Core Log!");
	int a = 5;
	PS_INFO("Var={0}", a);

	auto app = Passerine::CreateApplication();
	app->Run();
	delete app;
}
#endif