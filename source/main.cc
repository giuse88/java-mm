#include "stdafx.h"

using namespace std;

int main( int n, char** args ) {
	cout <<  INTRO << ANALYZE;
	if (n<2){
		cout << ERRARG;
		system("Pause");
		return 0;
	}
	// prende il file in ingresso
	ifstream fin;
	fin.open(args[1]);
	Analyzer* lexer = new Analyzer(&fin,&cerr);
	try {
		while(lexer->yylex() != 0);
		cout << TLCREATE;
		TokenList& tl = lexer->getTokenList();
		cout << TLOK;
		TokenList::iterator i = tl.begin();
		TokenList::const_iterator end = tl.end();
		bool error_ts=false;
		bool error_pr=false; 
		bool error_tf = false; 
		Tavola_funzioni prova(i, end, error_tf); 
		cout << MAINCH;
		ProgramInitChecker(tl,i); // controllo main 
		cout << MAINCHOK << ST;
		TabellaSimboli ts(i,end,error_ts);  //  creo tabella simboli
		cout << STOK /*<< ts << endl*/;
		Program pr(ts,i,end,error_pr);
		if (error_pr  || error_ts){
			cout << CANTRANS;
			exit(0);
		}	
		cout << TL;
		Translate(ts,prova,pr); 
		cout << TROK;
		system("java -jar jasmin.jar program.jtb"); 
		cout << FINALE;
		if (n==3 && strcmp(args[2],"-d")==0){
			ofstream fout;
			fout.open("st.txt");
			fout << ts << endl;
			fout.close();
			fout.open("pg.txt");
			fout << pr << endl;
			fout.close();
		}		 
	}
	catch (Exception* ex){
		cerr << "Eccezione al livello di main():\n\t";
		ex->toString();
		exit(0);
	}
	catch (ParserError e) {
		cerr << e << endl;
	}
	delete lexer;
	return 0;
}

