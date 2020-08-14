#ifndef ELEMENTS
#define ELEMENTS

#include <cs50.h>


typedef struct result
{
    char subID[20];
    char docid[20];
    char courseID[20];
    char assid[20];
    char studID[20];
    int grade;
}
result;

typedef struct submission
{
    char submission_id[20]; //sub ID == assID + courseID + studentID + docID
    char assID[20];
    char course_id[20];
    char student_id[20];
    char student_name[20];
    char docID[20];
    char solution[200];
    result *results[100];
    //struct submission *next;
}
submission;

// a struct which contains a specific assignment for a specific course for a specific student
typedef struct assignment_student
{
    char name[50];
    char description[200];
    char status[20];
    char grade[10];
    char submission[100];
    submission *submissions[100];
    //struct assignment_student *next;
}
assignment_student;

// a struct which contains the courses of one student
typedef struct course_student
{
    char docIndex[20];
    char name[50];
    char code[20];
    struct course_student *next;
    assignment_student *assignments[20];

}
course_student;

// a struct which contains all the students
typedef struct student
{
    char id[20];
    char username[20];
    char password[20];
    char name[20];
    char email[50];

    course_student *studentCourses[20];
    struct student *next;
}
student;


// a struct which contains the assignments included in one course
typedef struct assignment_doctor
{
    char name[50];
    char description[200];
    submission *submissions[100];
    //struct assignment_doctor *next;
}
assignment_doctor;

typedef struct course_doctor
{
    char name[50];
    char code[20];
    struct course_doctor *next;
    student *registeredStudents[10000];
    assignment_doctor *assignments[20];
}
course_doctor;

// a struct which contains all the doctors
typedef struct doctor
{
    char id[20];
    char username[20];
    char password[20];
    char name[20];
    char email[50];
    struct doctor *next;
    // a struct for the courses which the doctor teaches
    course_doctor *doctorCourses[20];
}
doctor;


doctor *table_doctors[100];
student *table_students[10000];
submission *table_submissions[20000];

#endif