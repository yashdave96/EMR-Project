#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <set>

using namespace std;


class emr{
	public:

        //Function that adds patient to EMR
		void addPt(string fName, string lName, int age){
            patient p = {fName, lName, age};

            for (int i = 1; i <= records.size(); ++i){  
                if (records[i] == p){
                    cout << "Patient already in EMR.\n\n";
                    return;
                }
            }

            p.mrn = static_cast<double>(records.size() + 1);
            records[p.mrn] = p;
            cout << "Patient " << p.firstName << " " << p.lastName << " succesfully added with MRN: " << p.mrn << "\n\n";
			return;
		}

        //Function that allows a user to look up a patient in the EMR
		void ptLookup(double mrn){  
            if (records.find(mrn) != records.end()){
                cout << records[mrn].firstName << " " << records[mrn].lastName << "\n";
                cout << "Age: " << records[mrn].age << "\n";
                cout << "MRN: " << records[mrn].mrn << "\n\n";
            }
            else{
                cout << "Invalid MRN\n\n";
            }


			return;
		}

        //Function that displays the Emergency Department Dashboard
		void displayED(){
            if (!dashboard.empty()){
                cout << "***EMERGENCY DEPARTMENT DASHBOARD***\n";
                for(set<patient>::iterator it = dashboard.begin(); it != dashboard.end(); ++it){
                    cout << it->firstName << " " << it->lastName << "\t" << "Chief Complaint: " << it->chiefComplaint 
                    << "\t" << "Concern Level: " << it->concernLevel << "\t" << "Time of Arrival: " << it->timeOfArrival << "\n";
                }
            }
            else{
                cout << "No patients are currently in the Emergency Department\n\n";
            }
            cout << "\n";
			return;
		}

        //Function that adds a patient to the Emergency Department
        void addED(){
            char inEMR;
            cout << "If patient exists in the EMR, type 'Y' otherwise type 'N':\n";
            cin >> inEMR;
            if (toupper(inEMR) == 'Y'){
                double mrn;
                char category;
                string cc;
                int cLevel;
                string tOA;
                
                cout << "Enter patient's MRN: \n";
                cin >> mrn;

                if(mrn > records.size()){
                    cout << "Invalid MRN\n\n";
                    return;
                }
                
                if(dashboard.find(records[mrn]) != dashboard.end()){
                    cout << "Patient is already in the Emergency Department\n\n";
                    return;
                }
                
                getEDVals(&cc, &cLevel, &tOA);

                records[mrn].chiefComplaint = cc;
                records[mrn].concernLevel = cLevel;
                records[mrn].timeOfArrival = tOA;

                dashboard.insert(records[mrn]);
                emergencyDepartment.push(records[mrn]);
            }
            else if(toupper(inEMR) == 'N') {

                string fName, lName;
                int age;
                double mrn;
                char category;
                string cc;
                int cLevel;
                string tOA;

                cout << "Enter the patient's First Name: \n";
                cin >> fName;
                cout << "Enter the patient's Last Name: \n";
                cin >> lName;
                cout << "Enter the patient's age in years: \n";  
                cin >> age;
                cout << "\n\n";

                addPt(fName, lName, age);

                mrn = static_cast<double>(records.size());
                
                getEDVals(&cc, &cLevel, &tOA);

                records[mrn].chiefComplaint = cc;
                records[mrn].concernLevel = cLevel;
                records[mrn].timeOfArrival = tOA;

                
                dashboard.insert(records[mrn]);
                emergencyDepartment.push(records[mrn]);

            }
            else{
                //Message to output if char input is not 'Y' or 'N'
                cout << "Invalid Option\n\n";
                //Handle if invalid input is not of type char
                cin.clear();
            }


            return;
        }

        //Function that simulates treating patients in the Emergency Department
		void roundTreat(){
            if (emergencyDepartment.empty()){
                cout << "No patients are currently in the Emergency Department\n\n";
            }
            else if(emergencyDepartment.size() < 10){
                int size = emergencyDepartment.size();
                for(int i = 0; i < size; ++i){
                    cout << "Treated: " << emergencyDepartment.top().firstName << " " << emergencyDepartment.top().lastName << "\n";
                    dashboard.erase(emergencyDepartment.top());

                    emergencyDepartment.top().chiefComplaint = "None";
                    emergencyDepartment.top().concernLevel = -1;
                    emergencyDepartment.top().timeOfArrival = -1;

                    displayED();

                    emergencyDepartment.pop();
                }
            }
            else{
                for (int i = 0; i < 10; ++i){
                    cout << "Treated: " << emergencyDepartment.top().firstName << " " << emergencyDepartment.top().lastName << "\n";
                    dashboard.erase(emergencyDepartment.top());

                    emergencyDepartment.top().chiefComplaint = "None";
                    emergencyDepartment.top().concernLevel = -1;
                    emergencyDepartment.top().timeOfArrival = -1;

                    emergencyDepartment.pop();
                }
            }
			cout << "\n";
			return;
		}



	private:	

    struct patient{    
        string firstName, lastName; 
        int age;
        mutable string chiefComplaint;
        double mrn;
        mutable int concernLevel;  //Concern level on scale of 1-5 depending on category of chiefComplaint
        mutable string timeOfArrival; //In format 0000 - 2359

        //Overload == operator for comparing instances of patient struct
        bool operator==(const patient& p) const{
            return firstName == p.firstName && lastName == p.lastName && age == p.age;
        }   
    };

    //Comparator for ED Dashboard
    struct dashboard_comparator{
        bool operator() (const patient& p1, const patient& p2){
            if (p1.concernLevel != p2.concernLevel){
                return p1.concernLevel > p2.concernLevel;
            }
            else if (p1.timeOfArrival != p2.timeOfArrival){
                return stoi(p1.timeOfArrival) < stoi(p2.timeOfArrival);
            }
            else if(p1.age != p2.age){
                return p1.age > p2.age;
            }
            else{
                return p1.mrn < p2.mrn;
            }
        }
    };

    //Comparator for ED Priority Queue
    struct pq_comparator{
        bool operator() (const patient& p1, const patient& p2){
            if (p1.concernLevel != p2.concernLevel){
                return p1.concernLevel < p2.concernLevel;
            }
            else if (p1.timeOfArrival != p2.timeOfArrival){
                return stoi(p1.timeOfArrival) > stoi(p2.timeOfArrival);
            }
            else if(p1.age != p2.age){
                return p1.age < p2.age;
            }
            else{
                return p1.mrn > p2.mrn;
            }
        }
    };

    //Function that checks for valid time input
    bool timeCheck(string time){
        if(time.size() != 4){
            return false;
        }
        else if (time[0] - '0' > 2){
            return false;
        }
        else if(time[2] - '0' > 5){
            return false;
        }
        else if(time[0] - '0' == 2 && time[1] - '0' > 3){
            return false;
        }
        else{
            return true;
        }
    }

    //Function that retrieves and updates chief complaint, concern level, and time of arrival for patient being added to ED
    void getEDVals(string *cc, int *cLevel, string *tOA){
        char category;
        string time;
        cout << "Enter the patient's Chief Complaint (\'C\' for cardiac," 
                         "\'N \'for neurological, \'R\' for respiratory, \'M\' for musculoskeletal, \'G\' for general): \n";
                
        cin >> category;
        category = tolower(category);

        if(category == 'c'){
            *cLevel = 5;
            *cc = "Cardiac";
        }
        else if(category == 'n'){
            *cLevel = 4;
            *cc = "Neurological";
        }
        else if(category == 'r'){
            *cLevel = 3;
            *cc = "Respiratory";
        }
        else if(category == 'm'){
            *cLevel = 2;
            *cc = "Musculoskeletal";
        }
        else if(category == 'g'){
            *cLevel = 1;
            *cc = "General";
        }
        else{
            cout << "Invalid Chief Complaint\n";
            return;
        }


        cout << "Enter patient's time of arrival (In format 0000-2359):  \n";
        cin >> time;
                
        if (!timeCheck(time)){
            cout << "Invalid Time\n\n";
            return;
        }
        else{
            *tOA = time;
        }

    }

    unordered_map <double, patient> records; //Unordered Map holding patient info of all patients who have visited the hospital
    priority_queue <patient, vector<patient>, pq_comparator> emergencyDepartment; //Priority Queue used to order patients to be treated
    set <patient, dashboard_comparator> dashboard;   //Ordered set to hold all patients currently in ED, and print when requested
};




string menu = 
         "Please Select From the Following Menu Options:\n" 
         "\t'A' to add a new patient to the EMR\n"
         "\t'L' to look up an existing patient in the EMR\n"
         "\t'D' to display the Emergency Department Dashboard\n"
         "\t'E' to add a patient to the Emergency Department\n"
         "\t'R' to round and treat patients in the Emergency Department\n"
		 "\t'X' to exit\n\n";


int main(){
   char option;

   emr e;

   cout << "Welcome to the EMR\n\n";
   cout << menu;

   cin >> option;
    
   while(toupper(option) != 'X'){

        switch(toupper(option)){
            case 'A':
                {cout << "You have chosen to add a new patient to the EMR\n\n";
                string fName, lName;
                int age;

                cout << "Enter the patient's First Name: \n";
                cin >> fName;
                cout << "Enter the patient's Last Name: \n";
                cin >> lName;
                cout << "Enter the patient's Age in years: \n";  
                cin >> age;
                cout << "\n\n";

                e.addPt(fName, lName, age);

                break;}
            case 'L':
                {cout << "You have chosen to look up an existing patient in the EMR\n\n";
                double mrn;

                cout << "Enter patient's Medical Record Number (MRN): \n";
                cin >> mrn;
                cout << "\n\n";

                e.ptLookup(mrn);

                break;}
            case 'D':
                e.displayED();

                break;
            case 'E':
                cout << "You have chosen to add a patient to the ED\n\n";
                e.addED();

                break;
            case 'R':
                e.roundTreat();

                break;
            default:
                cout << "Invalid input\n\n";

				break;   
        }

        cout << menu;
        cin >> option;
    }
    
    

    return 0;
}
