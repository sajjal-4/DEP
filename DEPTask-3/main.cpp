#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std; 

void compress(const string &input,string &output) 
{
    int n=input.length(); 
    for(int i=0;i<n;i++) 
    {
        int count=1;
        while(i+1<n&&input[i]==input[i+1])
        {
            count++;
            i++;
        }
        output+=input[i];
        output+=to_string(count);
    }
}
void decompress(const string &input,string &output)
{
    for(int i=0;i<input.length();i++)
    {
        char c=input[i]; 
        i++; 
        string CountStr; 
        while(i<input.length()&&isdigit(input[i]))
        {
            CountStr+=input[i];
            i++;
        }
        if(CountStr.empty()) 
        {
            cerr<<"=> Invalid format: no count found for character '"<<c<<"'"<< endl;
            return;
        }
        int count=stoi(CountStr);
        output.append(count,c);
        i--; 
    }
}
void readFile(const string &filename,string &content) 
{
    ifstream file(filename); 
    if (!file.is_open())
    {
        cerr<<"=> Error opening file: "<<filename<<endl;
        return; 
    }
    ostringstream ss; 
    ss<<file.rdbuf(); 
    content=ss.str(); 
    file.close(); 
}
void writeFile(const string &filename,const string &content)
{
    ofstream file(filename);
    if(!file.is_open())
    {
        cerr<<"=> Error writing to file: "<<filename<<endl;
        return;
    }
    file<<content; 
    file.close(); 
}
bool isValidFileType(const string &filename)
{
    if (filename.length()<4) 
    return false;
    return filename.substr(filename.length()-4)!=".txt";
}
class InvalidInput
{
    public:
    void what()
    {
        cout<<endl<<"=> Invalid Input.Try Again!"<<endl;
    }
};
int main() 
{
    string inputFile="input.txt"; 
    string compressedFile="compressed.txt";
    string decompressedFile="decompressed.txt";
    int choice;
    do
    {
        try
        {
            cout<<endl<<"MENU"<<endl;
            cout<<"1. Compression"<<endl;
            cout<<"2. Decompression"<<endl;
            cout<<"3. Exit"<<endl;
            cout<<"Enter choice: ";
            cin>>choice;
            string content;
            readFile(inputFile,content);
            if (cin.fail())
            {
                cin.clear();
                cin.ignore();
                throw InvalidInput();
            }
            if(!isValidFileType(inputFile)) 
            {
                cout<<"=> Invalid file type.Please provide a file that is not a .txt file."<<endl;
                continue;
            } 
            if(content.empty())
            {
                cout<<"=> The input file is empty."<<endl;
                continue; 
            }
            if(choice==1)
            {
                string compressedContent;
                compress(content,compressedContent);
                writeFile(compressedFile,compressedContent);
                cout<<endl<<"=>Compression done!"<<endl;
            } 
            else if(choice==2)
            {
                string compressedContent;
                readFile(compressedFile,compressedContent);
                if (compressedContent.empty())
                {
                    cout<<endl<<"=> The compressed file is empty! Cannot decompress."<<endl;
                } 
                else 
                {
                    string decompressedContent;
                    decompress(compressedContent,decompressedContent);
                    writeFile(decompressedFile,decompressedContent);
                    cout<<endl<<"=> Decompression done!"<<endl;
                }
            } 
        }
        catch (InvalidInput e)
        {
            e.what();
        }
    }while(choice!=3); 
    cout<<endl<<"=> Exiting the program! Thank You!"<<endl; 
    return 0; 
}