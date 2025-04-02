#include <iostream>
#include <string>
#include <limits>
#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "boxed_any.hpp"

using namespace std;

void PrintMenu() {
    cout << "\n===== Sequence UI =====\n";
    cout << "1. Show sequence\n";
    cout << "2. Append element\n";
    cout << "3. Prepend element\n";
    cout << "4. Insert element at index\n";
    cout << "5. Get element by index\n";
    cout << "6. Get subsequence\n";
    cout << "7. Concat another sequence\n";
    cout << "8. Exit\n";
    cout << "Select option: ";
}

BoxedAny ReadBoxed() {
    cout << "Choose type:\n1. int\n2. double\n3. string\n> ";
    int type;
    cin >> type;

    if (type == 1) {
        int v;
        while (!(cin >> v)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid int. Try again: ";
        }
        return BoxedAny(v);
    } else if (type == 2) {
        double v;
        while (!(cin >> v)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid double. Try again: ";
        }
        return BoxedAny(v);
    } else if (type == 3) {
        string v;
        cin >> ws;
        getline(cin, v);
        return BoxedAny(v);
    } else {
        cout << "Unknown type. Defaulting to int 0.\n";
        return BoxedAny(0);
    }
}

template<typename T>
void ShowSequence(Sequence<T>* seq) {
    cout << "[ ";
    for (int i = 0; i < seq->GetLength(); ++i) {
        cout << seq->Get(i);
        if (i != seq->GetLength() - 1)
            cout << ", ";
    }
    cout << " ]\n";
}

int main() {
    cout << "Choose sequence type:\n1. Mutable ArraySequence\n2. Mutable ListSequence\n> ";
    int type;
    cin >> type;

    Sequence<BoxedAny>* seq = nullptr;
    if (type == 1) seq = new MutableArraySequence<BoxedAny>();
    else if (type == 2) seq = new MutableListSequence<BoxedAny>();
    else {
        cout << "Invalid selection.\n";
        return 1;
    }

    while (true) {
        PrintMenu();
        int choice;
        cin >> choice;

        if (choice == 8) break;

        int index, start, end;
        BoxedAny value;

        switch (choice) {
            case 1:
                ShowSequence(seq);
                break;
            case 2:
                value = ReadBoxed();
                seq = seq->Append(value);
                break;
            case 3:
                value = ReadBoxed();
                seq = seq->Prepend(value);
                break;
            case 4:
                cout << "Enter index: ";
                cin >> index;
                value = ReadBoxed();
                seq = seq->InsertAt(value, index);
                break;
            case 5:
                cout << "Enter index: ";
                cin >> index;
                try {
                    cout << "Element: " << seq->Get(index) << endl;
                } catch (exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            case 6:
                cout << "Enter start index: ";
                cin >> start;
                cout << "Enter end index: ";
                cin >> end;
                try {
                    Sequence<BoxedAny>* sub = seq->GetSubsequence(start, end);
                    cout << "Subsequence: ";
                    ShowSequence(sub);
                    delete sub;
                } catch (exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            case 7: {
                Sequence<BoxedAny>* other = (type == 1)
                    ? (Sequence<BoxedAny>*)new MutableArraySequence<BoxedAny>()
                    : (Sequence<BoxedAny>*)new MutableListSequence<BoxedAny>();
                int count;
                cout << "How many elements to add: ";
                cin >> count;
                for (int i = 0; i < count; ++i) {
                    BoxedAny v = ReadBoxed();
                    other = other->Append(v);
                }
                seq = seq->Concat(other);
                delete other;
                break;
            }
            default:
                cout << "Invalid option.\n";
                break;
        }
    }

    delete seq;
    cout << "Goodbye!\n";
    return 0;
}

// доавить удаление элементов