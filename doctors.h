#include <stdio.h>
#include <ctype.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include "elements.h"


void signup_doctor(void); //works el7
void login_doctor(int trials); //works el7
void doctor_homepage(int index); //works el7
void CreateCourse(int index); //works el7
void ListCourses_doctor(int index); //works el7
void doctor_viewCourse(int course, int index);//works el7
void create_Assignment(int course, int index);//works el7
void doctor_listAssignments(int course, int index);//works el7
void viewAssignment(int course, int index, int assign);//works el7
void gradesReport(int course, int index, int assign);//works el7
void listSolutions(int course ,int index,int assign);//works el7
unsigned int hash(const char *username, int NUM);//works el7
void load_doctorsCourses(void);

int D = 100; // number of allowed doctors in the system
int C = 20; //number of allowed courses per doctor
int A = 20; //number of allowed assignments per course
int SUBS = 100; //Max number of submissions per assignment

// this is the signup function if the user choosed to signup as a doctor
void signup_doctor(void)
{
    bool emailCheck1 = false, emailCheck2 = false, emailValid = false;
    // getting data from the user
    char *id = get_string("Please enter your ID number: ");
    char *username = get_string("Username: ");
    char *password = get_string("Password: ");
    char *name = get_string("Name: ");
    char *email;

    do
    {
        email = get_string("Email: ");
        //check if email format is correct or not
        for (int i = 0, n = strlen(email); i < n; i++)
        {
            if (email[i] == '@')
            {
                emailCheck1 = true;
            }
            if (email[i] == '.' && emailCheck1 == true)
            {
                emailCheck2 = true;
            }
        }

        // if email format is incorrect prompt the user to enter email again
        if (emailCheck1 == true && emailCheck2 == true)
        {
            emailValid = true;
        }
        else
        {
            printf("Email format is invalid, correct usage: xxxxx@domain.com\n");
            emailCheck1 = false;
            emailCheck2 = false;
        }
    }
    while (emailValid == false);

    //generating unique index for the username
    int index = hash(username, D);
    //checking if the user name exists, if not then signup process is successful
    if (table_doctors[index] == NULL)
    {
        //opening the database to insert new user
        FILE *file = fopen("doctors_users.csv", "a");
        fprintf(file, "%s,%s,%s,%s,%s\n",id,username,password,name,email);
        fclose(file);
        printf("\nSignup completed successfully.\n\n");
    }
    else
    {
        printf("Username is taken.\n\n");
        signup_doctor();
    }
}

// login function for doctors
void login_doctor(int trials)
{
    if (trials != 0)
    {
        //prompts the user to enter username and password
        printf("Please enter username and password:\n");
        string username = get_string("username: ");
        string password = get_string("password: ");

        int index = hash(username, D);
        //if the index doesn't exist in the table then the username doesn't exist
        if (table_doctors[index] == NULL)
        {
            printf("This username doesn't exist.\n\n");
            login_doctor(trials - 1);
        }
        //if username exists then it checks for the password correcteness
        else if (strcmp(table_doctors[index]->password, password) != 0)
        {
            printf("Password is incorrect.\n");
            login_doctor(trials - 1);
        }
        //if both username and password are correct then it redirects the username to the homepage
        else
        {
            doctor_homepage(index);
        }
    }
    //if the user fails to enter the username and password correct for x times then it shutsdown the system
    else
    {
        printf("maximum number of incorrect logins reached. Shutting down the system.\n");
        return;
    }
}

// doctor's homepage
void doctor_homepage(int index)
{
    load_doctorsCourses();
    printf("\n\nWelcome %s. You are logged in.\n\n",table_doctors[index]->name);
    printf("Please make a choice:\n"
            "1 - List courses.\n"
            "2 - Create a course.\n"
            "3 - Logout\n"
            );

    int choice;
    do
    {
        choice = get_int("Enter choice: ");
        printf("\n");
    }
    while (choice != 1 && choice != 2 && choice != 3);
    if (choice == 1)
    {
        ListCourses_doctor(index);
        return;
    }
    else if (choice == 2)
    {
        CreateCourse(index);
        return;
    }
    else if (choice == 3)
    {
       return;
    }
}

void CreateCourse(int index)
{

    string name = get_string("Please enter course name: ");
    string code = get_string("Please enter course code: ");

    int index2 = hash(code, C);

    if (table_doctors[index]->doctorCourses[index2] == NULL)
    {
        FILE *file = fopen("doctors_courses.csv", "a");
        fprintf(file, "%d,%s,%s\n", index, name, code);
        fclose(file);

        printf("\nCourse added successfully.\n\n");
        doctor_homepage(index);
        return;
    }
    else
    {
        printf("This course has been already created.\n\n");
        doctor_homepage(index);
        return;
    }
}

void ListCourses_doctor(int index)
{
    int num = 0;
    string name;
    string code;
    for (int i = 0; i < C; i++)
    {
        if (table_doctors[index]->doctorCourses[i] != NULL)
        {
            num++;
            name = table_doctors[index]->doctorCourses[i]->name;
            code = table_doctors[index]->doctorCourses[i]->code;
            printf("%d - %s - %s\n",num, name, code);
        }
    }

    if (num == 0)
    {
        printf("\nThere's no courses to view.\n");
        doctor_homepage(index);
        return;
    }
    else
    {
        printf("Choose a course to view by entering its number or enter 0 to return to homepage.\n");
    }
    int choice;
    do
    {
        choice = get_int("Enter choice: ");
    }
    while (choice > num || choice < 0 || isalpha(choice) != 0);

    if (choice == 0)
    {
        doctor_homepage(index);
        return;
    }
    else
    {
        int course = hash(code, C);
        doctor_viewCourse(course, index);
        return;
    }
}


void doctor_viewCourse(int course, int index)
{

    printf("\nWelcome to %s - %s\n", table_doctors[index]->doctorCourses[course]->name, table_doctors[index]->doctorCourses[course]->code);
    printf("1 - List Assignments\n"
           "2 - Create Assignment\n"
           "3 - Back\n"
          );
    int choice;
    do
    {
        choice = get_int("Enter choice: ");
        printf("\n");
    }
    while (choice != 1 && choice != 2 && choice != 3);

    if (choice == 1)
    {
        doctor_listAssignments(course, index);
        return;
    }
    else if (choice == 2)
    {
        create_Assignment(course, index);
        return;
    }
    else if (choice == 3)
    {
        doctor_homepage(index);
        return;
    }
}

void create_Assignment(int course, int index)
{
    string name = get_string("Please enter assignment name: ");
    string description = get_string("Please enter assignment description: ");

    int index2 = hash(name, A);
    course_doctor *CD = table_doctors[index]->doctorCourses[course];
    if (CD->assignments[index2] == NULL)
    {
        FILE *file = fopen("doctors_assignments.csv", "a");
        fprintf(file, "%d,%d,%s,%s\n",index, course, name, description);
        fclose(file);
        printf("Assignment was added successfully.\n");
        load_doctorsCourses();
        doctor_viewCourse(course, index);
        return;
    }
    else
    {
        printf("A previous assignment with the same name has been created before.\n\n");
        doctor_viewCourse(course, index);
        return;
    }
}

void doctor_listAssignments(int course, int index)
{
    int num = 0;
    string names[A + 1];
    course_doctor *CD = table_doctors[index]->doctorCourses[course];

    for (int i = 0; i < A; i++)
    {
        if (CD->assignments[i] != NULL)
        {
            num++;
            names[num] = CD->assignments[i]->name;
            printf("%d - %s\n", num, CD->assignments[i]->name);
        }
    }

    if (num != 0)
    {
         printf("Choose an assignment to view by entering its number or enter 0 to return back.\n");
         int choice = 0;
         do
         {
             choice = get_int("Enter choice: ");
         }
         while (choice > num || choice < 0 || isalpha(choice) != 0);

         if (choice == 0)
         {
             doctor_viewCourse(course, index);
             return;
         }
         else
         {
             int assign = hash(names[choice], A);
             viewAssignment(course, index, assign);
         }
    }
    else
    {
        printf("No assignments exist.\n");
        doctor_viewCourse(course, index);
        return;
    }
}

void viewAssignment(int course, int index, int assign)
{
    load_doctorsCourses();
    printf("\n1 - Show Assignment details.\n"
           "2 - Show Grades Report.\n"
           "3 - List Solutions.\n"
           "4 - Go back\n"
          );
    int choice = 0;

    do
    {
        choice = get_int("Enter choice: ");
    }
    while (choice != 1 && choice != 2 && choice != 3 && choice != 4);

    if (choice == 1)
    {
        string name = table_doctors[index]->doctorCourses[course]->assignments[assign]->name;
        string details = table_doctors[index]->doctorCourses[course]->assignments[assign]->description;
        printf("\nAssignment name: %s\n Assignment details: %s\n", name, details);
        printf("Enter 0 to go back.\n");
        int choice2 = 1;
        do
        {
            choice2 = get_int("Enter choice: ");
        }
        while (choice2 != 0);
        viewAssignment(course, index, assign);
        return;
    }
    else if (choice == 2)
    {
        gradesReport(course, index, assign);
        return;
    }
    else if(choice == 3)
    {
        listSolutions(course, index, assign);
        return;
    }
    else if(choice == 4)
    {
        doctor_viewCourse(course, index);
        return;
    }
}


void gradesReport(int course, int index, int assign)
{
    int num = 0;
    assignment_doctor *AS = table_doctors[index]->doctorCourses[course]->assignments[assign];
    for (int i = 0; i < SUBS; i++)
    {
        if (AS->submissions[i] != NULL)
        {
            num++;
            printf("\n%d-) %s  %s  %d", num, AS->submissions[i]->student_id, AS->submissions[i]->student_name, AS->submissions[i]->results[i]->grade);
        }
    }
    if (num == 0)
    {
        printf("\nThere's no submission yet.\n\n");
        printf("Enter 0 to go back\n");
        do
        {
            num = get_int("Enter choice: ");
        }
        while (num != 0);
        viewAssignment(course, index, assign);
        return;
    }
    else
    {
        printf("Enter 0 to go back\n");
        do
        {
            num = get_int("Enter choice: ");
        }
        while (num != 0);
        viewAssignment(course, index, assign);
        return;
    }
}

void listSolutions(int course , int index, int assign)
{
    int num = 0;
    int solutions[100];
    assignment_doctor *AS = table_doctors[index]->doctorCourses[course]->assignments[assign];
    for (int i = 0; i < SUBS; i++)
    {
        if (AS->submissions[i] != NULL)
        {
            solutions[num] = i;
            printf("\n%d-) %s  %s  %s\n", num + 1, AS->submissions[i]->student_id, AS->submissions[i]->student_name, AS->submissions[i]->solution);
            num++;
        }
    }

    if (num == 0)
    {
        printf("\nNo submissions have been sumbitted yet.\n");
        printf("Enter 0 to go back\n");
        do
        {
            num = get_int("Enter choice: ");
        }
        while (num != 0);
        viewAssignment(course, index, assign);
        return;
    }
    else
    {
        printf("Please enter the number of solution you wish to view, or enter 0 to go back.\n");

        int choice = 0;
        do
        {
            choice = get_int("Enter choice: ");
        }
        while (choice > num || choice < 0 || isalpha(choice) != 0);

        if (choice == 0)
        {
            viewAssignment(course, index, assign);
            return;
        }
        else
        {
            printf("\n%d %s  %s  %s\n", num + 1, AS->submissions[solutions[choice - 1]]->student_id, AS->submissions[solutions[choice - 1]]->student_name, AS->submissions[solutions[choice - 1]]->solution);
            int grade = get_int("Set grade:");

            FILE *file = fopen("results.csv", "a");
            fprintf(file, "%s,%d,%d,%d,%s,%d\n", AS->submissions[solutions[choice - 1]]->submission_id, index, course, assign, AS->submissions[solutions[choice - 1]]->student_id,grade);
            fclose(file);
            printf("Grade added successfully.\n");
            viewAssignment(course, index, assign);
            return;
        }
    }
}


unsigned int hash(const char *username, int NUM)
{
    unsigned int hash_value = 0;
    for (int i = 0, n = strlen(username); i < n; i++)
    {
        hash_value = (hash_value << 2) ^ tolower(username[i]);
    }
    return hash_value % NUM; //N is size of hashtable
}


void load_doctorsCourses(void)
{
    FILE *file = fopen("doctors_courses.csv", "r");
    if (file == NULL)
    {
        printf("file doesn't exist");
    }


    char *temp = malloc(sizeof(char) * 100);
    char *token;
    string fields1[3];
    string fields2[3];
    string fields3[4];
    string fields4[7];
    string fields5[6];

    //load doctors courses
    while(fgets(temp, 100, file))
    {
        token = strtok(temp, ",");

        for (int i = 0; token != NULL; i++)
        {
            fields1[i] = token;
            token = strtok(NULL, ",");
        }

        int index = hash(fields1[2], 20);
        course_doctor *course = malloc(sizeof(course_doctor));
        //printf("%d ", atoi(fields[0]));
        strcpy(course->name, fields1[1]);
        //printf("%s ", course->name);
        strcpy(course->code, fields1[2]);
        //printf("%s ", course->code);

        for (int i =0; i < 10000; i++)
        {
            course->registeredStudents[i] = NULL;
        }
        for (int i = 0; i < A; i++)
        {
            course->assignments[i] = NULL;
        }

        table_doctors[atoi(fields1[0])]->doctorCourses[index] = course;
    }

    fclose(file);
    free(temp);

    //load registeredStudents
    FILE *file2 = fopen("registeredStudents.csv", "r");
    if (file2 == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }

    temp = malloc(sizeof(char) * 20);

    while (fgets(temp, 20, file2))
    {
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields2[i] = token;
            token = strtok(NULL, ",");
        }


        table_doctors[atoi(fields2[0])]->doctorCourses[atoi(fields2[1])]->registeredStudents[atoi(fields2[2])] = table_students[2];
    }

    free(temp);
    fclose(file2);

    //load assignments
    FILE *file3 = fopen("doctors_assignments.csv", "r");
    if (file3 == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }
    temp = malloc(sizeof(char) * 300);

    while (fgets(temp, 300, file3))
    {
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields3[i] = token;
            token = strtok(NULL, ",");
        }

        int index = hash(fields3[2], 20);
        assignment_doctor *assignment = malloc(sizeof(assignment_doctor));
        strcpy(assignment->name, fields3[2]);
        strcpy(assignment->description, fields3[3]);
        course_doctor *CD = table_doctors[atoi(fields3[0])]->doctorCourses[atoi(fields3[1])];

        CD->assignments[index] = assignment;
        for (int i = 0; i < SUBS; i++)
        {
            CD->assignments[index]->submissions[i] = NULL;
        }
    }
    free(temp);
    fclose(file3);

    //load solutions
    FILE *file4 = fopen("submissions.csv", "r");
    if (file4 == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }
    temp = malloc(sizeof(char) * 400);
    submission *sub = malloc(sizeof(submission));

    while (fgets(temp, 400, file4))
    {
        //printf("%s", temp);
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields4[i] = token;
            token = strtok(NULL, ",");
        }

        strcpy(sub->submission_id, fields4[0]);
        strcpy(sub->assID, fields4[1]);
        strcpy(sub->course_id, fields4[2]);
        strcpy(sub->student_id, fields4[3]);
        strcpy(sub->student_name, fields4[4]);
        strcpy(sub->docID, fields4[5]);
        strcpy(sub->solution, fields4[6]);

        for (int i = 0; i < 100; i++)
        {
            sub->results[i] = NULL;
        }

        table_doctors[atoi(fields4[5])]->doctorCourses[atoi(fields4[2])]->assignments[atoi(fields4[1])]->submissions[atoi(fields4[0])] = sub;
        table_students[atoi(fields4[3])]->studentCourses[atoi(fields4[2])]->assignments[atoi(fields4[1])]->submissions[atoi(fields4[0])] = sub;
    }
    free(temp);
    fclose(file4);

    //load grades
    FILE *file5 = fopen("results.csv", "r");
    if (file5 == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }
    temp = malloc(sizeof(char) * 30);
    result *res = malloc(sizeof(result));

    while (fgets(temp, 30, file5))
    {
        //printf("%s", temp);
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields5[i] = token;
            token = strtok(NULL, ",");
        }

        strcpy(res->subID, fields5[0]);
        strcpy(res->docid, fields5[1]);
        strcpy(res->courseID, fields5[2]);
        strcpy(res->assid, fields5[3]);
        strcpy(res->studID, fields5[4]);
        res->grade = atoi(fields5[5]);

        table_doctors[atoi(fields5[1])]->doctorCourses[atoi(fields5[2])]->assignments[atoi(fields5[3])]->submissions[atoi(fields5[0])]->results[atoi(fields5[0])] = res;
        table_students[atoi(fields5[4])]->studentCourses[atoi(fields5[2])]->assignments[atoi(fields5[3])]->submissions[atoi(fields5[0])]->results[atoi(fields5[0])] = res;
    }
    free(temp);
    fclose(file5);

}
