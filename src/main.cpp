
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "user.hpp"
#include "errors.hpp"

void ClearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int GetIntInput(const std::string& prompt = "") {
    int value;
    if (!prompt.empty()) std::cout << prompt;
    if (!(std::cin >> value)) {
        ClearInput();
        throw Errors::InvalidArgument();
    }
    return value;
}

template<typename T>
T GetTypedInput(const std::string& prompt = "Enter value: ") {
    T value;
    std::cout << prompt;
    if (!(std::cin >> value)) {
        ClearInput();
        throw Errors::InvalidArgument();
    }
    return value;
}

struct ISequenceWrapper {
    virtual ~ISequenceWrapper() = default;
    virtual void Show() const = 0;
    virtual void Append() = 0;
    virtual void Prepend() = 0;
    virtual void InsertAt() = 0;
    virtual void RemoveAt() = 0;
    virtual void GetAt() const = 0;
    virtual void GetSubsequence() const = 0;
    virtual std::shared_ptr<ISequenceWrapper> Concat(const std::shared_ptr<ISequenceWrapper>& other) const = 0;
    virtual const std::string& TypeKey() const = 0;
};

template<typename T>
struct SequenceWrapper : public ISequenceWrapper {
    std::shared_ptr<Sequence<T>> seq;
    std::string structure;
    std::string type_key;

    SequenceWrapper(const std::string& structure_, const std::string& key) : structure(structure_), type_key(key) {
        if (structure == "array") seq = std::make_shared<MutableArraySequence<T>>();
        else seq = std::make_shared<MutableListSequence<T>>();
    }

    void Show() const override {
        std::cout << "[ ";
        for (int i = 0; i < seq->GetLength(); ++i)
            std::cout << seq->Get(i) << " ";
        std::cout << "]\n";
    }

    void Append() override {
        T val = GetTypedInput<T>("Enter value to append: ");
        seq->Append(val);
    }

    void Prepend() override {
        T val = GetTypedInput<T>("Enter value to prepend: ");
        seq->Prepend(val);
    }

    void InsertAt() override {
        int index = GetIntInput("Enter position: ");
        if (index < 0 || index > seq->GetLength()) throw Errors::IndexOutOfRange();
        T val = GetTypedInput<T>();
        seq->InsertAt(val, index);
    }

    void RemoveAt() override {
        int index = GetIntInput("Enter index to remove: ");
        if (index < 0 || index >= seq->GetLength()) throw Errors::IndexOutOfRange();
        seq->Remove(index);
    }

    void GetAt() const override {
        int index = GetIntInput("Enter index to get: ");
        if (index < 0 || index >= seq->GetLength()) throw Errors::IndexOutOfRange();
        std::cout << "Element: " << seq->Get(index) << "\n";
    }

    void GetSubsequence() const override {
        int start = GetIntInput("Enter start: ");
        int end = GetIntInput("Enter end: ");
        if (start < 0 || end >= seq->GetLength() || start > end) throw Errors::IndexOutOfRange();
        auto sub = seq->GetSubsequence(start, end);
        std::cout << "Subsequence: [ ";
        for (int i = 0; i < sub->GetLength(); ++i)
            std::cout << sub->Get(i) << " ";
        std::cout << "]\n";
    }

    std::shared_ptr<ISequenceWrapper> Concat(const std::shared_ptr<ISequenceWrapper>& other) const override {
        auto other_casted = std::dynamic_pointer_cast<SequenceWrapper<T>>(other);
        if (!other_casted || other_casted->structure != structure)
            throw Errors::ConcatTypeMismatchError();

        Sequence<T>* raw = *seq + *other_casted->seq;
        auto result = std::make_shared<SequenceWrapper<T>>(structure, type_key);
        result->seq = std::shared_ptr<Sequence<T>>(raw);
        return result;
    }

    const std::string& TypeKey() const override {
        return type_key;
    }
};

void ShowTypeMenu() {
    std::cout << "Select type:\n1. int\n2. double\n3. string\n4. user\n";
}
void ShowStructureMenu() {
    std::cout << "Select structure:\n1. array\n2. list\n";
}

int interface(std::vector<std::shared_ptr<ISequenceWrapper>> sequences) {
    while (true) {
        std::cout << "\n1. Show\n2. Append\n3. Prepend\n4. Remove element\n5. Insert element at index\n";
        std::cout << "6. Get element by index\n7. Get subsequence\n8. Concat sequences\n";
        std::cout << "9. Add sequence\n10. Remove sequence\n11. Exit\nChoose action: ";

        try {
            int choice = GetIntInput();

            if (choice != 9 && choice != 11 && sequences.empty()) {
                std::cout << "No sequences yet. Add a sequence first.\n";
                continue;
            }

            switch (choice) {
                case 1: // Show all sequences
                    for (size_t i = 0; i < sequences.size(); ++i) {
                        std::cout << i << ": ";
                        sequences[i]->Show();
                    }
                    break;

                case 2: case 3: case 4: case 5: case 6: case 7: { 
                    std::cout << "Choose sequence index (from 0 to " << sequences.size() - 1 << "): ";
                    int idx = GetIntInput();
                    if (idx < 0 || static_cast<size_t>(idx) >= sequences.size()) throw Errors::IndexOutOfRange();
                    switch (choice) {
                        case 2: sequences[idx]->Append(); break; // Append
                        case 3: sequences[idx]->Prepend(); break; // Prepend
                        case 4: sequences[idx]->RemoveAt(); break; // Remove
                        case 5: sequences[idx]->InsertAt(); break; // Insert
                        case 6: sequences[idx]->GetAt(); break; // Get
                        case 7: sequences[idx]->GetSubsequence(); break;  // Get subsequence
                    }
                    break;
                }

                case 8: {  // Concat sequences
                    std::cout << "First sequence index (0 to " << sequences.size() - 1 << "): ";
                    int i1 = GetIntInput();
                    std::cout << "Second sequence index (0 to " << sequences.size() - 1 << "): ";
                    int i2 = GetIntInput();
                    if (i1 < 0 || i2 < 0 || static_cast<size_t>(i1) >= sequences.size() || static_cast<size_t>(i2) >= sequences.size())
                        throw Errors::IndexOutOfRange();
                    auto result = sequences[i1]->Concat(sequences[i2]);
                    sequences.push_back(result);
                    std::cout << "Concatenated sequence added as index " << sequences.size() - 1 << "\n";
                    break;
                }

                case 9: {  // Add new sequence
                    ShowTypeMenu();
                    int t = GetIntInput("Enter type: ");
                    std::string type;
                    switch (t) {
                        case 1: type = "int"; break;
                        case 2: type = "double"; break;
                        case 3: type = "string"; break;
                        case 4: type = "user"; break;
                        default: throw Errors::InvalidArgument();
                    }

                    ShowStructureMenu();
                    int s = GetIntInput("Enter structure: ");
                    std::string structure = (s == 1) ? "array" : (s == 2) ? "list" : throw Errors::InvalidArgument();

                    if (type == "int") sequences.push_back(std::make_shared<SequenceWrapper<int>>(structure, type));
                    else if (type == "double") sequences.push_back(std::make_shared<SequenceWrapper<double>>(structure, type));
                    else if (type == "string") sequences.push_back(std::make_shared<SequenceWrapper<std::string>>(structure, type));
                    else if (type == "user") sequences.push_back(std::make_shared<SequenceWrapper<User>>(structure, type));
                    break;
                }

                case 10: {
                    std::cout << "Index to remove (0 to " << sequences.size() - 1 << "): ";
                    int idx = GetIntInput();
                    if (idx < 0 || static_cast<size_t>(idx) >= sequences.size()) throw Errors::IndexOutOfRange();
                    sequences.erase(sequences.begin() + idx);
                    break;
                }

                case 11:
                    std::cout << "Exiting...\n";
                    return 0;

                default:
                    std::cout << "Invalid action.\n";
            }

        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

int main() {
    std::vector<std::shared_ptr<ISequenceWrapper>> sequences;
    interface(sequences);
}


//main пустой ---ВЫПОЛНЕНО---
//не везде const в аргументах(напимер concat) ---ВЫПОЛНЕНО---
//ошибки - так не надо(сделать как у володи(в телефоне лежит)) - Не могу, ибо ловлю ошибки
//concat работает за линию, а можно за константу (очень просто) ---ВЫПОЛНЕНО---
//в dynamicArray->remove - сдвиги лишние (можно перенести сразу) ---ВЫПОЛНЕНО---
//сделайте перегрузку для concat (оператор +) ---ВЫПОЛНЕНО---
//arraySequence - где capacity? ---ВЫПОЛНЕНО---
//зачем явно вызывать delete? --- ВЫПОЛНЕНО---
//тесты (существуют (сарказм)) - сделать нормальные  ---ВЫПОЛНЕНО---
//везде сначала объявление, а потом реализация(для последовательностей нарушил)  ---ВЫПОЛНЕНО---
//хочу как в первой лабе список последовательностей, чтобы можно было разные последовательности выбирать ---ВЫПОЛНЕНО---
//добавить пользовательский тип - какую-нибудь структуру (например, user) ---ВЫПОЛНЕНО---
//когда у меня спрашивают индекс - пусть скажут, в каких пределах он может изменяться ---ВЫПОЛНЕНО---
//подпоследовательность и сконкатинированна пос-ть сохраняются в новую пос-ть ---ВЫПОЛНЕНО---

