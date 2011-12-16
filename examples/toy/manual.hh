void example_manual()
{
  int block = 0;
  int secure[2] = {0,0};
  while (wait_for_input()) {
    sihft::assert(block == 0);
    block = 1;
    sihft::assert(secure[0] == secure[1]);
    if (is_button_pressed() && secure[0]) {
      sihft::assert(block == 1);
      block = 2;
      open_door();
      sihft::assert(block == 2);
      block = 1;
    }
    else if (is_switch_pressed()) {
      sihft::assert(block == 1);
      block = 3;
      sihft::assert(secure[0] == secure[1]);
      secure[0] = !secure[0];
      secure[1] = !secure[0];
      sihft::assert(block == 3);
      block = 1;
    }
    sihft::assert(block == 1);
    block = 0;
  }
  sihft::assert(block == 0);
}

