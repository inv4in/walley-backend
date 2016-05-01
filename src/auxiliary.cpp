// Copyright 2016 Nikolas Beisemann <github@beisemann.email>
// This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
// code package.

#include "auxiliary.hpp"
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>
#include <climits>

namespace auxiliary
{
std::string generate_password(std::size_t length, std::string const &special_characters)
{
  std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
  alphabet += special_characters;
  std::string output(length, 0x0);

  boost::random::random_device rng;
  boost::random::uniform_int_distribution<> index_dist(0, alphabet.size() - 1);
  for (std::size_t k = 0; k < length; ++k)
  {
    output[k] = alphabet[index_dist(rng)];
  }

  return output;
}

void secure_erase(std::string const &filename, std::size_t iterations)
{
  std::fstream file(filename.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary);
  if (file)
  {
    file.seekg(0, std::fstream::end);
    std::size_t file_length = static_cast< std::size_t >(file.tellg());
    file.seekg(0, std::fstream::beg);

    boost::random::random_device rng;
    boost::random::uniform_int_distribution<> index_dist(CHAR_MIN, CHAR_MAX);
    std::vector< char > random_data(file_length, 0x0);

    while (iterations--)
    {
      BOOST_FOREACH (char &c, random_data)
      {
        c = static_cast< char >(index_dist(rng));
      }
      file.write(random_data.data(), random_data.size());
      file.seekg(0, std::fstream::beg);
    }
    file.close();

    try
    {
      boost::filesystem::remove(filename);
    }
    catch (std::exception const &)
    {
      throw file_access_error();
    }
  }
  else
  {
    throw file_access_error();
  }
}

std::string map_file(std::string const &content)
{
  boost::filesystem::path temp = boost::filesystem::unique_path();
  std::ofstream file(temp.native().c_str(), std::ofstream::binary);
  if (file)
  {
    file.write(content.c_str(), content.size());
    file.close();
    return temp.native();
  }
  throw file_access_error();
}
}
