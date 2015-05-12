#include "StdAfx.h"
#include "HRBWin.h"
using namespace HRBCP;

void CreateForm()
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Window());
}