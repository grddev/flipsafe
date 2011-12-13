void example_cppdup()
{
  cppdup secure = 0;
  while (wait_for_input()) {
    if (is_button_pressed() && secure) {
      launch_missiles();
    }
    else if (is_switch_pressed()) {
      secure = !secure;
    }
  }
}
