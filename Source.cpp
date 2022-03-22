/*Задание 2. Описать структуру Man (Фамилия, Имя, Возраст, Дата рождения). Создать массив, предусмотреть:
■ Вывод информации с сортировкой по фамилии или
имени;
■ Вывод списка именинников месяца с указанием даты
рождения;
■ Изменение размеров массива при добавлении и удалении записей;
■ Поиск по фамилии и имени;
■ Редактирование записи.*/

#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdio.h>
using namespace std;

const int NAME_SIZE = 100; // размер имени и фамилии

struct Date { // структура дата
	size_t day;
	size_t month;
	size_t year;
};

struct Man {
	char* fname; // фамилия
	char* lname; // имя
	size_t phone;
	Date birthday;
};
// формирует из данных объект
Man* newMan(char* fname, char* lname, Date birthday, size_t phone = 0) {
	Man* tmpMan = new Man;
	tmpMan->fname = new char[NAME_SIZE];
	tmpMan->lname = new char[NAME_SIZE];
	strcpy_s(tmpMan->fname, NAME_SIZE, fname);
	strcpy_s(tmpMan->lname, NAME_SIZE, lname);
	tmpMan->birthday = birthday;
	tmpMan->phone = phone;
	return tmpMan;
}

Man* addFromConsole() {
	char fname[NAME_SIZE];
	char lname[NAME_SIZE];
	size_t phone;
	size_t day;
	size_t month;
	size_t year;

	cout << "Укажите имя: ";
	cin >> fname;
	cout << "Укажите фамилию: ";
	cin >> lname;
	cout << "Укажите телефон (6 цифр): ";
	cin >> phone;
	cout << "Укажите дату рождения (3 числа: день, месяц, год): ";
	cin >> day >> month >> year;
	return newMan(fname, lname, { day, month, year }, phone);
}
void printMan(Man* person) {
	cout << "Имя: " << person->fname << endl
		<< "Фамилия: " << person->lname << endl
		<< "телефон: " << person->phone << endl
		<< "Дата рождения: ";
	printf("%02u.%02u.%u\n", person->birthday.day, person->birthday.month, person->birthday.year);
}

void printAll(Man* arrMan, size_t size) {
	for (size_t i = 0; i < size; i++)
	{
		printMan(arrMan + i);
		cout << endl;
	}

}
// удаляем человека, очищаем память
void delMan(Man* person) {
	delete[]person->fname;
	delete[]person->lname;
//	delete person;
}
void delManArray(Man* arrMan, size_t size) {
	for (int i = size - 1; i >= 0; i--)
		delMan(arrMan + i);
	delete[]arrMan;
}
// добавляем нового человека
Man* addMan(Man* arrMan, size_t& size, Man* newPerson) {
	Man* newArray = new Man[size + 1];
	// перекопируем элементы, которые уже есть
	for (size_t i = 0; i < size; i++)
		newArray[i] = arrMan[i];
	newArray[size] = *(newPerson);
	//if (newPerson!=nullptr) delete newPerson; это надо делать отдельно
	if (arrMan != nullptr)delete[]arrMan;
	size++;
	return newArray;
}
// Поиск по фамилии и имени
void searchName(Man* arrMan, size_t size, char* fname, char* lname) {
	for (size_t i = 0; i < size; i++)
	{
		if (strstr((arrMan + i)->fname, fname) != NULL && strstr((arrMan + i)->lname, lname) != NULL)
			printMan(arrMan + i);
	}
}
void swap(Man* a, Man* b) {
	Man tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void sortByLname(Man* arrMan, size_t size) {
	for (size_t i = 1; i < size; i++)
	{
		for (size_t j = i; j >0&&strcmp(arrMan[j-1].lname, arrMan[j].lname)>0;j--)
		{
			swap(arrMan + j, arrMan + j - 1);
		}
	}
	printAll(arrMan, size);
}

/*
// сортировка по фамилии
void sort_by_lname(Man* arrMan, size_t size) {
	int j = 0;
	Man tmp;
	for (int i = 0; i < size; i++) {
		j = i;
		for (int k = i; k < size; k++) {
			if (strcmp(arrMan[j].lname, arrMan[k].lname) == 1)
				j = k;
		}
		tmp = arrMan[i];
		arrMan[i] = arrMan[j];
		arrMan[j] = tmp;
	}
	printAll(arrMan, size);
}*/
Date strToDate(char* str) {//23.12.2005
	Date newDate{};
	size_t* pDate[3];
	pDate[0] = &newDate.day;
	pDate[1] = &newDate.month;
	pDate[2] = &newDate.year;
	char number[10];
	int i{}; // какой символ
	int fields{}; // какое поле
	for (size_t j = 0; j < strlen(str); j++)
	{
		number[i] = str[j];
		i++;
		if (str[j] == '.'|| j == strlen(str)-1)
		{
			i = 0;
			*pDate[fields] = ((size_t)atoi(number));
			strcpy_s(number,10,"");
			fields++;
			continue;
		}
	}
		
	return newDate;
}
// чтение данных из файла
Man* readBook(FILE* MyBook, Man* contacts, size_t &cnum) {
	while (!feof(MyBook)) {
		Man person; // текущий контакт
		char fname[NAME_SIZE]; // имя
		char lname[NAME_SIZE]; // фамилия
		char dateStr[12]; // строка с датой
		// считываем имя
		fscanf_s(MyBook, "%s", fname, sizeof(fname));
		person.fname = new char[NAME_SIZE];
		strcpy_s(person.fname, NAME_SIZE, fname);
		// считываем фамилию
		fscanf_s(MyBook, "%s", lname, sizeof(lname));
		person.lname = new char[NAME_SIZE];
		strcpy_s(person.lname, NAME_SIZE, lname);
		// считываем телефон
		fscanf_s(MyBook, "%d", &person.phone);
		// считываем дату
		fscanf_s(MyBook, "%u.%u.%u", &person.birthday.day, &person.birthday.month, &person.birthday.year);
		//printMan(&person);
		contacts = addMan(contacts, cnum, &person);
	}
	return contacts;
}
int main() {
	setlocale(LC_ALL, "ru");
	FILE* mybook;
	Man* contacts{ nullptr }; // книга контактов
	Man* person;
	size_t cnum{ 0 }; // количество записей
	int answer{};
	char fname[NAME_SIZE];
	char lname[NAME_SIZE];
	do {
		person = addFromConsole();
		contacts = addMan(contacts, cnum, person);
		cout << "Продолжить?(1 - да, 0 - нет): ";
		cin >> answer;
	} while (answer);
	printAll(contacts, cnum);
	cout << endl;
/*	cout << "Введите фамилию для поиска: ";
	cin >> lname;
	cout << "Введите имя для поиска: ";
	cin >> fname;
	searchName(contacts, cnum, fname, lname);
	sort_by_lname(contacts, cnum);
	sortByLname(contacts, cnum);*/
//	char filename[] = { "myfile.txt" };
//	int arr[]{ 2,4,5,2,1 };
	/*if (fopen_s(&mybook, filename, "a") == 0) {
		// основная ветка программы работает с файлом
		for (int i = 0; i < 5; i++)
		{
			//fwrite(arr+i, sizeof(arr[i]), sizeof(arr[i] * 5), mybook);
			fprintf(mybook, "%1d ", arr[i]); // текстовый ввод
			fprintf(mybook, "\n");
		}
		if (fclose(mybook) == EOF) {
			//файл не может быть закрыт
		}
			
	}
	else {
		cout << "Файл " << filename << " не создан. Работа не возможна";
	}*/
	const char* BookFile = "MyBook.txt";
	if (fopen_s(&mybook, BookFile, "r") != NULL) {
		cout << "Can't open file " << BookFile;
	}
	else {// файл успешно открылся
		contacts = readBook(mybook, contacts, cnum);
	}
	printAll(contacts, cnum);
	//delManArray(contacts, cnum);
	return 0;
}