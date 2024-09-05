#include"Contact.h"
#include <iostream>
using namespace std;

Contact::Contact()
{
    name=" ";
    PhoneNumber=0;
}
Contact::Contact(string n,long p)
{
    name=n;
    PhoneNumber=p;
}
void Contact::Add(vector<Contact>& contacts)
{
    string Name;
    long phoneNumber;
    cout<<endl<<"=> Enter Name: ";
    getline(cin,Name);
    cout<<"=> Enter Phone Number: ";
    cin>>phoneNumber;
    contacts.emplace_back(Name,phoneNumber);
    cout<<endl<<"=> CONTACT ADDED SUCCESSFULLY!"<<endl;
}
void Contact::View(const vector<Contact>& contacts)
{
    if(contacts.empty())
    {
        cout<<endl<<"=> NO CONTACTS STORED."<<endl;
        return;
    }
    cout<<endl<<"=> CONTACTS STORED:"<<endl;
    for(int i=0;i<contacts.size();i++) 
    {
        contacts[i].display();
    }
}
void Contact::display() const
{
    cout<<endl<<"Name: "<<name<<endl;
    cout<<"Phone Number: "<<PhoneNumber<<endl;
}
void Contact::Delete(vector<Contact>& contacts)
{
    if(contacts.empty()) 
    {
        cout<<endl<<"=> NO CONTACTS SAVED."<<endl;
        return;
    }
    string Name;
    cout<<endl<<"=> Enter The Name Of The Contact To Delete: ";
    getline(cin, Name);
    bool found=false;
    for (int i=0;i<contacts.size();i++) 
    {
        if(contacts[i].name==Name) 
        {
            contacts.erase(contacts.begin()+i);
            found=true;
            cout<<endl<<"=> CONTACT DELETED SUCCESSFULLY!"<<endl;
            break;
        }
    }
    if(!found) 
    {
        cout<<endl<<"=> CONTACT NOT FOUND!"<<endl;
    }
}