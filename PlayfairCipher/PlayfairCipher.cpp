// PlayfairCipher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
Алгоритм:

1. Вводится ключевое слово(текст), для формирования таблицы шифрования.
	1.1 Текст фильтруется от присутствия лишних символов, отличных от букв.
2. Текст преобразуется в набор букв, которые нужно вставить в начало таблицы.
3. Заполнить остальную таблицу буквами так, что I = J.
4. Вводится текст для шифровки.
	4.1 Текст фильтруется от присутствия лишних символов, отличных от букв.
5. Шифруемый текст переводится в заглавные буквы, для простоты работы алгоритма.
6. Шифруемый текст делится на биграммы, между двойными буквами и в пустых местах добавляется Х.
7. Биграммы передаются на обработку в шифр.
	7.1 Если буквы являются углами прямоугольника, тогда беруться противоположные углы.
	7.2 Если буквы находятся в одной строке, тогда беруться буквы с сдвигом вправо.
	7.3 Если буквы находятся в одном столбце, тогда беруться буквы с сдвигом вниз.
8. Вывод зашифрованного текста.
9. Расшифровка.

*/

#include <iostream>
#include <string>

using namespace std;

	char alphabet[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
char alphabet_low[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };

// 1.1, 4.1, 5.
string StrFilter(string input) {
	// Создаём новую строку, в которой уже будут все буквы заглавными, не будет лишних символов, а также не будет J
	string new_str;
	// В этом цикле проходятся все символы входящей строки
	for (unsigned int i = 0; i < input.length(); i++) {
		// В этом цикле проходятся все символы строки алфавита
		for (unsigned int j = 0; j < sizeof(alphabet); j++) {
			// Если буква J, то для неё отдельный вариант
			if (j != 9) {
				// Если была маленькая буква, то меняет на большую
				if (input[i] == alphabet_low[j]) {
					new_str += alphabet[j];
					j = 25;
				}
				// если была большая, то так и оставляет
				else {
					if (input[i] == alphabet[j]) {
						new_str += input[i];
						j = 25;
					}
				}
				// другие символы просто отбрасываются
			}
			else {
				// если маленькая или большая J, то меняется на большую I
				if (input[i] == alphabet_low[j] or input[i] == alphabet[j]) {
					new_str += alphabet[8];
					j = 25;
				}
			}
		}
	}

	// Функция завершается с выводом фильтрованной строки
	return new_str;
}

string CreateTable(string keyword) {
	// Создаём пустую таблицу
	string table;

	// 2. Заполняем её символами из кодового слова
	for (unsigned int i = 0; i < keyword.length(); i++) {
		if (table.find(keyword[i]) == string::npos) {
			table += keyword[i];
		}
	}

	// Создаём строку, в которой после исключим букву J
	string remains(alphabet);

	// Исключаем букву J, т.к. она будет равна букве I
	remains = remains.erase(remains.find('J'), 1);

	// 3. Дозаполняем таблицу остальными символами
	for (unsigned int i = 0; i < remains.length(); i++) {
		if (table.find(remains[i]) == string::npos) {
			table += remains[i];
		}
	}

	// Вывод сделанной таблицы, для проверки
	cout << endl << "  Table:" << endl;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cout << table[i * 5 + j] << ' ';
		}
		cout << endl;
	}
	cout << endl;

	// Функция завершается с выводом готовой таблицы
	return table;
}

string Encrypting(string input, string table) {
	// Пустая строка, которая будет заполнятся шифром
	string chipertext, str;

	// Цикл, который подготавливает склеенные биграммы для шифра
	int i;
	for (i = 0; unsigned(i) < input.length() - 1; i += 2) {
		if (input[i] != input[i + 1]) {
			str += input[i];
			str += input[i+1];
		}
		else {
			str += input[i];
			str += 'X';
			i--;
		}
	}

	// Если последняя буква текста не была добавлена, тогда он её обрабатывает
	if (unsigned(i) < input.length()) {
		str += input[input.length() - 1];
	}

	// Если последняя биграмма не заполнена, то добавляет X
	if (str.length() % 2 == 1) {
		str += 'X';
	}

	// Вывод биграмм на экран
	/*for (unsigned int i = 0; i < str.length(); i += 2) {
		cout << str[i] << str[i + 1] << ' ';
	}
	cout << endl;*/

	int pos1, pos2, let1, let2;
	for (unsigned int i = 0; i < str.length(); i += 2) {
		pos1 = table.find(str[i]);
		pos2 = table.find(str[i+1]);

		// Если находятся в одной строке
		if (int(pos1 / 5) == int(pos2 / 5)) {

			// В первой части вычисляется строка, и так как у нас вместо таблицы string, то умножаем на 5, 
			// потом остаток от деления нужен, чтобы при сдвиге направо не перейти на следующую строку
			let1 = int(pos1 / 5) * 5 + (pos1 + 1) % 5;
			let2 = int(pos2 / 5) * 5 + (pos2 + 1) % 5;

			chipertext += table[let1];
			chipertext += table[let2];

		}
		else {
			// Если находятся в одном столбце
			if (pos1 % 5 == pos2 % 5) {

				// Добавляется 5, так как нужно перейти на следующую строку, 
				// а так же берётся остаток от 25, если нужно перейти на верхнюю строчку
				let1 = (pos1 + 5) % 25;
				let2 = (pos2 + 5) % 25;

				chipertext += table[let1];
				chipertext += table[let2];
			}
			// Если являются углами квадрата/прямоугольника
			else {

				// Берется та же строка, но стобец берётся из противоположной буквы
				let1 = int(pos1 / 5) * 5 + pos2 % 5;
				let2 = int(pos2 / 5) * 5 + pos1 % 5;

				chipertext += table[let1];
				chipertext += table[let2];

			}
		}
	}

	// Функция завершается с выводом готового шифра
	return chipertext;
}

string Decrypting(string input, string table) {
	string decrypttext;
	
	int pos1, pos2, let1, let2;
	for (unsigned int i = 0; i < input.length(); i += 2) {
		pos1 = table.find(input[i]);
		pos2 = table.find(input[i + 1]);

		// Если находятся в одной строке
		if (int(pos1 / 5) == int(pos2 / 5)) {

			// Алгоритм обратный шифрованию - отнимается 1, вместо добавления, только ещё добавляется 5,
			// если придётся остаться на той же строке, чтобы не уйти ниже нуля
			let1 = int(pos1 / 5) * 5 + ((pos1 - 1) + 5) % 5;
			let2 = int(pos2 / 5) * 5 + ((pos2 - 1) + 5) % 5;

			decrypttext += table[let1];
			decrypttext += table[let2];

		}
		else {
			// Если находятся в одном столбце
			if (pos1 % 5 == pos2 % 5) {

				// Алгоритм обратный шифрованию - отнимается 5, вместо добавления, только ещё добавляется 25, 
				// если придётся возвращаться с верней строки на нижнюю, чтобы не уйти ниже нуля
				let1 = ((pos1 - 5) + 25) % 25;
				let2 = ((pos2 - 5) + 25) % 25;

				decrypttext += table[let1];
				decrypttext += table[let2];
			}
			// Если являются углами квадрата/прямоугольника
			else {

				// Берется та же строка, но стобец берётся из противоположной буквы
				let1 = int(pos1 / 5) * 5 + pos2 % 5;
				let2 = int(pos2 / 5) * 5 + pos1 % 5;

				decrypttext += table[let1];
				decrypttext += table[let2];

			}
		}
	}

	// Функция завершается с выводом расшифровки
	return decrypttext;
}

int main()
{
	// Вывод правил формированрия квадрата и шифрования
	cout << "Rules for the formation of the Playfair square:" << endl;
	cout << "1. The letter 'J' is replaced with 'I' to form a 5x5 square" << endl;
	cout << "2. 'X' is used as an additional character when you need to complete a bigram or separate two identical letters" << endl;
	cout << "3. The Playfair square is filled line by line, starting with the keyword" << endl;

	// 1.
	string keyword;
	cout << "\nEnter a keyword:" << endl;
	getline(cin, keyword);

	// 1.1
	keyword = StrFilter(keyword);
	string table = CreateTable(keyword);

	// 4.
	string plaintext;
	cout << "Enter text to encrypt:" << endl;
	getline(cin, plaintext);

	// 4.1, 5.
	string filtredtext = StrFilter(plaintext);

	string chipertext = Encrypting(filtredtext, table);

	// 8.
	cout << "\nChipper Text:" << endl;
	cout << chipertext << endl;

	// 9.
	string decrypttext = Decrypting(chipertext, table);

	cout << "\nDecrypted Text:" << endl;
	cout << decrypttext << endl;

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
