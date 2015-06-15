#include "stdafx.h"

class Command{
protected:
	static int label_attuale;
	Command(){};
public:
	static int parentesi;
	virtual ~Command()=0;
	virtual ostream& stampa(ostream& os)=0;
	virtual bool Translate(ofstream&,TabellaSimboli&,Tavola_funzioni&)=0;
};

class ExpressionCommand: public Command{
private:
	Albero_espressione* exp;
public:
	ExpressionCommand(TabellaSimboli& , TokenList::iterator&,TokenList::const_iterator);
	ostream& stampa(ostream& os){
		os << "Eseguo l'espressione: ";
		os << (*exp) << endl;
		return os;
	}
	virtual bool Translate(ofstream&,TabellaSimboli&, Tavola_funzioni&);
	~ExpressionCommand();
};

class Assignment : public Command{
private:
	Identifier* Var;
	Albero_espressione* exp;
public:
	Assignment(TabellaSimboli&,TokenList::iterator&,TokenList::const_iterator);
	ostream& stampa(ostream& os){
		os << "Assegnamento alla variabile " << Var->name;
		os << " l'espressione " << (*exp) << endl;
		return os;
	}
	virtual bool Translate(ofstream&,TabellaSimboli&, Tavola_funzioni&);
	~Assignment();
};

class IFCommand: public Command{
private:
	Albero_espressione* exp;
	Command* ActionTrue;
	Command* ActionFalse;
public:
	IFCommand(TabellaSimboli&,TokenList::iterator&,TokenList::const_iterator);
	ostream& stampa(ostream& os){
		os << "IF (" << (*exp);
		os << ")" << endl;
		os << " \t ";
		(ActionTrue)->stampa(os); 
		os << endl;
		os << "ELSE" << endl;
		os << " \t ";
		(ActionFalse)->stampa(os);
		os << endl;
		return os;
	}
	virtual bool Translate(ofstream&,TabellaSimboli&, Tavola_funzioni&);
	~IFCommand();
};

class WHILECommand: public Command{
private:
	Albero_espressione* exp;
	Command* ActionVerified;
public:
	WHILECommand(TabellaSimboli&,TokenList::iterator&,TokenList::const_iterator);
	ostream& stampa(ostream& os){
		os << "While (" << (*exp) << ")" << endl;
		os << "\t";
		(ActionVerified)->stampa(os); 
		os << endl;
		return os;
	}
	virtual bool Translate(ofstream&,TabellaSimboli&, Tavola_funzioni&);
	~WHILECommand();
};

class RETURNCommand: public Command{
private:
	Albero_espressione* exp;
public:
	RETURNCommand(TabellaSimboli&,TokenList::iterator&,TokenList::const_iterator);
	ostream& stampa(ostream& os){
		os << "RETURN " << (*exp) << endl;
		return os;
	}
	virtual bool Translate(ofstream&,TabellaSimboli&, Tavola_funzioni&);
	~RETURNCommand();
};

class BlockCommand: public Command{
private:
	list<Command*> ListOfCommands;
public:
	BlockCommand(TabellaSimboli&,TokenList::iterator&,TokenList::const_iterator);
	ostream& stampa(ostream& os){
		os << "BLOCCO { " << endl;
		for (list<Command*>::iterator i=ListOfCommands.begin(); i!=ListOfCommands.end(); i++){
			os << "\t";
			(*i)->stampa(os);
			os << endl;
		}
		os << "} FINE BLOCCO" << endl;
		return os;
	}
	virtual bool Translate(ofstream&,TabellaSimboli&, Tavola_funzioni&);
	~BlockCommand();
};

class Program {
private:
	list<Command*> cmd;
	void deleteToken(TokenList&,TokenList::iterator&);
public:
	Program(TabellaSimboli&, TokenList::iterator&, TokenList::const_iterator ,bool&);
	bool Translate(ofstream&, TabellaSimboli&, Tavola_funzioni& );
	friend ostream& operator<<(ostream&,Program&);
	~Program(void);
};
