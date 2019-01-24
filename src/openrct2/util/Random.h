#ifndef OPENRCT2_RANDOM_H
#define OPENRCT2_RANDOM_H

#include <limits>
#include "../common.h"

namespace OpenRCT2 {
template<typename _UIntType, _UIntType __M, _UIntType __SA, _UIntType __SB>
class ror_xor_ror_engine {
 public:

  using result_type = _UIntType;

  static constexpr result_type mask = __M;

  static constexpr result_type shift_a = __SA;

  static constexpr result_type shift_b = __SB;

  static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }

  static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }

  void discard(size_t N)
  {
      for (; N != 0; --N)
          (*this)();
  }

  result_type operator()()
  {
      auto S0z = _S0;
      _S0 += ror<result_type>(_S1 ^ mask, shift_a);
      _S1  = ror<result_type>(S0z, shift_b);
      return _S1;
  }

  std::pair<result_type, result_type> state() const
  {
      return {_S0, _S1};
  }

  void seed(result_type S0)
  {
      _S0 = S0;
      _S1 = S0;
  }

  void seed(result_type S0, result_type S1)
  {
      _S0 = S0;
      _S1 = S1;
  }

 private:
  result_type _S0;
  result_type _S1;
};
}

#endif //OPENRCT2_RANDOM_H
