void example_cdup()
{
  cdup secure = cdup_init(0);
  while (wait_for_input()) {
    if (is_button_pressed()) {
      if (cdup_val(secure))
        launch_missiles();
    }
    else if (is_switch_pressed()) {
      secure = cdup_neg(secure);
    }
  }
}
