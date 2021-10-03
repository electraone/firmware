#include "System.h"
#include "helpers.h"

void System::initialise(void)
{
  // Initialize Random number generator
  randomSeed(analogRead(0));

  // initialize non-volatile storage if not set
  runtimeInfo.read();
}

SystemTasks System::tasks;
Context System::context;
RuntimeInfo System::runtimeInfo;
lua_State *System::L = nullptr;
