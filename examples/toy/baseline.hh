void example_baseline()
{
  int secure = 0;
  while (wait_for_input()) {
    if (is_button_pressed() && secure) {
      open_door();
    }
    else if (is_switch_pressed()) {
      secure = !secure;
    }
  }
}
