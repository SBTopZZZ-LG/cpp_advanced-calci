#include <iostream>

using namespace std;

struct NAO
{
    int num;
    char opr;
    NAO *next, *prev;

    NAO() : num(0), opr('\0'), next(nullptr) {}
    NAO(int num) : num(num), opr('\0'), next(nullptr) {}
    NAO(int num, char opr) : num(num), opr(opr), next(nullptr) {}

    ostream &operator<<(ostream &os)
    {
        cout << "PREV = " << prev << ", NEXT = " << next << ", ";
        os << "num = " << num << ", opr = ";
        if (opr != '\0')
            cout << opr;
        else
            cout << "null_char";

        return os;
    }
};

NAO *generateLL(char *);
int evaluateLL(NAO *);

int main()
{
    char str[100];
    cout << "Enter the mathematical expression (without whitespaces): ";
    cin >> str;

    NAO *start = generateLL(str), *current = start;

    cout << "Final answer = " << evaluateLL(start) << '\n';

    return 0;
}

bool isNumber(char x)
{
    return x >= '0' && x <= '9';
}

int toNumber(char x)
{
    return (x - '0');
}

NAO *generateLL(char *str)
{
    NAO *start = nullptr, *root = nullptr;

    int lastNumber = 0, lastNumberSign = 0;
    char lastOperation = '\0', numericalSign = '+';
    for (int i = 0; i < 100 && str[i] != '\0'; i++)
    {
        if (isNumber(str[i]))
        {
            if (i > 0)
                if (!isNumber(str[i - 1]))
                    if (numericalSign == '-')
                        lastNumberSign = 1;

            lastNumber *= 10;
            if (lastNumberSign)
                lastNumber -= toNumber(str[i]);
            else
                lastNumber += toNumber(str[i]);

            lastOperation = '\0';
        }
        else if (lastOperation == '\0')
        {
            lastOperation = str[i];
            numericalSign = '+';

            if (start == nullptr)
                start = new NAO(lastNumber, lastOperation);
            else if (root == nullptr)
            {
                root = new NAO(lastNumber, lastOperation);
                root->prev = start;
                start->next = root;
            }
            else
            {
                NAO *newRoot = new NAO(lastNumber, lastOperation);
                root->next = newRoot;
                newRoot->prev = root;
                root = newRoot;
            }

            lastNumber = lastNumberSign = 0;
        }
        else
            numericalSign = str[i];
    }

    if (lastNumber != 0)
    {
        if (start == nullptr)
            start = new NAO(lastNumber, '\0');
        else if (root == nullptr)
        {
            start->next = new NAO(lastNumber, '\0');
            start->next->prev = start;
        }
        else
        {
            root->next = new NAO(lastNumber, '\0');
            root->next->prev = root;
        }
    }

    return start;
}

int evaluateLL(NAO *start)
{
    NAO *current = start;
    bool pullBackLink = false;
    while (1)
    {
        int val = current->num;
        char opr = current->opr;

        if (opr == '\0')
        {
            // cout << "Character was null pointer\n";

            if (start == current)
                break;

            current = start;
            continue;
        }

        if (current->next != nullptr)
        {
            NAO *next = current->next;

            if ((next->opr == '*' || next->opr == '/') && (opr != '*' && opr != '/'))
            {
                // cout << "initiating pullback\n";

                pullBackLink = true;
                current = next;

                continue;
            }
            else
            {
                switch (opr)
                {
                case '+':
                    val += next->num;
                    break;
                case '-':
                    val -= next->num;
                    break;
                case '*':
                    val *= next->num;
                    break;
                case '/':
                    val /= next->num;
                    break;
                }

                next->num = val;

                // cout << "result = " << next->num << '\n';

                next->prev = current->prev;

                delete current;

                if (pullBackLink)
                {
                    // cout << "previous pullback detected\n";

                    current = current->prev;
                    current->next = next;
                    pullBackLink = false;

                    continue;
                }

                // cout << "no previous pullbacks\n";

                current = next;

                if (current->prev == nullptr)
                {
                    // cout << "previous of current was null, which means the previous of current was start\n";
                    start = next;
                }
            }
        }
    }

    return start->num;
}