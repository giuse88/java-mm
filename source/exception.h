#include "stdafx.h"

class Exception{
protected:
	int line;
	string Description;
	Exception(int l){
		line=l;
	}
	void printLog(){
		//viene salvato il log della compilazione su un file
		//così' lo recupero dalla GUI e ho la lista errori
		cout << "Sto per fare la printlog" << endl;
		fstream log_writer;
		log_writer.open("compile.log",ios::app);
		log_writer << "Line >>" << line << ": " << Description << endl;
		log_writer.close();
		cout << "Fine Printlog" << endl;
	}
public:
	virtual void toString(void)=0;
};

class SyntaxError: public Exception{
public:
	SyntaxError(int l):Exception(l) {
		Description =  ": Errore di Sintassi\nOgni riga deve essere o un comando chiave o un assegnamento";
		printLog();
	}
	void toString(void){
		cerr << "Line >> " << line << Description << endl;
	}
};

class DoubleDeclarationException: public Exception{
	string nome;
public:
	DoubleDeclarationException(int l, string stringa): Exception(l), nome(stringa) {
		Description = " : Variabile " + nome + " gia' dichiarata.";
		printLog();
	}
	void toString(void){
		cerr << "Line >> " << line << Description << endl;
	}
};

class NoIdentifierException: public Exception{
public:
	NoIdentifierException(int l):Exception(l){
		Description =  "Dichiarazione / Uso errato della variabile\nAssente identificatore";
		printLog();
	}
	void toString(void){
		cerr << "Line >> " << line << " : " << Description << endl;
	}
};

class NoCommaException: public Exception{
public:
	NoCommaException(int l):Exception(l){
		Description = "Previsto ; dopo identificatore. ";
		printLog();
	}
	void toString(void){
		cerr << "Line >> " << line << " : " << Description << endl;
	}
};

class InvalidKeyWordException:public Exception{
private:
	string kw;
public:
	InvalidKeyWordException(int l,KeywordName k):Exception(l){
		switch (k){
			case BREAK:
				kw="break";
				break;
			case CASE:
				kw="case";
				break;
			case CONTINUE:
				kw="continue";
				break;
			case ELSE:
				kw="else";
				break;
			case IF:
				kw="if";
				break;
			case RETURN:
				kw="return";
				break;
			case SWITCH:
				kw="switch";
				break;
			case VOID:
				kw="void";
				break;
			case WHILE:
				kw="while";
				break;
			default:
				kw="<>";
		}
		Description = "Parola chiave " + kw + " non valida in questo contesto";
		printLog();
	}
	void toString(void){
		cerr << "Line >> " << line << Description << endl;
	}
};

class GapException:public Exception{
private:
	char quale_parentesi;
public:
	GapException(int l, char c='*'):Exception(l){
		this->quale_parentesi=c;
		if (this->quale_parentesi=='('){
			Description = "Errore. Manca una parentesi. Si aspetta una (" ;
		}
		else if (this->quale_parentesi==')'){
			Description = "Errore. Manca una parentesi. Si aspetta una )" ;
		}
		else{
			Description = "Errore. Manca una parentesi. Impossibile stabilire quale parentesi manca." ;
		}
		printLog();
	}
	void toString(void){
		cerr << "Line >> " << line << Description << endl;
	}
};

class InvalidExpressionException:public Exception{
public:
	InvalidExpressionException(int l,string InvalidChar=""):Exception(l){
		Description = ": Espressione inserita non valida. "+InvalidChar;
		printLog();
	}
	void toString(void){
		cerr << "Line >> " << line << Description << endl;
	}
};

class NotMainException:public Exception{
private:
	string C;
public:
	NotMainException(string c):Exception(1){
		C=c;
		Description = "Il programma J-- deve obbligatoriamente iniziare con\n\tmain(){\ne finire con:\n\t}\nCarattere non trovato: " + C;
		printLog();
	}
	void toString(void){
		cerr << Description << endl;
	}
};

class UndeclaredIdentifierException:public Exception{
private:
	string Id;
public:
	UndeclaredIdentifierException(int l,string c):Exception(l){
		Id=c;
		Description = " Identificatore non dichiarato\nIdentificatore " + Id + " non trovato nella Tabella dei Simboli";
		printLog();
	}
	void toString(void){
		cerr << "Line >> " << line << Description << endl;
	}
};

class UnexpectedEndOfProgramException:public Exception{
private:
	string CharNotFound;
public:
	UnexpectedEndOfProgramException(int l, string s):Exception(l){
		CharNotFound=s;
		Description = "Raggiunta inaspettatamente la fine del programma.\nNon si e' trovato alcun " + CharNotFound + "\nLa compilazione verrà interrotta.";
		printLog();
	}
	void toString(void){
		cerr << "Line >> " << line << " : " << Description << endl;
		exit(1);
	}
};

class NoElseException: public Exception{
public:
	NoElseException(int l):Exception(l){
		Description = ": 'else' mancante.\nUn blocco IF è composto da: \n\t if (<expression>)\n\t\t <command>\n\t else \n\t<command>";
		printLog();
	}
	void toString(void){
		cerr << "Line >> " << line << Description << endl;
	}
};

class UnknownObjectException: public Exception{
public:
	UnknownObjectException(int l):Exception(l){
		Description = ": oggetto non riconosciuto. Eccezione nel calcolo dell'espressione.";
		printLog();
	}
	void toString(void){
		cerr << "Line >> " << line << Description << endl;
	}
};

class TypeMismatchException: public Exception{
public:
	TypeMismatchException(int l=-1):Exception(l){
		Description = ": datatypename non riconosciuto. Mi aspetto INT/DOUBLE/FLOAT/LONG";
		printLog();
	}
	void toString(void){
		cerr << "Line >> " << line << Description << endl;
	}
};

class VoidFileException: public Exception{
public:
	VoidFileException(int l=0):Exception(l){
		Description = "Impossibile compilare un file apparentemente vuoto!";
		printLog();
	}
	void toString(void){
		cerr << "Line >> " << line << Description << endl;
	}
};
