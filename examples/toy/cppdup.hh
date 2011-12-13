struct cppdup
{
  int orig, back;

  cppdup() {}

  cppdup(int x)
    : orig(x),
      back(sihft::protected_clone(x)) {}

  cppdup(int porig, int pback)
    : orig(porig), back(pback) {}

  operator int()
  {
    sihft::assert(orig == back);
    return orig;
  }
};

cppdup operator !(const cppdup & x)
{
  sihft::assert(x.orig == x.back);
  return cppdup(!x.orig, !x.back);
}

