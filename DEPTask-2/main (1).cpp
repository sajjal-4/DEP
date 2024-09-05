#include"Contact.h"
#include <iostream>
using namespace std;
int main()
{
    vector<Contact> contacts = {
    Contact("Ali", 3001234567),
    Contact("Alina", 3007654321),
    Contact("Arshad", 3005432167)
    };
    int choice;
    do 
    {
        Contact C;
        cout<<endl<<"\t\t====CONTACT MANAGEMENT SYSTEM====\t\t"<<endl;
        cout<<"1. Add A Contact"<<endl;
        cout<<"2. Display All Contacts"<<endl;
        cout<<"3. Delete A Contact"<<endl;
        cout<<"4. Exit"<<endl;
        cout<<endl<<"Enter your choice: ";
        cin>>choice;
        cin.ignore();
        switch (choice) 
        {
            case 1:
            C.Add(contacts);
            break;
            case 2:
            C.View(contacts);
            break;
            case 3:
            C.Delete(contacts);
            break;
            case 4:
            cout<<"Exiting the program.Goodbye!"<<endl;
            break;
            default:
            cout<<endl<<"=> Invalid choice.Try again!"<<endl;
        }
    }while(choice!=4);
    return 0;
}