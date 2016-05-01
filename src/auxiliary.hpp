// Copyright 2016 Nikolas Beisemann <github@beisemann.email>
// This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
// code package.

#ifndef BACKEND_AUXILIARY_HPP_INCLUDED
#define BACKEND_AUXILIARY_HPP_INCLUDED

#include <string>
#include <stdexcept>

namespace auxiliary
{
/// \brief Generate random password.
///
/// Passwords will always use the letters A-Z in upper and lower case and the digits 0-9. If
/// desired, an additional alphabet of special characters can be used to further increase entropy.
///
/// \param[in] length Length out the password to be generated
/// \param[in] special_characters Additional alphabet of special characters
/// \return Generated password
std::string generate_password(std::size_t length, std::string const &special_characters =
                                                      "!@#$%^&*()`~-_=+[{]}\\|;:'\",<.>/?");

/// \brief Erase file from hard drive and make it difficult to recover.
///
/// Overwrite the file on hard drive with random content for a given number of iterations before
/// deleting the file. On modern hard drives, even one iteration should be enough to make recovery
/// impossible. Especially on SSDs the iterations count should be reduced (or configurable) to
/// avoid useless writes.
///
/// \param[in] filename File to be erased
/// \param[in] iterations Number of times to overwrite the content of the file
void secure_erase(std::string const &filename, std::size_t iterations = 10);

/// \brief Map arbitrary data as a temporary file on disk.
///
/// A temporary file will be created to access given content. It is advisable to remove the file
/// manually when it is no longer in use. Ideally, the temporary file will be mapped into RAM
/// directly, making access to it virtually impossible when it's not mapped in the current session,
/// but there is no guarantee this will happen due to cross platform concerns.
///
/// \param[in] content Content to be mapped into file
/// \return Absolute path to temporary file
std::string map_file(std::string const &content);

/// \brief Error to be thrown in case of an invalid file access.
class file_access_error : public std::runtime_error
{
public:
  /// \brief Automatically set error appropriate error message.
  file_access_error() : std::runtime_error("file access failed") {}
};
}

#endif // BACKEND_AUXILIARY_HPP_INCLUDED
