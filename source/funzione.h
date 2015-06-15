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
/**
	@author giuseppe <giuse88@gmail.com>
*/


using namespace std; 

class Funzione{
struct Parametro_formale{
	string nome; 
	DataTypeName tipo; 
};

struct prototipo {
	string nome; 
	list<Parametro_formale> argomenti; // Lista argomenti;
	DataTypeName ritorno; 
};



private: 

	TabellaSimboli  *Tabella_variabili; 
	Program  *corpo_funzione;  
	string nome_codificato;  
	prototipo function;

	bool cerca_parametri( TabellaSimboli&, list<Parametro_formale>& , TokenList::iterator& , TokenList::const_iterator);
	void aggiungi_tipo( DataTypeName);
public:
	Funzione(TokenList::iterator& i, TokenList::const_iterator end, bool&);
	const string get_name_codificato() const { return nome_codificato;};  
	Funzione * operator = ( const Funzione&);

	list<Parametro_formale> get_parametri () { return function.argomenti;}	

	friend ostream& operator << (ostream&, Funzione&);
	void traduci (ofstream& ,Tavola_funzioni&); 
	~Funzione();

};

