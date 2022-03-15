#include <iostream>
#include <cstring>

#define FACTOR 1.5
#define BASE 10
#define NUMBER_OF_TEXTS 10

using namespace std;

// Класс одномерных массивов строк

class String {
  int len;
  char * body;
public :
  String() { len = 0; body = 0; }
  String(const char * str) { // работает для c-строк оканчивающихся 0
    body = new char[len = strlen(str)];
    strncpy(body, str, len);
  }
  String(const char * str, const int size) {
    body = new char[len = size];
    strncpy(body, str, len);
  }
  String(const String& str) {
    body = new char[len = str.len];
    strncpy(body, str.body, len);
  }
  ~String() { 
    if (body) {
      delete[] body;
    } 
  }
  String& operator = (const String& str) {
    if (body) {
      delete[] body;
    }
    body = new char[len = str.len];
    strncpy(body, str.body, len);
    return *this;
  }
  int operator == (const String& str) const {
    int min_len = str.len < len ? str.len : len;
    return !strncmp(str.body, body, min_len);
  }
  void print() const {
    for (int i = 0; i < len; ++i) {
      cout << body[i];
    }
    cout << endl;
  }
  int get_len() const { return len; };
  char * get_ptr() const { return body; }
};

class Text {
  int mem_size; // размер зарезервированной памяти  
  int actual_size; // размер используемой в данный момент памяти
  String * arr_of_str; 
public :
  Text() {
    actual_size = 0;
    mem_size = BASE;
    arr_of_str = new String[mem_size];
  }
  Text(int size) {
    actual_size = 0;
    mem_size = size;
    arr_of_str = new String[mem_size];
  }
  ~Text() {
    delete[] arr_of_str;
  }
  String& operator [] (int index) { 
    if (index >= actual_size) { 
      cerr << "operator [] : index not in range" << endl;
      abort();
    }
    return arr_of_str[index]; 
  }
  Text& operator = (const Text& txt) {
    delete[] arr_of_str;
    arr_of_str = new String[txt.actual_size];
    mem_size = actual_size = txt.actual_size;
    for (int i = 0; i < actual_size; ++i) {
      arr_of_str[i] = txt.arr_of_str[i];
    }
    return *this;
  }
  void resize() { // расширение массива строк
    mem_size *= FACTOR;
    String * tmp_ptr = new String[mem_size];
    for (int i = 0; i < actual_size; ++i) {
      tmp_ptr[i] = arr_of_str[i];
    } 
    delete[] arr_of_str;
    arr_of_str = tmp_ptr;
  }
  void append(const String& str) { // добавление строки в текст
    if (actual_size == mem_size) {
      this->resize();
    }
    arr_of_str[actual_size] = str;
    ++actual_size;
  }
  bool is_contain (const String& str) const { // проверка содержания в тексте строки передаваемой в качестве параметра
    bool answ = false;
    for (int i = 0; i < actual_size; ++i) {
      answ = str == arr_of_str[i];
      if (answ) {
        break;
      }
    }
    return answ;
  }
  /* Добавление вторго текста в конец к первому, в качестве
  результата возвращается новый объект класса Text */
  Text concatenate(const Text& txt) const { 
    Text res_txt(actual_size + txt.actual_size);
    int i;
    for (i = 0; i < actual_size; ++i) {
      res_txt.arr_of_str[i] = arr_of_str[i];
    }
    int j;
    for(j = 0; i < res_txt.mem_size; ++i, ++j) {
      res_txt.arr_of_str[i] = txt.arr_of_str[j];
    }
    res_txt.actual_size = res_txt.mem_size;
    return res_txt;
  }
  /* Объединение первого и второго текста без повторений, 
  в качестве результата возвращается новый объект класса Text */
  Text merge_with_no_repeats(const Text& txt) const {
    Text res_txt(actual_size + txt.actual_size);
    int i, j;
    for (i = 0, j = 0; i < actual_size; ++i) {
      if (!res_txt.is_contain(arr_of_str[i])) {
        res_txt.arr_of_str[j] = arr_of_str[i];
        ++res_txt.actual_size;
        ++j;
      }
    }
    for(i = 0; i < txt.actual_size; ++i) {
      if (!res_txt.is_contain(txt.arr_of_str[i])) {
        res_txt.arr_of_str[j] = txt.arr_of_str[i];
        ++res_txt.actual_size;
        ++j;
      }
    }
    return res_txt;
  }
  void print_string(int index) const {
    arr_of_str[index].print();
  }
  void print_text() const {
    for (int i = 0; i < actual_size; ++i) {
      arr_of_str[i].print();
    }
  }
}; 

void invitation() {
  cout << "You have " << NUMBER_OF_TEXTS << " texts available with numbers from 0 to " << NUMBER_OF_TEXTS - 1 << endl;
  cout << "You can choose one of the operations : " << endl;
  cout << "1. Add string to text" << endl;
  cout << "2. Access to string by index" << endl;
  cout << "3. Connect two texts" << endl;
  cout << "4. Create an intersection of two texts" << endl;
  cout << "5. Print string by index" << endl;
  cout << "6. Print whole text" << endl;
  cout << "7. End session" << endl << '>';
}

int main() {
  Text * txt_arr = new Text[5];
  while (invitation(), true) {
    int op;
    cin >> op;
    switch(op) {
      case 1 : { 
        cout << "Enter text number and then string up to 256 symbols" << endl << '>';
        int n;
        cin >> n;
        getchar();
        char str[256];
        memset(str, 0, 256);
        cin.getline(str, 256);
        txt_arr[n].append(str); 
        break;
      }
      case 2 : {
        cout << "Enter text number and then index of the string" << endl << '>';
        int n, index;
        cin >> n >> index;
        cout << "----------------------" << endl;
        txt_arr[n][index].print();
        cout << "size = " << txt_arr[n][index].get_len() << endl;;
        cout << "address = " << (void*)txt_arr[n][index].get_ptr() << endl;
        cout << "----------------------" << endl;
        break;
      }
      case 3 : {
        cout << "Enter two text numbers you want to connect and then text number where to put the result" << endl << '>';
        int n1, n2, n3;
        cin >> n1 >> n2 >> n3;
        txt_arr[n3] = txt_arr[n1].concatenate(txt_arr[n2]);
        break;
      } 
      case 4 : {
        cout << "Enter two text numbers which you want to intersect and then text number where to put the result" << endl << '>';
        int n1, n2, n3;
        cin >> n1 >> n2 >> n3;
        txt_arr[n3] = txt_arr[n1].merge_with_no_repeats(txt_arr[n2]); 
        break;
      }
      case 5 : {
        cout << "Enter text number and then index of the string" << endl << '>';
        int n, index;
        cin >> n >> index;
        cout << "----------------------" << endl;
        txt_arr[n].print_string(index);
        cout << "----------------------" << endl;
        break;
      }
      case 6 : {
        cout << "Enter text number" << endl << '>';
        int n;
        cin >> n;
        cout << "----------------------" << endl;
        txt_arr[n].print_text();
        cout << "----------------------" << endl;
        break;
      }
      case 7 :
        cout << "Goodbye" << endl;
        delete[] txt_arr;
        exit(0);
      default : 
        cerr << "Wrong input!" << endl;
        abort();
    }
  }
  return 0;
}
