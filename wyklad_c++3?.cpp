#include <iostream>

using namespace std;

int main()
{

    /**/
    string name;
    char status;

    cout << "What's your name? ";
    cin >> name;

    cout << "Your status: uczen/student [U/S]: ";
    cin >> status;

    if (status == 'U')
    {
        cout << name << " zapraszamy na nasza uczelnie po maturze...";
    }
    else
    {
        if (status == 'S')
        {
            cout << "mamy nadzeje " << name << " ze studiujesz informatyke lub pielegniarstwo";
        }
        else
        {
            cout << name << "Booooooo ^^ ";
        }
    }


    return 0;
}


