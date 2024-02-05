#include <string>
using namespace std;

class Password
{
public:
  string id;
  string platform;
  string name;
  string url;
  string developer;
  string username;
  string password;
  string createdAt;
  string updatedAt;

  Password(string id, string platform, string username,
           string password, string url,
           string dateCreated, string dateLastUpdated,
           string name, string gameDeveloper)
      : id(id), platform(platform), username(username),
        password(password), url(url), createdAt(dateCreated),
        updatedAt(dateLastUpdated), name(name), developer(gameDeveloper) {}
};