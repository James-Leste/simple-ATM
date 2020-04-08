/*In view of we cannot use actual bank cards in this programme,
user's card numbers will be  stored in a file.
The moment users open the programme,
it will ask users to enter card numbers and password.
If enters correctly, users will see a menu which shows them several functions
including checking balance, depositing and withdrawing, checking detailed accounts,
paying for water and electricity etc.
Each function will be realized by one or several functions*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define NLEN 36
#define MUSER 50
#define CARDID 21

/*store users' personal information in a file
The program will record the detailed account every time users make changes to their account
Water and electricity balance will be stored in one separated file.
After one service, programme will print the menu again and ask for the next command.
Here will soon be a series of functions to realize this services.
void enroll (void);
void login_enroll(void);
void login();*/

int no;// used in the showbalance() function
int status_register;
/*used when programme finds repeated card number. used in the function registered_user();*/

typedef struct{
    char name[NLEN];
    char cnumber[CARDID];
    char password[7];
    double balance;
}info;// the general structure of the personal information.

info list[MUSER];

void printMenu(void)//This function prints the menu.
{
    printf("Which service do you want to enjoy ?\n"
           "\n1. Balance\t\t\t"
           "2. Deposit\n3. Withdraw"
           "\t\t\t4. Transfer\n5. Detailed account\t\t"
           "6. Quit\n");
    return;
}

void infotostruct(void)// every time start the programme and put the original contents in the file to the main structure.
{
    int i;
    FILE * fp;
    fp = fopen("user.txt", "r");//open the file called "user.txt" to save the general information of users.
    for (i = 0; i < MUSER; i++)
    {
        char tname[NLEN];
        char tcnumber[CARDID];
        char tpassword[7];
        double tbalance;/*temporary variables corresponding to the main structure.*/


        fscanf(fp, "%s%s%s%lf", tname, tcnumber, tpassword, &tbalance);//copy original information from the file to temporary variables

        strcpy(list[i].name, tname);
        strcpy(list[i].cnumber, tcnumber);
        strcpy(list[i].password, tpassword);
        list[i].balance = tbalance;//copy temporary variables to the main structure array
    }
    fclose(fp);//close the file fp points to
    return;
}

void putinfile(info a)//put information from main structure into the file behind the original content
{
    FILE *fw;
    fw = fopen("user.txt", "a+");//open up the file in the update mode and add information behind the original contents
    fprintf(fw, a.name);//add name
    fprintf(fw, "    ");
    fprintf(fw, a.cnumber);//add card number
    fprintf(fw, "    ");
    fprintf(fw, a.password);//add password
    fprintf(fw, "    ");
    fprintf(fw, "%lf", a.balance);//add balance
    fprintf(fw, "\n\n");
    fclose(fw);//close the file fw points to
    return;
}

char * s_gets(char *st, int n)// process the input with '\n' in them
{
    char * ret_val;
    char * find;

    ret_val = fgets(st, n, stdin);//fgets return 1 when receiving successful input
    if (ret_val)//input successfully
    {
        find = strchr(st, '\n');//strchr() find the first '\n' in the array st, and returns the address of '\n'
        if (find)
            *find = '\0';//replace the '\n' with the '\0'
        else
            while (getchar() != '\n')//clear buffer
                continue;
    }
    return ret_val;
}

int registerUser(void)//add new card
{
    char tname[NLEN];
    char tcnumber[CARDID];
    char tpassword[7];
    double tbalance;// these are also temporary variables
    int i;//this is a searching counter
    info user;/* this is an in_function_structure having the same structure with the main structure
    representing one element of the main structure "list" */

    printf("Please enter your first name:\n>");
    s_gets(tname, NLEN);
    strcpy(user.name, tname);//get the name from the user without any "space" involved
    //attention: no spaces in the name, otherwise the programmer will function wrong.

    printf("Please enter your card number:\n>");
    scanf("%s", tcnumber);
    strcpy(user.cnumber, tcnumber);
    FILE * fw;

    fw = fopen(user.cnumber, "w");

    for (i = 0; i < MUSER; i++)
    {
        if (0 == strcmp(list[i].cnumber,tcnumber))
        {
            printf("User already exists.\n");
            return 0;
        }
    }//this loop checks if the entered card number is same as one in the original file.

    printf("Please set your password:\n>");
    scanf("%s", tpassword);
    printf("Please provide the balance of this card:\n>");
    scanf("%lf", &tbalance);
    strcpy(user.password, tpassword);
    user.balance = tbalance;

    putinfile(user);/*update, every time user registered an account,
    we put the changed main structure "list" back in the file again
    to realize a file update.*/
    infotostruct();//again renew the present structure programme is using.
    fprintf(fw, "%lf", tbalance);
    fclose(fw);

    FILE * de;//This module acts as a part of the detailed account part.
    de = fopen(tname, "w");
    fprintf(de, "Original fund: $%lf\n", tbalance);
    fclose(de);

    return 1;

}

int login(void)
{
    char tcnumber[CARDID];
    char tpassword[7];
    char ch;
    int a, i, t, status;
    /*i is a searching counter
      a is used to limit the maximum time that a user can enter the wrong password
      t is to mark the situation that the user used out all his times to enter his password
      status is used for show if the system find the card number users provided.*/
    printf("Please insert your bank card:\n>");
    scanf("%s", tcnumber);
    while ((ch = getchar()) != '\n')
        continue;//clear buffer, and make sure that there is no '\n behind at the end of the line

    for (i = 0; i < MUSER; i++)
    {
        if(0 == strcmp(list[i].cnumber, tcnumber))
        {
            printf("USER FOUND\nPlease enter your PIN code:\n>");
            scanf("%s", tpassword);
            status = 1;
            break;
        }//check the card number's existence.
        //if so, status == 1
        else
        {
            status = 0;
            continue;
        }
        // if not, status == 0, and the number will go through this loop again
    }

    if (status == 1)
    {
        for (a = 0; a <= 4; a++)//there are 4 times for the user to enter the password
        {
            if (0 == strcmp(list[i].password, tpassword))
            {
                printf("LOGIN SUCCESSFULLY, WELCOME, \nHello, %s\n", list[i].name);
                t = 1;
                no = i;
                return 1;
                break;
            }
            else
            {
                if (a == 4)
                {
                    t = 0;
                    break;
                }
                else
                {
                    printf("WRONG PIN, %d TRY LEFT\n>", 4 - a);
                    scanf("%s", tpassword);
                    t = 0;
                    continue;
                }
            }
        }
    }
    else
    {
        printf("USER NOT FUND, PLEASE REGISTER.\n");
        registerUser();
    }

    if(t == 0)
    {
        printf("FOR SAFETY CONCERN, YOUR ACCOUNT HAS BEEN FROZEN\n"
               "PLEASE CLOSE THIS WINDOW.\n");
               return 0;
    }
    return 2;


}

void login_enroll()//ask user to login or to register
{
    printf("\n1. No account? Register one\t2. Already have an account. Login\n");
}

void showbalance(void)
{
    FILE * fb = fopen(list[no].cnumber, "r");
    if (fb == NULL)
        printf("OPEN FAILURE");
    double balance;
    fscanf(fb, "%lf", &balance);
    printf("You have %lf dollars in your account right now\n", balance);
    fclose(fb);
    return;
}

void deposit(void)
{
    int depo;
    printf("PUT IN YOUR MONEY\n>");
    scanf("%d", &depo);
    while(1)
    {
        if (depo % 20 == 0 || depo % 50 == 0 || (depo % 50) % 20 == 0)
        {
            FILE * fr;
            fr = fopen (list[no].cnumber, "r");
            double tbalance;
            fscanf(fr, "%lf", &tbalance);
            fclose(fr);

            FILE * fw;
            fw = fopen (list[no].cnumber, "w");
            tbalance += (double)depo;
            fprintf(fw, "%lf", tbalance);
            fclose(fw);
            printf("SUCCESSFUL!\n");

            FILE * de;//deposit detailed account
            de = fopen(list[no].name, "a");
            fprintf(de, "Deposited $%d\n", depo);
            fclose(de);

            break;
        }
        else
        {
            printf("THIS MECHINE CAN ONLY RECEIVE $20 AND $50.\n");
            printf("Please put in your money again.\n>");
            scanf("%d", &depo);
            continue;
        }
    }
}

void withdraw(void)
{
    int withdr;
    int m, n;
    printf("PLEASE ENTER AMOUNT OF MONEY YOU WANT TO WITHDRAW\n>");
    scanf("%d", &withdr);
    FILE * fr;
    fr = fopen (list[no].cnumber, "r");
    double tbalance;
    fscanf(fr, "%lf", &tbalance);
    fclose(fr);

    while(1)
    {
        if((withdr % 20 == 0 || withdr % 50 == 0 || (withdr % 50) % 20 == 0) && withdr <= tbalance)
            {
                if(withdr % 50 == 0)
                {
                    m = withdr / 50;
                    n = 0;
                }
                else if (withdr % 20 == 0 && withdr < 50)
                {
                    m = 0;
                    n = withdr / 20;
                }
                else
                {
                    m = (withdr - (withdr % 50)) / 50;
                    n = (withdr % 50) / 20;
                }


                FILE * fw;
                fw = fopen (list[no].cnumber, "w");
                tbalance -= (double)withdr;
                fprintf(fw, "%lf", tbalance);
                fclose(fw);

                printf("You will get %d $50 cash and %d $20 cash.\n", m, n);

                FILE * wi;//withdraw detailed account
                wi = fopen(list[no].name, "a");
                fprintf(wi, "Withdrawn $%d\n", withdr);
                fclose(wi);

                break;
        }

        else if(withdr > tbalance)
        {
            printf("Balance of your account is not sufficient.\n");
            printf("PLEASE ENTER AMOUNT OF MONEY YOU WANT TO WITHDRAW\n>");
            scanf("%d", &withdr);
            continue;
        }
        else
        {
            printf("You cannot withdraw $%d from this ATM, enter again please.\n", withdr);
            printf("PLEASE ENTER AMOUNT OF MONEY YOU WANT TO WITHDRAW\n>");
            scanf("%d", &withdr);
            continue;
        }
    }
}

void transfer(void)
{
    int i;
    FILE * receive;
    FILE * trans;
    char ch;
    double transferred, tbalancer, tbalancet;
    char tnumber[CARDID];

    printf("Please enter the card number of the card you want to transfer to\n>");
    scanf("%s", tnumber);

    for (i = 0; i < MUSER; i++)
    {
        if ((strcmp(tnumber, list[i].cnumber)) == 0)
        {
            printf("USER FOUND\n");
            break;
        }
    }

    while ((ch = getchar()) != '\n')
        continue;
    receive = fopen(tnumber, "r");
    trans = fopen(list[no].cnumber, "r");
    if (receive == NULL)
    {
        printf("NO SUCH USER !\n");
        return;
    }
    else
    {
        fscanf(trans, "%lf", &tbalancet);

        fscanf(receive, "%lf", &tbalancer);

        fclose(receive);
        fclose(trans);
        while(1)
        {
            printf("You now have %lf dollars in your account.\n"
                   "How much do you want to transfer ?\n>", tbalancet);
            scanf("%lf", &transferred);
            if (transferred > tbalancet)
            {
                printf("Insufficient Balance\n");
                system("pause");
                continue;
            }
            else
            {
                tbalancer += transferred;
                tbalancet -= transferred;
                receive = fopen(tnumber, "w");
                fprintf(receive, "%lf", tbalancer);
                fclose(receive);
                trans = fopen(list[no].cnumber, "w");
                fprintf(trans, "%lf", tbalancet);
                fclose(trans);

                FILE * detailedt;
                detailedt = fopen(list[no].name, "a");
                fprintf(detailedt, "Transferred $%lf to %s\n", transferred, tnumber);
                fclose(detailedt);

                FILE * detailedr;
                detailedr = fopen(list[i].name, "a");
                fprintf(detailedr, "Received $%lf from %s\n", transferred, list[no].cnumber);
                fclose(detailedr);

                break;
            }
        }
    }
}

void detailed_account(void)
{
    printf("DETTAILED ACCOUNT\n\n");
    char ch;
    FILE * ac;
    ac = fopen(list[no].name, "r");
    ch = getc(ac);
    putchar('\n');
    while (ch != EOF)
    {
        putchar(ch);
        ch = getc(ac);
        if (ch == '\n')
            putchar('\n');
    }
    putchar('\n');
    fclose(ac);
    return;
}

int main()
{
    infotostruct();
    int a, b, choice;//a is the first choice, b is the login_in_status, and choice is the menu choice.
    char ch;
    login_enroll();
    scanf("%d", &a);
    while((ch = getchar()) != '\n')
        continue;
    while(1)//this loop processes the user's choice to
    {
        if (a == 1)
        {
            status_register = registerUser();
            if (status_register == 0)
            {
                login_enroll();
                scanf("%d", &a);
                while((ch = getchar()) != '\n')
                    continue;
                continue;
            }
            infotostruct();
            system("cls");
            b = login();
            break;
        }
        else if (a == 2)
        {
            b = login();
            break;
        }
        else
        {
            printf("NO SUCH CHOICE! CHOOSE AGAIN\n>");
            scanf("%d", &a);
            while((ch = getchar()) != '\n')
                continue;
            continue;
        }
    }


    if (b == 1)
        printMenu();//if login successfully, print the menu.
    else if (b == 2)
        {
            printf("Please restart this programme and login again.\n");
            return 0;// if can't find the user, programme will ask user to register and restart this programme to login again.
        }
    else
        return 0;//if login failed, terminate the programme immediately.


    while(1)
    {
        scanf("%d", &choice);
        if (choice == 1)
        {
            showbalance();
            printMenu();
            continue;
        }
        else if (choice == 2)
        {
            deposit();
            printMenu();
            continue;
        }
        else if (choice == 3)
        {
            withdraw();
            printMenu();
            continue;
        }
        else if (choice == 4)
        {
            transfer();
            printMenu();
            continue;
        }
        else if (choice == 5)
        {
            detailed_account();
            printMenu();
            continue;
        }
        else if (choice == 6)
        {
            printf("Thank you for using, BYE.\n");
            break;
        }
    }
    return 0;
}


