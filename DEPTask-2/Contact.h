#include <vector>
#include<string>
using namespace std;
class Contact
{
    private:
    string name;
    long PhoneNumber;
    public:
    Contact();
    Contact(string n,long p);
    static void Add(vector<Contact>& contacts);
    static void View(const vector<Contact>& contacts);
    void display() const;
    static void Delete(vector<Contact>& contacts);
};