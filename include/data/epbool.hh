// Copyright (C) 2012 Gustav Munkby, Hamburg University of Technology (TUHH)
template <unsigned P = 8, typename T = unsigned int>
class epbool {
public:
  static_assert((T)~0 > 0, "Unsigned type required");
  static_assert((8*sizeof(T)) % P == 0, "Partitions not same sizes");

  static const T l = (T)~0 / ((1<<P) - 1);
  static const T m = (1<<(P-1)) * l;

  T i;
  inline epbool() {}
  inline epbool(bool value) : i(-value) {}
  inline explicit epbool(T value) : i(value) {}

  operator bool() const {
    return (m & (i-l) & ~i) == 0;
  }
};

template <unsigned P, typename T>
epbool<P,T> operator !(const epbool<P,T>& b)
{
  return epbool<P,T>(~b.i);
}

