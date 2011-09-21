struct dupint_: public dupint
{
  dupint_() { }
  dupint_(const int & x) : dupint(x) { }
  dupint_(const dupint & x) : dupint(x) { }

  inline operator int() { return value(); }
};
