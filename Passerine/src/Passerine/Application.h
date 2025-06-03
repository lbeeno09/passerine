#pragma once

#include "Core.h"

namespace Passerine
{
	class PASSER_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		
	};
		
	// Define in client
	Application *CreateApplication();
}

