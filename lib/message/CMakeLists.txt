add_library(message Request/Request.pb.cc Response/Response.pb.cc)
target_include_directories(message PUBLIC Request Response)