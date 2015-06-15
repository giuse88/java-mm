#include "stdafx.h"

void gotoNextInterrupt(TokenList& tl,TokenList::iterator& ii,const string& ss){
	TokenList::iterator tmp;
	if (ii!=tl.end()){
		tmp=ii;
	}
	else{
		tmp=--ii;
	}
	while (ii!=tl.end() && (*ii)->toString().find("("+ss+")")==string::npos){
		ii++;
	}
	if (ii==tl.end()){
		throw new UnexpectedEndOfProgramException((*tmp)->lineNumber,ss);
	}
}

void gotoNextLine(TokenList::iterator& i,TokenList::const_iterator& end){
	if (i==end){
		return; //non posso gestirlo
		//throw new UnexpectedEndOfProgramException((*--i)->lineNumber,"<New Line>");
	}
	int line = (*i)->lineNumber;
	while (i!=end){
		if ((*i)->lineNumber>line){
			break;
		}
		i++;
	}
	i--;
}
//Expression::Expression(TokenList& tl, TabellaSimboli& ts, TokenList::iterator& i,const string& END_OF_EXPRESSION){
//	gotoNextInterrupt(tl,i,END_OF_EXPRESSION);
//}

void deleteIntro(TokenList& tt, TokenList::iterator END_OF_MAIN_DECLARATION){
	while (tt.begin() != END_OF_MAIN_DECLARATION){
		tt.erase(tt.begin());
	}
}
void ProgramInitChecker(TokenList& t,TokenList::iterator& it){
	//la rappresentazione è del tipo
	//		DataTypeName (stringa)
	//quindi per stringa = "(" cerchiamo "((" 
	//per stringa = ")" cerchiamo "()"
	const string CONTROL_ARRAY[5] = {"main","((","()","{","}"};
	TokenList::iterator j=t.end();
	for (int i=0; i<5; i++){
		try {
			//Se la rappresentazione in stringa contiene l'elemento dell'array
			//vuol dire che è esatto!!
			if (i!=4) {
				if (it==t.end()){
					throw new UnexpectedEndOfProgramException(1,CONTROL_ARRAY[i]);
				}
				if ((*it)->toString().find(CONTROL_ARRAY[i])==string::npos){
					it++;
					throw new NotMainException(CONTROL_ARRAY[i]);
				}
				//se siamo qui, il carattere precedente è esatto!!
				it++;
			}
			else {
				//con i= 4 c'è l'ultimo controllo
				//ultima parentesi
				j--;
				if ((*j)->toString().find(CONTROL_ARRAY[i])==string::npos){
					throw new NotMainException(CONTROL_ARRAY[4]);
				}
				//t.erase(--t.end());
			}
		}
		catch (Exception* ex){
			ex->toString();
		}
	}
	//qua vuol dire che il main è giusto
	//deleteIntro(t,it);
}

