// Student Management System: Create a
// student data management system that
// allows you to add, update, and search for
// student records. Implement features like
// storing student data in files, sorting
// students by name or ID, and generating
// reports.
#include <fstream>
#include <sstream>

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

#include <algorithm>

#include <ncurses.h>
using namespace std;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

void getSubjectMarks(WINDOW *inputwin, std::unordered_map<std::string, std::string> &subjects, int j = 3)
{
    char mark[10];
    const char *subjectNames[] = {"OOPS", "DBMS", "BE", "DM", "SS", "IWT"};
    for (int i = 0; i < 6; i++)
    {
        mvwgetstr(inputwin, j + i, 8, mark);
        subjects[subjectNames[i]] = string(mark);
    }
}

class Student
{
public:
    string name;
    string enroll;
    string branch;
    string year;
    string cgpa;
    unordered_map<string, string> subjects;

    Student(string name, string enroll, string branch, string year, string cgpa, unordered_map<string, string> subjects)
    {
        this->name = name;
        this->enroll = enroll;
        this->branch = branch;
        this->year = year;
        this->cgpa = cgpa;
        this->subjects = subjects;
    }

    void printReport()
    {
        system("clear");
        attron(A_BOLD);
        string heading = "----Student Report----";
        attroff(A_BOLD);

        mvprintw(0, (SCREEN_WIDTH - heading.length()) / 2, heading.c_str());

        // print name and enrollment
        string line1(SCREEN_WIDTH, ' ');
        line1.replace(0, 4, "Name:");
        line1.replace(5, name.length(), name);
        line1.replace(SCREEN_WIDTH - enroll.length() - 14, 9, "Enrollment:");
        line1.replace(SCREEN_WIDTH - enroll.length() - 2, enroll.length(), enroll);
        mvprintw(2, 0, line1.c_str());

        string line2(SCREEN_WIDTH, ' ');
        line2.replace(0, 6, "Branch:");
        line2.replace(7, branch.length(), branch);
        line2.replace(SCREEN_WIDTH - year.length() - 14, 5, "Year:");
        line2.replace(SCREEN_WIDTH - year.length() - 2, year.length(), year);
        mvprintw(3, 0, line2.c_str());

        string line3(SCREEN_WIDTH, ' ');
        line3.replace(0, 4, "CGPA:");
        line3.replace(5, cgpa.length(), cgpa);
        mvprintw(4, 0, line3.c_str());

        // print subjects
        string line4(SCREEN_WIDTH, ' ');
        line4.replace(0, 4, "OOPS:");
        line4.replace(5, SCREEN_WIDTH - subjects["OOPS"].length(), subjects["OOPS"]);
        mvprintw(6, 0, line4.c_str());

        string line5(SCREEN_WIDTH, ' ');
        line5.replace(0, 5, "DBMS:");
        line5.replace(5, SCREEN_WIDTH - subjects["DBMS"].length(), subjects["DBMS"]);
        mvprintw(7, 0, line5.c_str());

        string line6(SCREEN_WIDTH, ' ');
        line6.replace(0, 3, "BE:");
        line6.replace(5, SCREEN_WIDTH - subjects["BE"].length(), subjects["BE"]);
        mvprintw(8, 0, line6.c_str());

        string line7(SCREEN_WIDTH, ' ');
        line7.replace(0, 3, "DM:");
        line7.replace(5, SCREEN_WIDTH - subjects["DM"].length(), subjects["DM"]);
        mvprintw(9, 0, line7.c_str());

        string line8(SCREEN_WIDTH, ' ');
        line8.replace(0, 3, "SS:");
        line8.replace(5, SCREEN_WIDTH - subjects["SS"].length(), subjects["SS"]);
        mvprintw(10, 0, line8.c_str());

        string line9(SCREEN_WIDTH, ' ');
        line9.replace(0, 4, "IWT:");
        line9.replace(5, SCREEN_WIDTH - subjects["IWT"].length(), subjects["IWT"]);
        mvprintw(11, 0, line9.c_str());

        // instructions
        string instruction = "Press [q] to go back , [ENTER] to edit";
        mvprintw(SCREEN_HEIGHT - 1, 0, instruction.c_str());

        int ch = getch();
        int choice = 0;

        switch (ch)
        {
        case 'q':
            system("clear");
            break;

        case '\n':
            // Form to edit report
            clear();

            // Create a new window for editing a report
            WINDOW *editwin = newwin(10, 20, (SCREEN_HEIGHT - 10) / 2, (SCREEN_WIDTH - 20) / 2);

            // Draw a box around the window
            box(editwin, 0, 0);

            // Display some text in the window
            mvwprintw(editwin, 1, 1, "Edit Report");

            // Refresh the window to make the changes visible
            wrefresh(editwin);

            // Get the grades for each subject
            mvwprintw(editwin, 3, 2, "OOPS: ");
            mvwprintw(editwin, 4, 2, "DBMS: ");
            mvwprintw(editwin, 5, 2, "BE: ");
            mvwprintw(editwin, 6, 2, "DM: ");
            mvwprintw(editwin, 7, 2, "SS: ");
            mvwprintw(editwin, 8, 2, "IWT: ");

            nodelay(editwin, FALSE);
            echo();

            // Update the grades
            getSubjectMarks(editwin, subjects);

            // Update the CGPA
            this->cgpa = to_string((stof(subjects["OOPS"]) + stof(subjects["DBMS"]) + stof(subjects["BE"]) + stof(subjects["DM"]) + stof(subjects["SS"]) + stof(subjects["IWT"])) / 6);
            // Delete the window
            delwin(editwin);
            refresh();
            break;
        } // ...
    }
};

// read csv file and return a vector of Student objects
vector<Student> readFile(string filename)
{
    vector<Student> students;
    ifstream file(filename);

    if (file.is_open())
    {
        string line;
        // Skip the header line
        getline(file, line);

        while (getline(file, line))
        {
            stringstream ss(line);
            string id, name, branch, year, oops_str, dbms_str, be_str, dm_str, ss_str, iwt_str, cgpa_str;
            unordered_map<string, string> subjects;

            // Assuming the CSV format is: id,Name,OOPS,DBMS,BE,DM,SS,IWT,CGPA,
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, branch, ',');
            getline(ss, year, ',');
            getline(ss, oops_str, ',');
            getline(ss, dbms_str, ',');
            getline(ss, be_str, ',');
            getline(ss, dm_str, ',');
            getline(ss, ss_str, ',');
            getline(ss, iwt_str, ',');
            getline(ss, cgpa_str, ',');

            subjects["OOPS"] = oops_str;
            subjects["DBMS"] = dbms_str;
            subjects["BE"] = be_str;
            subjects["DM"] = dm_str;
            subjects["SS"] = ss_str;
            subjects["IWT"] = iwt_str;

            students.push_back(Student(name, id, branch, year, cgpa_str, subjects));
        }
        file.close();
    }

    return students;
}

int printStudents(vector<Student> students)
{
    for (int i = 0; i < students.size(); i++)
    {
        mvprintw(i, 0, "%s", students[i].name.c_str());
    }
    return 0;
}

void edit(int choice, vector<Student> students)
{
    clear();
    string heading = "----Student Information----";
    mvprintw(0, (SCREEN_WIDTH - heading.length()) / 2, heading.c_str());

    string line(SCREEN_WIDTH, ' '); // Create a string of spaces

    line.replace(0, 3, "SNo");                         // Add the serial number
    line.replace(5, 4, "Name");                        // Add the name
    line.replace((SCREEN_WIDTH - 6) / 2, 6, "Branch"); // Add the branch and year
    line.replace(SCREEN_WIDTH - 4, 4, "CGPA");         // Add the CGPA
    mvprintw(2, 0, line.c_str());

    for (int i = 0; i < 2; ++i)
    {
        if (i == choice)
        {
            attron(A_REVERSE);
        }
        string line(SCREEN_WIDTH, ' ');                                   // Create a string of spaces
        string sno = to_string(i + 1);                                    // Serial number
        string branch_year = students[i].branch + " " + students[i].year; // Branch and year

        line.replace(0, sno.length(), sno);                                                                  // Add the serial number
        line.replace(sno.length() + 4, students[i].name.length(), students[i].name);                         // Add the name
        line.replace((SCREEN_WIDTH - branch_year.length()) / 2, branch_year.length(), branch_year);          // Add the branch and year
        line.replace(SCREEN_WIDTH - students[i].cgpa.length(), students[i].cgpa.length(), students[i].cgpa); // Add the CGPA
        mvprintw(i + 3, 0, line.c_str());
        attroff(A_REVERSE);
    }
}
void writeFile(const vector<Student> &students, const string &filename)
{
    std::ofstream file(filename);

    // Write the header
    file << "id,Name,Branch,Year,OOPS,DBMS,BE,DM,SS,IWT,CGPA\n";

    // Write the student data
    for (const Student &student : students)
    {
        file << student.enroll << ",";
        file << student.name << ",";
        file << student.branch << ",";
        file << student.year << ",";
        file << student.subjects.at("OOPS") << ",";
        file << student.subjects.at("DBMS") << ",";
        file << student.subjects.at("BE") << ",";
        file << student.subjects.at("DM") << ",";
        file << student.subjects.at("SS") << ",";
        file << student.subjects.at("IWT") << ",";
        file << student.cgpa << "\n";
    }

    file.close();
}
void writeFile(const Student &student)
{
    // Open the file in append mode
    std::ofstream file("students.csv", std::ios::app);

    // Check if the file is open
    if (!file)
    {
        std::cerr << "Unable to open file";
        exit(1); // terminate with error
    }

    // Write the student's details to the file in CSV formate
    file << student.enroll << ",";
    file << student.name << ",";
    file << student.branch << ",";
    file << student.year << ",";
    file << student.subjects.at("OOPS") << ",";
    file << student.subjects.at("DBMS") << ",";
    file << student.subjects.at("BE") << ",";
    file << student.subjects.at("DM") << ",";
    file << student.subjects.at("SS") << ",";
    file << student.subjects.at("IWT") << ",";
    file << student.cgpa << "\n";

    // Close the file
    file.close();
}

int main()
{

    initscr();
    noecho();
    curs_set(FALSE);

    keypad(stdscr, TRUE);

    // read file and get vector of Student objects

    // number of students

    string filterModestr = "Random";

    int choice = 0;

    int filterMode = 0;

    getmaxyx(stdscr, SCREEN_HEIGHT, SCREEN_WIDTH);
    while (1)
    {
        vector<Student> studentsUnsorted = readFile("students.csv");
        int num_students = studentsUnsorted.size();

        clear();
        attron(A_BOLD);
        string heading = "----Student Information----";
        mvprintw(0, (SCREEN_WIDTH - heading.length()) / 2, heading.c_str());
        attron(A_COLOR);
        string line(SCREEN_WIDTH, ' '); // Create a string of spaces

        line.replace(0, 3, "SNo");                         // Add the serial number
        line.replace(5, 4, "Name");                        // Add the name
        line.replace((SCREEN_WIDTH - 6) / 2, 6, "Branch"); // Add the branch and year
        line.replace(SCREEN_WIDTH - 4, 4, "CGPA");         // Add the CGPA
        mvprintw(2, 0, line.c_str());

        attroff(A_BOLD);
        vector<Student> students = studentsUnsorted;

        switch (filterMode)
        {
        case 0:
            filterModestr = "Random";
            students = studentsUnsorted;
            break;

        case 1:
            // Name Filtering
            filterModestr = "Name[^]";
            sort(students.begin(), students.end(), [](const Student &a, const Student &b)
                 { return a.name < b.name; });
            break;
        case 2:
            // Name Filtering
            filterModestr = "Name[v]";
            sort(students.begin(), students.end(), [](const Student &a, const Student &b)
                 { return a.name > b.name; });
            break;

        case 3:
            // Branch Filtering
            filterModestr = "Branch[^]";
            sort(students.begin(), students.end(), [](const Student &a, const Student &b)
                 { return a.branch < b.branch; });
            break;

        case 4:
            // Branch Filtering
            filterModestr = "Branch[v]";
            sort(students.begin(), students.end(), [](const Student &a, const Student &b)
                 { return a.branch > b.branch; });
            break;

        case 5:
            // CGPA Filtering
            filterModestr = "CGPA[^]";
            sort(students.begin(), students.end(), [](const Student &a, const Student &b)
                 { return a.cgpa < b.cgpa; });
            break;

        case 6:
            // CGPA Filtering
            filterModestr = "CGPA[v]";
            sort(students.begin(), students.end(), [](const Student &a, const Student &b)
                 { return a.cgpa > b.cgpa; });
            break;
        }

        for (int i = 0; i < num_students; ++i)
        {
            if (i == choice)
            {
                attron(A_REVERSE);
            }
            string line(SCREEN_WIDTH, ' ');                                   // Create a string of spaces
            string sno = to_string(i + 1);                                    // Serial number
            string branch_year = students[i].branch + " " + students[i].year; // Branch and year

            line.replace(0, sno.length(), sno);                                                                  // Add the serial number
            line.replace(sno.length() + 4, students[i].name.length(), students[i].name);                         // Add the name
            line.replace((SCREEN_WIDTH - branch_year.length()) / 2, branch_year.length(), branch_year);          // Add the branch and year
            line.replace(SCREEN_WIDTH - students[i].cgpa.length(), students[i].cgpa.length(), students[i].cgpa); // Add the CGPA
            mvprintw(i + 3, 0, line.c_str());
            attroff(A_REVERSE);
        }

        string instruction = "Use [^] [v] to navigate, [ENTER] - view report, [q] - quit , [a] - add entry , [e] - change filter mode";
        mvprintw(SCREEN_HEIGHT - 1, 0, instruction.c_str());
        attron(A_REVERSE);
        string filter = " Filter: " + filterModestr + " ";
        mvprintw(SCREEN_HEIGHT - 1, SCREEN_WIDTH - filter.length(), filter.c_str());
        attroff(A_REVERSE);

        int ch = getch();

        switch (ch)
        {
        case KEY_UP:
            if (choice > 0)
                choice--;
            break;
        case KEY_DOWN:
            if (choice < num_students - 1)
                choice++;
            break;
        case '\n':
            // Enter key pressed
            // Call the function corresponding to the selected student
            students[choice].printReport();
            writeFile(students, "students.csv");
            refresh();
            break;
        case 'e':
            filterMode++;
            if (filterMode > 6)
                filterMode = 0;
            break;

        case 's':
        {
            WINDOW *searchwin = newwin(20, 40, (SCREEN_HEIGHT - 20) / 2, (SCREEN_WIDTH - 40) / 2);
            box(searchwin, 0, 0);
            mvwprintw(searchwin, 1, 2, "Enter enrollment number:");
            mvwprintw(searchwin, 2, 2, "Enrollment: ");

            nodelay(searchwin, FALSE);
            echo();

            char enroll[20];
            mvwgetstr(searchwin, 2, 13, enroll);

            // searching for the enrollment number
            for (int i = 0; i < num_students; i++)
            {
                if (studentsUnsorted[i].enroll == string(enroll))
                {
                    studentsUnsorted[i].printReport();
                    break;
                }
            }

            // display error message if enrollment number not found
            mvwprintw(searchwin, 4, 2, "Enrollment number not found");
            getch();
            delwin(searchwin);
            refresh();
            break;
        }
        case 'q':
            system("clear");
            endwin();
            return 0;

        case 'a':
        { // Create a new window for input
            WINDOW *inputwin = newwin(20, 40, (SCREEN_HEIGHT - 20) / 2, (SCREEN_WIDTH - 40) / 2);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 1, 2, "Enter student details:");
            mvwprintw(inputwin, 2, 2, "ID: ");
            mvwprintw(inputwin, 3, 2, "Name: ");
            mvwprintw(inputwin, 4, 2, "Branch: ");
            mvwprintw(inputwin, 5, 2, "CGPA: ");
            mvwprintw(inputwin, 6, 2, "Year: ");

            // marks for each subject
            mvwprintw(inputwin, 7, 2, "OOPS: ");
            mvwprintw(inputwin, 8, 2, "DBMS: ");
            mvwprintw(inputwin, 9, 2, "BE: ");
            mvwprintw(inputwin, 10, 2, "DM: ");
            mvwprintw(inputwin, 11, 2, "SS: ");
            mvwprintw(inputwin, 12, 2, "IWT: ");

            nodelay(inputwin, FALSE);
            echo();

            char id[20], name[20], branch[20], cgpa[10], year[10];
            unordered_map<string, string> subs;
            mvwgetstr(inputwin, 2, 6, id);
            mvwgetstr(inputwin, 3, 8, name);
            mvwgetstr(inputwin, 4, 10, branch);
            mvwgetstr(inputwin, 5, 8, cgpa);
            mvwgetstr(inputwin, 6, 8, year);

            // Get the marks for each subject
            getSubjectMarks(inputwin, subs, 7);

            // Create a new Student object and add it to the vector
            Student newStudent(string(name), string(id), string(branch), string(year), string(cgpa), subs);
            studentsUnsorted.push_back(newStudent);

            num_students = studentsUnsorted.size();

            writeFile(newStudent);

            // Delete the input window
            delwin(inputwin);

            refresh();

            break;
        }
        }
    }
}