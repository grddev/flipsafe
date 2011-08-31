#include <iostream>

struct snorkel
{
};

struct gurka
{
  /* Cannot have both... darn
  struct gurka* operator &() { return this; }
  bool operator &() { return value; }

  operator const bool *() const { return &value; }
   */
  gurka operator&&(const bool b)
  {
    std::cout << "Test" << std::endl;
    return *this;
  }
  operator const bool () const { return value; }

  bool value;
};


int main()
{
  gurka g;
  g.value = true;
  
  gurka & g2 = g;

  gurka g3 = g && g2;

  if (g && g2)
    return 2;

  return (&g == &g2);
}
