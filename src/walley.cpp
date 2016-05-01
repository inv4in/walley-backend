// Copyright 2016 Nikolas Beisemann <github@beisemann.email>
// This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
// code package.

#include "walley.hpp"
#include "aes.hpp"
#include "auxiliary.hpp"
#include "base64.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <fstream>

namespace walley
{
class corrupted_input_error : public std::runtime_error
{
public:
  corrupted_input_error() : std::runtime_error("corrupted input") {}
};

class invalid_lookup_error : public std::runtime_error
{
public:
  invalid_lookup_error() : std::runtime_error("invalid lookup") {}
};

void container::load(std::string const &password, std::string const &input)
{
  namespace pt = boost::property_tree;

  clear();

  std::stringstream indata;
  indata << aes::decrypt(password, input);

  pt::ptree tree;
  try
  {
    pt::read_json(indata, tree);

    BOOST_FOREACH (pt::ptree::value_type const &value, tree.get_child("logins"))
    {
      pt::ptree const &subtree = value.second;
      logins.push_back(login_type());
      logins.back().load(subtree);
    }

    BOOST_FOREACH (pt::ptree::value_type const &value, tree.get_child("notes"))
    {
      pt::ptree const &subtree = value.second;
      notes.push_back(note_type());
      notes.back().load(subtree);
    }

    BOOST_FOREACH (pt::ptree::value_type const &value, tree.get_child("files"))
    {
      pt::ptree const &subtree = value.second;
      files.push_back(file_type());
      files.back().load(subtree);
    }

    BOOST_FOREACH (pt::ptree::value_type const &value, tree.get_child("contacts"))
    {
      pt::ptree const &subtree = value.second;
      contacts.push_back(contact_type());
      contacts.back().load(subtree);
    }
  }
  catch (std::exception const &)
  {
    throw corrupted_input_error();
  }
}

void container::load_from_file(std::string const &password, std::string const &filename)
{
  std::ifstream file(filename.c_str(), std::ifstream::binary);
  if (file)
  {
    file.seekg(0, std::ifstream::end);
    std::size_t const file_size = static_cast< std::size_t >(file.tellg());
    file.seekg(0, std::ifstream::beg);

    std::string file_content(file_size, 0x0);
    file.read(&file_content[0], file_content.size());
    file.close();

    load(password, file_content);
  }
  else
  {
    throw auxiliary::file_access_error();
  }
}

std::string container::save(std::string const &password) const
{
  namespace pt = boost::property_tree;

  pt::ptree tree;

  pt::ptree logins_child;
  BOOST_FOREACH (login_type const &value, logins)
  {
    logins_child.push_back(std::make_pair("", value.save()));
  }
  tree.add_child("logins", logins_child);

  pt::ptree notes_child;
  BOOST_FOREACH (note_type const &value, notes)
  {
    notes_child.push_back(std::make_pair("", value.save()));
  }
  tree.add_child("notes", notes_child);

  pt::ptree files_child;
  BOOST_FOREACH (file_type const &value, files)
  {
    files_child.push_back(std::make_pair("", value.save()));
  }
  tree.add_child("files", files_child);

  pt::ptree contacts_child;
  BOOST_FOREACH (contact_type const &value, contacts)
  {
    contacts_child.push_back(std::make_pair("", value.save()));
  }
  tree.add_child("contacts", contacts_child);

  std::stringstream outdata;
  pt::write_json(outdata, tree);
  return aes::encrypt(password, outdata.str());
}

void container::save_to_file(std::string const &password, std::string const &filename) const
{
  std::ofstream file(filename.c_str(), std::ofstream::binary);
  if (file)
  {
    std::string const file_content = save(password);
    file.write(file_content.c_str(), file_content.size());
    file.close();
  }
  else
  {
    throw auxiliary::file_access_error();
  }
}

void container::clear()
{
  logins.clear();
  notes.clear();
  files.clear();
  contacts.clear();
}

std::set< std::string > container::categories(content_type t) const
{
  std::set< std::string > output;
  if (t == TYPE_LOGIN)
  {
    BOOST_FOREACH (login_type const &value, logins)
    {
      output.insert(value.category);
    }
    return output;
  }
  else if (t == TYPE_NOTE)
  {
    BOOST_FOREACH (note_type const &value, notes)
    {
      output.insert(value.category);
    }
    return output;
  }
  else if (t == TYPE_FILE)
  {
    BOOST_FOREACH (file_type const &value, files)
    {
      output.insert(value.category);
    }
    return output;
  }
  else if (t == TYPE_CONTACT)
  {
    BOOST_FOREACH (contact_type const &value, contacts)
    {
      output.insert(value.category);
    }
    return output;
  }
  throw invalid_lookup_error();
}

std::map< std::string, std::string > container::elements_by_category(content_type t,
                                                                     std::string const &cat) const
{
  std::map< std::string, std::string > output;
  if (t == TYPE_LOGIN)
  {
    BOOST_FOREACH (login_type const &value, logins)
    {
      if (value.category == cat)
      {
        output.insert(std::make_pair(value.uid, value.title));
      }
    }
    return output;
  }
  else if (t == TYPE_NOTE)
  {
    BOOST_FOREACH (note_type const &value, notes)
    {
      if (value.category == cat)
      {
        output.insert(std::make_pair(value.uid, value.title));
      }
    }
    return output;
  }
  else if (t == TYPE_FILE)
  {
    BOOST_FOREACH (file_type const &value, files)
    {
      if (value.category == cat)
      {
        output.insert(std::make_pair(value.uid, value.title));
      }
    }
    return output;
  }
  else if (t == TYPE_CONTACT)
  {
    BOOST_FOREACH (contact_type const &value, contacts)
    {
      if (value.category == cat)
      {
        output.insert(std::make_pair(value.uid, value.title()));
      }
    }
    return output;
  }
  throw invalid_lookup_error();
}

login_type const &container::login(std::string const &uid) const
{
  BOOST_FOREACH (login_type const &value, logins)
  {
    if (value.uid == uid)
    {
      return value;
    }
  }
  throw invalid_lookup_error();
}

note_type const &container::note(std::string const &uid) const
{
  BOOST_FOREACH (note_type const &value, notes)
  {
    if (value.uid == uid)
    {
      return value;
    }
  }
  throw invalid_lookup_error();
}

file_type const &container::file(std::string const &uid) const
{
  BOOST_FOREACH (file_type const &value, files)
  {
    if (value.uid == uid)
    {
      return value;
    }
  }
  throw invalid_lookup_error();
}

contact_type const &container::contact(std::string const &uid) const
{
  BOOST_FOREACH (contact_type const &value, contacts)
  {
    if (value.uid == uid)
    {
      return value;
    }
  }
  throw invalid_lookup_error();
}

std::string container::login(login_type const &value)
{
  if (!value.uid.empty())
  {
    BOOST_FOREACH (login_type &other, logins)
    {
      if (value.uid == other.uid)
      {
        other = value;
        return value.uid;
      }
    }
    throw invalid_lookup_error();
  }
  else
  {
    logins.push_back(value);
    boost::uuids::uuid uuid = boost::uuids::uuid(boost::uuids::random_generator()());
    logins.back().uid = boost::lexical_cast< std::string >(uuid);
    return logins.back().uid;
  }
}

std::string container::note(note_type const &value)
{
  if (!value.uid.empty())
  {
    BOOST_FOREACH (note_type &other, notes)
    {
      if (value.uid == other.uid)
      {
        other = value;
        return value.uid;
      }
    }
    throw invalid_lookup_error();
  }
  else
  {
    notes.push_back(value);
    boost::uuids::uuid uuid = boost::uuids::uuid(boost::uuids::random_generator()());
    notes.back().uid = boost::lexical_cast< std::string >(uuid);
    return notes.back().uid;
  }
}

std::string container::file(file_type const &value)
{
  if (!value.uid.empty())
  {
    BOOST_FOREACH (file_type &other, files)
    {
      if (value.uid == other.uid)
      {
        other = value;
        return value.uid;
      }
    }
    throw invalid_lookup_error();
  }
  else
  {
    files.push_back(value);
    boost::uuids::uuid uuid = boost::uuids::uuid(boost::uuids::random_generator()());
    files.back().uid = boost::lexical_cast< std::string >(uuid);
    return files.back().uid;
  }
}

std::string container::contact(contact_type const &value)
{
  if (!value.uid.empty())
  {
    BOOST_FOREACH (contact_type &other, contacts)
    {
      if (value.uid == other.uid)
      {
        other = value;
        return value.uid;
      }
    }
    throw invalid_lookup_error();
  }
  else
  {
    contacts.push_back(value);
    boost::uuids::uuid uuid = boost::uuids::uuid(boost::uuids::random_generator()());
    contacts.back().uid = boost::lexical_cast< std::string >(uuid);
    return contacts.back().uid;
  }
}

void login_type::load(boost::property_tree::ptree const &tree)
{
  uid = tree.get< std::string >("uid");
  title = tree.get< std::string >("title");
  category = tree.get< std::string >("category");
  username = tree.get< std::string >("username");
  password = tree.get< std::string >("password");
  url = tree.get< std::string >("url");
  last_change = tree.get< boost::posix_time::ptime >("last_change");
}

boost::property_tree::ptree login_type::save() const
{
  boost::property_tree::ptree tree;
  tree.put("uid", uid);
  tree.put("title", title);
  tree.put("category", category);
  tree.put("username", username);
  tree.put("password", password);
  tree.put("url", url);
  tree.put("last_change", last_change);
  return tree;
}

void login_type::generate_password(std::size_t length, std::string const &special_characters)
{
  password = auxiliary::generate_password(length, special_characters);
  last_change = boost::posix_time::second_clock::local_time();
}

void note_type::load(boost::property_tree::ptree const &tree)
{
  uid = tree.get< std::string >("uid");
  title = tree.get< std::string >("title");
  category = tree.get< std::string >("category");
  content = tree.get< std::string >("content");
}

boost::property_tree::ptree note_type::save() const
{
  boost::property_tree::ptree tree;
  tree.put("uid", uid);
  tree.put("title", title);
  tree.put("category", category);
  tree.put("content", content);
  return tree;
}

void file_type::load(boost::property_tree::ptree const &tree)
{
  uid = tree.get< std::string >("uid");
  title = tree.get< std::string >("title");
  category = tree.get< std::string >("category");
  content = tree.get< std::string >("content");
}

boost::property_tree::ptree file_type::save() const
{
  boost::property_tree::ptree tree;
  tree.put("uid", uid);
  tree.put("title", title);
  tree.put("category", category);
  tree.put("content", content);
  return tree;
}

void file_type::upload(std::string const &filename, bool secure_erase, std::size_t iterations)
{
  std::ifstream file(filename.c_str(), std::ifstream::binary);
  if (file)
  {
    file.seekg(0, std::ifstream::end);
    std::size_t const file_size = static_cast< std::size_t >(file.tellg());
    file.seekg(0, std::ifstream::beg);

    std::string file_content(file_size, 0x0);
    file.read(&file_content[0], file_content.size());
    file.close();

    content = base64::encode(file_content);

    if (secure_erase)
    {
      auxiliary::secure_erase(filename, iterations);
    }
  }
  else
  {
    throw auxiliary::file_access_error();
  }
}

std::string file_type::map()
{
  if (mapped_file.empty())
  {
    std::string const file_content = base64::decode(content);
    mapped_file = auxiliary::map_file(file_content);
  }
  return mapped_file;
}

void file_type::unmap(std::size_t iterations)
{
  if (!mapped_file.empty())
  {
    auxiliary::secure_erase(mapped_file, iterations);
    mapped_file = "";
  }
}

void contact_type::load(boost::property_tree::ptree const &tree)
{
  uid = tree.get< std::string >("uid");
  category = tree.get< std::string >("category");
  first_name = tree.get< std::string >("first_name");
  last_name = tree.get< std::string >("last_name");
  email = tree.get< std::string >("email");
  phone = tree.get< std::string >("phone");
  street = tree.get< std::string >("street");
  zip = tree.get< std::string >("zip");
  city = tree.get< std::string >("city");
  country = tree.get< std::string >("country");
  comment = tree.get< std::string >("comment");
}

boost::property_tree::ptree contact_type::save() const
{
  boost::property_tree::ptree tree;
  tree.put("uid", uid);
  tree.put("category", category);
  tree.put("first_name", first_name);
  tree.put("last_name", last_name);
  tree.put("email", email);
  tree.put("phone", phone);
  tree.put("street", street);
  tree.put("zip", zip);
  tree.put("city", city);
  tree.put("country", country);
  tree.put("comment", comment);
  return tree;
}

std::string contact_type::title() const
{
  return last_name + ", " + first_name;
}
}
