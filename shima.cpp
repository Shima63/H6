/*
 * H5 Program
 * To Understand How to Declare and Manipulate a Collections of Objects Using an Array, Enumerators, and Structures.
 * By Shima Azizzadeh-Roodpish
 * 25 Feb 2015
 * No Copyright
 * Github account: https://github.com/Shima63/H5.git
 */
 
// External Libraries

#include <stdlib.h>
#include <cstdlib> 
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctype.h>
#include <locale>
using namespace std;

// Global Variables

string inputfilename, outputfilename = "shima.out", logfilename = "shima.log", message;
int flag;

// Defining Struct

struct entry {
    string network_code;
    string station_code;
    string type_of_band;
    string type_of_instrument;
    string orientation;
};    
    
// Defining Enumerators
    
// start the enumeration from 1 to facilitate printing...
// since months are numbered starting at 1

enum months { 
    January = 1, February, March, April, May, June,
    July, August, September, October, November, December
}; 
 
// ********************************************************************************************************************

// Function Prototype

void open_input ( ifstream & );
void open_file ( string, ofstream & );
void print_file ( string, ofstream & );
void print_file ( int, ofstream & );
void check_date ( string, ofstream & );
void check_month ( int, ofstream & );
void check_time ( string, ofstream & );
void check_time_zone ( string, ofstream & );
void check_magnitude_type ( string, ofstream & );
void check_magnitude_size ( float, ofstream & );
void produce_signal ( ofstream &, string, string, string, string, string, string );
string uppercase ( string );
string monthstring ( months );
string change_band ( string );
string change_instrument ( string );
void check_network_code ( int, string, ofstream & );
void check_station_code ( int, string, ofstream & );
void check_type_of_band ( int, string , ofstream & );
void check_type_of_instrument ( int, string, ofstream & );        


// ********************************************************************************************************************

// Main Program.
// Return Zero on Success, Non-Zero in case of Failure.

int main () {

    // Defining Variables' Type
    
    string Event_ID, date, time, time_zone, earthquake_name, earthquake_name_continue, magnitude_type, day, month, year;
    string temp, temp1, temp2, temp3;
    int num_of_valid_entries = 0, num_of_input = 0, value = 0, num_of_signal = 0;
    double longitude, latitude, depth;
    float magnitude_size;
    months month_name;
   
    entry entry_array [ 300 ];
    entry entry_temp;
    
    // Prompt User for Input File Name.

    ifstream inputfile;
    open_input ( inputfile );
    
    // Preparing log file
    
    ofstream logfile;
    open_file ( logfilename, logfile );
    
    message = "Opening file: shima.in";
    print_file ( message, logfile );
    print_file ( "\n", logfile );
    message = "Processing input ...";
    print_file ( message, logfile );
    print_file ( "\n", logfile );
    
    // Reading and Checking Header
        
    inputfile >> Event_ID;
    inputfile >> date;   
    check_date ( date, logfile );
    
    // Finding and Checking  Validity of the Month
    
    temp =  temp.append ( date.begin() + 3 ,date.begin() + 5 );
    value = atoi(temp.c_str());
    check_month ( value, logfile );
    month_name = months(value);
    month = monthstring ( month_name );

    
    inputfile >> time;
    check_time ( time, logfile );
    inputfile >> time_zone;
    check_time_zone ( time_zone, logfile );
    inputfile >> earthquake_name;
    getline(inputfile, earthquake_name_continue);
    earthquake_name.append ( earthquake_name_continue ); 
    
    // Epicenter Location
    
    inputfile >> longitude;
    inputfile >> latitude; 
    inputfile >> depth;
    
    // Magnitude Information
    
    inputfile >> magnitude_type;
    check_magnitude_type ( magnitude_type, logfile );
    inputfile >> magnitude_size;
    check_magnitude_size ( magnitude_size, logfile );

    message = "Header read correctly!";
    print_file ( message, logfile );
    print_file ( "\n\n", logfile );

    // Preparing output file
    
    ofstream outputfile;
    open_file ( outputfilename, outputfile );

    outputfile << "# " << day.append( date.begin(),date.begin() + 2 ) << " " << month << " " << year.append( date.begin() + 6,date.end() );
    outputfile << " " << time << " " <<  time_zone << " " << magnitude_type << " " << magnitude_size << " " << earthquake_name << " ";
    outputfile << "[" << Event_ID << "]  (" << longitude << ", " << latitude << ", " << depth << ")" << endl;

    // Reading Entries

    while ( ( inputfile >> entry_temp.network_code ) && ( num_of_valid_entries < 300 ) ) {
        flag = 0;
        num_of_input = num_of_input + 1;
        
        // Checking
            
        check_network_code ( num_of_input, entry_temp.network_code, logfile );
        inputfile >> entry_temp.station_code;         
        check_station_code ( num_of_input, entry_temp.station_code, logfile );
        inputfile >> entry_temp.type_of_band;        
        check_type_of_band ( num_of_input, entry_temp.type_of_band, logfile );
        inputfile >> entry_temp.type_of_instrument;        
        check_type_of_instrument ( num_of_input, entry_temp.type_of_instrument, logfile );

       // Checking Orientation
        
        temp1 = "";
        temp2 = "";
        temp3 = "";
        inputfile >> entry_temp.orientation;        
        if ( ( entry_temp.orientation.length() < 1 ) ||  ( entry_temp.orientation.length() > 3 ) ) {
            flag = 5;
        }
        else {
            temp1 = entry_temp.orientation[0];
            if ( ( temp1 != "1" ) && ( temp1 != "2" ) && ( temp1 != "3" ) ) {
                if ( ( uppercase ( temp1 ) != uppercase ( "N" ) ) && ( uppercase ( temp1 ) != uppercase ( "E" ) ) && ( uppercase ( temp1 ) != uppercase ( "Z" ) ) ) { 
                    flag = 5;
                }
                else {
                    if ( entry_temp.orientation.length() > 1 ) {
                        temp2 = entry_temp.orientation[1];
                        if ( ( temp2 != "N" ) && ( temp2 != "E" ) && ( temp2 != "Z" ) ) {
                            flag = 5;
                        }
                        else {
                            if ( entry_temp.orientation.length() > 2 ) {
                                temp3 = entry_temp.orientation[2];
                                if ( ( temp3 != "N" ) && ( temp3 != "E" ) && ( temp3 != "Z" ) ) {
                                    flag = 5;
                                } 
                            }
                        }
                    }
                }
            }
            else {
                if ( entry_temp.orientation.length() > 1 ) {
                    temp2 = entry_temp.orientation[1];
                    if ( ( temp2 != "1" ) && ( temp2 != "2" ) && ( temp2 != "3" ) ) {
                        flag = 5;
                    }
                    else {
                        if ( entry_temp.orientation.length() > 2 ) {
                            temp3 = entry_temp.orientation[2];
                            if ( ( temp3 != "1" ) && ( temp3 != "2" ) && ( temp3 != "3" ) ) {
                                flag = 5;
                            } 
                        }
                    }
                }
            }
        }            
            
       if ( flag == 5 ) {
            print_file ( "Entry # ", logfile );
            print_file ( num_of_input, logfile );
            print_file ( " ignored. Invalid orientation. ", logfile ); 
            print_file ( "\n", logfile );  
        }
        if ( flag == 0 ) {   
            num_of_valid_entries = num_of_valid_entries + 1;
            entry_temp.orientation = temp1;
            entry_array [ num_of_signal ] = entry_temp;
            num_of_signal = num_of_signal + 1;
            if ( temp2 != "" ) {
                entry_temp.orientation = temp2;
                entry_array [ num_of_signal ] = entry_temp;
                num_of_signal = num_of_signal +1;
                if ( temp3 != "" ) {
                    entry_temp.orientation = temp3;
                    entry_array [ num_of_signal ] = entry_temp;
                    num_of_signal = num_of_signal +1;
                }
            }
        }            
    }

    print_file ( "Total invalid entries ignored: ", logfile );
    print_file ( ( num_of_input - num_of_valid_entries ), logfile );
    print_file ( "\n", logfile );
    print_file ( "Total valid entries read: ", logfile );
    print_file ( num_of_valid_entries, logfile );
    print_file ( "\n", logfile );
    print_file ( "Total signal names produced: ", logfile );
    print_file ( num_of_signal, logfile );
    print_file ( "\n", logfile );
    print_file ( "Finished!", logfile );
 
    // Printing Outputs
    
    outputfile << ( num_of_signal ) << endl;
    for (int i = 0; i < ( num_of_signal ); i++) {
    
        // Changing Names to Abbreviations
    
        entry_array[i].type_of_band = change_band ( entry_array[i].type_of_band );
        entry_array[i].type_of_instrument = change_instrument ( entry_array[i].type_of_instrument );
    
        // Producing Signal
        
        produce_signal ( outputfile, Event_ID, entry_array[i].network_code, entry_array[i].station_code, entry_array[i].type_of_band, entry_array[i].type_of_instrument, entry_array[i].orientation );    
    }
    
    return 0;
}

// ********************************************************************************************************************

// Functions

// "open_input" Function Will Ask the Name of the Input File and Will Check Its Validity. 

void open_input ( ifstream & ifs ) {

    // Prompt User for Input File Name.

    cout << "Enter input file name: ";
    cin >> inputfilename;
 
    ifs.open(inputfilename.c_str());

    // Check to Make Sure the File Is Opened Properly

    if ( !ifs.is_open() ) {
    	message = "Input file does not exist!";
    	
        // Making log file When There Is Error

	    ofstream logfile;
	    open_file ( logfilename, logfile );
	    print_file ( message, logfile );
        exit (EXIT_FAILURE);
    }   
    return;
} 

// "open_file" Function Will Make a New File.
    
void open_file ( string filename, ofstream & ofs ) {
    ofs.open(filename.c_str());
    if ( !ofs.is_open() ) {
        message = "File does not exist!";
            
        // Making log file When There Is Error

	    ofstream logfile;
	    open_file ( logfilename, logfile );
	    print_file ( message, logfile );
        exit (EXIT_FAILURE);
    }   
    return;
} 

// This "print_file" function Prints Messages on Files and Terminal. Message Is of String Type.

void print_file ( string message, ofstream & ofs ) {
    ofs << message << flush;
    cout << message << flush;
    return;
}

// This "print_file" function Prints numbers on Files and Terminal. Number Is of Int Type.

void print_file ( int number, ofstream & ofs ) {
    ofs << number << flush;
    cout << number << flush;
    return;
}

// This "check_date" function checks the validity of date entry.

void check_date ( string date, ofstream & logfile ) {
    message = "Date format is not right.";
    if ( date.length() != 10 ) {
        print_file ( message, logfile );
        exit (EXIT_FAILURE);
    }
    else {
        if ( ( ( date[2] != "-"[0] ) && ( date[2] != "/"[0] ) ) || ( ( date[5] != "-"[0] ) && ( date[5] != "/"[0] ) ) ) {
            print_file ( message, logfile );
            exit (EXIT_FAILURE);
        }
        else {
            if ( ( !isdigit ( date[0] ) ) || ( !isdigit ( date[1] ) ) || ( !isdigit ( date[3] ) ) || ( !isdigit ( date[4] ) ) ) {
                print_file ( message, logfile );
                exit (EXIT_FAILURE);
            }  
            if ( ( !isdigit ( date[6] ) ) || ( !isdigit ( date[7] ) ) || ( !isdigit ( date[8] ) ) || ( !isdigit ( date[9] ) ) ) {
                print_file ( message, logfile );
                exit (EXIT_FAILURE);
            }
        }
    }
    return;
}    

// This "check_month" function checks the validity of month entry.

void check_month ( int month, ofstream & logfile ) {
    if ( ( month > 12 ) || ( month < 1 ) ) {
        message = "Month format is not right.";
        print_file ( message, logfile );
        exit (EXIT_FAILURE);
    }
    return;
}    

// This "check_time" function checks the validity of time entry.

void check_time ( string time, ofstream & logfile ) {
    message = "Time format is not right.";
    if ( time.length() != 12 ) {
        print_file ( message, logfile );
        exit (EXIT_FAILURE);
    }
    else {
        if ( ( time[2] != ":"[0] ) || ( time[5] != ":"[0] ) || ( time[8] != "."[0] ) ) {
            print_file ( message, logfile );
            exit (EXIT_FAILURE);
        }
        else {
        if ( ( !isdigit ( time[0] ) ) || ( !isdigit ( time[1] ) ) || ( !isdigit ( time[3] ) ) || ( !isdigit ( time[4] ) ) ) {
            print_file ( message, logfile );
            exit (EXIT_FAILURE);
        }  
        if ( ( !isdigit ( time[6] ) ) || ( !isdigit ( time[7] ) ) || ( !isdigit ( time[9] ) ) || ( !isdigit ( time[10] ) ) || ( !isdigit ( time[11] ) ) ) {
            print_file ( message, logfile );
            exit (EXIT_FAILURE);
        }
        }
    }
    return;
}    

// This "check_time_zone" function checks the validity of time zone entry.

void check_time_zone ( string time_zone, ofstream & logfile ) {
    message = "Time_zone format is not right.";
    if ( ( time_zone.length() != 3 ) || ( !isalpha ( time_zone[0] ) ) || ( !isalpha ( time_zone[1] ) ) || ( !isalpha ( time_zone[2] ) ) ) {
        print_file ( message, logfile );
        exit (EXIT_FAILURE);
    }
    
    return;
}    

// This "check_magnitude_type" function checks the validity of magnitude type considering it case insensitive.

void check_magnitude_type ( string magnitude_type, ofstream & logfile ) {
    message = "magnitude_type is not right.";
    string mt = uppercase ( magnitude_type );
    if ( ( mt != uppercase ( "ml" ) ) && ( mt != uppercase ( "ms" ) ) && ( mt != uppercase ( "mb" ) ) && ( mt != uppercase ( "mw" ) ) ) {
        print_file ( message, logfile );
        exit (EXIT_FAILURE);
    }
    return;
}

// This "check_magnitude_size" function checks the validity of magnitude size as a positive real number (>0).

void check_magnitude_size ( float magnitude_size, ofstream & logfile ) {
    message = "magnitude_size is not right.";
    if ( magnitude_size <= 0 ) {
        print_file ( message, logfile );
        exit (EXIT_FAILURE);
    }
    return;
}

// This "string uppercase" function changes all the letters of the input string to upper case. ( For string )

string uppercase ( string s ) {
    string result = s;
    for (int i=0; i < (int)s.size(); i++)
        result[i] = toupper(s[i]);
    return result;
}

// Function to Convert from "month" to a String...

string monthstring (months month) {
    switch (month) {
        case January:
            return "January";
        case February:
            return "February";
        case March:
            return "March";
        case April:
            return "April";
        case May:
            return "May";
        case June:
            return "June";
        case July:
            return "July";
        case August:
            return "August";
        case September:
            return "September";
        case October:
            return "October";
        case November:
            return "November";
        case December:
            return "December";
        default:
            return "ILLEGAL";
    }
}

// Function to Convert Name to Abbreviation for Band

string change_band ( string name ) {
    if ( uppercase ( name ) == uppercase ( "Long-period" ) ) {
        name = "L";
    }    
    if ( uppercase ( name) == uppercase ( "Short-period" ) ) {
        name = "B";
    }
    if ( uppercase ( name ) == uppercase ( "broadband" ) ) {
        name = "H";
    }
    return name;   
}

// Function to Convert Name to Abbreviation for Instrument

string change_instrument ( string name ) {
    if ( uppercase ( name ) == uppercase ( "High-Gain" ) ) {
        name = "H";
    }    
    if ( uppercase ( name ) == uppercase ( "Low-Gain" ) ) {
        name = "L";
    }
    if ( uppercase ( name ) == uppercase ( "Accelerometer" ) ) {
        name = "N";
    }
    return name;
}     

// Function to Produce Signal Name as an String
        
void produce_signal ( ofstream & outputfile, string Event_ID, string network_code, string station_code, string type_of_band, string type_of_instrument, string orientation ) { 
    string temp= "";
    temp.append(Event_ID);
    temp.append(".");
    temp.append(network_code);
    temp.append(".");
    temp.append(station_code);
    temp.append(".");
    temp.append(type_of_band);
    temp.append(type_of_instrument);
    temp.append(orientation);
    outputfile << temp << endl;
    return;
}       


// Function to Check Network Code

void check_network_code ( int num_of_input, string code, ofstream & logfile ) {
    if ( ( code.length() != 2 ) || ( ( code != "CE" ) && ( code != "CI" ) && ( code != "FA" ) && ( code != "NP" ) && ( code != "WR" ) ) ) {
        flag = 1;
    }
    if ( flag == 1) {
        print_file ( "Entry # ", logfile );
        print_file ( num_of_input, logfile );
        print_file ( " ignored. Invalid network. ", logfile );
        print_file ( "\n", logfile );
    }
    return;
}

// Function to Check Station Code

void check_station_code ( int num_of_input, string code, ofstream & logfile ) {
    if ( code.length() != 3 ) {
        if ( code.length() != 5 ) {
            flag = 2;
        }
        else {
            if ( ( !isdigit ( code[0] ) ) || ( !isdigit ( code[1] ) ) || ( !isdigit ( code[2] ) ) || ( !isdigit ( code[3] ) ) || ( !isdigit ( code[4] ) ) ) {
                flag = 2;
            }    
        }
    }
    else {
        if ( ( !isalpha ( code[0] ) ) || ( !isalpha ( code[1] ) ) || ( !isalpha ( code[2] ) ) ) {
            flag = 2;
        }
        else {
            if ( uppercase ( code ) != code ) {
                flag = 2;
            }    
        }
    }
    if ( flag == 2) {
        print_file ( "Entry # ", logfile );
        print_file ( num_of_input, logfile );
        print_file ( " ignored. Invalid station code. ", logfile ); 
        print_file ( "\n", logfile );
    }
    return;
}            
    
// Function to Check Type of Band

void check_type_of_band ( int num_of_input, string band, ofstream & logfile ) {
    if ( ( uppercase ( band ) != uppercase ( "Long-period" ) ) && ( uppercase ( band ) != uppercase ( "Short-period" ) ) && ( uppercase ( band ) != uppercase ( "Broadband" ) ) ) {
        flag = 3;
    }
    if ( flag == 3 ) {
        print_file ( "Entry # ", logfile );
        print_file ( num_of_input, logfile );
        print_file ( " ignored. Invalid type of band. ", logfile ); 
        print_file ( "\n", logfile );
    }
    return;
}    
       
void check_type_of_instrument ( int num_of_input, string instrument, ofstream & logfile ) {        
    if ( ( uppercase ( instrument ) != uppercase ( "High-Gain" ) ) && ( uppercase ( instrument ) != uppercase ( "Low-Gain" ) ) && ( uppercase ( instrument ) != uppercase ( "Accelerometer" ) ) ) {
        flag = 4;
    }
    if ( flag == 4 ) {
        print_file ( "Entry # ", logfile );
        print_file ( num_of_input, logfile );
        print_file ( " ignored. Invalid type of instrument. ", logfile ); 
        print_file ( "\n", logfile );
    }
}    
            