// Copyright (C) 2012 Gustav Munkby
#pragma once
template <typename T, typename U, int A>
inline dup<T>& operator +=(dup<T>& lhs, const trump<U,A>& rhs) {
  lhs.original += rhs.original;
  lhs.backup += rhs.backup / A;
  return lhs;
}
