#include "stdafx.h"

void Program::deleteToken(TokenList& tl,TokenList::iterator& bound){
	while (tl.begin() != bound) {
		tl.erase(tl.begin());
	}
}

Program::Program(TabellaSimboli& ts, TokenList::iterator& i, TokenList::const_iterator end,bool& errs){
	//	l'iteratore i è sul punto iniziale subito dopo l'ultima dichiarazione
	//il num di parentesi è 0
	errs=false;
	for (; i != end && Command::parentesi>0; i++) {
		try {
			if ((*i)->isUnaryOperator() || (*i)->isSymbol() && ((Symbol*)(*i))->name=="("){
			/*
				casi
					++a;
					(a*b);
					(<expression>);
			*/
			Command* expCmd = new ExpressionCommand(ts,i,end);
			this->cmd.push_back(expCmd);
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
							this->cmd.push_back(Asseg);
						}
						else if ((*i)->isSymbol() && ((Symbol*)(*i))->name=="(") {
							i--;
							//cout << " SONO nella creazione del comando" << endl ; 
							//cout << ((Identifier*)(*i))->name << endl; 
							Command * expr = new ExpressionCommand(ts,i,end);
							//cout << "creato l'albero 2 " << endl; 
							cmd.push_back(expr); 
						}

						else if ((*i)->isUnaryOperator() || (*i)->isBinaryOperator()){
							Command* expCParentesi = new ExpressionCommand(ts,i,end);
							this->cmd.push_back(expCParentesi);
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
						this->cmd.push_back(ifc);
						break;
						}
					case WHILE:{ //while
						Command* wcd = new WHILECommand(ts,i,end);
						this->cmd.push_back(wcd);
						break;
						}
					case RETURN:{ //return
						Command* rtr = new RETURNCommand(ts,i,end);
						this->cmd.push_back(rtr);
						break;
						}
					default:{
						throw new InvalidKeyWordException((*i)->lineNumber,((Keyword*)(*i))->name);
						}
				}
			}
			else if ((*i)->isSymbol() && ((Symbol*)(*i))->name=="{"){
				//Blocco
				Command::parentesi++;
				Command* blk= new BlockCommand(ts,i,end);
				Command::parentesi--;
				this->cmd.push_back(blk);
			}
			else if ((*i)->isSymbol() && ((Symbol*)(*i))->name=="}"){
				//fine del main o della funzione!!! parentesi --. se sto a zero esco!
				Command::parentesi--;
			}
			else{
				//non è nè una parola chiave nè un identificatore nè {, nè opUn:
				//errore di sintassi
				throw new SyntaxError((*i)->lineNumber);
			}
		}
		catch (Exception* ex){
			errs=true;
			ex->toString();
		}
	}
	Command::parentesi = 1;
}

bool Program::Translate(ofstream& out, TabellaSimboli& ts, Tavola_funzioni& tf ){
	try {
		//apre un file di testo con estensione JTB (java text bytecode)
				
		out << ";############################ CORPO FUNZIONE ################################## "<< endl;
		for (list<Command*>::iterator i=cmd.begin(); i!=cmd.end(); i++){
			//se una sola operazione di traduzione fallisce allora blocco tutto!
			(*i)->Translate(out,ts, tf);
		}
	
		out << ";############################# FINE CORPO ##################################### " << endl; 
		return true;
	}
		
	catch (Exception* ex){
		//se ho catturato un'eccezione, allora qlcs è andato storto!!
		//gestisco e torno false
		ex->toString();
		return false;
	}
}


Program::~Program(void){
	cmd.clear();
}

	
ostream& operator<<(ostream& os,Program& pg){
	os << endl;
	os << "************************ Albero Sintattico **********************" << endl;
	os << "main(){ " << endl << "\t //DICHIARAZIONI" << endl;
	for (list<Command*>::iterator i=pg.cmd.begin(); i!=pg.cmd.end(); i++){
		os << '\t';
		(*i)->stampa(os);
	}
	os << "}" << endl;
	return os;
}

