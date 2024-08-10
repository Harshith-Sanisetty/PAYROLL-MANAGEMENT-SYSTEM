#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


class PayrollSystem;

class Employee {
private:
    string id;
    string name;
    double baseSalary;
    double bonus;
    double deductions;

public:
    Employee(string _id, string _name, double _baseSalary, double _bonus, double _deductions)
        : id(_id), name(_name), baseSalary(_baseSalary), bonus(_bonus), deductions(_deductions) {}

    double calculateSalary() const {
        return baseSalary + bonus - deductions;
    }

  
    friend void saveEmployeeToFile(const Employee& emp, const string& fileName);

  
    inline void displayEmployeeDetails() const {
        cout << "ID: " << id << "\tName: " << name << "\tBase Salary: $" << baseSalary
             << "\tBonus: $" << bonus << "\tDeductions: $" << deductions
             << "\tTotal Salary: $" << calculateSalary() << endl;
    }

    
    friend class PayrollSystem;
};


void saveEmployeeToFile(const Employee& emp, const string& fileName) {
    ofstream outFile(fileName.c_str(), ios::app);
    if (outFile.is_open()) {
        outFile <<"Employee id : "<< emp.id << "  Employee name :" << emp.name << " Employee Basesalary :" << emp.baseSalary << " Employee Bonus:"
                << emp.bonus << "  Employee Deductions : " << emp.deductions << "  Employee Total Salary :"<< emp.calculateSalary() << '\n';
        outFile.close();
        cout << "Employee details saved to file successfully." << endl;
    } else {
        cout << "Unable to open the file for saving employee details. Check file permissions." << endl;
    }
}

class PayrollSystem {
private:
    vector<Employee> employees;

public:
    void addEmployee(const Employee& emp) {
        employees.push_back(emp);
        
        saveEmployeeToFile(emp, "_project_payroll.txt");
    }

    void editEmployee(const string& empId) {
        vector<Employee>::iterator it = find_if(employees.begin(), employees.end(),
                          EmployeeIdMatches(empId));

        if (it != employees.end()) {
         
            cout << "Enter new base salary for employee " << it->name << ": ";
            cin >> it->baseSalary;

            cout << "Enter new bonus for employee " << it->name << ": ";
            cin >> it->bonus;

            cout << "Enter new deductions for employee " << it->name << ": ";
            cin >> it->deductions;

            cout << "Employee details updated successfully." << endl;
        } else {
            cout << "Employee with ID " << empId << " not found." << endl;
        }
    }

    void deleteEmployee(const string& empId) {
        employees.erase(remove_if(employees.begin(), employees.end(),
                                  EmployeeIdMatches(empId)),
                        employees.end());
        cout << "Employee record deleted successfully." << endl;
    }

    void searchEmployee(const string& empId) const {
        vector<Employee>::const_iterator it = find_if(employees.begin(), employees.end(),
                          EmployeeIdMatches(empId));

        if (it != employees.end()) {
            cout << "Employee found:" << endl;
            it->displayEmployeeDetails();
        } else {
            cout << "Employee with ID " << empId << " not found." << endl;
        }
    }

    void printPaySlip(const string& empId) const {
        vector<Employee>::const_iterator it = find_if(employees.begin(), employees.end(),
                          EmployeeIdMatches(empId));

        if (it != employees.end()) {
            cout << "Employee PaySlip:" << endl;
            it->displayEmployeeDetails();
        } else {
            cout << "Employee with ID " << empId << " not found." << endl;
        }
    }

    friend void printEmployeeDetails(const PayrollSystem& payroll);

    inline void displayEmployeeTable() const {
        cout << "Employee Table:" << endl;
        cout << "---------------------------------------------" << endl;

        for (vector<Employee>::const_iterator it = employees.begin(); it != employees.end(); ++it) {
            it->displayEmployeeDetails();
        }

        cout << "---------------------------------------------" << endl;
    }

private:
    struct EmployeeIdMatches {
        const string& empId;
        EmployeeIdMatches(const string& id) : empId(id) {}
        bool operator()(const Employee& emp) const {
            return emp.id == empId;
        }
    };
};

void printEmployeeDetails(const PayrollSystem& payroll) {
    cout << "Printing Employee Details:" << endl;
    for (vector<Employee>::const_iterator it = payroll.employees.begin(); it != payroll.employees.end(); ++it) {
        it->displayEmployeeDetails();
    }
}

void printMenu() {
    cout << "Menu:" << endl;
    cout << " 1 -> Insert New Record." << endl;
    cout << " 2 -> Edit a Record." << endl;
    cout << "3 -> Delete a Record." << endl;
    cout << "4 -> Search a Record." << endl;
    cout << "5 -> List The Employee Table." << endl;
    cout << "6 -> Print Employee PaySlip." << endl;
    cout << "7 -> Quit Program." << endl;
}

int main() {
    PayrollSystem payrollSystem;

    char choice;

    do {
        printMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case '1': {
                string id, name;
                double baseSalary, bonus, deductions;

                cout << "Enter employee ID: ";
                cin >> id;

                cout << "Enter employee name: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter base salary: ";
                cin >> baseSalary;

                cout << "Enter bonus: ";
                cin >> bonus;

                cout << "Enter deductions: ";
                cin >> deductions;

                payrollSystem.addEmployee(Employee(id, name, baseSalary, bonus, deductions));

                cout << "Employee record added successfully." << endl;
                break;
            }
            case '2': {
                string empId;
                cout << "Enter employee ID to edit: ";
                cin >> empId;
                payrollSystem.editEmployee(empId);
                break;
            }
            case '3': {
                string empId;
                cout << "Enter employee ID to delete: ";
                cin >> empId;
                payrollSystem.deleteEmployee(empId);
                break;
            }
            case '4': {
                string empId;
                cout << "Enter employee ID to search: ";
                cin >> empId;
                payrollSystem.searchEmployee(empId);
                break;
            }
            case '5':
                payrollSystem.displayEmployeeTable();
                break;
            case '6': {
                string empId;
                cout << "Enter employee ID to print PaySlip: ";
                cin >> empId;
                payrollSystem.printPaySlip(empId);
                break;
            }
            case '7':
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != '7');

    printEmployeeDetails(payrollSystem);

    return 0;
}
