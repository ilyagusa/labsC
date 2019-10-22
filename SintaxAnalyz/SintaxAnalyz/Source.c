#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <locale.h>



#define BUFSIZE 80
#define MAX_ID_SIZE 64
#define MAX_NUM_SIZE 15
#define SIZE_OF_TW_TABLE 20
#define SIZE_OF_TD_TABLE 22
#define SIZE_OF_TNUM_TABLE 64
#define SIZE_OF_TID_TABLE 128
#define WORD_SIZE_TW 9
#define WORD_SIZE_TD 3
#define allTableSize 300
#define STACK_SIZE 100

char c;//очередной символ

int j = 0;//для проверок

int jBuf = 0;

char buf[BUFSIZE];

int f = 0;

int r = 0;

int count = 0;//для скобок!

int check = 0;//проверка для начала работы  синтаксического анализатора 

int checkreal;// проверка

int allTable[allTableSize][2];

int decStack[STACK_SIZE];
int decStackPointer = 0;

int prog_name();

int dec_list();

int dec();

int id_list();

int type();

int stmt_list();

int stmt();

int read();

int write();

int assign_();

int exp();

int term();

int for_();

int index_exp();

int body();

int requirement();

int operation();

int while_();

int notdescribed();

void printtypeandidtable();

void ipush(int i);

int ipop();

// номер таблицы и номер лексемы в таблице

typedef struct record {
	int indextable;
	int indexintable;
	char type[10];
}record;


struct record trackid[MAX_ID_SIZE];

struct record trackconst[MAX_NUM_SIZE];

typedef struct lex {
	int numberTable;
	int numberInTable;
};

struct lex curr_lex;//текущая лексема

int lexindex = 0;

void error(char* error) {
	printf("%s\n", error);
	system("PAUSE");
	exit(0);
}

void pushallTable(int number, int k) {
	allTable[f][0] = number;
	allTable[f][1] = k;
	f++;
}
void printTableLex() {
	for (int i = 0; i < f; i++) {
		printf("<%d,%d>\n", allTable[i][0], allTable[i][1]);
	}
}
// первая таблица
char TW_[][WORD_SIZE_TW] = { "Program","begin","var","read","write",
"integer","real","for","do","to","repeat","until","and","or",
"while","end","div","end." };
// вторая таблица
char TD_[][WORD_SIZE_TD] = { "+", "-" ,"*","=",":=",".",";",">","<","!=",
">=","<=",":",","," ", "(" , ")" ,"{","}" };

typedef struct tabl {
	char* table;
	int size;
	int word_size;
}tabl;

tabl TW;
tabl TD;
tabl TID;
tabl TNUMBER;

char** TID_, ** TNUMBER_;
void tabl_init() {

	TW.table = (char*)TW_;
	TW.size = SIZE_OF_TW_TABLE;
	TW.word_size = WORD_SIZE_TW;

	TD.table = (char*)TD_;
	TD.size = SIZE_OF_TD_TABLE;
	TD.word_size = WORD_SIZE_TD;


	TNUMBER_ = (char**)malloc(sizeof(char*) * SIZE_OF_TNUM_TABLE);
	for (int i = 0; i < SIZE_OF_TNUM_TABLE; i++) {
		TNUMBER_[i] = (char*)malloc(sizeof(char) * MAX_NUM_SIZE);
	}


	TNUMBER.table = (char*)TNUMBER_;
	TNUMBER.size = 0;
	TNUMBER.word_size = MAX_NUM_SIZE;


	TID_ = (char**)malloc(sizeof(char*) * SIZE_OF_TID_TABLE);
	for (int i = 0; i < SIZE_OF_TID_TABLE; i++) {
		TID_[i] = (char*)malloc(sizeof(char) * MAX_ID_SIZE);
	}
	TID.table = (char*)TID_;
	TID.size = 0;
	TID.word_size = MAX_ID_SIZE;

}



void printTable(tabl t) {
	for (int i = 0; i < t.size; i++) {
		printf("%d.%s\n", i + 1, t.table + i * t.word_size);
	}
}


int Look(tabl t) {
	for (int i = 0; i < t.size; i++) {
		if (strcmp(buf, t.table + i * t.word_size) == 0) {
			return i;
		}
	}

	return -1;
}

int Putl(tabl* t) {
	int index = t->size++;
	strcpy(t->table + index * t->word_size, buf);
	return t->size - 1;
}


void Clear()
{
	for (int i = 0; i < jBuf; i++)
		buf[i] = '\0';
	jBuf = 0;
}

void Add()
{
	buf[jBuf] = c;// перевод переменной с из инт в чар
	jBuf++;
}


void Makelex(int k, int i)//вывод лексем 
{
	pushallTable(k, i);
}

//функция "выдающая" лексему
struct lex getlex() {
	curr_lex.numberTable = allTable[lexindex][0];
	curr_lex.numberInTable = allTable[lexindex][1];
	lexindex++;
	return curr_lex;
}

char* find() //смотрит номер таблицы и возвращает нужную строку
{
	switch (curr_lex.numberTable) {
	case 1:
		return TW.table + curr_lex.numberInTable * TW.word_size;
		break;
	case 2:
		return TD.table + curr_lex.numberInTable * TD.word_size;
		break;
	case 3:
		return TID.table + curr_lex.numberInTable * TID.word_size;
		break;
	case 4:
		return TNUMBER.table + curr_lex.numberInTable * TNUMBER.word_size;
		break;
	}
}

int id(void) {
	if (curr_lex.numberTable == 3) return 1;
	else return 0;
}

int num(void) {
	if (curr_lex.numberTable == 4) return 1;
	else return 0;
}

int eq(char* s) {
	return !strcmp(find(), s);
}


void scan(void)
{
	enum state
	{
		H, ID, NUM, COM, ASS, DLM, REAL, ER, FIN
	};
	enum state TC;
	FILE* fp;
	TC = H;
	fp = fopen("prog.txt", "r");
	c = fgetc(fp);
	do
	{
		switch (TC)
		{
		case H:
			checkreal = 0;
			if (c == '\n' || c == ' ') {
				c = fgetc(fp);
			}
			else if (isalpha(c))
			{
				Clear();
				Add();
				c = fgetc(fp);
				TC = ID;
			}
			else
				if (isdigit(c))
				{
					Clear();
					Add();
					c = fgetc(fp);
					TC = NUM;
				}
				else
					if (c == '{')
					{
						//Makelex(2, 17);
						c = fgetc(fp);
						TC = COM;
					}
					else
						if (c == ':')
						{
							c = fgetc(fp);
							TC = ASS;
						}
						else
							if (c == '<')
							{
								c = fgetc(fp);
								if (c == '=') {
									Makelex(2, 11);
									c = fgetc(fp);
									TC = H;
								}
								else {
									Makelex(2, 8);
									TC = H;
								}
							}
							else
								if (c == '>')
								{
									c = fgetc(fp);
									if (c == '=') {
										Makelex(2, 10);
										c = fgetc(fp);
										TC = H;
									}
									else {
										Makelex(2, 7);
										TC = H;
									}
								}
								else
									if (c == '!')
									{
										c = fgetc(fp);
										if (c == '=') {
											Makelex(2, 9);
											c = fgetc(fp);
											TC = H;
										}
										else {
											TC = ER;
											error("Неправильная лексема(в грамматике нет '!')");

										}
									}

									else
										if (c == '.' && Look(TW) == 15)
										{
											Makelex(2, 5);
											TC = FIN;
										}
										else if (c == '.') {
											TC = ER;
											error("Точка в неположенном месте");
										}
										else TC = DLM;
			break;
		case ID:// иидентификатор

			if (isalpha(c) || isdigit(c))
			{
				Add();
				c = fgetc(fp);
			}
			else
			{
				j = Look(TW);
				if (j != -1)
					Makelex(1, j);
				else
				{
					j = Look(TID);
					if (j == -1) {
						j = Putl(&TID);
						Makelex(3, j);
					}
					else
						Makelex(3, j);
				}

				TC = H;
			}

			break;
			///////////////////////////////////////////////////////////////////////
		case NUM://число ли это
			if (isdigit(c))
			{
				Add();
				c = fgetc(fp);
			}
			else if (isalpha(c))
			{
				//
				TC = ER;
				error("Неправильная лексема(после цифр идут буквы)");

				//
			}
			else if (c == '.') {
				Add();
				c = fgetc(fp);
				TC = REAL;
			}
			else
			{
				j = Look(TNUMBER);
				if (j != -1)
					Makelex(4, j);
				else {
					j = Putl(&TNUMBER);
					Makelex(4, j);
				}

				TC = H;
			}
			break;
		case REAL:
			if (isdigit(c)) {
				Add();
				c = fgetc(fp);
				checkreal++;
			}
			else if (c == '.' || isalpha(c)) {
				TC = ER;
				error("Неправильный риал");
			}
			else if (c == ';' && checkreal == 0) {
				TC = ER;
				error("Неправильный риал");
			}
			else
			{
				j = Look(TNUMBER);
				if (j != -1)
					Makelex(4, j);
				else {
					j = Putl(&TNUMBER);
					Makelex(4, j);
				}

				TC = H;
			}
			break;
			///////////////////////////////////////////////////////
		case COM:// проверка на закрытие фигурной скобки

			if (c == '}') {
				c = fgetc(fp);
				TC = H;
			}
			else
				c = fgetc(fp);
			break;

		case ASS:// проверка на присваивание 

			if (c == '=')
			{

				c = fgetc(fp);
				Makelex(2, 4);
				TC = H;
			}
			else
			{

				Makelex(2, 12);
				TC = H;
			}


			break;

		case DLM:// проверка на разделитель

			Clear();
			Add();
			j = Look(TD);
			if (j != -1)
			{

				c = fgetc(fp);
				Makelex(2, j);
				TC = H;
			}
			else {
				TC = ER;
				error("Неправильная лексема(разделитель)");
			}


		case ER:

			break;

		case FIN:

			break;

		}
	} while (TC != FIN && TC != ER);



	if (TC == ER) printf("Лексическая ошибка!!!\n");
	else  if (TC == FIN)
	{
		check = 1;
		printf("O.K.!!!\n");
	}
}



int prog()
{
	getlex();
	if (eq("Program"))
		prog_name();
	else
		error("Отсутствует Program");
	getlex();
	if (eq("var"))
		dec_list();
	else
		error("После название программы не  var");
	if (eq("begin"))
	{
		getlex();
		stmt_list();
	}
	else if (!eq("begin")) {
		error("Ошибка в определении переменных");
	}
	getlex();
	if (!eq("."))
		error("Последняя лексема принятая синтаксическим анализатором не является выводимой из stmt_list");
	else printf("Всё класс, синтаксис агонь!\n");
}



int prog_name() {
	getlex();
	if (curr_lex.numberTable != 3)
		error("отсутствует название программы");
}

int dec_list() {
	dec();
	if (!eq(";")) {
		error("Отсутствует ';'");
	}
	getlex();
	if (curr_lex.numberTable == 3)
		dec_list();
	else return;
}

int dec()
{
	id_list();
	if (eq(":"))
		type();
	else error("Отсутствует ':' перед типом");
	return;
}

int id_list() {
	getlex();
	if (curr_lex.numberTable == 3 || eq(",")) {
		id_list();
	}
	else return;
}

int type() {
	getlex();
	if (eq("real") || eq("integer")) {
		getlex();
		return;
	}
	else error("Недопустимый тип переменной");
}


int stmt_list() {
	stmt();
	if (!eq(";"))
		error("Отсутствует ';' или лексема невыводима из stmt_list");
	getlex();
	if (curr_lex.numberTable == 3 || eq("read") || eq("write") || eq("for") || eq("while"))
		stmt_list();
	else return;
}

int stmt() {
	if (eq("read"))
		read();
	else if (eq("write"))
		write();
	else if (eq("for"))
		for_();
	else if (eq("while"))
		while_();
	else if (curr_lex.numberTable == 3)
		assign_();
	return;
}

int read() {
	getlex();
	if (!eq("(")) error("Отсутствует '('");
	id_list();
	if (!eq(")"))
		error("Отсутствует ')'");
	getlex();
	return;

}


int write() {
	getlex();
	if (!eq("(")) error("Отсутствует '('");
	id_list();
	if (!eq(")"))
		error("Отсутствует ')'");
	getlex();
	return;

}


int assign_() {

	getlex();
	if (!eq(":="))
		error("Отсутсвует ':='");
	getlex();
	exp();
	return;
}

int flag = 0;

int exp() {
	if (curr_lex.numberTable == 3 || curr_lex.numberTable == 4) {
		getlex();
		if (eq("+") || eq("-") || eq("*") || eq("div")) {
			getlex();
			exp();
		}
		else if (eq("(")) {
			count++;
			getlex();
			exp();
		}
		else if (eq(")")) {
			count--;
			getlex();
			term();
		}
		else if ((eq(";") || eq("to") || eq("do") || eq("<=") || eq("=") || ("!=") || eq(">=") || eq(">") || eq("<")) && count == 0)
			return;
	}
	if (eq("(")) {
		count++;
		getlex();
		exp();
	}
	if (eq(")")) {
		count--;
		getlex();
		term();
	}
	else if ((eq(";") || eq("to") || eq("do") || eq("<=") || eq("=") || ("!=") || eq(">=") || eq(">") || eq("<")) && count == 0)
		return;
	else error("Неравильное выражение!");
}

int term() {
	if (count < 0)
		error(1);
	else
		if (eq(")")) {
			count--;
			getlex();
			term();
		}
		else
			if (eq("+") || eq("-") || eq("*") || eq("div")) {
				getlex();
				exp();
			}
			else
				if ((eq(";") || eq("to") || eq("do")) && count == 0) {
					return;
				}
				else error(" Не та лексема после закрывающейся скобки !");

}

int for_() {
	index_exp();
	if (!eq("do"))
		error("Отсутсвует do в цикле for!");
	getlex();
	body();

}

int index_exp() {
	getlex();
	if (curr_lex.numberTable != 3)
		error(1);
	assign_();
	if (!eq("to"))
		error("После выражения находится не to в цикле for");
	getlex();
	exp();

	return;
}

int body() {
	if (eq("begin")) {
		getlex();
		stmt_list();
		if (eq("end")) {
			getlex();
			return;
		}
		else if (curr_lex.numberTable == 4) {
			error("Неположенное место константы!");
		}
		else if (curr_lex.numberTable == 1) {
			error("Неположенное место для служебного слова!");
		}
		else if (curr_lex.numberTable == 2) {
			error("Неположенное место для разделителя!");
		}
	}
	else if (curr_lex.numberTable == 3 || eq("read") || eq("write") || eq("for") || eq("while")) {
		stmt();
		if (!eq(";")) {
			error(1);
		}
		else return;
	}
	else error("Неправильное тело цикла");
	return;
}

int while_() {
	requirement();
	if (!eq("do"))
		error(" Отсутствует do в цикле while! ");
	getlex();
	body();
	return;
}

int requirement() {
	operation();
	if (eq("and") | eq("or")) {
		requirement();
	}
	else return;
}

int operation() {
	getlex();
	exp();
	if (eq("<=") || eq("=") || ("!=") || eq(">=") || eq(">") || eq("<")) {
		getlex();
		exp();
	}
	else return;
}
////////////////////////////////////////////////////////////////////////
//проверка на то , встречается ли неописанный идент в программе
int pointvar;
int pointbegin;
int flagvar = 0;
int flagbegin = 0;
int idtable[MAX_ID_SIZE][2];
int idandtypetable[MAX_ID_SIZE][2];// таблица для заполнения типов и идентификаторов 
int consttable[SIZE_OF_TNUM_TABLE][MAX_NUM_SIZE];
int consttablenew[SIZE_OF_TNUM_TABLE][MAX_NUM_SIZE];
int consttableindex = 0;
int idtableindex = 0;
int idandtypetableindex = 0;
int flagpovtor = 0;
int	maxidintable;
int notdescribed() {
	for (int i = 0; i < f; i++) {
		if (allTable[i][0] == 1 && allTable[i][1] == 2 && flagvar == 0) {
			pointvar = i;
			flagvar = 1;
		}
		if (allTable[i][0] == 1 && allTable[i][1] == 1 && flagbegin == 0) {
			pointbegin = i;
			flagbegin = 1;
		}

	}
	//заполняю таблицу с ид и типами
	for (int i = pointvar; i < pointbegin; i++) {
		if (allTable[i][0] == 3 || (allTable[i][0] == 1 && allTable[i][1] == 5) || (allTable[i][0] == 1 && allTable[i][1] == 6)) {
			idandtypetable[idandtypetableindex][0] = allTable[i][0];
			idandtypetable[idandtypetableindex][1] = allTable[i][1];
			idandtypetableindex++;
		}
	}

	//заполняю таблицу с описанныымми идентификаторами
	for (int i = pointvar; i < pointbegin; i++) {
		if (allTable[i][0] == 3) {
			idtable[idtableindex][0] = allTable[i][0];
			idtable[idtableindex][1] = allTable[i][1];
			idtableindex++;
		}
	}

	//проверяю есть ли повторно описанные идентификаторы
	for (int i = 0; i < idtableindex - 1; i++) {
		for (int j = i + 1; j < idtableindex; j++) {
			if (idtable[i][1] == idtable[j][1])
				flagpovtor = 1;

		}
	}

	//нахожу индекс последней описанной переменной 
	for (int i = 0; i < idtableindex - 1; i++) {
		maxidintable = idtable[0][1];
		if (idtable[i][1] < idtable[i + 1][1]) {
			maxidintable = idtable[i + 1][1];
		}
	}



	if (flagpovtor == 1) {
		error("ОШИБКА!Переменная описывается повторно\n");
	}

	//если есть индекс больше найденного значит есть неоп.пер.
	for (int i = 0; i < f; i++) {
		if (allTable[i][0] == 3 && allTable[i][1] > maxidintable)
			error("ОШИБКА!Есть неописанные переменные\n");
	}

	printtypeandidtable();






	return 1;
}


void printtypeandidtable() {
	for (int i = 0; i < idandtypetableindex; i++) {
		printf("<%d,%d>\n", idandtypetable[i][0], idandtypetable[i][1]);
	}
}


void ipush(int i) {
	decStack[decStackPointer] = i;
	decStackPointer++;
}

int ipop() {
	decStackPointer--;
	return decStack[decStackPointer];
}

void typeid() {
	for (int i = 0; i < idandtypetableindex; i++) {
		trackid[i].indextable = idandtypetable[i][0];
		trackid[i].indexintable = idandtypetable[i][1];
	}

	for (int i = 0; i < idandtypetableindex; i++) {
		if (trackid[i].indextable == 3) {
			ipush(i);
		}
		else if (trackid[i].indexintable == 5 && trackid[i].indextable == 1) {

			{
				while (decStackPointer != 0) {
					strcpy(trackid[ipop()].type, "integer");
				}
			}
		}
		else if (trackid[i].indexintable == 6 && trackid[i].indextable == 1) {
			{
				while (decStackPointer != 0) {
					strcpy(trackid[ipop()].type, "real");
				}
			}
		}


	}

	for (int i = 0; i < idandtypetableindex; i++) {
		printf("%d %d %s\n", trackid[i].indextable, trackid[i].indexintable, trackid[i].type);
	}
}
int tmp;

void consttype() {

	for (int i = 0; i < f; i++) {
		if (allTable[i][0] == 4) {
			consttable[consttableindex][0] = allTable[i][0];
			consttable[consttableindex][1] = allTable[i][1];
			consttableindex++;
		}
	}
	for (int i = 0; i < consttableindex; i++) {
		trackconst[i].indextable = consttable[i][0];
		trackconst[i].indexintable = consttable[i][1];
	}

	for (int i = 0; i < consttableindex; i++) {
		printf("%d %d %s\n", trackconst[i].indextable, trackconst[i].indexintable, trackconst[i].type);
	}

}



void main() {

	setlocale(LC_ALL, "Rus");

	tabl_init();
	scan();
	printTableLex();
	printf("ident:\n");
	printTable(TID);
	printf("const\n");
	printTable(TNUMBER);
	if (check == 1) {
		prog();
	}
	else printf("Невозможно начать синтаксический анализ из-за лексической ошибки!!!\n");
	if (notdescribed()) {
		printf("Повторно описанных и не описанных переменных нет!\n");
	}

	typeid();

	printf("\n");
	printf("\\\\\\\\\\\n");
	//consttype();


	getch();
	getch();

}