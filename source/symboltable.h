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

/**
	@author Pes <root@localhost>
*/

typedef const string Key_name; 

class TabellaSimboli{
private:
	map <Key_name, Dichiarazione *> Mappa_simboli;
	bool isComma( TokenList::const_iterator i);
public:
	TabellaSimboli() {Dichiarazione::set_registro_attuale(0);};
	TabellaSimboli( TokenList::iterator&, TokenList::const_iterator, bool&); //carica  gli elemnti nella tabella dei simboli
	//TabellaSimboli(const TabellaSimboli &);
	bool present(Key_name&); 
	const Dichiarazione* operator[] (Key_name&); 
	TabellaSimboli operator += (TabellaSimboli&); 
	bool aggiungi (TabellaSimboli&); 
	TabellaSimboli& operator = ( const TabellaSimboli&); 
	void traduci(ofstream&); 
	friend ostream& operator << (ostream&, const TabellaSimboli&); 
	bool aggiungi_elemento(int linea, DataTypeName tipo, string nome );
	~TabellaSimboli(); 
};
