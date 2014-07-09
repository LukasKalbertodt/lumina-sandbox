#include "../lumina/service/Logger.hpp"
// #include "../lumina/util/TerminalLook.hpp"

#include <iostream>
using namespace std;

using namespace lumina;

int main() {
  Logger l;
  l.log<LogLevel::Critical>(" CriticalTest");
  l.log<LogLevel::Error>(" ErrorTest");
  l.log<LogLevel::Warning>(" WarningTest");
  l.log<LogLevel::Notice>(" NoticeTest");
  l.log<LogLevel::Info>(" InfoTest");
  l.log<LogLevel::Debug>(" DebugTest");
}