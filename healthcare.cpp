#include <bits/stdc++.h>
using namespace std;

class User
{
protected:
    string user_id, name, email, password, role;

public:
    User(string id, string n, string e, string p, string r)
        : user_id(id), name(n), email(e), password(p), role(r) {}

    virtual void registerUser() = 0;
    virtual bool login(string e, string p) = 0;
    virtual void updateProfile(string newName, string newEmail) = 0;
    string getRole() { return role; }
    string getEmail() { return email; }
};

class Patient : public User
{
private:
    vector<string> medical_history;
    vector<string> appointments;
    unordered_map<string, string> insurance_details;

public:
    Patient(string id, string n, string e, string p, unordered_map<string, string> insurance)
        : User(id, n, e, p, "Patient"), insurance_details(insurance) {}

    void registerUser() override
    {
        cout << "Patient " << name << " registered." << endl;
    }

    bool login(string e, string p) override
    {
        return (email == e && password == p);
    }

    void updateProfile(string newName, string newEmail) override
    {
        name = newName;
        email = newEmail;
        cout << "Profile updated for " << name << endl;
    }

    void requestAppointment(string doctor_id, string date_time)
    {
        appointments.push_back(doctor_id + " on " + date_time);
        cout << "Appointment requested with Doctor ID " << doctor_id << " at " << date_time << endl;
    }

    void addMedicalRecord(string record)
    {
        medical_history.push_back(record);
        cout << "Added Medical Record: " << record << endl;
    }
};

class Doctor : public User
{
private:
    string specialization;
    vector<string> appointments;
    vector<string> patients;

public:
    Doctor(string id, string n, string e, string p, string spec)
        : User(id, n, e, p, "Doctor"), specialization(spec) {}

    void registerUser() override
    {
        cout << "Doctor " << name << " registered." << endl;
    }

    bool login(string e, string p) override
    {
        return (email == e && password == p);
    }

    void updateProfile(string newName, string newEmail) override
    {
        name = newName;
        email = newEmail;
        cout << "Profile updated for Dr. " << name << endl;
    }

    void addMedicalRecord(string patient_id, string record)
    {
        patients.push_back(patient_id + ": " + record);
        cout << "Medical record added for Patient ID: " << patient_id << endl;
    }
};



class Administrator : public User
{
public:
    Administrator(string id, string n, string e, string p)
        : User(id, n, e, p, "Administrator") {}

    void addUser(shared_ptr<User> user)
    {
        user->registerUser();
    }

    void removeUser(string user_id)
    {
        cout << "User ID " << user_id << " removed from the system." << endl;
    }

    void generateReports()
    {
        cout << "Generating reports..." << endl;
    }

    void registerUser() override
    {
        cout << "Administrator " << name << " registered." << endl;
    }

    bool login(string e, string p) override
    {
        return (email == e && password == p);
    }

    void updateProfile(string newName, string newEmail) override
    {
        name = newName;
        email = newEmail;
        cout << "Profile updated for " << name << endl;
    }
};

int main()
{

    shared_ptr<Administrator> admin = make_shared<Administrator>("ADMIN001", "Admin", "admin@hospital.com", "adminpass");

    vector<shared_ptr<User>> users;
    users.push_back(admin);
    int choice;
    while (true)
    {
        cout << "\nWelcome to Healthcare Management System!\n";
        cout << "1. Login as Administrator\n2. Login as Doctor\n3. Login as Patient\n4. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (choice == 1)
        {
            // Administrator login
            string email, password;
            cout << "Enter Admin Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;

            if (admin->login(email, password))
            {
                cout << "Admin logged in successfully!\n";
                int admin_choice;
                while (true)
                {
                    cout << "\nAdmin Actions:\n";
                    cout << "1. Add User\n2. Remove User\n3. Generate Reports\n4. Logout\n";
                    cout << "Choose an option: ";
                    cin >> admin_choice;

                    if (admin_choice == 1)
                    {

                        string role, id, name, email, password, specialization, insurance_provider, policy_number;
                        cout << "Enter 1 for Doctor 2 for Patient): ";
                        cin >> role;

                        if (role == "1")
                        {
                            cout << "Enter Doctor ID: ";
                            cin >> id;
                            cout << "Enter Doctor Name: ";
                            cin.ignore();
                            getline(cin, name);
                            cout << "Enter Doctor Email: ";
                            cin >> email;
                            cout << "Enter Doctor Password: ";
                            cin >> password;
                            cout << "Enter Specialization: ";
                            cin >> specialization;

                            shared_ptr<Doctor> new_doctor = make_shared<Doctor>(id, name, email, password, specialization);
                            admin->addUser(new_doctor);
                            users.push_back(new_doctor);
                        }
                        else if (role == "2")
                        {
                            cout << "Enter Patient ID: ";
                            cin >> id;
                            cout << "Enter Patient Name: ";
                            cin.ignore();
                            getline(cin, name);
                            cout << "Enter Patient Email: ";
                            cin >> email;
                            cout << "Enter Patient Password: ";
                            cin >> password;
                            cout << "Enter Insurance Provider: ";
                            cin.ignore();
                            getline(cin, insurance_provider);
                            cout << "Enter Policy Number: ";
                            cin >> policy_number;

                            unordered_map<string, string> insurance = {{"provider", insurance_provider}, {"policy_number", policy_number}};
                            shared_ptr<Patient> new_patient = make_shared<Patient>(id, name, email, password, insurance);
                            admin->addUser(new_patient);
                            users.push_back(new_patient);
                        }
                    }
                    else if (admin_choice == 2)
                    {
                        string user_id;
                        cout << "Enter User ID to remove: ";
                        cin >> user_id;
                        admin->removeUser(user_id);
                    }
                    else if (admin_choice == 3)
                    {
                        admin->generateReports();
                    }
                    else if (admin_choice == 4)
                    {
                        break;
                    }
                }
            }
            else
            {
                cout << "Invalid login credentials for Admin!\n";
            }
        }
        else if (choice == 2 || choice == 3)
        {
            string email, password;
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;

            bool loggedIn = false;
            shared_ptr<User> loggedInUser = nullptr;

            for (auto &user : users)
            {
                if (user->getEmail() == email && user->login(email, password))
                {
                    loggedIn = true;
                    loggedInUser = user;
                    break;
                }
            }

            if (loggedIn && loggedInUser->getRole() == "Doctor")
            {
                cout << "Doctor logged in successfully!\n";
                int doc_choice;
                while (true)
                {
                    cout << "\nDoctor Actions:\n";
                    cout << "1. Add Medical Record\n2. Logout\n";
                    cout << "Choose an option: ";
                    cin >> doc_choice;

                    if (doc_choice == 1)
                    {
                        string patient_id, record;
                        cout << "Enter Patient ID: ";
                        cin >> patient_id;
                        cout << "Enter Medical Record: ";
                        cin.ignore();
                        getline(cin, record);
                        dynamic_pointer_cast<Doctor>(loggedInUser)->addMedicalRecord(patient_id, record);
                    }
                    else if (doc_choice == 2)
                    {
                        break;
                    }
                }
            }
            else if (loggedIn && loggedInUser->getRole() == "Patient")
            {
                cout << "Patient logged in successfully!\n";
                int pat_choice;
                while (true)
                {
                    cout << "\nPatient Actions:\n";
                    cout << "1. Request Appointment\n2. Add Medical Record\n3. Logout\n";
                    cout << "Choose an option: ";
                    cin >> pat_choice;

                    if (pat_choice == 1)
                    {
                        string doctor_id, appointment_time;
                        cout << "Enter Doctor ID: ";
                        cin >> doctor_id;
                        cout << "Enter Appointment Date & Time (format: YYYY-MM-DD HH:MM): ";
                        cin.ignore();
                        getline(cin, appointment_time);
                        dynamic_pointer_cast<Patient>(loggedInUser)->requestAppointment(doctor_id, appointment_time);
                    }
                    else if (pat_choice == 2)
                    {
                        string record;
                        cout << "Enter Medical Record: ";
                        cin.ignore();
                        getline(cin, record);
                        dynamic_pointer_cast<Patient>(loggedInUser)->addMedicalRecord(record);
                    }
                    else if (pat_choice == 3)
                    {
                        break;
                    }
                }
            }
            else
            {
                cout << "Invalid login credentials!\n";
            }
        }
        else if (choice == 4)
        {
            cout << "Exiting system...\n";
            break;
        }
    }

    return 0;
}
