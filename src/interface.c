#include <iostream>

using namespace std;

void PrintMenu() {
    cout << "\n===== Sequence UI =====\n";
    cout << "1. Show sequence\n";
    cout << "2. Append element\n";
    cout << "3. Prepend element\n";
    cout << "4. Remove element\n";
    cout << "5. Insert element at index\n";
    cout << "6. Get element by index\n";
    cout << "7. Get subsequence\n";
    cout << "8. Concat another sequence\n";
    cout << "9. Exit\n";
    cout << "Select option: ";
}

BoxedAny ReadBoxed() {
    cout << "Choose type:\n1. int\n2. double\n3. string\n> ";
    int type;
    
    while (!(cin >> type) || (type < 1 || type > 3)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid type. Choose 1, 2, or 3: ";
    }

    try {
        cout << "Enter value: ";
        if (type == 1) { // int
            int v;
            while (!(cin >> v)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid int. Try again: ";
            }
            return BoxedAny(v);
        } else if (type == 2) { // double
            double v;
            while (!(cin >> v)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid double. Try again: ";
            }

            return BoxedAny(v);
        } else if (type == 3) { // string
            string v;
            cin >> ws;        
            getline(cin, v); 
            return BoxedAny(v);
        }
    } catch (exception& e) {
        cout << "Error: " << e.what() << endl;
        return BoxedAny();  
    }

    return BoxedAny(); 
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



void start_interface(){
    cout << "Choose sequence type:\n1. Mutable ArraySequence\n2. Mutable ListSequence\n> ";
    int type;

    while (!(cin >> type) || (type < 1 || type > 2)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid type. Choose 1 or 2: ";
    }

    Sequence<BoxedAny>* seq = nullptr;
    if (type == 1) seq = new MutableArraySequence<BoxedAny>(); // ArraySequence
    else if (type == 2) seq = new MutableListSequence<BoxedAny>(); // ListSequence
    else {
        cout << "Invalid selection.\n";
        return 1;
    }

    while (true) {
        PrintMenu();
        int choice;

        while (!(cin >> choice) || (choice < 1 || choice > 9)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choise. Try again: ";
        }

        if (choice == 9) break; // exit

        int index, start, end;
        BoxedAny value;

        switch (choice) {
            case 1: // Show sequence
                ShowSequence(seq);
                break;
            case 2: // Append element
                value = ReadBoxed();
                seq = seq->Append(value);
                break;
            case 3: // Prepend element
                value = ReadBoxed();
                seq = seq->Prepend(value);
                break;
            case 4: // Remove element
                cout << "Enter index: ";
                cin >> index;
                try {
                    seq = seq->Remove(index);
                    cout << "Element has been removed";                 
                }catch(exception& e){
                    cout << "Error: " << e.what() << endl;
                }
                break;
            case 5: // Insert element at index
                cout << "Enter index: ";
                cin >> index;
                value = ReadBoxed();
                try {
                    seq = seq->InsertAt(value, index);
                } catch (exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            
            case 6: // Get element by index
                cout << "Enter index: ";
                cin >> index;
                try {
                    cout << "Element: " << seq->Get(index) << endl;
                } catch (exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            case 7: // Get subsequence
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
            case 8: { // Concat another sequence
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
}