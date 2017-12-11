#include "catch.hpp"

#include <iostream>

#include <nano_engine/systems/display/display_system.hpp>
#include <nano_engine/systems/input/input_system.hpp>
#include <nano_engine/systems/renderer/renderer.hpp>
#include <nano_engine/systems/vr/vr_system.hpp>
#include <nano_engine/engine.hpp>

extern "C"
{
  _declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001;
}

TEST_CASE("Engine is tested.", "[engine]") {
  ne::engine engine;
  auto display_system  = engine.add_system<ne::display_system>();
  auto input_system    = engine.add_system<ne::input_system>  ();
  auto renderer_system = engine.add_system<ne::renderer>      ();
  if(ne::vr_system::available()) 
    engine.add_system<ne::vr_system>(ne::tracking_mode::seated);
  auto opengl_window = display_system->create_opengl_window("Test", std::array<std::size_t, 2>{32, 32}, std::array<std::size_t, 2>{640, 480});
  input_system ->on_key_press.connect([ ] (ne::key key) { std::cout << key.name(); });
  engine.run();
}