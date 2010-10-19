#include <xercesc/parsers/SAXParser.hpp>
#include "MovieSAXHandler.hpp"
#include "InvertedIndex.hpp"
#include <xercesc/util/XMLString.hpp>
#include "mex.h"
#include "matrix.h"

// Writes it to ostream
std::ostream& operator<<(std::ostream& out, const NodeCollection& nc) {
   // use const_iterator to walk through elements of pairs
  for ( multimap< int, int >::const_iterator iter = nc.begin(); iter != nc.end(); iter++ )
    out << (*iter).first << '\t' << (*iter).second << '\n';

  return out;
}


// Writes it to ostream
std::ostream& operator<<(std::ostream& out, const InvertedIndex* ii) {
  // get the vocab
  out << "vocab\n\n";
  for(map<string, int>::const_iterator it = ii->vocab.begin(); it != ii->vocab.end(); it++)
    out << (*it).first << " " << (*it).second << "\n";
  out
    << "\n\n"
    << "docs\n"
    << ii->docs
    << "\n\n"
    << "enbtities\n"
    << ii->entities
    << "\n\n"
    << "doc_entities\n"
    << ii->doc_entities
    << "\n\n"
    << "entity_weights\n"
    << ii->entity_weights
    << "\n\n"
    << "syns\n"
    << ii->syns
    << "\n\n"
    << "entity_syns\n"
    << ii->entity_syns
    << "\n\n"
    << "names"
    << ii->names
    << "\n\n"
    << "entity_names\n"
    << ii->entity_names;

  return out;
}

// searches the vocabulary for an id and returns the word
string searchVocab(int id, map<string, int>* vocab) {
  
  string returnStr;

  // first have to check if the id is 0
  if(id == 0)
    returnStr = "";

  for(map<string, int>::iterator it = vocab->begin(); it != vocab->end(); it++)
    if(it->second == id)
      returnStr = it->first;
  
  // what if it can't find a word associated with an id? have to throw an exception!
  
  return returnStr;
  
}


// output features to files
void OUTProcedure() {

  // get the vocab
  map<string, int>* vocab = &(InvertedIndex::instance()->vocab);

  multimap<int, int>::const_iterator doc_ent_it = InvertedIndex::instance()->doc_entities.begin();
  // go through all the document_entities
  for(; doc_ent_it != InvertedIndex::instance()->doc_entities.end(); ++doc_ent_it) {
    // go through all the entities
    multimap<int, int>::const_iterator ent_it = InvertedIndex::instance()->entities.begin((*doc_ent_it).second);
    cout << "Entity: ";
    for(; ent_it != InvertedIndex::instance()->entities.end((*doc_ent_it).second); ++ent_it) {
      // map the entities into words using vocab
      cout << searchVocab((*ent_it).second, vocab) << " ";
    }
    // go through all the syns
    multimap<int, int>::const_iterator ent_syn_it = InvertedIndex::instance()->entity_syns.begin((*doc_ent_it).second);
    for(; ent_syn_it != InvertedIndex::instance()->entity_syns.end((*doc_ent_it).second); ++ent_syn_it) {
      cout << "Syn: ";
      multimap<int, int>::const_iterator syn_it = InvertedIndex::instance()->syns.begin((*ent_syn_it).second);
      for(; syn_it != InvertedIndex::instance()->syns.end((*ent_syn_it).second); ++syn_it) {
	// map the syns into words using vocab
	cout << searchVocab((*syn_it).second, vocab) << " ";
      }
    }
    // get the weight of that entity
    multimap<int, int>::const_iterator w_it = InvertedIndex::instance()->entity_weights.begin((*doc_ent_it).second);
    cout << "weight: ";
    for(; w_it != InvertedIndex::instance()->entity_weights.end((*doc_ent_it).second); ++w_it) {
      // output the weights
      cout << (*w_it).second << " ";
    }      
    cout << "\n"; 
  }
  
}

#ifdef MAT

// creates a MAT sparse matrix
mxArray* createMATSparseMatrix(int rows, int cols, int nzmax,
			       multimap<int, int>::const_iterator begin,
			       multimap<int, int>::const_iterator end) {

  mxArray* sparse = mxCreateSparse(rows, cols, nzmax, mxREAL);

  double* pr = mxGetPr(sparse);
  mwIndex* ir = mxGetIr(sparse);
  mwIndex* jc = mxGetJc(sparse);

  for(multimap<int, int>::const_iterator it = begin;
      it != end;
      it++) {
    *(ir++) = (*it).second-1;
    *(pr++) = 1;
    jc[(*it).first]++;
  }

  int jc_length = cols+1;

  int accum = 0;
  for(int i = 0; i < jc_length-1; i++) {
    jc[i] = accum;
    if(jc[i+1] > 0)
      accum += jc[i+1];
  }

  jc[jc_length-1] = accum;


  return sparse;


}
// To run SAXParser in MATLAB: 
// [v, ds, ws, es, ews, des, ss, ess, ns, ens] = SAXParser('')
void MATProcedure(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {


    // pass the inverted index back to matlab
    // here we have to individually pass all the data from the maps into mxArrays
    
    // create the vocab array
    //int vocab_size = InvertedIndex::instance()->getVocabSize();
    
    map<string, int>* vocab = &(InvertedIndex::instance()->vocab);
    int vocab_size = InvertedIndex::instance()->vocab.size();

    //int i = 0;
    mwSize vsize = static_cast<mwSize>(vocab_size);
    plhs[0] = mxCreateCellArray(1, &vsize);
    for(map<string, int>::iterator it = vocab->begin(); it != vocab->end(); it++) {
      mxSetCell(plhs[0], (*it).second - 1, mxCreateString((*it).first.c_str()));
    }
    
    // create WS and DS arrays
    // WS, DS is size 1 x n where n is the number of word tokens. 
    // The word stream WS contains word indices in order of occurence with WS=0 
    // representing the end-of-sentence-end marker. 
    // The document indices DS contains all document indices and max(DS) = D = number of documents
    // they should be double-precision vectors
    
    int numwords = InvertedIndex::instance()->docs.size();
    plhs[1] = mxCreateDoubleMatrix(numwords, 1, mxREAL);
    plhs[2] = mxCreateDoubleMatrix(numwords, 1, mxREAL); 
    double *Z = mxGetPr(plhs[1]);
    double *D = mxGetPr(plhs[2]);
    
    for(multimap<int, int>::const_iterator it = InvertedIndex::instance()->docs.begin();
	it != InvertedIndex::instance()->docs.end();
	it++) {
      //cout << (*it).first << " " << (*it).second << "\n";
      *(Z++) = (*it).first;
      *(D++) = (*it).second;
    } 
    
    // Create sparse arrays for entities
    int rows, cols, nzmax;
    rows = InvertedIndex::instance()->entities.highest(); // highest index in entities;
    cols = vocab_size;
    nzmax = InvertedIndex::instance()->entities.size();
    
    multimap<int, int> rev = InvertedIndex::instance()->entities.reverseMap();
    
    plhs[3] = createMATSparseMatrix(rows, cols, nzmax, rev.begin(), rev.end());
    
    
  // create an array for entity_weights
    mwSize dims[2];
    dims[1] = 1;
    dims[0] = InvertedIndex::instance()->entities.highest();
    plhs[4] = mxCreateNumericArray(2, dims, mxINT32_CLASS, mxREAL);
    int *W = (int*)mxGetData(plhs[4]);
    for(multimap<int,int>::const_iterator it = InvertedIndex::instance()->entity_weights.begin(); 
	it != InvertedIndex::instance()->entity_weights.end();
	it++) {
      W[(*it).first-1] = (*it).second;
    //cout << (*it).second << " " << W[(*it).first] << "\n";
    }
    
    
    // create a sparse matrix for doc_entities
    rows = InvertedIndex::instance()->docs.highest();
    cols = InvertedIndex::instance()->entities.highest();
    nzmax = InvertedIndex::instance()->doc_entities.size();
    rev = InvertedIndex::instance()->doc_entities.reverseMap();
    plhs[5] = createMATSparseMatrix(rows, cols, nzmax, rev.begin(), rev.end());
    
    // crate a sparse matrix for syns
    rows = InvertedIndex::instance()->syns.highest();
    cols = vocab_size;
    nzmax = InvertedIndex::instance()->syns.size();
    rev = InvertedIndex::instance()->syns.reverseMap();
    plhs[6] = createMATSparseMatrix(rows, cols, nzmax, rev.begin(), rev.end());
    
    // create a sparse matrix for entity_syn 
    rows = InvertedIndex::instance()->entities.highest();
    cols = InvertedIndex::instance()->syns.highest();
    nzmax = InvertedIndex::instance()->entity_syns.size();
    rev = InvertedIndex::instance()->entity_syns.reverseMap();
    plhs[7] = createMATSparseMatrix(rows, cols, nzmax, rev.begin(), rev.end());

    // crate a sparse matrix for names
    rows = InvertedIndex::instance()->names.highest();
    cols = vocab_size;
    nzmax = InvertedIndex::instance()->names.size();
    rev = InvertedIndex::instance()->names.reverseMap();
    plhs[8] = createMATSparseMatrix(rows, cols, nzmax, rev.begin(), rev.end());
    
    // create a sparse matrix for entity_names
    rows = InvertedIndex::instance()->entities.highest();
    cols = InvertedIndex::instance()->names.highest();
    nzmax = InvertedIndex::instance()->entity_names.size();
    rev = InvertedIndex::instance()->entity_names.reverseMap();
    plhs[9] = createMATSparseMatrix(rows, cols, nzmax, rev.begin(), rev.end());
    
}

#endif

#ifdef MAT
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
#else
int main (int argc, char* argv[]) {
  //int nlhs;
  //mxArray *plhs[3];
  //int nrhs;
  //const mxArray *prhs[];
#endif

#ifdef MAT
  if (nrhs < 1) {
#else
  if (argc < 1) {
#endif
    cout << "XML corpus file required";
    exit(1);
  }

  // get the corpus file name
#ifdef MAT
  int strlen = mxGetN(prhs[0])+1;
  char* corpus_file = (char*)mxCalloc(strlen, sizeof(char)); //mxCalloc is similar to malloc in C
  mxGetString(prhs[0], corpus_file, strlen);
#else
  char* corpus_file = argv[1];
#endif

#ifdef DEBUG
  cout << "Parsing document: " <<  corpus_file << "\n";
#endif

  try {
    XMLPlatformUtils::Initialize();
  }
  catch (const XMLException& toCatch) {
    char* message = XMLString::transcode(toCatch.getMessage());
    cout << "Error during initialization! :\n"
	 << message << "\n";
    XMLString::release(&message);
    exit(1);//return 1;
  }
  
  SAXParser* parser = new SAXParser();
  parser->setValidationScheme(SAXParser::Val_Always);
  parser->setDoNamespaces(true);    // optional
  
  MovieSAXHandler* content = new MovieSAXHandler();
  ErrorHandler* errHandler = (ErrorHandler*) content;
  parser->setDocumentHandler(content);
  parser->setErrorHandler(errHandler);
  
  try {
    parser->parse(corpus_file);
  }
  catch (const XMLException& toCatch) {
    char* message = XMLString::transcode(toCatch.getMessage());
    cout << "Exception message is: \n"
	 << message << "\n";
    XMLString::release(&message);
    exit(1);//return -1;
  }
  catch (const SAXParseException& toCatch) {
    char* message = XMLString::transcode(toCatch.getMessage());
    cout << "Exception message is: \n"
                 << message << "\n";
    XMLString::release(&message);
    exit(1);//return -1;
  }
  catch (...) {
    cout << "Unexpected Exception \n" ;
    exit(1);//return -1;
  }

#ifdef DEBUG
  
  cout << "Printing the inverted index: \n";
  
  cout << InvertedIndex::instance() << "\n";

    
#endif

#ifdef MAT

  // call this function to output all the matrices to the MATLAB environment.
  MATProcedure(nlhs, plhs, nrhs, prhs);

#else
  
  // call this function to output features to files
  OUTProcedure();

#endif

#ifdef DEBUG
  cout << "Done \n";
#endif

  delete InvertedIndex::instance();
  delete parser;
  delete content;

}

