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

Tavola_funzioni::Tavola_funzioni(TokenList::iterator& i, TokenList::const_iterator end, bool& err) {

	err = false; 

	while (i != end && (*i)->isDataType()) { 
		Funzione *  temp = new Funzione (i, end, err); 
		/// contollo che la funzione non sia gia stat deifinita; 
		string name = temp->get_name_codificato();

		name.erase(name.length() -1); // elimino l'ultimo elemento
		
		cout << name << endl; 
		cout << (*temp) << endl;

	

		tavola_funzioni[name] = temp; // creo il blocco
			

	}
}

Funzione * Tavola_funzioni::operator[] (string name) {
	return tavola_funzioni[name]; 
} 

void Tavola_funzioni::stampa() {

map <Key_name , Funzione * >::iterator i;

for ( i = tavola_funzioni.begin(); i != tavola_funzioni.end(); i++) 
cout << *((*i).second) << endl; 


}

void Tavola_funzioni::traduci(ofstream& out) {

	map <Key_name , Funzione * >::iterator i;

	for ( i = tavola_funzioni.begin(); i != tavola_funzioni.end(); i++) 
		((*i).second)->traduci(out, (*this)); 
}

bool Tavola_funzioni::present(Key_name key) const {

	map<Key_name, Funzione *>::const_iterator i = tavola_funzioni.find(key);
	if ( i == tavola_funzioni.end()) return false; 
	else return true;


}
