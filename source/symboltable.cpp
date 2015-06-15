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

TabellaSimboli::TabellaSimboli(TokenList::iterator& i, TokenList::const_iterator end, bool& errore){ 

	//TokenList::iterator i = token_list.begin(); 
	Dichiarazione::set_registro_attuale(0); 

	while (i != end && (*i)->isDataType()) {	

		try {		
			DataTypeName tipo = ((DataType *)(*i))->name; // converto il token in datatype
			int linea_dichiarazione = (*i)->lineNumber;		
			i++; // passo al successivo token!!
			if (i != end && (*i)->isIdentifier()) { //Controollo che il successivo sia un identificatore
				Key_name nome = ((Identifier *)(*i))->name; // salvo il nome dell'identificator	
				if (present(nome)) {
					i++; // mi sposto al token successivo per saper che c'è
					if (i != end && isComma(i)) {
						i++; // se c'è un punto e virgola salto il token 
					}
					errore = true; 
					//prima di lanciare l'eccezione vado alla linea dopo
					gotoNextLine(i,end);
					throw new DoubleDeclarationException(linea_dichiarazione, nome);  
				}			
				i++; // passo al token succesivo per fare il controllo 
				if(i != end && isComma(i)) {	
					// la sintassi è corretta! fai gli inserimenti
					Mappa_simboli[nome] = new Identificatore; //creo l'oggetto identificatore
					Identificatore *  Ident = ((Identificatore*)Mappa_simboli[nome]);
					Ident->set_nome(nome); 
					Ident->set_tipo(tipo);
					Ident->set_linea_dichiarazione(linea_dichiarazione);
					Ident->inserisci_in_memmoria();  // funzione virtuale */
					i++; // incremento per controllare il token successivo 
				}
				else {
					//i++;// Incremento che verra controllato nella condizione di validita del while
					errore = true; 
					throw new NoCommaException(linea_dichiarazione); 
				}
			}
			else {
				if(isComma(i)){
					i++; //Incremento che verra controllato nella condizione di validita del while 
				}
				errore = true; 
				throw new  NoIdentifierException(linea_dichiarazione); 
			}
		}
		catch(Exception* ex) {
			ex->toString(); 	
		}
	}
	//token_list.erase ( token_list.begin(), i); //elimino tutti gli elementi presenti tra l'inizio è  la fine 	
	if (errore) cout << ("Tabella simboli terminata con errori\n"); 
	else  cout << ("Tabella simboli terminata senza errori\n");
}


bool TabellaSimboli::present(Key_name& key) { // funzione che cerca se è presente quella chiave
	map<Key_name, Dichiarazione*>::const_iterator i = Mappa_simboli.find(key);
	if ( i == Mappa_simboli.end()) return false; 
	else return true;
} 

TabellaSimboli TabellaSimboli::operator += ( TabellaSimboli& copia) {

	map<Key_name, Dichiarazione*>::const_iterator i = copia.Mappa_simboli.begin();
	
	for ( ; i != copia.Mappa_simboli.end(); i++){
		Mappa_simboli[(*i).first] = (*i).second; 
	}
	return *this;

}

bool TabellaSimboli::aggiungi (TabellaSimboli& copia) {

	map<Key_name, Dichiarazione*>::const_iterator i = copia.Mappa_simboli.begin();
	
	for ( ; i != copia.Mappa_simboli.end(); i++){
	if(this->present((*i).first)) { cerr << "ariabile gia dichiarata nello scopoe" << endl; exit(1); }
	Mappa_simboli[(*i).first] = i->second; 
	}


	cout << *this << endl;

}


const Dichiarazione* TabellaSimboli::operator[] (Key_name& key) { // io rendo un puntatore in modo che possa essere modificare??
	return Mappa_simboli[key]; 
} 

ostream& operator << ( ostream& out, const TabellaSimboli& ts) {
	out << "######################### TABELLA SIMBOLI ##############################\n" << endl; 
	out << "\tTipo\tKey_name\tRegistro"<< endl;
	map<Key_name,Dichiarazione *>::const_iterator i = ts.Mappa_simboli.begin(); 

	cout <<"inizio la stampa" << endl;

	for (; i != ts.Mappa_simboli.end(); i++) {
		 
		Identificatore * ident = dynamic_cast<Identificatore *>(i->second); 

		if ( ident != NULL)  {

			switch (ident->get_tipo()) {
				case INT : 
				out << "\tINT\t"; 
				break; 
			case DOUBLE: 
				out << "\tDOUBLE\t"; 
				break; 
			case  FLOAT: 
				out << "\tFLOAT\t";
				break; 
			case LONG: 
				out<< "\tLONG\t"; 
				break; 
			}
			out << ident->get_nome() << "\t\t" << ident->get_registro() << endl; 
		} 
		else {
			/*Funzione * funz = dynamic_cast<Funzione *> (i->second); 
			out << "Implementare caso funzione" << endl; */
		}
	}
	return out; 
}
/*
TabellaSimboli::TabellaSimboli(const TabellaSimboli & copia) {
	Mappa_simboli = copia.Mappa_simboli; 
	cout << "Sono nel costruttore di coppia " << endl; 
	cout << *this << endl; 
}
*/
TabellaSimboli& TabellaSimboli::operator = ( const TabellaSimboli& copia) {

	Mappa_simboli = copia.Mappa_simboli; 
	return *this; 

}

bool TabellaSimboli::aggiungi_elemento(int linea, DataTypeName tipo, string nome ) {

	if (present(nome)) return false; 
	else {
		Mappa_simboli[nome] = new Identificatore; //creo l'oggetto identificatore
		Identificatore *  Ident = ((Identificatore*)Mappa_simboli[nome]);
		Ident->set_nome(nome); 
		Ident->set_tipo(tipo);
		Ident->set_linea_dichiarazione(linea);
		Ident->inserisci_in_memmoria();
		return true; 
	}

}

bool TabellaSimboli::isComma( TokenList::const_iterator i) {
	if ( (*i)->isSymbol() && ((Symbol *)(*i))->name == ";")
			return true; 
	return false; 	
}

TabellaSimboli::~TabellaSimboli() {
	map<Key_name,Dichiarazione *>::iterator i =Mappa_simboli.begin(); 
	for (;i != Mappa_simboli.end(); i++) {
		delete (i->second);  // rende l'oggetto che ha immagzinato
	}
	Mappa_simboli.clear(); 
}

void TabellaSimboli::traduci(ofstream& out) {

	map<Key_name,Dichiarazione *>::iterator i =Mappa_simboli.begin(); 
	
	out << endl << endl <<"; Inizializzo le variabili a valori di default" << endl << endl; 

	for (; i != Mappa_simboli.end(); i++ ) {
		Identificatore *temp = ((Identificatore *)(*i).second); 
	
		out << "ldc 0" << endl;
		switch( temp->get_tipo()) {

			case INT : 
				out << "istore " << temp->get_registro()<< endl; 
			break; 
	
			case FLOAT: 
				out << "i2f" << endl; 
				out << "fstore " << temp->get_registro() << endl; 
			break; 

			case LONG: 
				out << "i2l" << endl; 
				out << "lstore " << temp->get_registro() << endl; 
			break; 

			case  DOUBLE: 
				out << "i2d" << endl; 
				out << "dstore " << temp->get_registro() << endl; 
			break; 

			default : 
				cerr <<"Errore nella traduci tabella simboli" << endl; 
			break; 
		}

		

	} 
		
	out << endl<< ";Fine dell'inizializzazione delle variabili\n" << endl; 

}