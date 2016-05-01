// Copyright 2016 Nikolas Beisemann <github@beisemann.email>
// This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
// code package.

#include "base64.hpp"
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <algorithm>

namespace base64
{
std::string encode(std::string const &input)
{
  namespace iterators = boost::archive::iterators;
  typedef iterators::base64_from_binary<
      iterators::transform_width< std::string::const_iterator, 6, 8 > > iterator;

  std::string output(iterator(input.begin()), iterator(input.end()));
  return output.append((3 - input.size() % 3) % 3, '=');
}

std::string decode(std::string input)
{
  namespace iterators = boost::archive::iterators;
  typedef iterators::transform_width<
      iterators::binary_from_base64< iterators::remove_whitespace< std::string::const_iterator > >,
      8, 6 > iterator;

  std::size_t padding = std::count(input.begin(), input.end(), '=');
  std::replace(input.begin(), input.end(), '=', 'A');
  std::string output(iterator(input.begin()), iterator(input.end()));
  output.erase(output.end() - padding, output.end());

  return output;
}
}
