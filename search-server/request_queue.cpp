#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server)
        : search_server_(search_server)
        , no_results_requests_(0)
        , current_time_(0) {
    }
  

    std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
        const auto result = search_server_.FindTopDocuments(raw_query, status);
        AddRequest(result.size());
        return result;
    }
    std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
        const auto result = search_server_.FindTopDocuments(raw_query);
        AddRequest(result.size());
        return result;
    }
    int RequestQueue::GetNoResultRequests() const {
        return no_results_requests_;
    }
