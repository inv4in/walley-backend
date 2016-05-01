// Copyright 2016 Nikolas Beisemann <github@beisemann.email>
// This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
// code package.

#ifndef BACKEND_STORAGE_HPP_INCLUDED
#define BACKEND_STORAGE_HPP_INCLUDED

#include <boost/property_tree/ptree.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stdexcept>

namespace walley
{
struct login_type;
struct note_type;
struct file_type;
struct contact_type;

/// \class container walley.hpp walley.hpp
/// \brief Password store manager.
///
/// A password store manages four different types of data: login credentials, arbitary textual
/// notes, arbitrary binary data, and contacts. Each type of data is subdivided into user defined
/// categories for an easier navigation within the store, essentially creating a folder structure
/// with fixed depth of one, where an empty category resembles the root folder.
///
/// Password stores are encrypted with AES-256 block chiffre, which is a symmetric algorithm deemed
/// secure for confidential documents. Of course, the integrity of the store is determined by a
/// strong master password.
struct container
{
  /// \brief Load store from memory.
  ///
  /// Given that the contents of a store are already located in memory, this functions can be used
  /// for decryption and subsequent data access. An exception is thrown if the password is invalid,
  /// or the data is not of valid format after decryption. It is not possible to tell whether the
  /// file is actually a valid password store without the correct password.
  ///
  /// \param[in] password Master password for store
  /// \param[in] input Saved store from memory
  ///
  /// \see load_from_file()
  /// \see save()
  void load(std::string const &password, std::string const &input);

  /// \brief Load store from file.
  ///
  /// Load a file into memory and use the load() function for decryption of that file's contents.
  /// Throws an exception if the file cannot be opened, decrypted, or is not of valid format.
  ///
  /// \param[in] password Master password for store
  /// \param[in] filename Path of store to be decrypted
  ///
  /// \see load()
  /// \see save_to_file()
  void load_from_file(std::string const &password, std::string const &filename);

  /// \brief Save to memory.
  ///
  /// Encrypts content of a password store to memory for storing it somewhere. Has no other effects
  /// on the store.
  ///
  /// \param[in] password Master password for store
  /// \return Encrypted data
  ///
  /// \see save_to_file()
  /// \see load()
  std::string save(std::string const &password) const;

  /// \brief Save to file.
  ///
  /// Saves content of a store to memory using the save() function and then writes the encrypted
  /// data to disk for further usage. Throws an exception if the data could not be written to disk
  /// successfully. Will silently overwrite if such a file exists already.
  ///
  /// \param[in] password Master password for store
  /// \param[in] filename Path of the store to be encrypted
  ///
  /// \see save()
  /// \see load_from_file()
  void save_to_file(std::string const &password, std::string const &filename) const;

  /// \brief Clears all stored data.
  ///
  /// Any unsaved changes will be lost. After this function completes, the store is in the same
  /// state as a freshly constructed store.
  void clear();

  /// \brief Different types of stored information.
  enum content_type
  {
    TYPE_LOGIN,
    TYPE_NOTE,
    TYPE_FILE,
    TYPE_CONTACT
  };

  /// \brief List available categories of a given content type.
  ///
  /// \param[in] t Content type
  /// \return Set of user defined categories for selected content type
  std::set< std::string > categories(content_type t) const;

  /// \brief List available elements by category of a given content type.
  ///
  /// \param[in] t Content type
  /// \param[in] cat Category to search for
  /// \return Map of elements (unique id to title)
  std::map< std::string, std::string > elements_by_category(content_type t,
                                                            std::string const &cat) const;

  /// \brief Get element by unique id
  ///
  /// Throws an exception if no element is found by the given id.
  ///
  /// \param[in] uid Unique id
  /// \return Element
  login_type const &login(std::string const &uid) const;

  /// \brief Get element by unique id
  ///
  /// Throws an exception if no element is found by the given id.
  ///
  /// \param[in] uid Unique id
  /// \return Element
  note_type const &note(std::string const &uid) const;

  /// \brief Get element by unique id
  ///
  /// Throws an exception if no element is found by the given id.
  ///
  /// \param[in] uid Unique id
  /// \return Element
  file_type const &file(std::string const &uid) const;

  /// \brief Get element by unique id
  ///
  /// Throws an exception if no element is found by the given id.
  ///
  /// \param[in] uid Unique id
  /// \return Element
  contact_type const &contact(std::string const &uid) const;

  /// \brief Set element
  ///
  /// When adding a new element, leave unique id variable of the element empty. Do not use your own
  /// unique ids, this will cause an exception. When updating an element already stored, set unique
  /// id of the element to the unique id of the element to be updated.
  ///
  /// \param[in] value Value to be stored
  /// \return Assigned unique id of the stored element
  std::string login(login_type const &value);

  /// \brief Set element
  ///
  /// When adding a new element, leave unique id variable of the element empty. Do not use your own
  /// unique ids, this will cause an exception. When updating an element already stored, set unique
  /// id of the element to the unique id of the element to be updated.
  ///
  /// \param[in] value Value to be stored
  /// \return Assigned unique id of the stored element
  std::string note(note_type const &value);

  /// \brief Set element
  ///
  /// When adding a new element, leave unique id variable of the element empty. Do not use your own
  /// unique ids, this will cause an exception. When updating an element already stored, set unique
  /// id of the element to the unique id of the element to be updated.
  ///
  /// \param[in] value Value to be stored
  /// \return Assigned unique id of the stored element
  std::string file(file_type const &value);

  /// \brief Set element
  ///
  /// When adding a new element, leave unique id variable of the element empty. Do not use your own
  /// unique ids, this will cause an exception. When updating an element already stored, set unique
  /// id of the element to the unique id of the element to be updated.
  ///
  /// \param[in] value Value to be stored
  /// \return Assigned unique id of the stored element
  std::string contact(contact_type const &value);

private:
  std::vector< login_type > logins;
  std::vector< note_type > notes;
  std::vector< file_type > files;
  std::vector< contact_type > contacts;
};

/// \brief Login credential storage.
///
/// Login credentials are managed by stores by their unique ids. All other fields do not have to be
/// unique. Unique ids can only be assigned by their parent store.
struct login_type
{
  /// \brief Load from tree, used by parent store.
  void load(boost::property_tree::ptree const &tree);
  /// \brief Save to tree, used by parent store.
  boost::property_tree::ptree save() const;

  /// \brief Generate random password.
  ///
  /// The `password` field is overwritten with a new randomly generated password of a given
  /// length. The letters A-Z in upper and lower case, and digits 0-9 are always used for password
  /// generation. It is possible to provide an additional alphabet of special characters used to
  /// further increase entropy. The `last_change` field is updated to local time.
  ///
  /// \param[in] length Desired password length
  /// \param[in] special_characters Additional alphabet of special characters
  void generate_password(std::size_t length, std::string const &special_characters =
                                                 "!@#$%^&*()`~-_=+[{]}\\|;:'\",<.>/?");

  /// \brief Unique id to be managed by the parent store.
  std::string uid;

  /// \brief User defined.
  std::string title;
  /// \brief User defined.
  std::string category;
  /// \brief User defined.
  std::string username;
  /// \brief User defined.
  std::string password;
  /// \brief User defined.
  std::string url;
  /// \brief Should be set when a password was updated to keep track of password age.
  boost::posix_time::ptime last_change;
};

/// \brief Textual note storage.
///
/// Notes are managed by stores by their unique ids. All other fields do not have to be unique.
/// Unique ids can only be assigned by their parent store.
struct note_type
{
  /// \brief Load from tree, used by parent store.
  void load(boost::property_tree::ptree const &tree);
  /// \brief Save to tree, used by parent store.
  boost::property_tree::ptree save() const;

  /// \brief Unique id to be managed by the parent store.
  std::string uid;

  /// \brief User defined.
  std::string title;
  /// \brief User defined.
  std::string category;
  /// \brief User defined.
  std::string content;
};

/// \brief Binary data storage.
///
/// Files are managed by stores by their unique ids. All other fields do not have to be unique.
/// Unique ids can only be assigned by their parent store.
struct file_type
{
  /// \brief Load from tree, used by parent store.
  void load(boost::property_tree::ptree const &tree);
  /// \brief Save to tree, used by parent store.
  boost::property_tree::ptree save() const;

  /// \brief Store binary data from file on disk, optionally removing the file afterwards.
  ///
  /// A file's content will be stored into the `content` field (please note that the file is
  /// base64 encoded, so do not attempt to use the file content directly). If desired the source
  /// file is securely erased from disk after successful storage. An exception is thrown if the
  /// file's content could not be stored, or the file could not be erased.
  ///
  /// \param[in] filename Path to source file
  /// \param[in] secure_erase Whether to remove the file after successful storage
  /// \param[in] iterations Number of times the file will be overwritten with random data
  void upload(std::string const &filename, bool secure_erase = false, std::size_t iterations = 10);

  /// \brief Map binary data as a temporary file to disk or RAM.
  ///
  /// The file will be created in the temporary files location given by the system, and its up to
  /// system configuration whether this will be on hard disk or RAM. Due to this uncertainty, it is
  /// strongly advised to explicitly unmap() the file when no longer used. The `mapped_file` field
  /// is updated with this function, and should not be changed because unmap() will not work
  /// otherwise. Throws an exception if the data could not be mapped into a temporary file. The file
  /// is assumed to be mapped while the `mapped_file` field is set, and will not be mapped again.
  ///
  /// \return Path to temporary file.
  std::string map();

  /// \brief Remove temporary file.
  ///
  /// Performs no operation if the binary data is not currently mapped into a temporary file.
  /// Securely erases the file otherwise.
  ///
  /// \param[in] iterations Number of times the file will be overwritten with random data
  void unmap(std::size_t iterations = 10);

  /// \brief Unique id to be managed by the parent store.
  std::string uid;

  /// \brief User defined.
  std::string title;
  /// \brief User defined.
  std::string category;
  /// \brief Base64 encoded binary data set by the map() function.
  std::string content;

  /// \brief This field is not persisted on save(), and only to be used by map() and unmap().
  std::string mapped_file;
};

/// \brief Contact data storage.
///
/// Contacts are managed by stores by their unique ids. All other fields do not have to be unique.
/// Unique ids can only be assigned by their parent store.
struct contact_type
{
  /// \brief Load from tree, used by parent store.
  void load(boost::property_tree::ptree const &tree);
  /// \brief Save to tree, used by parent store.
  boost::property_tree::ptree save() const;

  /// \brief Returns `last_name, first_name`.
  std::string title() const;

  /// \brief Unique id to be managed by the parent store.
  std::string uid;

  /// \brief User defined.
  std::string category;
  /// \brief User defined.
  std::string first_name;
  /// \brief User defined.
  std::string last_name;
  /// \brief User defined.
  std::string email;
  /// \brief User defined.
  std::string phone;
  /// \brief User defined.
  std::string street;
  /// \brief User defined.
  std::string zip;
  /// \brief User defined.
  std::string city;
  /// \brief User defined.
  std::string country;
  /// \brief User defined.
  std::string comment;
};
}

#endif // BACKEND_STORAGE_HPP_INCLUDED
