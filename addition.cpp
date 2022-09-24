#include "papaya.hpp"

using namespace basic;
using namespace basic::extension;

int main()
{
  std::cout << RetVal(calldata(0) + calldata(0x20));
}
