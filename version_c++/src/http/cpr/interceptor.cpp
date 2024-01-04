#include "interceptor.h"

#include <exception>

namespace cpr {

Response Interceptor::proceed(Session& session) {
    return session.proceed();
}

Response Interceptor::proceed(Session& session, ProceedHttpMethod httpMethod) {
    switch (httpMethod) {
        case ProceedHttpMethod::DELETE_REQUEST:
            return session.Delete();
        case ProceedHttpMethod::GET_REQUEST:
            return session.Get();
        case ProceedHttpMethod::HEAD_REQUEST:
            return session.Head();
        case ProceedHttpMethod::OPTIONS_REQUEST:
            return session.Options();
        case ProceedHttpMethod::PATCH_REQUEST:
            return session.Patch();
        case ProceedHttpMethod::POST_REQUEST:
            return session.Post();
        case ProceedHttpMethod::PUT_REQUEST:
            return session.Put();
        default:
            break;
    }
}

Response Interceptor::proceed(Session& session, ProceedHttpMethod httpMethod, std::ofstream& file) {
    if (httpMethod == ProceedHttpMethod::DOWNLOAD_FILE_REQUEST) {
        return session.Download(file);
    }
}

Response Interceptor::proceed(Session& session, ProceedHttpMethod httpMethod, const WriteCallback& write) {
    if (httpMethod == ProceedHttpMethod::DOWNLOAD_CALLBACK_REQUEST) {
        return session.Download(write);
    }
}

} // namespace cpr