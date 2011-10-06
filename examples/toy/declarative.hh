void example_declarative()
{
  sihft::dup<int> secure = 0;
  sihft::cflow_check s0(0);
  while (wait_for_input()) {
    sihft::cflow_check s1(1, 0);
    if (is_button_pressed() && secure) {
      sihft::cflow_check s2(2, 1);
      launch_missiles();
    }
    else if (is_switch_pressed()) {
      sihft::cflow_check s3(3, 1);
      secure = !secure;
    }
  }
}
