/*

QUESTO FILE CONTIENE TUTTI I FILE DI INCLUSIONE NECESSARI ALLO SVILUPPO DEL PROGETTO
POICHE' VERRA' INCLUSO POI UNA SOLA VOLTA NON VI SONO PROBLEMI DI AMBIGUITA' O DI
RIDEFINIZIONE DI FILE/CLASSI/FUNZIONI.

GENERATO CON MICROSOFT VISUAL STUDIO 2008 ENTERPRISE

*/

#pragma once

//DEFINE

#define INTRO "*************J-- Command Line Compiler (Revision 2: v.2.4.3F)**************\n\n"
#define ANALYZE  "Analyzing J-- Sorce Code...\n"
#define TLCREATE  "Creating TokenList...\n"
#define TLOK "TokenList succesfully Created!\n"
#define MAINCH "Checking Main(){}..."
#define MAINCHOK "Main(){} Checked!\n"
#define ST "Creating Symbol Table...\n"
#define STOK "Symbol Table succesfully Created!\n"
#define TL "Translating..."
#define CANTRANS "E' stato impossibile tradurre il programma Jmm in ByteCode. Consultare il file compile.log per ulteriori informazioni\n"
#define TROK "Translation Complete!\n"
#define JJ "Transforming JTB in JBB...\n"
#define FINALE "Compilation Accomplished!\n"
#define ERRARG "Failed to find file.\nE' stato impossibile recuperare il nome del file dall'elenco degli argomenti\nE' possibile che il numero di argomenti fosse errato.\nDigitare\n\tJmmCompiler - nomefile.estensione\n"

//	INCLUDE TUTTE LE LIBRERIE C++

#include <string>
#include <cstring>
#include <stdexcept>
#include <fstream>
#include <map>
#include <iostream>
#include <list>
#include <cctype>
#include <sstream>
#include <stdio.h>


//	INCLUDE TUTTE LE INTESTAZIONI DELL'ANALYZER

#include "FlexLexer.h"
#include "token.hh"
#include "analyzer.hh"

class Albero_espressione; 
class Tavola_funzioni; 

//	INCLUDE I SORGENTI DI JmmCompiler R2

#include "commonFunctions.h"
#include "exception.h"
#include "declarations.h"
#include "Identificatore.h"
#include "symboltable.h"
#include "sintassi.h"
#include "exprTree.h"
#include "Program.h"
#include "funzione.h"
#include "tavola_funzioni.h"
#include "translate.h"

extern DataTypeName TIPO_GENERALE;
extern bool primo;
//	INCLUDE I NAMESPACE

using namespace std;

