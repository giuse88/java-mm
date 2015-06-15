/***************************************************************************
 *   Copyright (C) 2008 by Pes   *
 *   root@localhost   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "stdafx.h"

static int livello = 0; 
static bool err = false; // è un flag se si sono verificati errori
int numero_parentesi = 0; 
bool Cerca_virgola; 

Albero_espressione::Albero_espressione(TabellaSimboli& ts  ,TokenList::iterator& i,TokenList::const_iterator end,  bool v) {	
	tree = NULL; 
	//numero_parentesi = 0; 
	Cerca_virgola = v; 	    
	espr_in_albero(tree, i, end, ts); 
}


Albero_espressione::~Albero_espressione(){
	del(tree); 
}


void Albero_espressione::del(nodo_ptr& root) {
	if(!root) {
		return; 
	}
	del(root->destra); 
	del(root->sinistra); 
	delete root->info; 
	delete root; 
}

void Albero_espressione::stampa () const  {
	livello = 0; 
	/*cout << "Sono nella prima stampa" << endl*/;
	stampa(tree); 	
};

void Albero_espressione::stampa(nodo_ptr root)  const {
	if (!root) {
		return; 
	}
	for ( int i = 0; i < livello; i++){
		cout << '\t';
	}
	root->info->stampa(); 
	cout << endl; 
	livello++; 
	stampa(root->sinistra); 
	stampa(root->destra); 
	livello--; 
}

void Albero_espressione::espr_in_albero(nodo_ptr& root,  TokenList::iterator& i, TokenList::const_iterator end, TabellaSimboli& ts) {
	if (i ==  end) {
		if(numero_parentesi > 0) {
			gotoNextLine(i,end);
			throw new GapException((*i)->lineNumber-1,')'); 
		}
		gotoNextLine(i,end);
		throw new NoCommaException((*i)->lineNumber-1);   // si presenta il problema quando non c'è 
	}

	else if ((*i)->isSymbol() &&( (((Symbol *)(*i))->name == ";" ||  (((Symbol *)(*i))->name == "," )))) {
		//cout << "Siamo nel punto è virgola : --> "<< ((Symbol *)(*i))->name << endl;
		return;	
	} 
	else if ((*i)->isSymbol() &&  ((Symbol *)(*i))->name == ")")	{
		//cout << "Siamo nella prentesi chiusa"<< endl; 
		numero_parentesi--; // decremento parentesi 
		/*cout << numero_parentesi << endl;*/  
		i--; 
		if ((*i)->isBinaryOperator()){
			gotoNextLine(i,end);
			throw new NoIdentifierException((*i)->lineNumber-1);
		}
		i++; 		
		//cout <<numero_parentesi << endl; 
		if (numero_parentesi < 0) {
			gotoNextLine(i,end);
			throw new InvalidExpressionException((*i)->lineNumber-1);
		}
		//cout << "sono nella parentedsi chiusa" << endl; 
		return; 
	}
	else if ((*i)->isIdentifier()) { 
		string name = ((Identifier *)(*i))->name; // mi salvo il nome della variabile 	
		  // creo la foglia 
		i++; // passo al token successivo
		nodo_ptr nuovo; 
		
		//cout << "Sono nell'identificatore :  " << name << endl; 

		if( i == end) { 
			throw new UnexpectedEndOfProgramException((*--i)->lineNumber,"<BinOp>,<Comma>");
		}

		if ( (*i)->isSymbol() &&  ((Symbol *)(*i))->name == "(")  { // caso funzioni 
			//cout << "Siamo nel caso delle funzioni " <<endl;
			numero_parentesi++;
			//cout << numero_parentesi << endl; 
			i++;
			if( i == end) { 
				throw new UnexpectedEndOfProgramException((*--i)->lineNumber,"<BinOp>,<Comma>");
			}

 			nuovo = new nodo (NULL, NULL, new Call_function(ts, i,end,name)); 

			//cout << "ss" <<endl;
			
		} 
		else  {
			if (!ts.present(name)) {
				gotoNextLine(i,end);
				throw new UndeclaredIdentifierException((*i)->lineNumber-1,name);
			}
			//cout <<" Ho identificato la varibile : " << name << " é pesente nella tabella dei simboli" << endl; 
			DataTypeName tipo = ((Identificatore *)ts[name])->get_tipo();
			///controllo sull'esistenza della variabile
			nuovo = new nodo( NULL,NULL ,new Tree_identifier(name, tipo));
		}
	
		appendi_operatore(root, nuovo, i,end, ts); //  
		
		if (root != NULL) {
			// ad aggiungere il nodo precedente, ci pensa l'appendi operatore
			espr_in_albero(root->destra, i, end, ts); 
			//cout <<"controllo quello che c'è dopo" << endl; 
		} 
		else { 
			//cout <<"Siamo nell'identificatore finale : \t"<< name << endl;
			root = nuovo;  
			return; // l'ho aggiunto ora per piu chiarezza
		}
	}
	else if ((*i)->isSymbol() && ((Symbol *)(*i))->name == "(") {
		numero_parentesi++;
		nodo_ptr espr = NULL; 
		i++; // punto al primo token all'inteno della prentesi 
		espr_in_albero(espr, i, end, ts); // salvo l'espressione delle prentesi 
		 // non si apetta il punto è virgola dopo le parentesi 
		i++;// punto al token succesiovo; 
		//esco nel caso sono in un qualcosa che è o IF o WHILE (passato da program)
		if (Cerca_virgola == true && numero_parentesi == 0)  {
			root = espr; 
			return;
		}
		appendi_operatore(root, espr, i,end, ts); 
		if(!root) {
			/*cout <<"siamo nela prentesi finale";  */
		}
		else {
			espr_in_albero(root->destra, i, end, ts); 
		}
	}
	else if ((*i)->isNumber()) {  
		// caso in cui capiti un numero  // LO POSSIAMO UNGLOBARE NELL 'ALTRO 
		//cout << "Creo il letterale" << endl; 
		nodo_ptr new_number = crea_leterale( (Number *)(*i));  // mi creo il nuovo nodo
		i++; // passo al token successivo
		appendi_operatore(root, new_number, i, end, ts); 
		if(root) {
			espr_in_albero(root->destra, i, end, ts);  //  richiamo per gestire il prosimo token  aspetandomi che sia un operatorte  
		}
		else {
			root = new_number; 
		}
	} 
	else if ((*i)->isUnaryOperator() ) {		
		bool no_more_not=false;
		string type = ((Symbol *)(*i))->name;
		/*cout << "simbolo nella trasforma in albero" << type << endl;*/
		if (type!="!"){
				no_more_not=true;
		}
		nodo_ptr nuovo = new nodo(NULL, NULL, new Operatore_unario(type));
		nodo_ptr  op  = nuovo; // riferimento a nuovo che rapresenta la root
		i++; 
		//VERIFICA SFORO
		if (i==end){
			throw new UnexpectedEndOfProgramException((*--i)->lineNumber,"<Identifier>,<UnOp>");
		}
		//bisognerebbe usare un temp 
		// se è un operatore  binario inserisci a destra 
		while ( nuovo != NULL && (*i)->isUnaryOperator() ) {
			//(*i)->toString();
			if (((Symbol*)(*i))->name !="!"){
				no_more_not=true;
			}
			if (no_more_not && ((Symbol*)(*i))->name=="!"){
				throw new InvalidExpressionException((*i)->lineNumber,"Impossibile usare un ! dopo un <UnOp>");
			}
			/*nuovo->info->stampa();*/ 
			string type = ((Symbol *)(*i))->name;
			nuovo->destra = new nodo(NULL, NULL, new Operatore_unario(type));
			i++; 
			nuovo = nuovo->destra; 
		}
		if( (*i)->isIdentifier()) {
			// è presente un errore; 
			/*cout << "l'identiificatore dopo è questo : " << (((Identifier *)(*i))->name) << endl; */
			string nome = ((Identifier *)(*i))->name;
			if (!ts.present(nome)) {
				gotoNextLine(i,end);
				throw new UndeclaredIdentifierException((*i)->lineNumber-1,nome);
			}
			DataTypeName tipo = ((Identificatore *)ts[nome])->get_tipo();
			nuovo->destra = new nodo(NULL, NULL,  new Tree_identifier(nome, tipo)); 
			// creo la foglia destra ma , puo essere anche un espressione 
			//incremento e proseguo l'analisi
		}
		else if ((*i)->isNumber()) {
			TokenList::iterator j=i;
			j--;
			if (((Symbol*)(*j))->name!="!" && ((Symbol*)(*j))->name!="+" && ((Symbol*)(*j))->name!="-"){
				//caso ++3; errore
				throw new InvalidExpressionException((*i)->lineNumber,"Impossibile usare un <UnOp> diverso da ! sui letterali");
			}
			nodo_ptr new_number = crea_leterale( (Number *)(*i));
			nuovo->destra = new_number;
		} 
		else if ((*i)->isSymbol() && ((Symbol *)(*i))->name == "(") {
			numero_parentesi++; 
			/*cout <<"sono nella prentesi aperta dell'operatore" << endl;*/
			nodo_ptr espr = NULL; 
			i++; // punto al primo elemento dentro le parerntesi 
			espr_in_albero(espr, i, end, ts); 
			nuovo->destra = espr; 
		} 
		i++;// passo il token succesivo; 
		appendi_operatore(root, op, i, end, ts); 
		if(root){
			espr_in_albero(root->destra, i, end, ts);  //  richiamo per gestire il prosimo token  aspetandomi che sia un operatorte  
		}
		else {
			root = op;
		}
		return; 
	}
	else {
		gotoNextLine(i,end);
		throw new TypeMismatchException((*i)->lineNumber);	// sarebbe l'ideale di sapre qual'è l'oggetto non riconosciuto 
	}
}

void Albero_espressione::appendi_operatore(nodo_ptr& root, nodo_ptr& temp, TokenList::iterator& i , TokenList::const_iterator end , TabellaSimboli& ts){

	//cout << "funzione appendi operatore " << endl; 
	
	if (i ==  end) {
		if(numero_parentesi > 0) {
			gotoNextLine(i,end);
			throw new GapException((*i)->lineNumber-1,')'); 
		}
		gotoNextLine(i,end);
		throw new NoCommaException((*i)->lineNumber-1);   // si presenta il problema quando non c'è 
	}
	if ((*i)->isSymbol() && (((Symbol *)(*i))->name == ";"  ||  (((Symbol *)(*i))->name == "," ))) {
		if(!root) {
			root = temp;
		} // apendo l'espressione  che c'è tra le parentesi nel caso ci sia * / o
		//else root->sinistra = temp; 
		//cout << "Siamo nel punto è virgola : --> "<< ((Symbol *)(*i))->name << endl;
		return;	
	} 
	else if ((*i)->isSymbol() &&  ((Symbol *)(*i))->name == ")")	{
		//cout << "sono nella parentesi " << endl; 
		if(!root) { 
			root = temp; 
		} // appendo l'espressione che c'è tra le parentesi nel caso ci sia * 0 /
		//else root->sinistra = temp; 
		// bisogna fare lo stesso controllo di su!
		return; 	 
	}
	if ( (*i)->isBinaryOperator()) {
		string type = ((Symbol *)(*i))->name; 
		if (  type == "*" || type  == "/") {		
			//cout << "Sono nell'operatore " << type <<endl;
			nodo_ptr nuovo = new nodo(NULL, temp, new Operatore_binario(type));  // creo l'operatore appendendo alla sua sinistra 
			i++; // guardo il prossimo token 
			if (i==end){
				throw new UnexpectedEndOfProgramException((*--i)->lineNumber,"<Identifier>,<UnOp>");
			}
			if( (*i)->isIdentifier()) {
				// è presente un errore; 
				//cout << "l'identiificatore dopo è questo : " << (((Identifier *)(*i))->name) << endl; 
				string name = (((Identifier *)(*i))->name);
				i++;
				if(i != end && (*i)->isSymbol() && ((Symbol *)(*i))->name == "(") {
					//cout << "Siamo nel caso delle funzioni " <<endl;
					numero_parentesi++;
					cout << numero_parentesi << endl; 
					i++;
					if (i==end){
						throw new UnexpectedEndOfProgramException((*--i)->lineNumber,"<Function Argument>");
					}
 					nuovo->destra = new nodo (NULL, NULL, new Call_function(ts, i,end,name)); 
				//	cout << "ss" <<endl;
					i--;
				}
				else { 
					i--; 
					DataTypeName tipo = ((Identificatore *)ts[name])->get_tipo();
					nuovo->destra = new nodo(NULL, NULL,  new Tree_identifier(name ,tipo)); 
				}// creo la foglia destra ma , puo essere anche un espressione 
				//incremento e proseguo l'analisi
			}
			else if ((*i)->isNumber()) {
				nodo_ptr new_number = crea_leterale( (Number *)(*i));
				nuovo->destra = new_number;
			}
			else if ((*i)->isSymbol() && ((Symbol *)(*i))->name == "(") {
				numero_parentesi++; 
				/*cout <<"sono nella prentesi aperta dell'operatore" << endl;*/
				nodo_ptr espr = NULL; 
				i++; // punto al primo elemento dentro le parerntesi 
				espr_in_albero(espr, i, end, ts); 
				nuovo->destra = espr; 
			}
			i++; 
			appendi_operatore(root, nuovo, i, end, ts); 
			return; 
		} 
		else  if  ( ((*i)->isBinaryOperator()) && ((Symbol *)(*i))->name == "=") {
			gotoNextLine(i,end);
			throw new InvalidExpressionException((*i)->lineNumber-1); 
     	}
		else { // tutti gli altri operatori 
			//cout << "Siamo nell ' operatore: " << type << endl; 
			root = new nodo(NULL, temp, new Operatore_binario(type)); 	
			i++;
			return; 
		}
	} 
	else if ((*i)->isUnaryOperator() ) {		
		string type = ((Symbol *)(*i))->name;
		/*cout << "simbolo nell'appendi operatore " << endl;*/ 
		nodo_ptr nuovo = new nodo(NULL, temp, new Operatore_binario(type)); 
		i++; 
		appendi_operatore(root,nuovo, i, end, ts); 
		return; 
	}  
	else {
		gotoNextLine(i,end);
		throw new UnknownObjectException((*i)->lineNumber-1); 
	}
	// la gestione dell'opeatore post fisso la devo fare qui 
	return;
}

Albero_espressione::nodo_ptr Albero_espressione::crea_leterale(Number * numero) {
	DataTypeName tipo = numero->t; // mi salvo il tipo ; 
	switch (tipo) {		
		case INT :
			 return new nodo(NULL, NULL, new Tree_literal(*((int *)numero->number), tipo));
		case DOUBLE : 
			 return new nodo(NULL, NULL, new Tree_literal(*((double *)numero->number), tipo));
		case LONG: 
			 return new nodo(NULL, NULL, new Tree_literal(*((long int *)numero->number), tipo));
		case FLOAT : 
			 return new nodo(NULL, NULL, new Tree_literal(*((float *)numero->number), tipo));
		default : 
			throw new TypeMismatchException(); 
	} 
}


void Albero_espressione::stampa_espressione(const nodo_ptr root) {
	if (!root){
		return; 
	}
	stampa_espressione (root->sinistra); 
	root->info->stampa(); 
	stampa_espressione(root->destra); 
}

void Albero_espressione::stampa_posticipata(nodo_ptr root) {
	if (!root){
		return; 
	}
	stampa_espressione (root->sinistra); 
	stampa_espressione(root->destra); 
	root->info->stampa(); 
}

void Albero_espressione::stampa_posticipata(void) {
	stampa_posticipata(tree); 
}	

void  Albero_espressione::stampa_espressione(void) {
	if (!tree){
		return ; 
	}
	cout << endl; 
	stampa_espressione(tree); 
	cout << endl << endl;
}

bool Albero_espressione::traduci( TabellaSimboli& ts,Tavola_funzioni& tf,  ofstream& out) {
	/*cout <<" sono nella traduci" << endl;*/
	TIPO_GENERALE = INT; 
	primo = true; 
	traduci(tree,ts,tf, out); 
	return true;
}




void Albero_espressione:: traduci(nodo_ptr r,  TabellaSimboli& ts, Tavola_funzioni& tf, ofstream& out) {
	if(!r) {
		return; 	
	}

	traduci(r->sinistra, ts, tf, out); 
	traduci(r->destra,ts, tf,  out); 
	if ( r->info->isCall_function()) ((Call_function *)r->info)->traduci_f(out, ts,tf);
	else r->info->traduci(ts, out); 
}


// deve trovarmi tutti gli errori che non sono creati in fase di costrzione

bool Albero_espressione::Error( const nodo_ptr&  root,  TabellaSimboli &TS) {
	if (!root){
		if (err) {
			return false;
		}
		else {
			return true; 
		}
	}
	if(root->info->isTree_Identifier())  { // se temp è un identificatore 
		/*cout << "E' un identificatore: è dichiarato??" << endl;*/
		if(!TS.present(((Tree_identifier *)(root->info))->get_name())){
			cerr << "Variabile non dichiarata" << endl; 
		}
	}

	if (( root->info->IsOperetorUnary()) && (((Operatore_unario *)(root->info))->get_name() == "++" || ((Operatore_unario *)(root->info))->get_name() == "--"))
		if( root->destra && !root->destra->info->isTree_Identifier())
			cerr << "Mi apetto un valore di left-value" << endl;  

	bool error = Error(root->destra, TS); 
	return error || Error(root->sinistra, TS); 
}


bool Albero_espressione::Error( TabellaSimboli &TS) {
	err = false; 
	return Error(tree, TS); 
}

ostream& operator<<(ostream& os, Albero_espressione& alb){
	if (!alb.tree){
		return os; 
	}
	alb.stampaStream(os,alb.tree); 
	os << endl;
	return os;
}

void Albero_espressione::stampaStream(ostream& os,nodo_ptr& root){
	if (!root){
		return; 
	}
	stampaStream(os,root->sinistra); 
	os << *(root->info); 
	stampaStream(os,root->destra); 
	return;
}


DataTypeName Albero_espressione::get_tipo_espressione() {

return get_tipo_espressione(tree); 

}

DataTypeName maggiore_tipo(DataTypeName t1, DataTypeName t2);

DataTypeName Albero_espressione::get_tipo_espressione(nodo_ptr root) {

	if (!root->destra && !root->sinistra)  //foglia
		 {
		DataTypeName tipo; 
		if ( root->info->isTree_Identifier()) 
			tipo = ((Tree_identifier *)(root->info))->get_tipo(); 
		else if (root->info->isLiteral()) 
			tipo = ((Tree_literal *)(root->info))->get_tipo(); 
		//else if ( root->info->isCall_function()); 
		return tipo; 
		}
	else { 
		DataTypeName temp ; 
		temp =  get_tipo_espressione(root->destra); 
		return maggiore_tipo(temp, get_tipo_espressione(root->sinistra)); 
	}
}

DataTypeName maggiore_tipo(DataTypeName t1, DataTypeName t2) {


	switch(t1) {
		case INT: 
			if (t2 == DOUBLE) 
				return DOUBLE; 
			if (t2 == FLOAT) 
				return FLOAT; 
			if (t2 == LONG) 
				return LONG;
			if (t2 == INT) 
				return INT;  
		break; 
		case DOUBLE: 
			return DOUBLE; 
		break; 
		case FLOAT: 
			if (t2 == DOUBLE) 
				return DOUBLE; 
			if (t2 == FLOAT) 
				return FLOAT; 
			if (t2 == LONG) 
				return DOUBLE;
			if (t2 == INT) 
				return FLOAT;  
		break; 
		case LONG : 
			if (t2 == DOUBLE) 
				return DOUBLE; 
			if (t2 == FLOAT) 
				return DOUBLE; 
			if (t2 == LONG) 
				return LONG;
			if (t2 == INT) 
				return LONG;  
		break; 
		default : 
			cerr << "Errore nella riconosci tipi della get tipo espressione " << endl;
		break; 
	}
}
