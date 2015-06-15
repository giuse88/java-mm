#include "stdafx.h"


int Command::label_attuale=0;
int Command::parentesi=1;
static int IFL=0;
static int WHL=0;

Command::~Command(){}; //virtuale puro!!

Assignment::~Assignment(){
	delete Var;
	delete exp;
}

IFCommand::~IFCommand(){
	delete exp;
	delete ActionTrue;
	delete ActionFalse;
}

WHILECommand::~WHILECommand(){
	delete exp;
	delete ActionVerified;
}

RETURNCommand::~RETURNCommand(){
	delete exp;
}

BlockCommand::~BlockCommand(){
	this->ListOfCommands.clear();
}

ExpressionCommand::~ExpressionCommand(){
	delete exp;
}


bool Command::Translate(ofstream&,TabellaSimboli&,Tavola_funzioni&){
	//virtuale puro, mai chiamato!!
	return true;
}

bool Assignment::Translate(ofstream& fout, TabellaSimboli& ts, Tavola_funzioni& tf){
	/*
		sarà del tipo 
		fout << "ldc store " << exp->evaluate() << endl;
	*/
	Identificatore* myVar= (Identificatore*)(ts[this->Var->name]);
   
	//valuta l'espressione. nello stack ho il RISULTATO!!
	if (!exp->traduci(ts,tf,fout)) {
		//eccezione se fallisco la evaluate()
		throw new InvalidExpressionException(myVar->get_linea_dichiarazione());
	}
	//l'operazione cambia a seconda del tipo della variabile	
	switch (myVar->get_tipo()) {
		case INT:{
			//la traducni mi rende un valore ch pèuò anke essere double!!
			/*
				X2i --> X=getExpressionType(exp);
			*/
			if (TIPO_GENERALE==DOUBLE){
                 		fout << "d2i" << endl;
            		}                                    
            		else if (TIPO_GENERALE==FLOAT){
                 		fout << "f2i" << endl;
            		}
            		else if (TIPO_GENERALE==LONG){
                 		fout << "l2i" << endl;
            		}
            		fout << "istore " << myVar->get_registro();
			break;
		}
		case DOUBLE:{
			//la traducni mi rende un valore ch pèuò anke essere double!!
			/*
				X2d --> X=getExpressionType(exp);
			*/
            		if (TIPO_GENERALE==INT){
                	 	fout << "i2d" << endl;
            		}                                    
            		else if (TIPO_GENERALE==FLOAT){
						fout << "f2d" << endl;
            		}
            		else if (TIPO_GENERALE==LONG){
						fout << "l2d" << endl;
            		}
            		fout << "dstore " << myVar->get_registro();
			break;
		}
		case FLOAT:{
			//la traducni mi rende un valore ch pèuò anke essere double!!
			/*
				X2i --> X=getExpressionType(exp);
			*/
			if (TIPO_GENERALE==DOUBLE){
                 		fout << "d2f" << endl;
            		}                                    
            		else if (TIPO_GENERALE==INT){
                 		fout << "i2f" << endl;
            		}
            		else if (TIPO_GENERALE==LONG){
                 		fout << "l2f" << endl;
            		}
            		fout << "fstore " << myVar->get_registro();
			break;
		}
		case LONG:{
			/*
				X2i --> X=getExpressionType(exp);
			*/
			//fout << "; carico un long" << endl;

			if (TIPO_GENERALE==DOUBLE){
                 		fout << "d2l" << endl;
            		}                                    
            		else if (TIPO_GENERALE==FLOAT){
                 		fout << "f2l" << endl;
            		}		
            		else if (TIPO_GENERALE==INT){
                 		fout << "i2l" << endl;
            		}
			//fout << "; carico un long" << endl;
            		fout << "lstore " << myVar->get_registro();
			break;
		default:
			throw new TypeMismatchException(myVar->get_linea_dichiarazione());
		}
	}
	fout << endl;
	return true;
}

bool IFCommand::Translate(ofstream& fout, TabellaSimboli& ts, Tavola_funzioni& tf){
	/*
		sarà del tipo
		fout << CONDIZIONE Si/No => CONTINUA/GOTO Label 1 /... Goto Label 2/ Label 1: /.../ Label 2:
	*/
	if (exp->traduci(ts,tf,fout)==false){
		throw new InvalidExpressionException(-1);
	}
	//la traducni mi rende un valore ch pèuò anke essere double!!
	/*
	X2i --> X=getExpressionType(exp);
	*/
	//
	if (TIPO_GENERALE==DOUBLE){
        	fout << "d2i" << endl;
        }                                    
        else if (TIPO_GENERALE==FLOAT){
        	fout << "f2i" << endl;
        }
        else if (TIPO_GENERALE==LONG){
         	fout << "l2i" << endl;
        }
	fout << "ifeq IFL" << IFL << endl;
	IFL++;
	//se uguale a zero=>salta a lbl altrimenti fai questo:
	ActionTrue->Translate(fout,ts,tf);
	fout << "goto IFL" << IFL << endl;
	fout << "IFL" << IFL - 1 << ":" << endl;
	ActionFalse->Translate(fout,ts,tf);
	fout << "IFL" << IFL << ":" << endl;	
	//rende disponibile la prox label
	label_attuale++;
	return true;
}

bool WHILECommand::Translate(ofstream& fout, TabellaSimboli& ts,Tavola_funzioni& tf){
	/*
		sarà del tipo
		fout << Label 1: CONDIZIONE Si/No => CONTINUA/GOTO Label 2 /.../ Label 2:
	*/
	fout << "WHL" << WHL << ":" << endl;
	if (exp->traduci(ts,tf,fout)==false){
		throw new InvalidExpressionException(-1);
	}
	//la traducni mi rende un valore ch pèuò anke essere double!!
	/*
	X2i --> X=getExpressionType(exp);
	*/
	//
	if (TIPO_GENERALE==DOUBLE){
        	fout << "d2i" << endl;
        }                                    
        else if (TIPO_GENERALE==FLOAT){
        	fout << "f2i" << endl;
        }
        else if (TIPO_GENERALE==LONG){
         	fout << "l2i" << endl;
        }
	WHL++;
	fout << "ifeq WHL" << WHL << endl;
	ActionVerified->Translate(fout,ts,tf);
	fout << "goto WHL" << WHL-1 << endl;
	fout << "WHL" << WHL << ":" << endl;	
	return true;
}

bool RETURNCommand::Translate(ofstream& fout, TabellaSimboli& ts,Tavola_funzioni& tf){
	/*
		sarà del tipo
		fout << ireturn exp->evaluate();
	*/
	if (exp->traduci(ts,tf,fout)==false){
		throw new InvalidExpressionException(-1);
	}
	//nello stack ho il risultato dell'espressione
	//la traducni mi rende un valore ch pèuò anke essere double!!
	/*
	X2i --> X=getExpressionType(exp);*/
	if (TIPO_GENERALE==DOUBLE){
      		 fout << "d2i" << endl;
    	}                                    
    	else if (TIPO_GENERALE==FLOAT){
       		fout << "f2i" << endl;
    	}
    	else if (TIPO_GENERALE==LONG){
       		fout << "l2i" << endl;
    	}
    
	fout << "ireturn" << endl;
	return true;
}

bool BlockCommand::Translate(ofstream& fout, TabellaSimboli& ts,Tavola_funzioni& tf){
	/*
		nn si fanno traduzioni nel senso che traduce comando per comando		
	*/
	for (list<Command*>::iterator i=ListOfCommands.begin(); i!=ListOfCommands.end(); i++){
		if ((*i)->Translate(fout,ts,tf)==false){
			return false;
		}
	}
	return true;
}

bool ExpressionCommand::Translate(ofstream & fout, TabellaSimboli & ts,Tavola_funzioni& tf) {
	return exp->traduci(ts,tf,fout);
}

ExpressionCommand::ExpressionCommand(TabellaSimboli& ts, TokenList::iterator& i, TokenList::const_iterator end) {
	this->exp=new Albero_espressione(ts ,i,end);
}

Assignment::Assignment(TabellaSimboli& ts, TokenList::iterator& i, TokenList::const_iterator end){
	//il primo è l'identificatore: se nn è stato dichiarato lancio eccezione
	string nm = ((Identifier*)(*i))->name;
	if (!ts.present(nm)){
		//c'è un errore: avanzo i fino al ; e lancio l'eccezione. Tutta la riga verrà ignorata
		int line=(*i)->lineNumber;
		gotoNextLine(i,end);
		throw new UndeclaredIdentifierException(line,nm);
	}
	//l'identificatore è presente, quindi lo metto nella classe
	Var = new Identifier(nm);
	//avanzo. Devo trovare un =
	i++;
	if (i==end || !((*i)->isSymbol() && ((Symbol*)(*i))->name=="=")) {
		//se ne occupa il gotonextinterrupt a lanciare l'eccezione
		//di fine programma!! e in questo caso, non lancio la syntaxerror
		gotoNextLine(i,end);
		throw new SyntaxError((*i)->lineNumber);
	}
	//avanzo: deve esserci un'espressione terminante con ;!!
	i++;
	//se ne occupa il costruttore dell'albero a verificare se il programma finisce
	this->exp = new Albero_espressione(ts,i,end);
	exp->Error(ts);
	if (i==end || !((*i)->isSymbol() && ((Symbol*)(*i))->name==";")) {
		int ln=(*i)->lineNumber;
		gotoNextLine(i,end);
		throw new NoCommaException(ln);
	}
}



BlockCommand::BlockCommand(TabellaSimboli& ts, TokenList::iterator& i,  TokenList::const_iterator end){
	//SOno sul token {
	i++;
	//scorro fino al }
	while (i!=end && (!((*i)->isSymbol() && ((Symbol*)(*i))->name=="}")) ){
		if ((*i)->isUnaryOperator()){
			/*
				casi
					++a;
					(a*b);
					(<expression>);
			*/
			Command* expCmd = new ExpressionCommand(ts,i,end);
			this->ListOfCommands.push_back(expCmd);
		}
		else if ((*i)->isIdentifier()){
			/*se è identificatore ho 3 casi:
			1) a= ... ;
			2) a++;
			3) a
			*/
			//vado sul successivo:
			i++;
			if (i!=end){
				//ok, c'è qualcosa dopo.
				if ((*i)->isSymbol() && ((Symbol*)(*i))->name=="="){
					//ASSEGNAMENTO
					i--;
					Command* Asseg = new Assignment(ts,i,end);
					this->ListOfCommands.push_back(Asseg);
				}
				else {
					// dopo l'identificatore c'è qualcosa che non è nè = ne opUnario
					int linea_errore =(*i)->lineNumber;
					gotoNextLine(i,end);
					throw new SyntaxError(linea_errore);
				}
			}
			else {
				i--;
				throw new UnexpectedEndOfProgramException((*i)->lineNumber,"=, ++, --");
			}
		}
		else if ((*i)->isKeyword()){
			//sono nel caso dei comandi. SWITCHIAMO la keyword
			switch (((Keyword*)(*i))->name){
				case IF:{ //if
					Command* ifc = new IFCommand(ts,i,end);
					this->ListOfCommands.push_back(ifc);
					break;
					}
				case WHILE:{ //while
					Command* wcd = new WHILECommand(ts,i,end);
					this->ListOfCommands.push_back(wcd);
					break;
					}
				case RETURN:{ //return
					Command* rtr = new RETURNCommand(ts,i,end);
					this->ListOfCommands.push_back(rtr);
					break;
					}
				default:{
					throw new InvalidKeyWordException((*i)->lineNumber,((Keyword*)(*i))->name);
					}
			}
		}
		else if ((*i)->isSymbol() && ((Symbol*)(*i))->name=="{"){
			//Blocco
			Command* blk= new BlockCommand(ts,i,end);
			this->ListOfCommands.push_back(blk);
		}
		else{
			//non è nè una parola chiave nè un identificatore nè {, nè opUn:
			//errore di sintassi
			throw new SyntaxError((*i)->lineNumber);
		}
		//prox comando del blocco
		i++;
	}
	//se sono alla fine errore!
	if (i==end){
		throw new UnexpectedEndOfProgramException(0,"}");
	}
}


RETURNCommand::RETURNCommand( TabellaSimboli& ts, TokenList::iterator& i, TokenList::const_iterator end){
	//sono sul token RETURN. avanzo e ved se c'è un'espressione
	i++;
	this->exp=new Albero_espressione(ts, i,end);
	exp->Error(ts);
	//dovrei stare sul ;
	if (i==end || !((*i)->isSymbol() && ((Symbol*)(*i))->name==";")){
		throw new NoCommaException((*i)->lineNumber);
	}
}


WHILECommand::WHILECommand( TabellaSimboli& ts, TokenList::iterator& i, TokenList::const_iterator end){
	//sono sul token WHILE. c'è una parentesi??
	//con un'espressione?
	i++;
	this->exp=new Albero_espressione(ts,i,end,true);
	exp->Error(ts);
	if (i==end){
		throw new UnexpectedEndOfProgramException((*--i)->lineNumber,"<Command>");
	}
	//il prox è un comando?
	if ((*i)->isIdentifier()){
		//siamo nel caso di un assegnamento!!!
		this->ActionVerified  = new Assignment(ts,i,end);
	}
	else if ((*i)->isKeyword()){
		//siamo nei comandi standard!
		switch (((Keyword*)(*i))->name){
					case IF:{ //if
						this->ActionVerified=new IFCommand(ts,i,end);
						break;
							}
					case WHILE:{ //while
						this->ActionVerified=new WHILECommand(ts,i,end);
						break;
							   }
					case RETURN:{ //return
						this->ActionVerified=new RETURNCommand(ts,i,end);
						break;
								}
					default:{
						throw new InvalidKeyWordException((*i)->lineNumber,((Keyword*)(*i))->name);
							}
		}
	}
	else if ((*i)->isSymbol() && ((Symbol*)(*i))->name=="{"){
		//Blocco
		parentesi++;
		this->ActionVerified=new BlockCommand(ts,i,end);
		parentesi--;
	}
	else{
		//non è nè una parola chiave nè un identificatore nè {: errore di sintassi
		throw new SyntaxError((*i)->lineNumber);
	}
}


IFCommand::IFCommand(TabellaSimboli& ts, TokenList::iterator& i, TokenList::const_iterator end){
	//se sto qua, il mio iteratore si trova sul token IF
	//il prox, è una parentesi con expr?
	i++;
	this->exp=new Albero_espressione(ts ,i,end, true);
	exp->Error(ts);
	/*


	*/
	//verifico il comando1
	if (i==end){
		throw new UnexpectedEndOfProgramException((*--i)->lineNumber,"<Command>");
	}
	if ((*i)->isIdentifier()){
		//siamo nel caso di un assegnamento!!!
		this->ActionTrue  = new Assignment(ts,i,end);
	}
	else if ((*i)->isKeyword()){
		//siamo nei comandi standard!
		switch (((Keyword*)(*i))->name){
					case IF:{ //if
						this->ActionTrue=new IFCommand(ts,i,end);
						break;
							}
					case WHILE:{ //while
						this->ActionTrue=new WHILECommand(ts,i,end);
						break;
							   }
					case RETURN:{ //return
						this->ActionTrue=new RETURNCommand(ts,i,end);
						break;
								}
					default:{
						throw new InvalidKeyWordException((*i)->lineNumber,((Keyword*)(*i))->name);
							}
		}
	}
	else if ((*i)->isSymbol() && ((Symbol*)(*i))->name=="{"){
		//Blocco
		parentesi++;
		this->ActionTrue=new BlockCommand(ts,i,end);
		parentesi--;
	}
	else{
		//non è nè una parola chiave nè un identificatore nè {: errore di sintassi
		throw new SyntaxError((*i)->lineNumber);
	}
	//ok. verifico l'else
	i++;
	if(i==end || !((*i)->isKeyword() && ((Keyword*)(*i))->name==ELSE)){
		//manca l'else!! vado al prox } e lancio eccezione
		gotoNextLine(i,end);
		throw new NoElseException((*i)->lineNumber);
	}
	//ok, verifico comando2
	i++;
	if (i==end){
		throw new UnexpectedEndOfProgramException((*--i)->lineNumber,"<Command>");
	}
	if ((*i)->isIdentifier()){
		//siamo nel caso di un assegnamento!!!
		this->ActionFalse  = new Assignment(ts,i,end);
	}
	else if ((*i)->isKeyword()){
		//siamo nei comandi standard!
		switch (((Keyword*)(*i))->name){
					case IF:{ //if
						this->ActionFalse=new IFCommand(ts,i,end);
						break;
							}
					case WHILE:{ //while
						this->ActionFalse=new WHILECommand(ts,i,end);
						break;
							   }
					case RETURN:{ //return
						this->ActionFalse=new RETURNCommand(ts,i,end);
						break;
								}
					default:{
						throw new InvalidKeyWordException((*i)->lineNumber,((Keyword*)(*i))->name);
							}
		}
	}
	else if ((*i)->isSymbol() && ((Symbol*)(*i))->name=="{"){
		//Blocco
		parentesi++;
		this->ActionFalse=new BlockCommand(ts,i,end);
		parentesi--;
	}
	else{
		//non è nè una parola chiave nè un identificatore nè {: errore di sintassi
		throw new SyntaxError((*i)->lineNumber);
	}

}


