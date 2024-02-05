#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <random>
#include <sstream>

#include "Password.h"
#include "Table.h"

const int MAX_ARRAY_SIZE = 7;

using namespace std;

vector<string> splitString(const string &inputString)
{
  vector<string> result;
  stringstream ss(inputString);
  string token;

  while (getline(ss, token, ','))
  {
    result.push_back(token);
  }

  return result;
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const string currentDateTime()
{
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = *localtime(&now);
  // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

  return buf;
}

string random_string(string::size_type length)
{
  static auto &chrs = "0123456789"
                      "abcdefghijklmnopqrstuvwxyz"
                      "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  thread_local static mt19937 rg{random_device{}()};
  thread_local static uniform_int_distribution<string::size_type> pick(0, sizeof(chrs) - 2);

  string s;

  s.reserve(length);

  while (length--)
    s += chrs[pick(rg)];

  return s;
}

void writePassword(Password password)
{
  fstream file;

  file.open("Passwords.csv", fstream::out | fstream::app);

  file << password.id
       << ","
       << password.platform
       << ","
       << password.name
       << ","
       << password.username
       << ","
       << password.password
       << ","
       << password.url
       << ","
       << password.developer
       << ","
       << password.createdAt
       << ","
       << password.updatedAt
       << "\n";

  file.close();
}

vector<string> readPasswords()
{
  fstream file;
  vector<string> records;

  file.open("Passwords.csv");

  while (file.peek() != EOF)
  {
    string record;
    getline(file, record);

    records.push_back(record);
  }

  file.close();
  return records;
}

void displayTable(vector<string> records)
{
  TextTable t('-', '|', '+');

  // t.add("ID");
  // t.add("Platform");
  // t.add("Name");
  // t.add("Username");
  // t.add("Password");
  // t.add("URL");
  // t.add("Developer");
  // t.endOfRow();

  for (auto &record : records)
  {
    vector<string> arr = splitString(record);

    for (auto &val : arr)
    {
      t.add(val);
    }
    t.endOfRow();
  }

  t.setAlignment(2, TextTable::Alignment::RIGHT);
  cout << t;
}

void showPasswords()
{
  vector<string> passwords = readPasswords();
  displayTable(passwords);
}

void addPassword()
{
  string platform = "";
  string name = "";
  string url = "";
  string developer = "";
  string username = "";
  string password = "";

  cout << "Enter Platform (Website/Application/Game): ";
  cin >> ws;
  getline(cin, platform);
  cout << "Enter Name: ";
  cin >> ws;
  getline(cin, name);
  cout << "Enter Username: ";
  cin >> ws;
  getline(cin, username);
  cout << "Enter Password: ";
  cin >> ws;
  getline(cin, password);
  if (platform == "website")
  {
    cout << "Enter URL: ";
    cin >> ws;
    getline(cin, url);
  }
  else if (platform == "game")
  {
    cout << "Enter Developer: ";
    cin >> ws;
    getline(cin, developer);
  }

  string createdAt = currentDateTime();
  string updatedAt = currentDateTime();

  string result = "" + random_string(8) + "," + platform + "," + name + "," + username + "," + password + "," + url + "," + developer + "\n";

  Password newPassword(random_string(8), platform, username, password, url, currentDateTime(), currentDateTime(), name, developer);

  writePassword(newPassword);
}

void updatePassword()
{
  string idToUpdate;
  cout << "Enter the ID of the password to update: ";
  cin >> idToUpdate;

  vector<string> passwords = readPasswords();
  vector<string> updatedPasswords;

  bool found = false;

  // Process the header row (first row)
  if (!passwords.empty())
  {
    updatedPasswords.push_back(passwords.front());
    passwords.erase(passwords.begin()); // Skip the header row
  }

  for (auto &record : passwords)
  {
    vector<string> arr = splitString(record);

    if (arr.size() > 0 && arr[0] == idToUpdate)
    {
      // Match found, update the password fields
      string platform, name, username, password, url, developer;

      cout << "Enter Updated Platform (Website/Application/Game): ";
      cin >> ws;
      getline(cin, platform);
      cout << "Enter Updated Name: ";
      cin >> ws;
      getline(cin, name);
      cout << "Enter Updated Username: ";
      cin >> ws;
      getline(cin, username);
      cout << "Enter Updated Password: ";
      cin >> ws;
      getline(cin, password);

      if (platform == "website")
      {
        cout << "Enter Updated URL: ";
        cin >> ws;
        getline(cin, url);
      }
      else if (platform == "game")
      {
        cout << "Enter Updated Developer: ";
        cin >> ws;
        getline(cin, developer);
      }

      string updatedAt = currentDateTime();

      // Create the updated record
      string updatedRecord = idToUpdate + "," + platform + "," + name + "," + username + "," + password + "," + url + "," + developer + "\n";
      found = true;
      updatedPasswords.push_back(updatedRecord);
    }
    else
    {
      // No match found, keep the original record
      updatedPasswords.push_back(record);
    }
  }

  if (!found)
  {
    cout << "Password with ID " << idToUpdate << " not found.\n";
    return;
  }

  // Write the updated passwords to the file
  ofstream file("Passwords.csv");
  for (const auto &updatedRecord : updatedPasswords)
  {
    file << "\n"
         << updatedRecord;
  }
  file.close();

  cout << "Password updated successfully.\n";
}

void deletePassword()
{
  string idToDelete;
  cout << "Enter the ID of the password to delete: ";
  cin >> idToDelete;

  vector<string> passwords = readPasswords();
  vector<string> updatedPasswords;

  bool found = false;

  for (auto &record : passwords)
  {
    vector<string> arr = splitString(record);

    if (arr.size() > 0 && arr[0] == idToDelete)
    {
      // Match found, skip this record (delete it)
      found = true;
    }
    else
    {
      // No match found, keep the original record
      updatedPasswords.push_back(record);
    }
  }

  if (!found)
  {
    cout << "Password with ID " << idToDelete << " not found.\n";
    return;
  }

  // Write the updated passwords to the file
  ofstream file("Passwords.csv");
  for (const auto &updatedRecord : updatedPasswords)
  {
    file << updatedRecord;
  }
  file.close();

  cout << "Password deleted successfully.\n";
}

int menu()
{
  int choice;
  cout << "Menu: " << endl;
  cout << "1. Add Password" << endl;
  cout << "2. List Passwords" << endl;
  cout << "3. Update Password" << endl;
  cout << "4. Delete Password" << endl;
  cout << "5. Exit" << endl;
  cout << "Enter Choice: ";
  cin >> choice;

  switch (choice)
  {
  case 1:
    addPassword();
    break;
  case 2:
    showPasswords();
    break;
  case 3:
    updatePassword();
    break;
  case 4:
    deletePassword();
    break;
  default:
    break;
  }

  return choice;
}

void createFileIfNotExist(string fileName)
{
  // Check if the file "Passwords.csv" exists
  ifstream file(fileName);

  if (!file.is_open())
  {
    // If the file does not exist, create it
    ofstream newFile(fileName);

    if (newFile.is_open())
    {
      newFile << "id"
              << ","
              << "platform"
              << ","
              << "name"
              << ","
              << "username"
              << ","
              << "password"
              << ","
              << "url"
              << ","
              << "developer"
              << ","
              << "createdAt"
              << ","
              << "updatedAt"
              << "\n";
      cout << "File '" << fileName << "' created successfully." << endl;
      // You can perform additional actions here, such as writing default content to the file.
      newFile.close();
    }
    else
    {
      cerr << "Error creating file '" << fileName << "'." << endl;
    }
  }
}

int main()
{
  cout << "My Password Manager\n";

  createFileIfNotExist("Passwords.csv");

  int choice = 0;

  while (choice <= 0 || choice > 5)
  {
    choice = menu();
  }

  return 0;
}