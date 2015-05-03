#include "Script.h"
// #include <GameObject.h>
#include <scripts/ScriptsRegistration.h>

void Script::OnCreate(){
	// Do nothing
};
	
void Script::OnUpdate(){
	// Do nothing
};

void Script::OnDestroy(){
	// Do nothing
};

Script * Script::GetScriptByName(Transform * transform, std::string scriptName) {
	// Create script
	Script * script;
#include "ScriptsRegistration.cpp"

	// Set transform reference
	script->transform = transform;

	// Return script
	return script;
}