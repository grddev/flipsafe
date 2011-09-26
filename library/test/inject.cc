#include <climits>
#include <iostream>

static int global_counter;

template <typename T>
struct faulty
{
  mutable T value;

  faulty() {}
  faulty(const T & x) : value(x) {}

  faulty & operator=(const T & x) { value = x; }

  inline void inject() const
  {
    if (global_counter >= 0)
    {
      const int bitcount = CHAR_BIT * sizeof(T);
      if (global_counter < bitcount)
        ((char *)&value)[global_counter / CHAR_BIT] ^= (1 << (global_counter % CHAR_BIT));

      global_counter -= bitcount;
    }
  }


  operator const int () const
  {
    inject();
    return value;
  }

};

// TODO: Compound assignment operators

int under_test(int input)
{
  faulty<float> x = input;
  if (x <= 2)
    x = x + 1;
  return x;
}

int main()
{
  int start = 0;
  do {
    global_counter = start++;
    std::cout << "Result " << under_test(1) << " expected " << 2 << std::endl;
  }
  while (global_counter < 0);

  // Last injection is fault free
  std::cout << "Injected " << (start - 1) << " faults" << std::endl;

}
