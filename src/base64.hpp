// Copyright 2016 Nikolas Beisemann <github@beisemann.email>
// This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
// code package.

#ifndef BACKEND_BASE64_HPP_INCLUDED
#define BACKEND_BASE64_HPP_INCLUDED

#include <string>

namespace base64
{
/// \brief Encode blob to base64.
///
/// Arbitrary input is encoded to base64 format for storage in text formats where binary data may
/// lead to problems.
///
/// \param[in] input Blob to be encoded
/// \return Encoded data
std::string encode(std::string const &input);

/// \brief Decode base64 to blob.
///
/// Base64 input is decoded to binary format for usage.
///
/// \param[in] input Base64 input to be decoded
/// \return Decoded data
std::string decode(std::string input);
}

#endif // BACKEND_BASE64_HPP_INCLUDED
