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


	implementazione della classe dichiarazione virtuale pura
*/



class Dichiarazione{
private:
	static int attuale_registro; 
	string nome; // nome dell 'oggetto // posizione attuale del registro 
	int registro; // posizione nella quale si deve immagazinare il valore; 
	int linea_dichiarazione;  // posizione della dichiarazione; 
protected:
	bool set_nome ( const string&);  // setta il nome dell'oggetto
	bool set_linea_dichiarazione(int);  // setta la linea di dichiarazzione
	bool set_registro(int); 	
	int  get_registro_attuale(void) const;
	static bool set_registro_attuale(int); 
public:
	string get_nome (void) const; // rende il nome
	int get_linea_dichiarazione(void) const ; // rende la line
	int get_registro(void) const; // rende il registro iniziale in cui e memmorizata	
	//carica le variabili nel registro di sistem! 
	//rende il numero di registri occupati!	
	virtual int inserisci_in_memmoria(void) = 0;
	friend class TabellaSimboli;
		
};

