#pragma once
/**
 * \file InputEvent.hpp
 * This file is part of the Lumina Graphics Framework.
 * 
 * \author Lukas Kalbertodt <lukas.kalbertodt@gmail.com>
 * 
 * This file defines some enums and structs used for input events (mouse
 * and keyboard).
 */
#include "KeyCode.hpp"

#include <cstring>

namespace lumina {

// type of input event
enum class InputType {
  Key,
  Mouse
};

// type of mouse event
enum class MouseInputType {
  LButtonPressed, 
  MButtonPressed,
  RButtonPressed,
  LButtonReleased,
  MButtonReleased,
  RButtonReleased,
  MoveDirection,
  MovePosition
};

// data about mouse input event
struct MouseInput {
  float x;
  float y;
  int wheel;
  MouseInputType type;
};

// type of key event
enum KeyEventType {
  KeyReleased,
  KeyPressed,
  KeyHold,
  Character
};

// data about key input event
struct KeyInput {
  KeyCode key;
  unsigned char c;
  KeyEventType type;
};

// input event 
struct InputEvent {
  InputType type;

  union {
    KeyInput keyInput;
    MouseInput mouseInput;
  };

  InputEvent() {
    memset(this, 0, sizeof(*this));
  }
};

// says what was done with the event
enum class EventResult {
  Skipped,
  Processed
};

} // namespace lumina