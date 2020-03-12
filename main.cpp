#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class Datetime
{
private:
    int day, month, year;

public:

    Datetime():day(0), month(0), year(0){}

    /// getters
    int getDay() { return day; }
    int getMonth() { return month;}
    int getYear() { return year;}


    friend istream& operator >>(istream &in, Datetime &d)
    {
        in >> d.day;
        in.ignore(); /// to ignore the "-" letter
        in >> d.month;
        in.ignore(); /// to ignore the "-" letter
        in >> d.year;

        return in;
    }

    friend ostream& operator <<(ostream &out, Datetime &d)
    {
        out << d.day << "-" << d.month << "-" << d.year;
        return  out ;
    }
};

class Student
{
private:
    int StudentId;
    char StudentName[100];
    float GPA;
    Datetime StudentDOB;
    int level;

public:

    /// add student to the file
    void addStudent(fstream& stream){
        stream << StudentId   << "|";
        stream << StudentName << "|";
        stream << GPA         << "|";
        stream << StudentDOB  << "|";
        stream << level       << "|";
        stream << "#";
    }

    /// get the student data from the file
    void getStudent(fstream& stream, int pos){

        /// seeks to the desired record
        stream.seekg(pos, stream.beg);

        stream >> StudentId;
        stream.ignore(); /// to ignore the | letter
        stream.getline(StudentName, 100, '|');
        stream >> GPA;
        stream.ignore();
        stream >> StudentDOB;
        stream.ignore();
        stream >> level;
    }

    int calcAge(){
        /// given that we're in 2020 the age = 2020 - the birth year
        return 2020 - StudentDOB.getYear();
    }
    /*------------------------------Delete student by StudentId---------------------------*/
    void   delete_student()
    {
        fstream stud;
        fstream temp;

        stud.open("students.txt",ios::in);
        temp.open("temp.txt",ios::out);
        char Student_Name [25];
        char student_gpa[25];
        char Student_Id [25];
        char student_db[25];
        char student_level[25];
        char user_id_input[25];
        cin.ignore();
        cout<<" ENTER ID : "<<endl;
        cin.getline(user_id_input,25);
        while(!stud.eof())
        {
            stud.getline(Student_Id,25,'|');
            stud.getline(Student_Name,25,'|');
            stud.getline(student_gpa,25,'|');
            stud.getline(student_db,25,'|');
            stud.getline(student_level,25,'|');
            if(strcmp(Student_Id,user_id_input)==0)
            {
                continue;
            }
            else
            {
                temp<<Student_Id<<'|'<< Student_Name<<'|'<<student_gpa<<'|'<<student_db<<'|'<<student_level<<'|';
            }


        }
        temp.close();
        stud.close();

        stud.open("students.txt",ios::out);
        temp.open("temp.txt",ios::in);
        while(!temp.eof())
        {
            temp.getline(Student_Id,25,'|');
            temp.getline(Student_Name,25,'|');
            temp.getline(student_gpa,25,'|');
            temp.getline(student_db,25,'|');
            temp.getline(student_level,25,'|');

            stud<<Student_Id<<'|'<< Student_Name<<'|'<<student_gpa<<'|'<<student_db<<'|'<<student_level<<'|';
        }
        temp.close();
        stud.close();
        remove("temp.txt");
        cout<<"\n done !!! \n";

    }
    /*--------------------------------------------------------------------*/

    friend istream& operator >>(istream &in, Student &s){
        cout << "ID: ";
        in >> s.StudentId;

        cout << "Name: ";
        in.ignore();
        in.getline(s.StudentName, 100);

        cout << "GPA: ";
        in >> s.GPA;

        cout << "Date of Birth (ex: d-m-y): ";
        in >> s.StudentDOB;

        cout << "Level: ";
        in >> s.level;
        cout << endl;

        return in;
    }

    friend ostream& operator <<(ostream &out, Student &s){
        out << "ID: " << s.StudentId << endl;
        out << "Name: " << s.StudentName << endl;
        out << "GPA: " << s.GPA << endl;
        out << "Date of Birth: " << s.StudentDOB << endl;
        out << "Level: " << s.level << endl;

        return out;
    }

};

void menu();
int validateInput();

int main(){

    menu();

    return 0;
}

/// Input integer validation
int validateInput(){
    int choose, INT_MAX = 0;
    cout << "> ";
    cin >> choose;

    // input validation
    while(true){
        if( cin.fail() ){
            cin.clear(); // clear the stream
            cin.ignore(INT_MAX,'\n'); // ignore any thing afterwards

            cout << "\nNot valid choice .. please, try again!\n" << endl;

            cout << "> ";
            cin >> choose;
        }

        if( !cin.fail() ) break;
    }

    return choose;
}

void menu(){
    int choice;
    fstream file;
    Student stud;

    cout << "------------------------------------------------------" << endl;
    cout << "[1] Add Student" << endl;
    cout << "[2] Delete student" << endl;
    cout << "[3] Update student info" << endl;
    cout << "[4] Print student" << endl;
    cout << "[5] Print all" << endl;
    cout << "[6] Print students with less than or equal a given GPA" << endl;
    cout << "[7] Calculate student age" << endl;
    cout << "[8] Print students in a specific level" << endl;
    cout << "------------------------------------------------------" << endl;

    choice = validateInput();
    cout << endl;

    /// add a student to the file
    if(choice == 1){
        cin >> stud;
        file.open("students.txt", ios::out | ios::app);

        stud.addStudent(file);

        file.close();
    }
    /// DELETING USER FROM FILE
    else if (choice ==2)
    {

        stud.delete_student();

    }

        /// print student data given the student name
    else if(choice == 4){
        file.open("students.txt", ios::in);

        char field[100], name[100];
        bool found = false;
        int record_pos = 0, counter=0;

        cout << "Student Name: ";
        cin.ignore();
        cin.getline(name, 100);

        while(!file.eof()){

            if(counter%5 == 0) record_pos = file.tellg();

            file.getline(field, 100, '|');

            if(!strcmp(field, name)){
                found = true;
                break;
            }

            counter++;

        }

        if(found){
            /// +1 to ignore the # sign (but first recored doesn't have # before it)
            if(record_pos > 0) record_pos++;

            stud.getStudent(file, record_pos);

            cout << stud;
        } else {
            cout << endl;
            cout << "-------------------" << endl;
            cout << " Name is not found " << endl;
            cout << "-------------------" << endl;
            cout << endl;
        }

        file.close();
    }

        /// Calculate the student age given the id
    else if(choice == 7){
        file.open("students.txt", ios::in);

        char c;
        int field, id;
        bool found = false;
        int record_pos = 0, counter=0;

        cout << "Student ID: ";
        cin.ignore();
        cin >> id;

        file.seekg(0, file.end);

        /// to calculate the file size so if it's empty it won't cause an error
        int file_size = file.tellg();
        file.seekg(0);

        while(!file.eof() && file_size != 0){

            file >> c;

            /// to see if it's first records or the records afterwards
            if(counter == 0 || c == '#'){
                /// so if it's the first record resets the get pointer to 0
                /// to put the id value from the first digit
                if(counter == 0) file.seekg(0);
                record_pos = file.tellg();
                file >> field;
            }

            if(field == id){
                found = true;
                break;
            }

            counter++;

        }

        if(found){

            stud.getStudent(file, record_pos);

            cout << stud.calcAge() << endl;
        } else {
            cout << endl;
            cout << "-----------------" << endl;
            cout << " ID is not found " << endl;
            cout << "-----------------" << endl;
            cout << endl;
        }

        file.close();
    }

    cout << endl;
    menu();
}