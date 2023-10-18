#include <iostream>
#include "calc.h"

int run()
{
  Calc c;
  std::cout << "2 + 3 = " << c.Sum(2, 3) << std::endl;
  std::cout << "3 * 4 = " << c.Multiply(3, 4) << std::endl;
  return 0;
}
