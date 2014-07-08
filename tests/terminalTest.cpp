#include "../lumina/util/TerminalLook.hpp"

#include <iostream>
using namespace std;

using namespace lumina;

int main() {
  cout << "    \t" << "Black\t" << "Red\t" << "Green\t" 
       << "Yellow\t" << "Blue\t" << "Magenta\t" << "Cyan\t" << "White\t"
       << "Reset" << endl;

  cout << TL::Reset << "Normal\t"
       << TL::Black << "Text \t" 
       << TL::Red << "Text \t" 
       << TL::Green << "Text \t" 
       << TL::Yellow << "Text \t" 
       << TL::Blue << "Text \t"
       << TL::Magenta << "Text \t" 
       << TL::Cyan << "Text \t" 
       << TL::White << "Text \t"
       << TL::ResetText << "Text \t"
       << endl;


  cout << TL::Reset << "Bold  \t" << TL::Bold
       << TL::Black << "Text \t" 
       << TL::Red << "Text \t" 
       << TL::Green << "Text \t" 
       << TL::Yellow << "Text \t" 
       << TL::Blue << "Text \t"
       << TL::Magenta << "Text \t" 
       << TL::Cyan << "Text \t" 
       << TL::White << "Text \t"
       << TL::ResetText << "Text \t"
       << endl;


  cout << TL::Reset << "Thin  \t" << TL::Thin
       << TL::Black << "Text \t" 
       << TL::Red << "Text \t" 
       << TL::Green << "Text \t" 
       << TL::Yellow << "Text \t" 
       << TL::Blue << "Text \t"
       << TL::Magenta << "Text \t" 
       << TL::Cyan << "Text \t" 
       << TL::White << "Text \t"
       << TL::ResetText << "Text \t"
       << endl;



  cout << TL::Reset << "Reset  \t" << TL::ResetWeight
       << TL::Black << "Text \t" 
       << TL::Red << "Text \t" 
       << TL::Green << "Text \t" 
       << TL::Yellow << "Text \t" 
       << TL::Blue << "Text \t"
       << TL::Magenta << "Text \t" 
       << TL::Cyan << "Text \t" 
       << TL::White << "Text \t"
       << TL::ResetText << "Text \t"
       << endl;

  cout << TL::Reset << "BG  \t"
       << TL::BlackBG << "     \t" 
       << TL::RedBG << "     \t" 
       << TL::GreenBG << "     \t" 
       << TL::YellowBG << "     \t" 
       << TL::BlueBG << "     \t"
       << TL::MagentaBG << "     \t" 
       << TL::CyanBG << "     \t" 
       << TL::WhiteBG << "     \t"
       << TL::ResetBG << "     \t"
       << endl;

  cout << TL::Reset << endl;
  cout << "Everything resetted!" << endl;
}