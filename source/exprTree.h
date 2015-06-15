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
/**
	@author Pes <root@localhost>
*/



#include "stdafx.h"



class Albero_espressione{
private:
	///oggeto che mi rapresenta i nodo dell'aplicazione
	struct nodo {
		nodo( nodo * d, nodo * s, Tree_expression * e) :  destra(d), sinistra(s),info(e) {} 
		nodo * sinistra; 
		nodo * destra; 
		Tree_expression* info;  // albero delle espressioni 
	}; 
	typedef nodo * nodo_ptr; 
	nodo_ptr tree; 
	void appendi_operatore(nodo_ptr&, nodo_ptr&, TokenList::iterator&, TokenList::const_iterator, TabellaSimboli&);
	void stampa(nodo_ptr) const;
	void espr_in_albero(nodo_ptr&, TokenList::iterator&, TokenList::const_iterator, TabellaSimboli&); 
	nodo_ptr crea_leterale(Number*); 
	void stampa_espressione(const nodo_ptr); 
	void stampaStream(ostream&,nodo_ptr&);
	void stampa_posticipata(nodo_ptr); 
	void traduci (nodo_ptr, TabellaSimboli&,Tavola_funzioni&,  ofstream&);
	void del (nodo_ptr&); 
	bool Error( const nodo_ptr&, TabellaSimboli &TS); 
	DataTypeName get_tipo_espressione(nodo_ptr); 
public:
	friend ostream& operator << (ostream&, Albero_espressione&); 
	void stampa() const ; 
	Albero_espressione(TabellaSimboli&,TokenList::iterator&, TokenList::const_iterator, bool = false);
	~Albero_espressione();
	void stampa_espressione(void);  
	void stampa_posticipata(void); 
	bool traduci(TabellaSimboli&, Tavola_funzioni& ,ofstream&); 
	bool Error(TabellaSimboli&); 
	DataTypeName get_tipo_espressione (); 

};
