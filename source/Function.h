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


#ifndef FUNCTION_H
#define FUNCTION_H

#include "token.hh"

void deleteIntro(TokenList& tt){
	for (int i=0; i<4; i++){
		tt.erase(tt.begin());
	}
	tt.erase(--tt.end());
}

bool isProgramInitialized(TokenList& t){
	TokenList::const_iterator i=t.begin();
	TokenList::const_iterator j=t.end();
	j--;
	if (i != t.end() && (*i)->isIdentifier() && ((Identifier*)(*i))->name=="main"){
		i++;
		if (i != t.end() && (*i)->isSymbol() && ((Symbol*)(*i))->name=="("){
			i++;
			if (i != t.end() && (*i)->isSymbol() && ((Symbol*)(*i))->name==")"){
				i++;
				if (i != t.end() && (*i)->isSymbol() && ((Symbol*)(*i))->name=="{"){
				
					if ( j != t.end() && (*j)->isSymbol() && ((Symbol*)(*j))->name=="}"){ // anche se il primo è inutile
						deleteIntro(t);
						return true;
					}
				}
			}
		}
	}
	return false;
}



#endif 
