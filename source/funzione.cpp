/***************************************************************************
 *   Copyright (C) 2008 by giuseppe   *
 *   giuse88@gmail.com   *
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

Funzione::Funzione(TokenList::iterator& i, TokenList::const_iterator end, bool & err){
//cout << "Sono nel costruttore della funzione" << endl; 

if(i == end) cerr <<"File vuoto"<< endl; 
else {
	
	bool error_ts = false , error_pr = false; // salvo eventuali errori  
	 // mi creo la stringa che mi da il tipo es : IDFL
	TabellaSimboli  *parametri_formali = new TabellaSimboli();  // bisogna farlo perchè crascia senza

	if ((*i)->isDataType() ) {

		function.ritorno = ((DataType *)(*i))->name; 
		//cout << "Il tipo di ritorno è " << function.ritorno << endl; 
		i++;

		if ( i != end &&  (*i)->isIdentifier()) {			
			function.nome = ((Identifier *)(*i))->name;
			nome_codificato  = ((Identifier *)(*i))->name; // nome(
			
			//cout << "Il nome è " << function.nome << nome_codificato << endl; 
			i++;

			if (i != end  && (*i)->isSymbol() && ((Symbol *)(*i))->name == "(") { // controllo la parentesi 
				nome_codificato += "("; // nome(
				// chiamo una funzione ricorsiva che mi salva tutti i paramtri formali; 
				i++;
				//cout << nome_codificato << endl; 
				 cerca_parametri(*parametri_formali, function.argomenti, i, end);
				
				cout << *parametri_formali << endl; 


				} else cerr << "Manca la parantesi" << endl; 
			
			aggiungi_tipo(function.ritorno); 
		}  else cerr << "manca il nome del tipo " << endl; 
	} else cerr << "Manca il tipo " << endl; 

	if (i != end  && (*i)->isSymbol() && ((Symbol *)(*i))->name == "{") 
	i++; // mi sposto sull'elemto successivo perchè sono sulla parentesi grafa 
	else cerr <<"erore parentesi" << endl; 

	Tabella_variabili  = new TabellaSimboli(i, end, error_ts); // salvo le variabili	
	
	cout << *parametri_formali << endl; 

		Tabella_variabili->aggiungi(*parametri_formali); 
 
	cout << (*Tabella_variabili) << endl; 


	cout <<"creo il corpo del program" <<endl; 
         corpo_funzione = new Program( *Tabella_variabili, i, end, error_pr); // salvo il corpo
	cout <<"Corpo program creato corretamente" <<endl;  

	cout << (*Tabella_variabili) << endl; 
	cout << *this << endl; 

	if ( error_ts || error_pr)  { cerr <<"WWWWWWWWWWWWWW errori" << endl; err = true;} 

	}

}

bool Funzione::cerca_parametri(TabellaSimboli& parametri_formali, list<Parametro_formale>&  argomenti, TokenList::iterator& i, TokenList::const_iterator end) {
	//if (i != end) cerr << "error" << endl;
	Parametro_formale temp; 
	if ( i == end) 
		return true; 
	else {
		if ((*i)->isDataType() ) {
			int linea = (*i)->lineNumber;; 
			temp.tipo = ((DataType *)(*i))->name; 
			//cout << ((DataType *)(*i))->name << endl; 
			aggiungi_tipo(temp.tipo); // modifico il nome codificatot
			i++;
			if ( i != end && (*i)->isIdentifier()) {
				temp.nome = ((Identifier *)(*i))->name;  
				if(!parametri_formali.aggiungi_elemento(linea, temp.tipo, temp.nome)) {
					cerr << "Errore nella riconosci  funzione " << endl; 
					exit(1); 
				} 
				argomenti.push_back(temp); // sempre nella coda n modo da trovarmi gli elementi in fila
				i++;
			}
		}
		//cout << parametri_formali << endl; 
		if( i != end &&  (*i)->isSymbol()  && ((Symbol *)(*i))->name == ",") {
			i++; 
			cerca_parametri(parametri_formali, argomenti, i, end); 
		}
		else if  ( i != end &&  (*i)->isSymbol()  && ((Symbol *)(*i))->name == ")"){
			nome_codificato += ")"; 
			i++;
			cout<<"FFFFFFFFFFF"<<endl;
			return true; 
		}
		else cerr << "false" << endl; 

	}

}


void Funzione::aggiungi_tipo( DataTypeName t) {

	switch (t)  {
		case INT: 
			nome_codificato += "I"; 
		break; 
		case FLOAT :
			nome_codificato += "F"; 
		break; 	
		case DOUBLE: 
			nome_codificato += "D"; 
		break; 
		case LONG: 
			nome_codificato += "J"; 
		break; 
		default : 
			cerr << "errore nella riconosci tipo della funzione" << endl; 
		break; 

	}
}

string stampa_DataType(DataTypeName t){
	switch (t){
		case INT: return "INT";
		case DOUBLE: return "DOUBLE";
		case LONG: return "LONG";
		case FLOAT: return "FLOAT";
		default: return "<Unknown>";
	}
}

ostream& operator << (ostream& out,  Funzione& copia) {

	Funzione::prototipo temp = copia.function; 
	
	out << "############################# FUNZIONE #################################\n" << endl; 
	
	out << stampa_DataType(temp.ritorno); 
	out  << "  "  << temp.nome <<  " ("; 

	list<Funzione::Parametro_formale>::iterator i = temp.argomenti.begin(); 

	for (; i != temp.argomenti.end(); i++) {
		out << stampa_DataType((*i).tipo); 
		out << "  "  << (*i).nome << " ," ; 
	}
	
	out << ")" << endl; 	
	
	out << "NOME CODIFICATO DELLA FUNZIONE : " << copia.nome_codificato << endl << endl; 

	out << *(copia.Tabella_variabili) << endl; 
	out << *(copia.corpo_funzione) << endl;
	out << endl; 

	return out; 
}

void Funzione::traduci(ofstream& out, Tavola_funzioni& tf) {


	// crascia se la funzione non esiste

	out << endl << ";************************ TRADUZIONE FUNZIONE "<< nome_codificato <<"*****************" << endl; 
	out << ".method static " << nome_codificato << endl; 
	out << ".limit stack 500" << endl; 
	out << ".limit locals 500" << endl; 
	
	//Tabella_variabili->traduci(out); 

	
	list<Parametro_formale>::iterator i = function.argomenti.begin(); 

//	for ( ; i != function.argomenti.end(); i++)
//		out << "istore " << ((*Tabella_variabili)[i->nome])->get_registro() << endl; // poi bisogna fare tutti i casi

	corpo_funzione->Translate(out, *Tabella_variabili, tf );   

	out << "ldc 0" << endl; 
	out << "ireturn " << endl; 
	out << ".throws java/io/IOException" << endl; 
	out << ".end method" << endl; 

	out << endl << ";************************      FINE     *****************" << endl; 

}

Funzione::~Funzione() {

	delete Tabella_variabili; 
	delete corpo_funzione;  

}
