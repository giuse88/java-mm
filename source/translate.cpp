/***************************************************************************
 *   Copyright (C) 2008 by pessssssssssssssss   *
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

 

#define EPILOGO "\nldc 0\nireturn\n.end method\n.method public static final main([Ljava/lang/String;)V\n.limit stack 2\n.limit locals 1\nLabel0:\ngetstatic java.lang.System.out Ljava/io/PrintStream;\n invokestatic Oggetto/main()I\ninvokevirtual java/io/PrintStream/println(I)V\nLabel1:\nreturn\n.end method"


Translate::Translate(  TabellaSimboli& ts,  Tavola_funzioni& tf, Program& pr)
{

	ofstream out;
	out.open("program.jtb");

	if (!out) { 
			cerr <<"Errore apertura file" << endl; 
			exit(1); 
	}

	int Limit_stack = 5000; 

	out << "; PRELUDIO" << endl;
	out << ".class public Oggetto" << endl; 
	out << ".super java/lang/Object" << endl; 
	out << ".method public <init>()V" << endl; 
	out << ".limit stack 1" << endl; 
	out << ".limit locals 1" << endl; 
	out << "Label0:" << endl; 
	out << "aload_0" << endl; 
	out << "invokespecial java/lang/Object/<init>()V\n";
	out << "Label1:\n"; 
	out << "return\n";
	out << ".end method\n";

	tf.traduci(out); 

	out << ".method static main()I\n";
	out << ".limit stack "<< Limit_stack << endl; 
	out << ".limit locals "<< Limit_stack << endl; 
	out << ";FINE Preludio" << endl; 


	ts.traduci(out);
	pr.Translate(out, ts, tf); 
		
		
	out << EPILOGO << endl;
	out.close();
}


Translate::~Translate()
{
}


