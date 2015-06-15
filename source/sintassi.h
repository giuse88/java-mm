/***************************************************************************
 *   Copyright (C) 2008 by sss   *
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
	@author sss <root@localhost>
*/

#include "stdafx.h"

class Tree_expression {
protected:
	string name;
public:
	virtual bool isLiteral()  const { return false;};
	virtual bool isOperetorBinary() const { return false;}
	virtual bool isTree_Identifier() const { return false; }  // e fia definito da qualche parte
	virtual bool IsOperetorUnary() const  { return false; }
	virtual bool isCall_function() const {return false;}
	virtual void stampa() const = 0; 
	virtual void traduci( TabellaSimboli&, ofstream&) = 0; 
	friend ostream& operator<< (ostream& os,Tree_expression& te);
};

class Tree_identifier : public Tree_expression {
	DataTypeName tipo; // mi serve la tavola dei simboli
public: 
	Tree_identifier(const string s, DataTypeName t) : tipo(t){ name=s; }
	bool set_tipo(DataTypeName t) { tipo = t; return true; };
	string get_name () { return name; }
	DataTypeName get_tipo() { return tipo; }
	virtual bool isTree_Identifier() const { return true; }
	void stampa() const { cout << name; }
	virtual void traduci(TabellaSimboli&, ofstream&);
};


class Tree_literal : public Tree_expression {
private:
	void* number;
	DataTypeName tipo;
public: 
	Tree_literal(int i, DataTypeName t) : tipo(t)      { 
		number = new int(i); 
		ostringstream s;
		s << i;
		name = s.str();
	}		 
	Tree_literal (float f, DataTypeName t) :tipo(t)    { 
		number = new float(f); 
		ostringstream s;
		s << f;
		name = s.str();
	} 
	Tree_literal(double d, DataTypeName t )  :tipo(t)  { 
		number = new double (d);
		ostringstream s;
		s << d;
		name = s.str();
	} 
	Tree_literal (long int l, DataTypeName t) :tipo(t) { 
		number = new long int (l); 
		ostringstream s;
		s << l;
		name = s.str();
	}
	bool isLiteral() const  { return true;};
	void * get_value() const { return number ;}; // sperando che si possa convertire in dato normale 
	DataTypeName get_tipo() const { return tipo;};
	virtual void stampa () const;
	void stampa_file(ofstream&) const;
	virtual void traduci( TabellaSimboli&, ofstream&); 
};

class Operatore_unario : public Tree_expression {
public: 
	Operatore_unario(string op_u) { name = op_u;}; 
	string get_operator_unary() const { return name ;} 
	bool IsOperetorUnary() const { return true; }
	string get_name () const { return name;}
	void stampa () const  { cout << name ; }
	virtual void traduci(  TabellaSimboli&, ofstream&);
};




class Operatore_binario : public Tree_expression {
public: 
	Operatore_binario(const string op_b)  { name = op_b;}
	virtual bool isOperetorBinary() const { return true; }
	void stampa() const { cout << name; }
	virtual void traduci( TabellaSimboli&, ofstream&) ; 
};

class Call_function : public Tree_expression {
	private : 
	string name;


	list < Albero_espressione *> argomenti; 
	public: 
	Call_function(TabellaSimboli&, TokenList::iterator&, TokenList::const_iterator,  const string );
	virtual bool isCall_function() const { return true; }
	virtual void traduci(  TabellaSimboli&, ofstream&){};
	void traduci_f ( ofstream&, TabellaSimboli&, Tavola_funzioni&); 
	virtual void stampa () const { cout << name << endl; }
	

};
