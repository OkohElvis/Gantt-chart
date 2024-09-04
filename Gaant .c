#include <stdio.h>
#include <string.h>

#include <stdlib.h>
// https://csgitlab.ucd.ie/mark3/Assignment_2-Mark_Elvis.git
struct task
{
    char name[50];
    int start_month;
    int end_month;
    int num_dependencies;
    int dependencies[9]; // dependencies start from 0 in example but input is 1,2 so????
    // I'll let dependencies start at 1 as more human readable
};

enum month
{
    Jan,
    Feb,
    Mar,
    Apr,
    May,
    June,
    July,
    Aug,
    Sep,
    Oct,
    Nov,
    Dec
};
enum month current;
void clear_screen();
void print_spaces(int num_spaces);
void print_ascii_art_diamond();
void checkifcircular(struct task tasks[], int taskId, int targetId);
void print_dependent_tasks(struct task tasks[], int taskId, int visitedTasks[]);
void print_line_underscores(void);
void print_gaant_chart(int num_tasks, struct task tasks[]);
void test_example(void);
void get_user_input(int num_tasks, struct task *tasks);

int main()
{
    print_ascii_art_diamond();
    int num_tasks;
    printf("Welcome to the Gantt Generator");
    printf("\nWould you like to use the test example or create your own Gantt from scratch? (yes or no)\n");
    char choice[10];
    scanf("%s", choice);
    if (choice[0] == 'y' || choice[0] == 'Y')
    {
        test_example();
    }
    printf("How many tasks would you like to add ? (1-10) \n");
    scanf("%d", &num_tasks);
    struct task tasks[num_tasks];
    get_user_input(num_tasks, tasks);
    clear_screen();
    print_gaant_chart(num_tasks, tasks);

    char answer[80];
    char change[80];
    char newTask[80];
    char dependtask[80];
    int j = 0;
    int i = 0;
    int visitedtasks[80];

    while (1)
    {
        int i = 0;
        printf("If you wish to edit the Gantt please type ''edit'' / If you wish to run a test, type ''test'' or to exit, type ''quit'' and then press enter to execute your option. \n");
        scanf("%s", &answer);

        if (answer[0] == 'e')
        {
            printf("Please enter the task name which you want to change exactly\n");
            scanf("%s", &change);

            while (strcmp(change, tasks[i].name) != 0) // compares task names until index of user input is found
            {
                i++;
            }

            printf("Please enter the new task name or write its old one\n"); // overwritting the old valued with the users edits
            scanf("%s", &tasks[i].name);

            printf("Start month(1-12):\n");
            scanf("%d", &tasks[i].start_month);

            printf("End month (1-12)\n");
            scanf("%d", &tasks[i].end_month);
            printf("Enter how many dependencies this task has\n");
            scanf("%d", &tasks[i].num_dependencies);

            for (int j = 0; j < tasks[i].num_dependencies; j++)
            {
                printf("Enter dependent task: ");
                scanf("%d", &tasks[i].dependencies[j]);
            }
            clear_screen();
            print_gaant_chart(num_tasks, tasks);
        }

        else if (answer[0] == 't')
        {
            printf("Please enter the task name to test\n");

            scanf("%s", &dependtask);

            while (strcmp(dependtask, tasks[j].name) != 0) // compares tasks until task id is found
            {
                j++;
            }
            print_dependent_tasks(tasks, j, visitedtasks);
        }
        else if (answer[0] == 'q')
        {
            return 0;
        }
    }

    return 0;
}

void clear_screen()
{
// clears the terminal screen based on if the computer is on windows or UNIX
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void print_spaces(int num_spaces)
{
    for (int i = 0; i < num_spaces; i++)
    {
        printf(" ");
    }
}
void print_ascii_art_diamond()
{
    // prints a pyramid then an upside down pyramid to make a diamond
    int height = 20;
    int center = height + 10;
    for (int i = 0; i < height; i++)
    {
        print_spaces(center - i + 1);
        int width = 2 * i + 1; // 1,3,5
        for (int j = 0; j < width; j++)
        {
            printf("*");
        }
        printf("\n");
        ;
    }
    for (int i = height - 2; i >= 0; i--)
    {
        print_spaces(center - i + 1);
        int width = 2 * i + 1; // 1,3,5
        for (int j = 0; j < width; j++)
        {
            printf("*");
        }
        printf("\n");
        ;
    }
}

void checkifcircular(struct task tasks[], int taskId, int targetId) // RECURSIVE FUNCTON GUARANTEES IF FUNCTION IS CIRCULAR
{
    for (int i = 0; i < tasks[taskId].num_dependencies; i++)
    {
        int dependentId = tasks[taskId].dependencies[i] - 1;
        if (dependentId == targetId)
        {
            printf("Circular dependency confirmed for task %d \n", taskId + 1);
        }
        else
        {
            checkifcircular(tasks, dependentId, targetId);
        }
    }
}
void print_dependent_tasks(struct task tasks[], int taskId, int visitedTasks[]) // FUNCTION PRINTS THE DEPENDENT TASKS AND CHECKS IF POSSIBLE CIRCULAR
{

    printf("%d -> ", taskId + 1);
    visitedTasks[taskId] = 1;

    for (int i = 0; i < tasks[taskId].num_dependencies; i++)
    {
        int dependentId = tasks[taskId].dependencies[i] - 1;
        if (visitedTasks[dependentId] == 0)
        {
            print_dependent_tasks(tasks, dependentId, visitedTasks);
        }
        else
        {
            printf("(Possible cirular dependency found)\n");
            checkifcircular(tasks, dependentId, dependentId);
        }
    }
}

void print_line_underscores(void)
{
    int num_underscores = 209;
    for (int l = 0; l < num_underscores; l++)
    {
        printf("_");
    }
    printf("\n");
}

void print_gaant_chart(int num_tasks, struct task tasks[])
{
    enum month current;
    const char *months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    print_line_underscores();
    printf("             ");
    int left_spaces, len, right_spaces, i;
    int width = 14;
    // prints row of all the months
    for (enum month current = Jan; current <= Dec; current++)
    {
        printf("|");
        len = strlen(months[current]);
        left_spaces = (width - len) / 2;
        for (i = 0; i < left_spaces; i++)
        {
            printf(" ");
        }
        printf("%s", months[current]);
        right_spaces = width - left_spaces - len;
        for (i = 0; i < right_spaces; i++)
        {
            printf(" ");
        }
    }
    printf("| Dependancies\n");
    print_line_underscores();
    // prints a row for each task which shows the months they are to be done and their dependencies
    for (int k = 0; k < num_tasks; k++)
    {
        printf("%-13s", tasks[k].name);
        printf("|");
        for (enum month current = Jan; current <= Dec; current++)
        {
            if (current >= tasks[k].start_month && current <= tasks[k].end_month)
            {
                printf("     XXX      |");
            }
            else
            {
                printf("              |");
            }
        }
        for (int i = 0; i < tasks[k].num_dependencies; i++)
        {
            printf("%d ", tasks[k].dependencies[i]);
        }
        printf("\n");
        print_line_underscores();
        printf("\n");
    }
}
void test_example(void)
{
    // example task list
    struct task tasks[10] = {
        {"Project plan", 1, 1, 0, {0}},
        {"Research", 2, 3, 1, {1}},
        {"Development", 4, 4, 2, {1, 2}},
        {"Prototyping", 5, 5, 1, {3}},
        {"Testing", 5, 5, 1, {6}},
        {"Copywriting", 5, 5, 2, {4, 5}},
        {"Integration", 6, 8, 1, {6}},
        {"System tests", 9, 10, 1, {7}},
        {"User testing", 11, 11, 1, {5}},
        {"Deployment", 11, 11, 2, {8, 9}}};

    int num_tasks = 10;
    print_gaant_chart(num_tasks, tasks);
    int visitedTasks[10] = {0}; // going to be from 1 - num_tasks
    int taskId = 9;
    print_dependent_tasks(tasks, taskId, visitedTasks);
    return;
}

void get_user_input(int num_tasks, struct task *tasks)
{
    for (int i = 0; i < num_tasks; i++)
    {
        printf("Please enter the task name\n");
        scanf("%s", tasks[i].name);
        printf("Start month (1-12)\n");
        scanf("%d", &tasks[i].start_month);
        printf("End month (1-12)\n");
        scanf("%d", &tasks[i].end_month);
        printf("Enter how many dependencies this task has\n");
        scanf("%d", &tasks[i].num_dependencies);
        for (int j = 0; j < tasks[i].num_dependencies; j++)
        {
            printf("Enter dependent task: ");
            scanf("%d", &tasks[i].dependencies[j]);
        }
    }
}
