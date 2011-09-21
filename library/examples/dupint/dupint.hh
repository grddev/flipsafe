struct dupint
{
  int original;
  volatile int backup;

  dupint(const int & x) : original(x), backup(x) { }

  dupint() { }
  dupint(const dupint & x) : original(x.original), backup(x.backup) { }

  inline int value() const {
    if ( original != backup )
      fault_detected();
    return original;
  }

};
