// Copyright (C) 2012 Gustav Munkby, Hamburg University of Technology (TUHH)
template <unsigned R = 8*sizeof(unsigned int), typename T = unsigned int>
class rmbool {
public:
  static_assert( (T)~0 > 0, "Unsigned type required" );
  static_assert( (8*sizeof(T)) >= R, "Shift must not exceed data type size" );

  T i;
  inline rmbool() {}
  inline rmbool(bool value) : i(-value) {}
  inline explicit rmbool(T value) : i(value) {}

  operator bool() const {
    return (i & (i >> R)) != 0;
  }
};

template <unsigned R, typename T>
rmbool<R,T> operator !(const rmbool<R,T>& b)
{
  return rmbool<R,T>(~b.i);
}

