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


Identificatore::Identificatore() {
	set_registro(get_registro_attuale()); 
}

bool Identificatore::set_tipo ( DataTypeName t){   // in questi caso il controllo di corretezza viene fatto dall'enum
	tipo = t;
	return true;
}

DataTypeName Identificatore::get_tipo(void) const {
	return tipo; 
} 


bool Identificatore::set_nome ( const string& stringa) { 
	return  Dichiarazione::set_nome(stringa); 
}

bool Identificatore::set_linea_dichiarazione( int riga) {
	return  Dichiarazione::set_linea_dichiarazione(riga); 
}

int Identificatore::inserisci_in_memmoria(void){
	switch(get_tipo()){
		case INT: 
			set_registro_attuale(get_registro_attuale() +1); 
			return 1; 
			break; 
		case DOUBLE: 
			set_registro_attuale(get_registro_attuale() +2); 
			return 2; 
			break; 
		case LONG:
			set_registro_attuale(get_registro_attuale() +2); 
			return 2;
			break; 
		case FLOAT: 
			set_registro_attuale(get_registro_attuale() +1);
			return 1;
			break; 
	}
}

void Identificatore::stampa() const { 
	cout << "Linea Dichiarazione : " <<get_linea_dichiarazione() << endl; 
	cout << "TIPO :" << get_tipo() << endl; 
	cout << "NOME : " << get_nome() << endl; 
	cout << "Registro : " << get_registro() << "Registro_attuale :" << get_registro_attuale() <<endl; 
}
