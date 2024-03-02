#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
};

struct Query {
    set <string> pluswords;
    set <string> minuswords;
};

class SearchServer {
public:

    int document_count_ = 0;

    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);

        for (const string& word : words) {
            if (!stop_words_.count(word)) {
                word_to_document_freqs_[word][document_id] += 1.0 / words.size();
            }
        }

    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        Query query_words = ParseQuery(raw_query);
        vector <Document> matched_documents = FindAllDocuments(query_words);

        sort(matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) {
                return lhs.relevance > rhs.relevance;
            });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:



    map<string, map<int, double>> word_to_document_freqs_;
    // word id freq id freq id freq...


    set<string> stop_words_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }



    Query ParseQuery(const string& text) const {
        Query groupedwords;
        for (string word : SplitIntoWordsNoStop(text)) {
            if (word.at(0) == '-') {
                word = word.substr(1);
                groupedwords.minuswords.insert(word);
            }
            else {
                groupedwords.pluswords.insert(word);
            }
        }
        return groupedwords;
    }

    vector<Document> FindAllDocuments(const Query& query_words) const {
        vector<Document> matched_documents;
        map<int, double> id_REL;
        map<string, map<int, double>> result_docs_freq;


        for (const string& word : query_words.pluswords) {
            if (word_to_document_freqs_.count(word)) {
                result_docs_freq[word] = word_to_document_freqs_.at(word);
            }
        }
        for (const string& word : query_words.minuswords) {
            if (word_to_document_freqs_.count(word)) {
                result_docs_freq.erase(word);
            }
        }


        for (const auto& [word, id_rel] : result_docs_freq) {
            double IDF = log(static_cast<double> (document_count_) / static_cast<double> (word_to_document_freqs_.at(word).size()));

            for (const auto& [id, rel] : id_rel) {
                id_REL[id] += rel * IDF;
            }

        }

        for (const auto& doc : id_REL) {
            matched_documents.push_back({ doc.first, doc.second });
        }

        return matched_documents;
    }



};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    search_server.document_count_ = document_count;

    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
            << "relevance = "s << relevance << " }"s << endl;
    }
}