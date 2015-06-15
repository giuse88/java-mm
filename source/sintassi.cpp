#include "stdafx.h"
 
DataTypeName TIPO_GENERALE = INT; // imposto al piu basso  
bool primo = true; 
int registro; 


static int label = 0; 

void priorita_tipi(DataTypeName); // dichiarazione 
void switch_sotrazione(ofstream& out); 
void switch_somma(ofstream& out); 
void switch_divisione(ofstream& out); 
void switch_moltiplicazione(ofstream& out); 
void switch_uguaglianza(ofstream&); 
void switch_maggiore(ofstream&);
void switch_minore(ofstream&);
void switch_diverso(ofstream&);
void switch_maggiore_uguale(ofstream&);
void switch_minore_uguale(ofstream&);
void switch_piupiu(ofstream&);  
void switch_menomeno(ofstream&); 
void switch_negazione(ofstream&); 
void switch_meno_unario(ofstream&);
void converti_tipo_primo(ofstream&, DataTypeName);

void converti_tipo_dopo(ofstream&, DataTypeName); 
void aggiungi_tipo(string&, DataTypeName); 
inline void traduci_negazione(ofstream& out);

ostream& operator<< (ostream& os,Tree_expression& te){
	os << te.name;
	return os;
}


Call_function::Call_function(TabellaSimboli& ts , TokenList::iterator& i, TokenList::const_iterator end,  const string n) {


	name = n; // nome della funzione; 

	cout << (*i) << endl; 

	//cout << "§§§§§§§§§§§§§§§ " << ((Identifier *)(*i))->name <<endl; 

	
	
	//for (int j = 0; j < 4; j++) {
	cout << "############################### creo espressione " << endl; 
	while ( i != end) {
		argomenti.push_back(new Albero_espressione(ts , i,end)); 
		cout << " E la virogola" << ((Symbol *)(*i))->name << endl; 
		cout << "Espressione creata corretamente " << endl; 
		if ( ((Symbol *)(*i))->name == ")")  break; 
		i++;
	}

	i++; // vado all'operaotore oltre la parentesi



	///se il tipo non è presente il programma crascia
	/// controllo se la variabile è presente nella tabella

	name += "(";
	list< Albero_espressione *>::iterator a = argomenti.begin(); 

	for ( ; a != argomenti.end(); a++) 
		aggiungi_tipo(name , (*a)->get_tipo_espressione() ); 
	
	name +=")";
	cout <<">>>>>>>>>>>>>>>>>>>>>>>>>>>> " << name << endl;


}

void Call_function::traduci_f(ofstream& out, TabellaSimboli& ts,  Tavola_funzioni& tf) {
	
	list< Albero_espressione *>::iterator i= argomenti.begin(); 

	for ( ; i != argomenti.end(); i++) 
		(*i)->traduci(ts,tf, out);
	
	cout << " sono  nella traduci della funzione" << endl; 

	
	cout << name << endl; 

	if(!tf.present(name))  { cerr << "ERRORE" << endl; exit(1); };

	out <<"invokestatic Oggetto/" << tf[name]->get_name_codificato() << endl;  
	
	cout << "tradotto l'invocazione di funzione" << endl; 


}

void aggiungi_tipo( string& nome_codificato, DataTypeName t) {

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




void Tree_literal::traduci(TabellaSimboli& TS, ofstream& out){
	//cout << "Traduci letterale" << endl; 

	if (!primo)  converti_tipo_primo(out, tipo);
	priorita_tipi(tipo); 

	stampa_file(out); 

	priorita_tipi(tipo);
	converti_tipo_dopo(out, tipo);
	primo = false; 
}


//bisogna ricontrollare la traduccim dei leterali sopratutto 


void Tree_identifier::traduci( TabellaSimboli& TS, ofstream& out){
	


	set_tipo(((Identificatore*)TS[name])->get_tipo()); // devo prendere il tipo dalla tabella dei simboli 
	registro = TS[name]->get_registro();

	if (!primo)  converti_tipo_primo(out, tipo); // questa fa la conversione della variabile gia nello stack
	//priorita_tipi(tipo); 
	 switch (tipo) {
				// carico nello stack
		case INT :
			out << "iload "; 
			out << registro << endl; 
		break;

		case DOUBLE : 
			out << "dload " ;
			out << registro << endl; 
		break;

		case LONG: 
			out << "lload " ;
			out << registro << endl; 
		break;

		case FLOAT : 
			out << "fload " ;
			out << registro << endl; 
		break;

		default : 
			cerr <<("Eccezzione nel riconoscimento dei tipi "); 
		break; 
	} 

	priorita_tipi(tipo);
	converti_tipo_dopo(out, tipo);
	primo = false; 
}


void converti_tipo_primo(ofstream& out , DataTypeName t) {

// tipo generale mi rapresenta l'elemento nello stack

	switch (TIPO_GENERALE) {

		case INT : 
			if ( t == DOUBLE) 
				out << "i2d" << endl; 
			else if ( t == FLOAT) 
				out << "i2f" << endl; 
			else if ( t ==  LONG)
 				out << "i2l" << endl; 			
		break; 
		
		case FLOAT : 
			if ( t == DOUBLE || t == LONG) 
				out << "f2d" << endl; 
		break; 
	
		case LONG :
			if ( t == DOUBLE || t == FLOAT){ 
				out << "l2d" << endl; 
			}
		case DOUBLE: 
		break; 
	
		default : 
			cerr << "Errore nella riconosci dei tipi (prima) " << endl; 
	}
}

void converti_tipo_dopo(ofstream& out , DataTypeName t) {

	
	switch (TIPO_GENERALE) {

		case INT : 		
		break; 
		
		case FLOAT : 
			if ( t == INT)
				out << "i2f" << endl; 
		else if ( t == LONG) {
				out << "l2d" << endl; 
		}
		break; 
	
		case LONG :
		
			if ( t == INT) 
				out << "i2l" << endl; 
			else if(t == FLOAT) {
				out << "l2d" << endl; 
			}
			break;
		case DOUBLE: 
			if ( t == INT) 
				out << "i2d" << endl; 
			else if ( t == FLOAT) 
				out << "f2d" << endl; 
			else if ( t ==  LONG)
				out << "l2d" << endl; 	
			break; 
	
		default : 
			cerr << "Errore nella riconosci dei tipi (prima) " << endl; 
	}

}

void Tree_literal::stampa () const { 
	switch (tipo) {
		case INT :
			cout << *((int *)number);
		break;  
		case DOUBLE : 
			cout << *((double *)number);
		break; 
		case LONG: 
			cout << *((long int  *)number);
		break; 
		case FLOAT : 
			cout << *((float *)number);
		break; 
		default : 
			throw ("Eccezzione nel riconoscimento dei tipi "); 
		break; 
		} 
}


void Tree_literal::stampa_file(ofstream& out ) const {

	switch (tipo) {
		case INT :
			out << "ldc "; 
			out << *((int *)number) << endl;
		break;  
		case DOUBLE : 
			out << "ldc2_w " ;
			out << *((double *)number) << endl;
		break; 
		case LONG: 
			out << "ldc2_w "; 
			out << *((long int  *)number) << endl;
		break; 
		case FLOAT : 
			out << "ldc_w " << endl; // uguale a ldc 
			out << *((float *)number) << endl;
		break; 
		default : 
			cout << ("Eccezzione nel riconoscimento dei tipi "); 
		break; 
		} 
}


void Operatore_binario::traduci(TabellaSimboli& TS , ofstream& out ) {
	//out << "traducci Operatore binario " << endl; 
	if (name == "+") 
			switch_somma(out);  
	else if ( name ==  "-")
			switch_sotrazione(out); 
	else if ( name == "*")  
			switch_moltiplicazione(out); 
	else if ( name == "/")  
			switch_divisione(out); 
	else if ( name == "==") 
			switch_uguaglianza(out); 
	else if ( name == ">") 
			switch_maggiore(out); 
	else if ( name == "<" ) 
			switch_minore(out); 
	else if ( name == "<=")
			switch_minore_uguale(out);
	else if ( name == ">=") 
			switch_maggiore_uguale(out);
	else if  ( name == "!=") 
			switch_diverso(out); 
	else 	cerr << "Errore nel riconosci espressione" << endl; 
}


void Operatore_unario::traduci(TabellaSimboli& TS , ofstream& out) {

	if (name == "++") 
			switch_piupiu(out);  
	else if ( name ==  "--")
			switch_menomeno(out); 
	else if ( name == "!")  // questa si puo applaicare solo agli interi! opto per una converti 
			switch_negazione(out);
	else if ( name == "+");
	else if  ( name == "-") 
		switch_meno_unario(out);
	else 	cerr << "Errore nel riconosci espressione" << endl; 

}


void priorita_tipi( DataTypeName t ){

	switch(TIPO_GENERALE) { 
		
		case INT: 
			if(t == LONG) 
				TIPO_GENERALE = LONG; 
			else if ( t == FLOAT) 
				TIPO_GENERALE = FLOAT; 
			else if ( t == DOUBLE) 
				TIPO_GENERALE = DOUBLE; 
			else 	TIPO_GENERALE = INT;
		break; 
	
		case DOUBLE : 
			TIPO_GENERALE = DOUBLE; 
		break; 
		
		case FLOAT : 

			if ( t == DOUBLE) 
				TIPO_GENERALE = DOUBLE; 
			else  if ( t == LONG) 
				TIPO_GENERALE = DOUBLE; 
		break; 
		
		case LONG : 
			if ( t == DOUBLE) 
				TIPO_GENERALE = DOUBLE; 
			else if  ( t == FLOAT) 
				TIPO_GENERALE = DOUBLE; 
		break; 
		
		default :
			cerr << "ERRORE  NELLA GESTIONE DEI TIPI" << endl;
		break; 
	}

}

void switch_somma(ofstream& out) {

	switch (TIPO_GENERALE) { 

		case INT : 
			out << "iadd " << endl; 
		break; 
		case DOUBLE:
			out << "dadd " << endl; 
		break;  
	
		case FLOAT : 
			out << "fadd " << endl; 
		break; 
		
		case LONG : 
			out << "ladd " << endl; 
		break; 

		default : 
			cerr << "Errore nel riconoscimento degli operatori" << endl; 
		break; 
	} 
}

void switch_sotrazione(ofstream& out) {

	switch (TIPO_GENERALE) { 

		case INT : 
			out << "isub " << endl; 
		break; 
		case DOUBLE:
			out << "dsub " << endl; 
		break;  
	
		case FLOAT : 
			out << "fsub " << endl; 
		break; 
		
		case LONG : 
			out << "lsub " << endl; 
		break; 

		default : 
			cerr << "Errore nel riconoscimento degli operatori" << endl; 
		break; 
	} 
}

void switch_divisione(ofstream& out) {

	switch (TIPO_GENERALE) { 

		case INT : 
			out << "idiv " << endl; 
		break; 
		case DOUBLE:
			out << "ddiv " << endl; 
		break;  
	
		case FLOAT : 
			out << "fdiv " << endl; 
		break; 
		
		case LONG : 
			out << "ldiv " << endl; 
		break; 

		default : 
			cerr << "Errore nel riconoscimento degli operatori" << endl; 
		break; 
	} 






}; 

void switch_moltiplicazione(ofstream& out) {

	switch (TIPO_GENERALE) { 

		case INT : 
			out << "imul " << endl; 
		break; 
		case DOUBLE:
			out << "dmul " << endl; 
		break;  
	
		case FLOAT : 
			out << "fmul " << endl; 
		break; 
		
		case LONG : 
			out << "lmul " << endl; 
		break; 

		default : 
			cerr << "Errore nel riconoscimento degli operatori" << endl; 
		break; 
	} 
}; 


void switch_uguaglianza(ofstream& out){

	switch (TIPO_GENERALE) {
		case INT:
			out << "if_icmpeq LABEL" << label << endl; 
			out << "ldc 0 " << endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label-1 << ":" << endl; 
			out << "ldc 1" << endl; 
			out << "LABEL" << label <<":" << endl; 
			label++; 
		break; 
		case DOUBLE: 
			out << "dcmpg "<< endl; 
			out << "ifeq LABEL" << label << endl; 
			out << "ldc 0 "<< endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label -1 << ":" << endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL"<< label <<":" << endl; 
			label++;
		break; 
		case FLOAT: 
			out << "fcmpg "<< endl; 
			out << "ifeq LABEL" << label << endl; 
			out << "ldc 0 "<< endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label -1 << ":" <<endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL"<< label << ":" <<endl; 
			label++;
		case LONG: 
			out << "lcmpg "<< endl; 
			out << "ifeq LABEL" << label << endl; 
			out << "ldc 0 "<< endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label -1 << ":" <<endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL"<< label << ":" <<endl; 
			label++;
		break; 
		default: 
			cerr << "ERRORE nella traduci dei tipi" << endl; 
		break; 
			
	}
	TIPO_GENERALE=INT;
}; 

void switch_maggiore(ofstream& out) {

	switch (TIPO_GENERALE) {
		case INT:
			out << "if_icmpgt LABEL" << label << endl; 
			out << "ldc 0 " << endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label-1 << ":" << endl; 
			out << "ldc 1" << endl; 
			out << "LABEL" << label <<":" << endl; 
			label++; 
		break; 
		case DOUBLE: 
			out << "dcmpg "<< endl; 
			out << "ldc 1" << endl; // carico la variabile con cui fare il paragone 
			out << "if_icmpeq LABEL" << label << endl; 
			out << "ldc 0 "<< endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label -1 << ":" <<endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL"<< label << ":" <<endl; 
			label++;
		break; 
		case FLOAT: 
			out << "fcmpg "<< endl; 
			out << "ldc 1" << endl; // carico la variabile con cui fare il paragone 
			out << "if_icmpeq LABEL" << label << endl; 
			out << "ldc 0 "<< endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label -1 << ":" <<endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL"<< label << ":" <<endl; 
			label++;
		break; 
		case LONG : 
			out << "lcmpg "<< endl; 
			out << "ldc 1" << endl; // carico la variabile con cui fare il paragone 
			out << "if_icmpeq LABEL" << label << endl; 
			out << "ldc 0 "<< endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label -1 <<":" << endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL"<< label << ":" <<endl; 
			label++;
		break; 
		default : 
			cerr << " Errore nella riconosci tipi " << endl; 
		break; 
		
	}
	TIPO_GENERALE=INT;
};


void switch_minore(ofstream& out){
	switch (TIPO_GENERALE) {
		case INT:
			out << "if_icmplt LABEL" << label << endl; 
			out << "ldc 0 " << endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label-1 << ":" << endl; 
			out << "ldc 1" << endl; 
			out << "LABEL" << label <<":" << endl; 
			label++; 
		break; 
		case DOUBLE: 
			out << "dcmpg "<< endl; 
			out << "ldc -1" << endl; // carico la variabile con cui fare il paragone 
			out << "if_icmpeq LABEL" << label << endl; 
			out << "ldc 0 "<< endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label -1 << ":" <<endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL"<< label << ":" <<endl; 
			label++;
		break; 
		case FLOAT: 
			out << "lcmpg "<< endl; 
			out << "ldc -1" << endl; // carico la variabile con cui fare il paragone 
			out << "if_icmpeq LABEL" << label << endl; 
			out << "ldc 0 "<< endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label -1 <<":" << endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL"<< label <<":" << endl; 
			label++;
		break; 
		case LONG : 
			out << "fcmpg "<< endl; 
			out << "ldc -1" << endl; // carico la variabile con cui fare il paragone 
			out << "if_icmpeq LABEL" << label << endl; 
			out << "ldc 0 "<< endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label -1 << ":" <<endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL"<< label << ":" <<endl; 
			label++;
		break; 
		default : 
			cerr << "Errore nella traduci dei tipi" << endl; 
		break; 
	}
	TIPO_GENERALE=INT;
};


void switch_diverso(ofstream& out) {

	switch (TIPO_GENERALE) {
		case INT:
			out << "if_icmpne LABEL" << label << endl; 
			out << "ldc 0 " << endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label-1 << ":" << endl; 
			out << "ldc 1" << endl; 
			out << "LABEL" << label <<":" << endl; 
			label++; 
		break; 
		case DOUBLE: 
			out <<"dcmpg" << endl; // se sono uguali rende zero ed è giusto altrimenti rende un valore diverso da zero  
		break; 
		case FLOAT: 
			out << "fcmpg" << endl; 
		break; 
		case LONG :
			out << "lcmfg" << endl; 
		break; 
		default : 
			cerr << "Errore nella traduci dell' operatore != " << endl; 
		break; 
	}
	TIPO_GENERALE=INT;
}



void switch_maggiore_uguale(ofstream& out) {

	switch (TIPO_GENERALE) {
		case INT:
			out << "if_icmpge LABEL" << label << endl; 
			out << "ldc 0 " << endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label-1 << ":" << endl; 
			out << "ldc 1" << endl; 
			out << "LABEL" << label <<":" << endl; 
			label++; 
		break; 
		case DOUBLE: 
			out << "dcmpg "<< endl; 
			out << "ldc 0" << endl; 
			out << "if_icmpge LABEL" << label << endl;  // se è maggiore o uguale a zero vai al label e carica 1 
			out << "ldc 0" << endl; 
			out <<  "goto LABEL"<< ++label << endl; 
			out << "LABEL" << label -1 << ":" <<  endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL" << label << ":" << endl; 			 
			label++;
		break; 
		case FLOAT: 
			out << "fcmpg "<< endl; 
			out << "ldc 0" << endl; 
			out << "if_icmpge LABEL" << label << endl;  // se è maggiore o uguale a zero vai al label e carica 1 
			out << "ldc 0" << endl; 
			out <<  "goto LABEL"<< ++label << endl; 
			out << "LABEL" << label -1 << ":" <<  endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL" << label << ":" << endl; 			 
			label++;
		break; 
		case LONG : 
			out << "lcmpg "<< endl; 
			out << "ldc 0" << endl; 
			out << "if_icmpge LABEL" << label << endl;  // se è maggiore o uguale a zero vai al label e carica 1 
			out << "ldc 0" << endl; 
			out <<  "goto LABEL"<< ++label << endl; 
			out << "LABEL" << label -1 << ":" <<  endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL" << label << ":" << endl; 			 
			label++;
		break; 
		default : 
			cerr << "ERRORE nella traduci dell 'operatore >="<< endl; 
	}
	TIPO_GENERALE=INT;
};


void switch_minore_uguale(ofstream& out){
	switch (TIPO_GENERALE) {
		case INT:
			out << "if_icmple LABEL" << label << endl; 
			out << "ldc 0 " << endl; 
			out << "goto LABEL" << ++label << endl; 
			out << "LABEL" << label-1 << ":" << endl; 
			out << "ldc 1" << endl; 
			out << "LABEL" << label <<":" << endl; 
			label++; 
		break; 
		case DOUBLE: 
			out << "dcmpg "<< endl; 
			out << "ldc 0" << endl; 
			out << "if_icmple LABEL" << label << endl;  // se è maggiore o uguale a zero vai al label e carica 1 
			out << "ldc 0" << endl; 
			out <<  "goto LABEL"<< ++label << endl; 
			out << "LABEL" << label -1 << ":" <<  endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL" << label << ":" << endl;
			label++;
		break; 
		case LONG:
			out << "lcmpg "<< endl; 
			out << "ldc 0" << endl; 
			out << "if_icmple LABEL" << label << endl;  // se è maggiore o uguale a zero vai al label e carica 1 
			out << "ldc 0" << endl; 
			out <<  "goto LABEL"<< ++label << endl; 
			out << "LABEL" << label -1 << ":" <<  endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL" << label << ":" << endl;
			label++;
		break; 
		case FLOAT: 
			out << "fcmpg "<< endl; 
			out << "ldc 0" << endl; 
			out << "if_icmple LABEL" << label << endl;  // se è maggiore o uguale a zero vai al label e carica 1 
			out << "ldc 0" << endl; 
			out <<  "goto LABEL"<< ++label << endl; 
			out << "LABEL" << label -1 << ":" <<  endl; 
			out << "ldc 1 " << endl; 
			out << "LABEL" << label << ":" << endl;
			label++;
		break; 
		default : 
			cerr << "Errore nella traduci dell 'operatore < = " << endl; 
		break; 
	}
	TIPO_GENERALE=INT;
};

void switch_piupiu(ofstream& out){

	switch(TIPO_GENERALE) {
		case INT: 
			out << "ldc 1" << endl; 
			out << "iadd" << endl;
			out << "istore " << registro << endl; 
			out << "iload "<< registro << endl; 
			break; 
		
		case DOUBLE : 
			out << " ldc2_w 1.0" << endl; 
			out << "dadd" << endl; 
			out << "dstore " << registro << endl; 
			out << "dload "<< registro << endl; 
			break; 
		
		case LONG : 
			out << "ldc2_w 1" << endl;
			out << "ladd" << endl;
			out << "lstore " << registro << endl; 
			out << "lload "<< registro << endl; 
			break;
		case FLOAT :
			out << "ldc_w 1.0" << endl;
			out << "fadd" << endl;
			out << "fstore " << registro << endl;
			out << "fload "<< registro << endl; 
			break;
		default:
			cerr << "errore nel meno unario" << endl;
			break;
	}


	

};  
void switch_menomeno(ofstream& out){

  switch(TIPO_GENERALE) {
		case INT: 
			out << "ldc -1" << endl; 
			out << "iadd" << endl;
			out << "istore " << registro << endl; 
			out << "iload "<< registro << endl; 
			break; 
		
		case DOUBLE : 
			out << " ldc2_w -1.0" << endl; 
			out << "dadd" << endl; 
			out << "dstore " << registro << endl; 
			out << "dload "<< registro << endl; 
			break; 
		
		case LONG : 
			out << "ldc2_w -1" << endl;
			out << "ladd" << endl;
			out << "lstore " << registro << endl; 
			out << "lload "<< registro << endl; 
			break;
		case FLOAT :
			out << "ldc_w -1.0" << endl;
			out << "fadd" << endl;
			out << "fstore " << registro << endl;
			out << "fload "<< registro << endl; 
			break;
		default:
			cerr << "errore nel meno unario" << endl;
			break;
	}



}; 
void switch_meno_unario(ofstream& out){	
	switch(TIPO_GENERALE) {
		case INT: 
			out << "ldc -1" << endl; 
			out << "imul" << endl; 
			break; 
		
		case DOUBLE : 
			out << " ldc2_w -1.0" << endl; 
			out << "dmul" << endl; 
			break; 
		
		case LONG : 
			out << "ldc2_w -1" << endl;
			out << "lmul" << endl;
			break;
		case FLOAT :
			out << "ldc_w -1.0" << endl;
			out << "fmul" << endl;
			break;
		default:
			cerr << "errore nel meno unario" << endl;
			break;
	}

}
void switch_negazione(ofstream& out){

	switch(TIPO_GENERALE) {
		case INT: 
			traduci_negazione(out); 
		break; 
		case DOUBLE: 
			out << "d2i" << endl; 
			traduci_negazione(out); 
		break; 
		case FLOAT : 
			out << "f2i" << endl;
			traduci_negazione(out); 
		break; 
		case LONG: 
			out << "l2i" << endl; 
			traduci_negazione(out); 
		break; 
		default: 
			cerr << "Errore nella traduzione dell'operatore ! " << endl; 
		break;		
	}

} 

inline void traduci_negazione(ofstream& out) {
		out << "ifeq LABEL" << label << endl; 
		out << "ldc 0" << endl; 
		out << "goto LABEL" << ++label << endl; 
		out << "LABEL" << label -1 << ":" << endl; 
		out << "ldc 1" << endl; 
		out << "LABEL" << label << ":" << endl; 
		label++; 
}
