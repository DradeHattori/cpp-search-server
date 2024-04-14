#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Document {
    Document();
    Document(int doc_id, double doc_relevance, int doc_rating);
    int id;
    double relevance;
    int rating;
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED
};

ostream& operator<<(ostream& output, const Document& document);

void PrintDocument(const Document& document);

void PrintMatchDocumentResult(int document_id, const vector<string>& words, DocumentStatus status);