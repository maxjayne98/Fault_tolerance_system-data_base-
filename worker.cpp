// #include<iostream>
// #include<fstream>
// #include<vector>
// #include <sstream>
// #define FIFO_PATH "/tmp/os_ca2"
#include"defines.hpp"

using namespace std;

void send_to_presenter(string result){
        stringstream ssss;
    ssss << result << endl;

    ofstream pipe_stream(FIFO_PATH, ofstream::in | ofstream::out);
    if (pipe_stream.is_open()) {
        pipe_stream << ssss.str();
        pipe_stream.close();
    }
    else
        cerr << "error in opening file." << endl;
}
string find_sensor_data(vector<string> words,string sensor_number){
    string result;

    for(int i = 0; i < words.size(); i++)
    {
        
        if (i % 2 == 0) {
            if (words[i] == sensor_number) {
                result = words[i+1];
                // cout<< "data of "<<sensor<<"th sensor is : " << words[i+1]<<endl;
                break;
            }    
        }
    }
    return result;
}
int main(int argc, char const *argv[])
{
    vector <string> words;
    ifstream infile;
    string temp;

    string recived;
    string sensor_number;

    getline(cin,recived);
    getline(cin,sensor_number);  
    infile.open(recived);
    string sensor = sensor_number;

    if (!infile) {
        cerr << "Unable to open file datafile.txt";
        exit(1);   // call system to stop
    }
    while(infile >> temp){
        words.push_back(temp);
    }
    string result = find_sensor_data(words,sensor_number);
    send_to_presenter(result);
    // for(int i = 0; i < words.size(); i++)
    // {
    //     if (i % 2 == 0) {
    //         cout<<"sensor is : "<< words[i]<< " ";
    //     }
        
    //     else if (i % 2 == 1) {
    //         cout <<" data is :"<< words[i]<<endl;
    //     }
    // }
    
    // for(int i = 0; i < words.size(); i++)
    // {
        
    //     if (i % 2 == 0) {
    //         if (words[i] == sensor_number) {
    //             result = words[i+1];
    //             // cout<< "data of "<<sensor<<"th sensor is : " << words[i+1]<<endl;
    //             break;
    //         }    
    //     }
    // }
 
    // stringstream ssss;
    // ssss << result << endl;

    // ofstream pipe_stream(FIFO_PATH, ofstream::in | ofstream::out);
    // if (pipe_stream.is_open()) {
    //     pipe_stream << ssss.str();
    //     pipe_stream.close();
    // }
    // else
    //     cerr << "error in opening file." << endl;
    return 0;
}
