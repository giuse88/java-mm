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

int Dichiarazione::attuale_registro = 0;  // setto la variabile static a zero


bool Dichiarazione::set_nome( const string& stringa) {
	 nome = stringa; 
	 return true; 
}

string Dichiarazione::get_nome(void) const {
	return nome; 
}


bool Dichiarazione::set_linea_dichiarazione( int numero_linea) {
	if ( numero_linea<= 0) 	{
		return false; 
	}
	else {
		linea_dichiarazione = numero_linea; 
		return true; 
	}
}

int Dichiarazione::get_linea_dichiarazione(void) const {
	return linea_dichiarazione;	
}

bool Dichiarazione::set_registro(int valore) { 
	if ( valore < 0) {
		return false; 
	}
	registro = valore; 
	return true; 	
}

int Dichiarazione::get_registro (void) const {
	return registro; 
}

int Dichiarazione::get_registro_attuale(void) const {
	return attuale_registro; 
}

bool Dichiarazione::set_registro_attuale(int valore) {
	if ( valore < 0) {
		return false; 
	}
	else {
		attuale_registro = valore; 
	}
	return true; 
}
