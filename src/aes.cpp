// Copyright 2016 Nikolas Beisemann <github@beisemann.email>
// This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
// code package.

#include "aes.hpp"
#include <crypto++/modes.h>
#include <crypto++/aes.h>
#include <crypto++/filters.h>
#include <vector>

namespace aes
{
static std::vector< unsigned char > create_key(std::string const &password)
{
  std::size_t const key_size = 32;

  std::vector< unsigned char > output(password.begin(), password.end());
  output.resize(key_size, 0x0);
  return output;
}

std::string encrypt(std::string const &password, std::string const &input)
{
  std::string output;
  std::vector< unsigned char > key = create_key(password);
  std::vector< unsigned char > iv(CryptoPP::AES::BLOCKSIZE, 0x0);

  CryptoPP::AES::Encryption aes(key.data(), key.size());
  CryptoPP::CBC_CTS_Mode_ExternalCipher::Encryption cbc(aes, iv.data());
  CryptoPP::StreamTransformationFilter filter(cbc, new CryptoPP::StringSink(output));
  filter.Put(reinterpret_cast< unsigned char const * >(&input[0]), input.size());
  filter.MessageEnd();

  return output;
}

std::string decrypt(std::string const &password, std::string const &input)
{
  std::string output;
  std::vector< unsigned char > key = create_key(password);
  std::vector< unsigned char > iv(CryptoPP::AES::BLOCKSIZE, 0x0);

  CryptoPP::AES::Decryption aes(key.data(), key.size());
  CryptoPP::CBC_CTS_Mode_ExternalCipher::Decryption cbc(aes, iv.data());
  CryptoPP::StreamTransformationFilter filter(cbc, new CryptoPP::StringSink(output));
  filter.Put(reinterpret_cast< unsigned char const * >(&input[0]), input.size());
  filter.MessageEnd();

  return output;
}
}
