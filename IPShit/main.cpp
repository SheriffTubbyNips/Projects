#include <iostream>
#include <fstream>
#include <list>
#include <stdio.h>

using namespace std;

int main()
{
    bool repeat;
    int lineCount = 0; int ipCount = 0; int uniqueIpCount = 0;
    string fileName; string fileText; string input;
    list<string> ipList;

    cout << "Welcome to the program son. Enter your file path.\n";
    cin >> fileName; //User input for file read target
    ifstream file ("E:\\Desktop\\" + fileName + ".csv");//Create a filestream for reading data
    if(file.is_open()){//Check if the file was opened
        cout << "Working on that. Give me a sec...";
        while(file.good()){//Loop until the file has been fully read
            if(lineCount == 4){//Check if file has read 4 comma breaks into the current line
                lineCount = 0;//Reset counter
                getline(file,fileText);//Skip to next line
            }
            if(lineCount == 2 || lineCount == 3){
                file.ignore(100,'"'); //Advance to start quote and remove it
                getline(file,fileText,'"'); //Read until before end quote
                ipCount++; //Advance the total count of all IP addresses read
                repeat = false; //Var reset
                for(string ips : ipList){//Read through the list of unique IPs
                    if(fileText == ips){//Check if the IP that was just read from the file is already in the list of unique IPs
                        repeat = true;//Flag that the list already contains this IP address
                        break;//Stop checking the list
                    }
                }
                if(!repeat){//If the IP wasn't already in the list
                    if( ( (int(fileText[0])) - int('0') ) > 0 && ( (int(fileText[0])) - int('0') ) < 9){ //Check if first char is numerical as to avoid 1pv6 for now
                        ipList.push_back(fileText);//Add new unique IP to the list
                        uniqueIpCount++;//Advance the total count of unique IP addresses
                    }
                }
            }
            else{
                getline(file,fileText,','); //Read file while treating commas as line breaks
            }
            lineCount++;//Counter for denoting what part of the file the fstream is reading
        }
        cout << "Counted " << ipCount << " total IPs. With " << uniqueIpCount << " total unique addresses.\n";//Display IP counts
        file.close();
        ofstream file ("E:\\Desktop\\LOG.txt");
        for(string ips : ipList){
            file << ips << "\n";
        }
        file.close();
    }
    else{cout << "Couldn't open the file jack! Maybe learn to code better/fix your shit eh?";}//Error output for when file cannot be read
    file.close();//Always close ya files when you're done 8^)
    return 0;//End of program
}
