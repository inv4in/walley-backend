// Copyright 2016 Nikolas Beisemann <github@beisemann.email>
// This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
// code package.

#ifndef BACKEND_AES_HPP_INCLUDED
#define BACKEND_AES_HPP_INCLUDED

#include <string>

namespace aes
{
/// \brief Encrypt data blob with AES chiffre.
///
/// Outputs the encrypted data from given input data using the given password as key rightpadded by
/// some value up to 256 bits. Passwords larger than 256 bits are truncated.
///
/// \param[in] password Key to be used for encryption
/// \param[in] input Data to be encrypted
/// \return Encrypted data
std::string encrypt(std::string const &password, std::string const &input);

/// \brief Decrypt data blob with AES chiffre.
///
/// Outputs the decrypted data from given input data using the given password as key rightpadded by
/// some value up to 256 bits. Passwords larger than 256 bits are truncated.
///
/// \param[in] password Key to be used for decryption
/// \param[in] input Data to be decrypted
/// \return Decrypted data
std::string decrypt(std::string const &password, std::string const &input);
}

#endif // BACKEND_AES_HPP_INCLUDED
