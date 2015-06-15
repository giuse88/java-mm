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
using namespace std;
/**
	@author giuseppe <giuse88@gmail.com>
*/



class Tavola_funzioni{

private : 
	
	map <Key_name , Funzione * > tavola_funzioni; 
public:
	Tavola_funzioni(TokenList::iterator& i, TokenList::const_iterator end, bool&); // creo la tavola_funzioni
	~Tavola_funzioni(){}; // cancellare le funzioni};
	bool present(Key_name) const; 
	Funzione * operator[] (string); 
	void stampa () ;
	void traduci(ofstream&); 
};	


