#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Struct for student information
typedef struct {
	int id;
	char name[50];
	float attendance_percentage;
	float total_present_days;
	float monthly_present_days[12];
	float daily_present_days[12][31];
	int late_days[12][31];
} Student;

// Constants
#define MAX_STUDENTS 100
#define DATA_FILE "C:\Users\sasik\OneDrive\Desktop\attendance_data.txt.txt"

// Global variables
Student students[MAX_STUDENTS];
int student_count = 0;

// Function declarations
void save_data();
void load_data();
void register_student();
void mark_attendance_date();
void mark_half_day_attendance();
void mark_late_attendance();
void generate_monthly_report();
void generate_yearly_report();
void delete_student();
void update_student();
void calculate_total_present_days();
void display_all_students();
void check_attendance_status();
void validate_date(int month, int date);
void validate_future_date(int month, int date);

void check_attendance_validity();
int main() {
	load_data();
	int choice;

	do {
		printf("\n=========================================");
		printf("\n CLASSROOM ATTENDANCE MANAGEMENT SYSTEM  ");
		printf("\n=========================================\n");
		printf("\n[1]  Register Student");
		printf("\n[2]  Mark Attendance by Date");
		printf("\n[3]  Mark Half-Day Attendance (Morning/Afternoon)");
		printf("\n[4]  Mark Late Attendance");
		printf("\n[5]  Generate Monthly Attendance Report");
		printf("\n[6]  Generate Yearly Attendance Report");
		printf("\n[7]  Delete Student Record");
		printf("\n[8]  Update Student Details");
		printf("\n[9]  Calculate Total Present Days");
		printf("\n[10] Display All Students");
		printf("\n[11] Check Attendance Status");
		printf("\n[12] Check Attendance Validity");
		printf("\n[13] Exit\n");
		printf("\nEnter your choice: ");
		scanf("%d", &choice);


		switch (choice) {
		case 1:
			register_student();
			break;
		case 2:
			mark_attendance_date();
			break;
		case 3:
			mark_half_day_attendance();
			break;
		case 4:
			mark_late_attendance();
			break;
		case 5:
			generate_monthly_report();
			break;
		case 6:
			generate_yearly_report();
			break;
		case 7:
			delete_student();
			break;
		case 8:
			update_student();
			break;
		case 9:
			calculate_total_present_days();
			break;
		case 10:
			display_all_students();
			break;
		case 11:
			check_attendance_status();
			break;
		case 12:
			check_attendance_validity();
			break;
		case 13:
			save_data();
			printf("Data saved. Exiting program.\n");
			break;
		default:
			printf("Invalid choice. Please try again.\n");
		}
	} while (choice != 13);

	return 0;
}

// Function to validate month and date input
void validate_date(int month, int date) {
	if (month < 1 || month > 12 || date < 1 || date > 31) {
		printf("Invalid month or date. Please try again.\n");
		exit(1);
	}
}

// Function to validate future date
void validate_future_date(int month, int date) {
	time_t t;
	struct tm *current_time;
	time(&t);
	current_time = localtime(&t);

	int current_month = current_time->tm_mon + 1;
	int current_day = current_time->tm_mday;

	if (month > current_month || (month == current_month && date > current_day)) {
		printf("Error: Cannot mark attendance for a future date!\n");
		exit(1);
	}
}

// Function to save data to file
void save_data() {
    FILE *file = fopen(DATA_FILE, "w");
    if (!file) {
        printf("Error: Unable to save data. File not opened.\n");
        return;
    }

    fprintf(file, "%d\n", student_count);
    for (int i = 0; i < student_count; i++) {
        fprintf(file, "%d %s %.2f %.2f\n",
                students[i].id, students[i].name, students[i].attendance_percentage,
                students[i].total_present_days);
        for (int j = 0; j < 12; j++) {
            fprintf(file, "%.2f ", students[i].monthly_present_days[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Data saved successfully.\n");
}


// Function to load data from file
void load_data() {
	FILE *file = fopen(DATA_FILE, "r");
	if (!file) {
		printf("No existing data found. Starting fresh.\n");
		return;
	}

	fscanf(file, "%d", &student_count);
	for (int i = 0; i < student_count; i++) {
		fscanf(file, "%d %s %f %f",
		       &students[i].id, students[i].name, &students[i].attendance_percentage,
		       &students[i].total_present_days);
		for (int j = 0; j < 12; j++) {
			fscanf(file, "%f", &students[i].monthly_present_days[j]);
		}
	}

	fclose(file);
}

// Function to register a new student
void register_student() {
	if (student_count >= MAX_STUDENTS) {
		printf("Error: Cannot add more students.\n");
		return;
	}

	Student new_student;
	printf("Enter Student ID: ");
	scanf("%d", &new_student.id);
	printf("Enter Student Name: ");
	scanf("%49s", new_student.name);
	new_student.attendance_percentage = 100.0;
	new_student.total_present_days = 0;
	memset(new_student.monthly_present_days, 0, sizeof(new_student.monthly_present_days));

	students[student_count++] = new_student;
	printf("Student registered successfully.\n");
}

// Function to mark attendance by date
void mark_attendance_date() {
	int id, month, date;
	char status[10];

	printf("Enter Student ID: ");
	scanf("%d", &id);
	printf("Enter Month (1-12): ");
	scanf("%d", &month);
	printf("Enter Date (1-31): ");
	scanf("%d", &date);
	validate_date(month, date);
	validate_future_date(month, date);
	printf("Enter Attendance Status (Present[P]/Absent[A]): ");
	scanf("%s", status);

	for (int i = 0; i < student_count; i++) {
		if (students[i].id == id) {
			students[i].total_present_days++;
			if (strcmp(status, "P") == 0 || strcmp(status, "p") == 0 ) {
				students[i].monthly_present_days[month - 1]++;
			} else if (strcmp(status, "A") == 0 || strcmp(status, "a") == 0) {
				students[i].attendance_percentage -= 2.0;
				if (students[i].attendance_percentage < 0) {
					students[i].attendance_percentage = 0;
				}
			} else {
				printf("Invalid status. Use 'Present[P]' or 'Absent[A]'.\n");
				return;
			}
			printf("Attendance updated for %d/%d.\n", date, month);
			return;
		}
	}

	printf("Error: Student not found.\n");
}

// Function to mark late attendance
void mark_late_attendance() {
	int id, month, date;
	printf("Enter Student ID: ");
	scanf("%d", &id);
	printf("Enter Month (1-12): ");
	scanf("%d", &month);
	printf("Enter Date (1-31): ");
	scanf("%d", &date);
	validate_date(month, date);
	validate_future_date(month, date);

	for (int i = 0; i < student_count; i++) {
		if (students[i].id == id) {
			if (students[i].daily_present_days[month - 1][date - 1] == 1) {
				printf("Student is already marked as present for this day.\n");
				return;
			}
			students[i].daily_present_days[month - 1][date - 1] = 0.5;
			students[i].monthly_present_days[month - 1] += 0.5;
			students[i].total_present_days += 0.5;
			students[i].late_days[month - 1][date - 1] = 1;
			students[i].attendance_percentage -= 1.0;
			if (students[i].attendance_percentage < 0) {
				students[i].attendance_percentage = 0;
			}
			printf("Late attendance marked for %d/%d.\n", date, month);
			return;
		}
	}

	printf("Error: Student not found.\n");
}

// Function to mark half-day attendance
void mark_half_day_attendance() {
	int id, month, date;
	char session[10];
	printf("Enter Student ID: ");
	scanf("%d", &id);
	printf("Enter Month (1-12): ");
	scanf("%d", &month);
	printf("Enter Date (1-31): ");
	scanf("%d", &date);
	validate_date(month, date);
	validate_future_date(month, date);
	printf("Enter Session (Morning[M]/Afternoon[A]): ");
	scanf("%s", session);

	for (int i = 0; i < student_count; i++) {
		if (students[i].id == id) {
			if (strcmp(session, "M") == 0 || strcmp(session, "A") == 0 || strcmp(session, "m") == 0 || strcmp(session, "a") == 0) {
				students[i].daily_present_days[month - 1][date - 1] += 0.5;
				students[i].monthly_present_days[month - 1] += 0.5;
				students[i].total_present_days += 0.5;
				students[i].attendance_percentage -= 1.0;
				if (students[i].attendance_percentage < 0) {
					students[i].attendance_percentage = 0;
				}
				printf("Half-day attendance marked successfully for %s session on %d/%d.\n", session, date, month);
			} else {
				printf("Invalid session. Use 'morning' or 'afternoon'.\n");
			}
			return;
		}
	}

	printf("Error: Student not found.\n");
}

// Function to generate monthly attendance report
void generate_monthly_report() {
	int month;
	printf("Enter Month (1-12): ");
	scanf("%d", &month);
	validate_date(month, 1);

	printf("Monthly Attendance Report for Month %d:\n", month);
	for (int i = 0; i < student_count; i++) {
		printf("ID: %d, Name: %s, Present Days: %.2f\n", students[i].id, students[i].name, students[i].monthly_present_days[month - 1]);
	}
}

// Function to generate yearly attendance report
void generate_yearly_report() {
	printf("Yearly Attendance Report:\n");
	for (int i = 0; i < student_count; i++) {
		printf("ID: %d, Name: %s, Total Present Days: %.2f\n", students[i].id, students[i].name, students[i].total_present_days);
	}
}

// Function to delete a student
void delete_student() {
	int id;
	printf("Enter Student ID to delete: ");
	scanf("%d", &id);

	for (int i = 0; i < student_count; i++) {
		if (students[i].id == id) {
			for (int j = i; j < student_count - 1; j++) {
				students[j] = students[j + 1];
			}
			student_count--;
			printf("Student deleted successfully.\n");
			return;
		}
	}

	printf("Error: Student not found.\n");
}

// Function to update student details
void update_student() {
	int id;
	printf("Enter Student ID to update: ");
	scanf("%d", &id);

	for (int i = 0; i < student_count; i++) {
		if (students[i].id == id) {
			printf("Enter new name: ");
			scanf("%49s", students[i].name);
			printf("Student details updated.\n");
			return;
		}
	}

	printf("Error: Student not found.\n");
}

// Function to calculate total present days for individual students
void calculate_total_present_days() {
	char choice;
	do {
		int student_id;
		printf("Enter Student ID to calculate total present days: ");
		scanf("%d", &student_id);

		int found = 0;
		for (int i = 0; i < student_count; i++) {
			if (students[i].id == student_id) {
				found = 1;

				students[i].total_present_days = 0;
				for (int j = 0; j < 12; j++) {
					students[i].total_present_days += students[i].monthly_present_days[j];
				}

				printf("Total Present Days for %s: %.2f\n", students[i].name, students[i].total_present_days);
				break;
			}
		}

		if (!found) {
			printf("Student ID %d not found.\n", student_id);
		}

		printf("\nDo you want to check another student? (y/n): ");
		scanf(" %c", &choice);

	} while (choice == 'y' || choice == 'Y');

	printf("Total present days calculation complete.\n");
}


// Function to display all students
void display_all_students() {
	printf("Displaying all students:\n");
	for (int i = 0; i < student_count; i++) {
		printf("ID: %d, Name: %s\n", students[i].id, students[i].name);
	}
}

// Function to check attendance status of a student
void check_attendance_status() {
	int id;
	printf("Enter Student ID: ");
	scanf("%d", &id);

	for (int i = 0; i < student_count; i++) {
		if (students[i].id == id) {
			printf("Attendance Status for %s (ID: %d):\n", students[i].name, id);
			printf("Attendance Percentage: %.2f%%\n", students[i].attendance_percentage);
			return;
		}
	}

	printf("Error: Student not found.\n");
}

// Function to check attendance validity for individual students
void check_attendance_validity() {
	printf("Checking attendance validity...\n");

	char choice;
	do {
		int student_id;
		printf("Enter Student ID to check: ");
		scanf("%d", &student_id);

		int found = 0;
		for (int i = 0; i < student_count; i++) {
			if (students[i].id == student_id) {
				found = 1;

				if (students[i].attendance_percentage < 75) {
					printf("Student ID: %d, Name: %s - Low Attendance (%.2f%%)\n", students[i].id, students[i].name, students[i].attendance_percentage);
				} else {
					printf("Student ID: %d, Name: %s - Perfect Attendance (%.2f%%)\n", students[i].id, students[i].name, students[i].attendance_percentage);
				}
			}
		}

		if (!found) {
			printf("Student ID %d not found.\n", student_id);
		}

		printf("\nDo you want to check another student? (y/n): ");
		scanf(" %c", &choice);

	} while (choice == 'y' || choice == 'Y');

	printf("Attendance validity check complete.\n");
}
