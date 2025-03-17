#include <WString.h>
#ifndef __CONSOLE__
#define __CONSOLE__

class Console
{
public:
  Console();
  void log(const char *msg);
  void log(const String &msg);
  void log(int value);
  void log(float value);
};

#endif