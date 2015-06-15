#include "stdafx.h"

//JmmCompiler Function Headers

void gotoNextInterrupt(TokenList& tl,TokenList::iterator& ii,const string& ss);
void deleteIntro(TokenList& tt, TokenList::iterator END_OF_MAIN_DECLARATION);
void ProgramInitChecker(TokenList& t,TokenList::iterator&);
void gotoNextLine(TokenList::iterator&,TokenList::const_iterator&);
