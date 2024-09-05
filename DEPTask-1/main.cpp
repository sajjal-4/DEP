#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <utility>
#include <map>
#include <curl/curl.h>
#include "json.hpp"
using namespace std;

size_t WriteCallback(void* contents,size_t size,size_t nmemb,string* userp) 
{
    size_t totalSize=size*nmemb;
    userp->append((char*)contents,totalSize);
    return totalSize;
}

class Location 
{
    public:
    string name;
    double latitude;
    double longitude;
    Location(string locName,double lat,double lon):name(locName),latitude(lat),longitude(lon)
    {
        name=locName;
        latitude=lat;
        longitude=lon;
    }
    static vector<Location> locations; 
    void addLocation(const Location& loc) 
    {
        locations.push_back(loc);
    }
    void removeLocation(const string& locName) 
    {
        locations.erase(remove_if(locations.begin(),locations.end(),[&](Location& loc){ return loc.name==locName;}),locations.end());
    }
    static void listLocations()
    {
        for (const auto& loc:locations) 
        {
            cout<<"Name: "<<loc.name<<"; Latitude: "<< loc.latitude<<"; Longitude: "<<loc.longitude<<endl;
        }
    }
};
vector<Location> Location::locations;
class Weather_Variable 
{
    private:
    map<string, double> variables;
    public:
    void defineVariable(const string& name,double value)
    {
        variables[name]=value;
    }
    void updateVariable(const string& name,double value) 
    {
        if (variables.find(name)!=variables.end())
        {
            variables[name]=value;
        }
    }
    void displayVariables() const
    {
        for(const auto& var:variables)
        {
            cout<<var.first<<": "<<var.second<<endl;
        }
    }
};
class Weather_Forecasting_System 
{
    public:
    Weather_Forecasting_System() {}
    string fetchWeatherData(double latitude, double longitude)
    {
        CURL* curl;
        CURLcode res;
        string readBuffer;
        curl = curl_easy_init();
        if (curl) 
        {
            string url = "https://api.open-meteo.com/v1/forecast?latitude="+to_string(latitude)+"&longitude="+to_string(longitude)+"&current_weather=true";
            curl_easy_setopt(curl,CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl,CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST, 0L);
            res = curl_easy_perform(curl);
            if(res!=CURLE_OK)
            {
                cerr<<"curl_easy_perform() failed: "<<curl_easy_strerror(res)<<endl;
                readBuffer.clear(); 
            }
            curl_easy_cleanup(curl);
        }
        return readBuffer;
    }
    void displayWeatherData(double latitude,double longitude)
    {
        string data=fetchWeatherData(latitude,longitude);
        cout<<"Weather Data for ("<<latitude<<", "<<longitude<<"):"<<data<<endl;
    }
    void exportToCSV(const string& filename,const vector<string>& weatherData) 
    {
        ofstream file(filename);
        if(file.is_open()) 
        {
            file<<"Location,Temperature,Wind Speed\n"; 
            for(const auto& data:weatherData)
            {
                file<<data<< "\n"; 
            }
            file.close();
            cout<<"Data exported to "<<filename<<" successfully."<<endl;
        } 
        else 
        {
            cerr<<"Unable to open the file for writing."<<endl;
        }
    }
    void exportToJSON(const string& filename,const vector<pair<string, map<string, double>>>& weatherData)
    {
        nlohmann::json jsonData;
        for(const auto& entry:weatherData) 
        {
            jsonData[entry.first]=entry.second; 
        }
        ofstream file(filename);
        if(file.is_open())
        {
            file<<jsonData.dump(4);
            file.close();
            cout<<"Data exported to "<<filename<<" successfully."<<endl;
        }
        else 
        {
            cerr<<"Could not open the file for writing."<<endl;
        }
    }
};
class Historical_Weather_System
{
    private:
    string apiKey;
    public:
    Historical_Weather_System(const string& key):apiKey(key) {}
    string fetchHistoricalWeatherData(const string& location)
    {
        return "{\"location\": \"" + location + "\",\"temperature\":26.0,\"wind_speed\":4.0}";
    }
    void displayHistoricalData(const string& location)
    {
        string data=fetchHistoricalWeatherData(location);
        cout<<"Historical Weather Data for "<<location<<": "<<data<<endl;
    }
    void exportHistoricalDataToCSV(const string& filename,const vector<string>& historicalData) 
    {
        ofstream file(filename);
        if(file.is_open()) 
        {
            file<<"Date,Temperature,Wind Speed\n"; 
            for (const auto& data:historicalData) 
            {
                file<<data<<"\n"; 
            }
            file.close();
            cout<<"Historical data exported to "<<filename<<" successfully."<< endl;
        } 
        else 
        {
            cerr<<"Could not open the file for writing."<< endl;
        }
    }
    void exportHistoricalDataToJSON(const string& filename, const vector<pair<string, map<string, double>>>& historicalData) 
    {
        nlohmann::json jsonData;
        for (const auto& entry:historicalData)
        {
            jsonData[entry.first]=entry.second; 
        }
        ofstream file(filename);
        if (file.is_open()) 
        {
            file<<jsonData.dump(4); 
            file.close();
            cout<<"Historical data exported to "<<filename <<" successfully."<< endl;
        } 
        else 
        {
            cerr<<"Could not open the file for writing."<<endl;
        }
    }
};
int main() 
{
    Location loc1("Islamabad",33.6844,73.0479);
    Location loc2("Karachi",24.8607,67.0011);
    loc1.addLocation(loc1);
    loc1.addLocation(loc2);
    cout<<"Locations:"<<endl;
    Location::listLocations();

    Weather_Variable weatherVar; 
    weatherVar.defineVariable("Temperature",30.0);
    weatherVar.defineVariable("Wind Speed",10.0);
    cout<<"\nWeather Variables:"<<endl;
    weatherVar.displayVariables();

    Weather_Forecasting_System weatherSystem; 
    weatherSystem.displayWeatherData(loc1.latitude,loc1.longitude); 

    vector<string> weatherData = 
    {
        "Islamabad,30.0,10.0",
        "Karachi,32.0,8.0"
    };
    weatherSystem.exportToCSV("weather_forecast.csv",weatherData);

    vector<pair<string, map<string, double>>> jsonData = 
    {
        {"Islamabad", {{"Temperature",32.0}, {"Wind Speed",19.0}}},
        {"Karachi", {{"Temperature",42.0}, {"Wind Speed",9.0}}}
    };
    weatherSystem.exportToJSON("weather_forecast.json",jsonData);

    Historical_Weather_System historical    v    System("YOUR_API_KEY"); 
    historicalSystem.displayHistoricalData("33.6844,73.0479"); 

    vector<string> historicalData =
    {
        "2023-08-01,28.0,5.0","2023-08-02,29.0,6.5"
    };
    historicalSystem.exportHistoricalDataToCSV("historical_weather.csv", historicalData);

    vector<pair<string, map<string, double>>> historicalJsonData = 
    {
        {"2023-08-01", {{"Temperature",20.0}, {"Wind Speed", 2.0}}},
        {"2023-08-02", {{"Temperature",21.0}, {"Wind Speed", 3.5}}}
    };
    historicalSystem.exportHistoricalDataToJSON("historical_weather.json", historicalJsonData);
    return 0;
}
